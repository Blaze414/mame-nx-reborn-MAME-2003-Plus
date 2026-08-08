#include <stdio.h>
#include <string.h>

#include "driver.h"
#include "datafile.h"
#include "input.h"
#include "nx_mameui.h"

#define MAX_ENTRIES 500

/*=====================================================================
	Game history
=====================================================================*/
int nx_history_load(char *buffer, int size)
{
	if (!buffer || size <= 0)
		return 1;

	buffer[0] = 0;

	if (!Machine || !Machine->gamedrv)
		return 1;

	/* load_driver_history() returns non-zero when history.dat is absent or has
	   no entry for this driver, which is the normal case - the file is an
	   optional download, not part of a romset. */
	if (load_driver_history(Machine->gamedrv, buffer, size) != 0 || buffer[0] == 0)
	{
		buffer[0] = 0;
		return 1;
	}
	return 0;
}

/*=====================================================================
	Analog controls

	Same filter settraksettings() applies: analog port types only, and
	IPF_CHEAT entries stay hidden unless the core's cheat input ports are
	enabled.
=====================================================================*/
static int analog_collect(struct InputPort **entries, int capacity)
{
	struct InputPort *in;
	int total = 0;

	if (!Machine || !Machine->input_ports)
		return 0;

	in = Machine->input_ports;

	while (in->type != IPT_END && total < capacity)
	{
		if (((in->type & 0xff) > IPT_ANALOG_START) &&
			((in->type & 0xff) < IPT_ANALOG_END) &&
			!(!options.cheat_input_ports && (in->type & IPF_CHEAT)))
			entries[total++] = in;
		in++;
	}
	return total;
}

static struct InputPort *analog_at(int index)
{
	struct InputPort *entries[MAX_ENTRIES];
	int total = analog_collect(entries, MAX_ENTRIES);

	if (index < 0 || index >= total)
		return 0;
	return entries[index];
}

int nx_analog_count(void)
{
	struct InputPort *entries[MAX_ENTRIES];
	return analog_collect(entries, MAX_ENTRIES);
}

const char *nx_analog_name(int index)
{
	struct InputPort *entry = analog_at(index);
	const char *name = entry ? input_port_name(entry) : 0;

	return name ? name : "";
}

const char *nx_analog_field_name(int field)
{
	switch (field)
	{
		case NX_ANALOG_DELTA:       return "Digital Speed";
		case NX_ANALOG_REVERSE:     return "Reverse";
		case NX_ANALOG_SENSITIVITY: return "Sensitivity";
	}
	return "";
}

const char *nx_analog_field_value(int index, int field)
{
	static char buffer[32];
	struct InputPort *entry = analog_at(index);

	buffer[0] = 0;
	if (!entry)
		return buffer;

	switch (field)
	{
		case NX_ANALOG_DELTA:
			sprintf(buffer, "%d", IP_GET_DELTA(entry));
			break;
		case NX_ANALOG_REVERSE:
			strcpy(buffer, (entry->type & IPF_REVERSE) ? "On" : "Off");
			break;
		case NX_ANALOG_SENSITIVITY:
			sprintf(buffer, "%d%%", IP_GET_SENSITIVITY(entry));
			break;
	}
	return buffer;
}

void nx_analog_change(int index, int field, int direction)
{
	struct InputPort *entry = analog_at(index);
	int value;

	if (!entry || direction == 0)
		return;

	switch (field)
	{
		case NX_ANALOG_DELTA:
			value = IP_GET_DELTA(entry) + (direction > 0 ? 1 : -1);
			if (value < 1)   value = 1;
			if (value > 255) value = 255;
			IP_SET_DELTA(entry, value);
			break;

		case NX_ANALOG_REVERSE:
			/* A flag, so either direction flips it - stepping a boolean in two
			   directions is the same move twice. */
			if (entry->type & IPF_REVERSE) entry->type &= ~IPF_REVERSE;
			else                           entry->type |=  IPF_REVERSE;
			break;

		case NX_ANALOG_SENSITIVITY:
			value = IP_GET_SENSITIVITY(entry) + (direction > 0 ? 1 : -1);
			if (value < 1)   value = 1;
			if (value > 255) value = 255;
			IP_SET_SENSITIVITY(entry, value);
			break;
	}
}

/*=====================================================================
	Input remapping

	Two independent lists. "General" is inputport_defaults - the bindings every
	driver inherits. "This game" is Machine->input_ports - overrides that live
	with the running set. Same filters the two usrintrf.c pages apply.
=====================================================================*/
extern struct ipd inputport_defaults[];

static int general_collect(struct ipd **entries, int capacity)
{
	struct ipd *in = inputport_defaults;
	int total = 0;

	while (in->type != IPT_END && total < capacity)
	{
		if (in->name != 0 &&
			(in->type & ~IPF_MASK) != IPT_UNKNOWN &&
			(in->type & ~IPF_MASK) != IPT_OSD_DESCRIPTION &&
			(in->type & IPF_UNUSED) == 0 &&
			!(!options.cheat_input_ports && (in->type & IPF_CHEAT)))
			entries[total++] = in;
		in++;
	}
	return total;
}

