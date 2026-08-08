#include "driver.h"
#include "nx_dips.h"

#define MAX_DIPS 128

/* IPF_CHEAT dips are the driver author marking a switch as not-really-hardware
   (infinite lives service modes and such). Upstream hides them unless the cheat
   option is on; this core has no options.cheat, so they stay hidden - a dip
   list that matches the physical cabinet is the honest default. */
static int collect(struct InputPort **entries, int capacity)
{
	struct InputPort *in = Machine->input_ports;
	int total = 0;

	while (in->type != IPT_END && total < capacity)
	{
		if ((in->type & ~IPF_MASK) == IPT_DIPSWITCH_NAME &&
				input_port_name(in) != 0 &&
				(in->type & IPF_UNUSED) == 0 &&
				!(in->type & IPF_CHEAT))
			entries[total++] = in;
		in++;
	}
	return total;
}

static struct InputPort *entry_at(int index)
{
	struct InputPort *entries[MAX_DIPS];
	int total = collect(entries, MAX_DIPS);

	if (index < 0 || index >= total)
		return 0;
	return entries[index];
}

/* The settings for a dip are the IPT_DIPSWITCH_SETTING rows immediately after
   its NAME row; the live one is whichever carries the port's current value. */
static struct InputPort *current_setting(struct InputPort *entry)
{
	struct InputPort *setting = entry + 1;

	while ((setting->type & ~IPF_MASK) == IPT_DIPSWITCH_SETTING &&
			setting->default_value != entry->default_value)
		setting++;

	return (setting->type & ~IPF_MASK) == IPT_DIPSWITCH_SETTING ? setting : 0;
}

int nx_dip_count(void)
{
	struct InputPort *entries[MAX_DIPS];

	if (!Machine || !Machine->input_ports)
		return 0;
	return collect(entries, MAX_DIPS);
}

const char *nx_dip_name(int index)
{
	struct InputPort *entry = entry_at(index);
	return entry ? input_port_name(entry) : "";
}

const char *nx_dip_value(int index)
{
	struct InputPort *entry = entry_at(index);
	struct InputPort *setting = entry ? current_setting(entry) : 0;

	return setting ? input_port_name(setting) : "-";
}

void nx_dip_change(int index, int direction)
{
	struct InputPort *entry = entry_at(index);
	struct InputPort *setting;
	struct InputPort *candidate;

	if (!entry)
		return;

	setting = current_setting(entry);

	/* No setting matched the current value, so there is nothing to step from.
	   Land on the first declared setting instead of doing nothing, which is
	   the only way out of that state. */
	if (!setting)
	{
		candidate = entry + 1;
		if ((candidate->type & ~IPF_MASK) == IPT_DIPSWITCH_SETTING)
			entry->default_value = candidate->default_value & entry->mask;
		return;
	}

	candidate = direction > 0 ? setting + 1 : setting - 1;

	/* Stepping back off the top of the list would walk into the NAME row, and
	   forward off the end into whatever follows - both are checked by type. */
	if ((candidate->type & ~IPF_MASK) == IPT_DIPSWITCH_SETTING &&
			!(candidate->type & IPF_CHEAT))
		entry->default_value = candidate->default_value & entry->mask;
}
