#ifndef _NX_COMPAT_INTERFACE_STREAM_H
#define _NX_COMPAT_INTERFACE_STREAM_H

/* replaces libretro-common's streams/interface_stream.h (+ its rzip backend).
   cheat.c uses this purely as a cheat.dat -> cheat.rzip caching layer;
   the "compression" was never load-bearing for correctness, so this is a
   plain stdio passthrough. cheat.rzip ends up byte-identical to cheat.dat -
   still opens fine as its own cache file, just uncompressed. */

#include <stdio.h>
#include <stdlib.h>

#define RETRO_VFS_FILE_ACCESS_READ       0
#define RETRO_VFS_FILE_ACCESS_WRITE      1
#define RETRO_VFS_FILE_ACCESS_HINT_NONE  0

typedef struct { FILE *fp; } intfstream_t;

static INLINE intfstream_t *intfstream_open_file(const char *path, unsigned mode, unsigned hint)
{
	intfstream_t *s = (intfstream_t*)malloc(sizeof(intfstream_t));
	if (!s) return NULL;
	s->fp = fopen(path, mode == RETRO_VFS_FILE_ACCESS_WRITE ? "wb" : "rb");
	if (!s->fp) { free(s); return NULL; }
	return s;
}

/* no real rzip backend here - the passthrough above already covers both
   read and write, so cheat.dat/cheat.rzip are handled identically */
static INLINE intfstream_t *intfstream_open_rzip_file(const char *path, unsigned mode)
{
	return intfstream_open_file(path, mode, RETRO_VFS_FILE_ACCESS_HINT_NONE);
}

static INLINE int64_t intfstream_read(intfstream_t *s, void *buf, size_t len)
{
	return (int64_t)fread(buf, 1, len, s->fp);
}

static INLINE int64_t intfstream_write(intfstream_t *s, const void *buf, size_t len)
{
	return (int64_t)fwrite(buf, 1, len, s->fp);
}

static INLINE char *intfstream_gets(intfstream_t *s, char *buf, size_t len)
{
	return fgets(buf, (int)len, s->fp);
}

static INLINE void intfstream_rewind(intfstream_t *s)
{
	rewind(s->fp);
}

static INLINE void intfstream_close(intfstream_t *s)
{
	if (s && s->fp) fclose(s->fp);
}

#endif
