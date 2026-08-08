#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************

	Standalone log interface (replaces libretro's log_cb/RETRO_LOG_* shim)

******************************************************************************/

#define LOGPRE          "[MAME-NX] "

enum
{
	NX_LOG_DEBUG = 0,
	NX_LOG_INFO,
	NX_LOG_WARN,
	NX_LOG_ERROR
};

typedef void (*nx_log_printf_t)(int level, const char *fmt, ...);

extern nx_log_printf_t log_cb;

/* keep RETRO_LOG_* names working across the ~250 driver/core files that
   still reference them, without pulling in libretro.h anywhere */
#define RETRO_LOG_DEBUG NX_LOG_DEBUG
#define RETRO_LOG_INFO  NX_LOG_INFO
#define RETRO_LOG_WARN  NX_LOG_WARN
#define RETRO_LOG_ERROR NX_LOG_ERROR

/* logerror has a real implementation in nx_misc.c, declared extern in
   osdepend.h - not redeclared here to avoid a static/extern clash wherever
   both headers end up in the same translation unit. */

#ifdef __GNUC__
static INLINE int CLIB_DECL fatalerror(const char *string,...) __attribute__ ((format (printf, 1, 2)));
#endif

static INLINE int CLIB_DECL fatalerror(const char *string, ...)
{
	static char log_buffer[2048];
	va_list arg;
	va_start(arg,string);
	vsprintf(log_buffer,string,arg);
	va_end(arg);
	log_cb(NX_LOG_DEBUG, "(LOGERROR) %s",log_buffer);
	exit(1);
}
#define osd_die fatalerror
#endif /* LOG_H */
