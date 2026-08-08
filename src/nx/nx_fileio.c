
#include "osd_cpu.h"
#include "fileio.h"
#include "mame.h"
#include "unzip.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* MAME2003-Plus's own src/fileio.c now owns osd_get_path/osd_fopen - it
   resolves paths itself from struct GameOptions (src/mame.h) instead of
   asking the OSD layer for them one call at a time. Duplicating that here
   would be a link error. osd_fopen() returns a plain FILE*, but unzip.c
   still calls osd_fclose/fread/fseek/ftell as opaque-handle wrappers from
   the older contract, so those are implemented below as thin forwards to
   the standard C library. */

//---------------------------------------------------------------------
//	nx_fileio_init - point core's path resolution (src/fileio.c's
//	osd_get_path) at the same flat directory layout the old NX osd_get_path
//	used ("roms", "nvram", "cfg", "hi", "artwork", "samples" as siblings of
//	the working directory), so existing save/cfg/nvram data isn't orphaned.
//---------------------------------------------------------------------
void nx_fileio_init(void)
{
	options.libretro_content_path = strdup("roms");
	options.libretro_save_path    = strdup(".");
	options.libretro_system_path  = strdup(".");
	options.save_subfolder        = false;
	options.system_subfolder      = false;
	options.crc_only              = 0;
}

int osd_fclose(FILE *file)
{
	return fclose(file);
}

UINT32 osd_fread(FILE *file, void *buffer, UINT32 length)
{
	return (UINT32)fread(buffer, 1, length, file);
}

int osd_fseek(FILE *file, INT64 offset, int whence)
{
	return fseek(file, (long)offset, whence);
}

UINT64 osd_ftell(FILE *file)
{
	return (UINT64)ftell(file);
}
