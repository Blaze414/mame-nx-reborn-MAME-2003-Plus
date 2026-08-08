# the core object files (without target specific objects;
# those are added in the target.mak files)
COREOBJS = $(OBJ)/version.o $(OBJ)/mame.o \
	$(OBJ)/drawgfx.o $(OBJ)/common.o $(OBJ)/usrintrf.o $(OBJ)/ui_text.o \
	$(OBJ)/cpuintrf.o $(OBJ)/cpuexec.o $(OBJ)/cpuint.o $(OBJ)/memory.o $(OBJ)/timer.o \
	$(OBJ)/palette.o $(OBJ)/input.o $(OBJ)/inptport.o $(OBJ)/unzip.o \
	$(OBJ)/audit.o $(OBJ)/info.o $(OBJ)/png.o $(OBJ)/artwork.o \
	$(OBJ)/tilemap.o $(OBJ)/fileio.o \
	$(OBJ)/state.o $(OBJ)/datafile.o $(OBJ)/hiscore.o \
	$(sort $(CPUOBJS)) \
	$(OBJ)/sndintrf.o \
	$(OBJ)/sound/streams.o $(OBJ)/sound/mixer.o $(OBJ)/sound/filter.o \
	$(sort $(SOUNDOBJS)) \
	$(OBJ)/sound/votrax.o \
	$(OBJ)/machine/tmp68301.o \
	$(OBJ)/machine/6532riot.o \
	$(OBJ)/machine/z80fmly.o $(OBJ)/machine/6821pia.o \
	$(OBJ)/machine/8255ppi.o $(OBJ)/machine/7474.o \
	$(OBJ)/machine/74123.o $(OBJ)/machine/74153.o \
	$(OBJ)/machine/74148.o \
	$(OBJ)/vidhrdw/generic.o $(OBJ)/vidhrdw/vector.o \
	$(OBJ)/vidhrdw/avgdvg.o $(OBJ)/machine/mathbox.o \
	$(OBJ)/vidhrdw/poly.o $(OBJ)/vidhrdw/matrix3d.o \
	$(OBJ)/vidhrdw/tlc34076.o \
	$(OBJ)/machine/ticket.o $(OBJ)/machine/eeprom.o \
	$(OBJ)/machine/6522via.o $(OBJ)/machine/mb87078.o \
	$(OBJ)/machine/random.o \
	$(OBJ)/mamedbg.o $(OBJ)/window.o \
	$(OBJ)/profiler.o \
	$(OBJ)/hash.o $(OBJ)/sha1.o \
	$(OBJ)/harddisk.o $(OBJ)/md5.o $(OBJ)/machine/idectrl.o \
	$(OBJ)/lib/mame-chd/chd.o \
	$(OBJ)/nx/compat_shim/utils/md5.o \
	$(OBJ)/controls.o \
	$(OBJ)/bootstrap.o $(OBJ)/config.o $(OBJ)/ost_samples.o \
	$(OBJ)/sound/wavwrite.o \
	$(OBJ)/x86drc.o \
	$(sort $(DBGOBJS))

# hdcomp (CHD authoring CLI) is excluded: it's written against a much older,
# richer hard_disk_* API (hard_disk_interface/hard_disk_header/HDERR_*) than
# what this vendored src/harddisk.c (MAME2003-Plus's CHD-based one) provides.
# Not used by the emulator itself at runtime - only by people hand-building
# CHD disk images offline. `make hdcomp.exe` still works standalone if
# someone wants to fix it up later.
TOOLS = romcmp$(EXE) xml2info$(EXE)