static int specific_collect(struct InputPort **entries, int capacity)
{
	struct InputPort *in;
	int total = 0;

	if (!Machine || !Machine->input_ports)
		return 0;

	in = Machine->input_ports;

	while (in->type != IPT_END && total < capacity)
	{
		if (input_port_name(in) != 0 &&
			seq_get_1(&in->seq) != CODE_NONE &&
			(in->type & ~IPF_MASK) != IPT_UNKNOWN &&
			(in->type & ~IPF_MASK) != IPT_OSD_DESCRIPTION &&
			(in->type & IPF_UNUSED) == 0 &&
			!(!options.cheat_input_ports && (in->type & IPF_CHEAT)))
			entries[total++] = in;
		in++;
	}
	return total;
}

/* The two lists hold different types, so every accessor resolves to one or the
   other rather than to a common pointer. */
static struct ipd *general_at(int index)
{
	struct ipd *entries[MAX_ENTRIES];
	int total = general_collect(entries, MAX_ENTRIES);

	if (index < 0 || index >= total)
		return 0;
	return entries[index];
}

static struct InputPort *specific_at(int index)
{
	struct InputPort *entries[MAX_ENTRIES];
	int total = specific_collect(entries, MAX_ENTRIES);

	if (index < 0 || index >= total)
		return 0;
	return entries[index];
}

/* The sequence a capture writes into. For "this game" that is the port's own
   seq; for general it is the default entry's. */
static InputSeq *seq_at(int index, int general)
{
	if (general)
	{
		struct ipd *entry = general_at(index);
		return entry ? &entry->seq : 0;
	}
	else
	{
		struct InputPort *entry = specific_at(index);
		return entry ? &entry->seq : 0;
	}
}

int nx_input_count(int general)
{
	if (general)
	{
		struct ipd *entries[MAX_ENTRIES];
		return general_collect(entries, MAX_ENTRIES);
	}
	else
	{
		struct InputPort *entries[MAX_ENTRIES];
		return specific_collect(entries, MAX_ENTRIES);
	}
}

const char *nx_input_name(int index, int general)
{
	const char *name = 0;

	if (general)
	{
		struct ipd *entry = general_at(index);
		name = entry ? entry->name : 0;
	}
	else
	{
		struct InputPort *entry = specific_at(index);
		name = entry ? input_port_name(entry) : 0;
	}
	return name ? name : "";
}

const char *nx_input_binding(int index, int general)
{
	static char buffer[128];
	InputSeq *seq = seq_at(index, general);

	buffer[0] = 0;
	if (!seq)
		return buffer;

	/* CODE_DEFAULT means "whatever the general list says", which is a real and
	   useful state to see on the per-game page - not the same as unbound. */
	if (!general && seq_get_1(seq) == CODE_DEFAULT)
	{
		struct InputPort *entry = specific_at(index);

		if (entry)
		{
			seq_name(input_port_seq(entry), buffer, sizeof(buffer));
			return buffer;
		}
	}

	seq_name(seq, buffer, sizeof(buffer));
	return buffer;
}

void nx_input_capture_start(void)
{
	seq_read_async_start();
}

/* seq_read_async() returns -1 while it is still collecting and 0 once the
   sequence has settled, so the sense is inverted here to the obvious one. */
int nx_input_capture_poll(int index, int general)
{
	InputSeq *seq = seq_at(index, general);

	if (!seq)
		return 1;

	if (seq_read_async(seq, 1) != 0)
	{
		/* An empty result would leave the control permanently dead, so an
		   unbindable capture falls back to the default rather than to nothing.
		   Matches what setcodesettings() does on the same condition. */
		if (seq_get_1(seq) == CODE_NONE)
			seq_set_1(seq, general ? CODE_NONE : CODE_DEFAULT);
		return 1;
	}
	return 0;
}

void nx_input_capture_clear(int index, int general)
{
	InputSeq *seq = seq_at(index, general);

	if (seq)
		seq_set_1(seq, general ? CODE_NONE : CODE_DEFAULT);
}

/*=====================================================================
	Memory card (Neo Geo)
=====================================================================*/
extern int  memcard_status;
extern int  memcard_number;
extern int  neogeo_memcard_load(int);
extern void neogeo_memcard_save(void);
extern void neogeo_memcard_eject(void);
extern int  neogeo_memcard_create(int);

int nx_memcard_available(void)
{
	return options.content_flags[CONTENT_NEOGEO] ? 1 : 0;
}

int nx_memcard_inserted(void) { return memcard_status; }
int nx_memcard_slot(void)     { return memcard_number; }

int nx_memcard_action(int action, int slot)
{
	if (!nx_memcard_available())
		return 1;

	if (slot < 0)    slot = 0;
	if (slot > 1000) slot = 1000;

	switch (action)
	{
		case NX_MEMCARD_LOAD:
			/* Eject first: loading over an inserted card is what usrintrf.c
			   does, and skipping it leaves the old card's state half-live. */
			neogeo_memcard_eject();
			if (neogeo_memcard_load(slot))
			{
				memcard_status = 1;
				memcard_number = slot;
				return 0;
			}
			memcard_status = 0;
			return 1;

		case NX_MEMCARD_SAVE:
			if (!memcard_status)
				return 1;
			neogeo_memcard_save();
			return 0;

		case NX_MEMCARD_EJECT:
			if (!memcard_status)
				return 1;
			neogeo_memcard_eject();
			memcard_status = 0;
			return 0;

		case NX_MEMCARD_CREATE:
			if (memcard_status)
				return 1;
			if (neogeo_memcard_create(slot))
				return 0;
			return 1;
	}
	return 1;
}
