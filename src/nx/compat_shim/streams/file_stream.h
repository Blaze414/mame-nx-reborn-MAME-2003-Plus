#ifndef _NX_COMPAT_FILE_STREAM_H
#define _NX_COMPAT_FILE_STREAM_H

/* replaces libretro-common's streams/file_stream.h: fileio.c only needs
   existence + directory checks, plain POSIX covers both on NX/devkitA64 */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static INLINE int filestream_exists(const char *path)
{
	return access(path, F_OK) == 0;
}

static INLINE int path_is_directory(const char *path)
{
	struct stat st;
	if (stat(path, &st) != 0)
		return 0;
	return (st.st_mode & S_IFDIR) != 0;
}

/* mkdir -p equivalent; mirrors libretro-common's path_mkdir() contract
   (returns true if the dir exists or was created) */
static INLINE int path_mkdir(const char *dir)
{
	char buf[512];
	size_t len = strlen(dir);
	size_t i;

	if (len == 0 || len >= sizeof(buf))
		return 0;

	strcpy(buf, dir);
	if (buf[len - 1] == '/')
		buf[len - 1] = '\0';

	for (i = 1; buf[i]; i++)
	{
		if (buf[i] == '/')
		{
			buf[i] = '\0';
			mkdir(buf, 0777);
			buf[i] = '/';
		}
	}
	mkdir(buf, 0777);

	return path_is_directory(dir);
}

#endif
