 
 
#include "osdepend.h"
#include "osd_cpu.h"
#include "mame2003.h"
#include "log.h"

#include "nx_mame.h"


#include "mame.h"
#include "cpuexec.h"
#include "palette.h"
#include "common.h"
#include "usrintrf.h"


#include <stdio.h>
#include <stdarg.h>

//---------------------------------------------------------------------
//	log_cb - log.h declares this extern (used by ~250 core/driver files
//	via logerror/fatalerror/RETRO_LOG_*); something has to own the storage.
//---------------------------------------------------------------------
static void nx_log_printf( int level, const char *fmt, ... )
{
	char buf[512];
	va_list arg;
	va_start( arg, fmt );
	vsnprintf( buf, sizeof(buf), fmt, arg );
	va_end( arg );
	printf( "%s%s", LOGPRE, buf );
}

nx_log_printf_t log_cb = nx_log_printf;



//---------------------------------------------------------------------
//  osd_malloc_debug
//---------------------------------------------------------------------
void *osd_malloc_debug( size_t size, const char *filename, unsigned int line, const char *function )
{
 
  void *ret = malloc( size );
 
  return ret;
}


//---------------------------------------------------------------------
//  osd_calloc_debug
//---------------------------------------------------------------------
void *osd_calloc_debug( size_t num, size_t size, const char *filename, unsigned int line, const char *function )
{
 

  void *ret = calloc( num, size );
 
  return ret;
}

//---------------------------------------------------------------------
//  osd_realloc_debug
//---------------------------------------------------------------------
void *osd_realloc_debug( void *memblock, size_t size, const char *filename, unsigned int line, const char *function )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = realloc( memblock, size );

  return ret;
}

//---------------------------------------------------------------------
//  osd_malloc_retail
//---------------------------------------------------------------------
void *osd_malloc_retail( size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = malloc( size );
 
  return ret;
}


//---------------------------------------------------------------------
//  osd_calloc_retail
//---------------------------------------------------------------------
void *osd_calloc_retail( size_t num, size_t size )
{
 
  void *ret = calloc( num, size );

  return ret;
}

//---------------------------------------------------------------------
//  osd_realloc_retail
//---------------------------------------------------------------------
void *osd_realloc_retail( void *memblock, size_t size )
{
  // [EBA] - "Safe" malloc, exits the program if the malloc fails, rather than
  // relying on MAME to actually check for failure (which it does not, in numerous
  // places)

  void *ret = realloc( memblock, size );

  return ret;
}


//---------------------------------------------------------------------
//	osd_display_loading_rom_message
//---------------------------------------------------------------------
int osd_display_loading_rom_message( const char *name, struct rom_load_data *romdata )
{
 
	
	RenderProgress(name,romdata);
	
	return 0;
}

//---------------------------------------------------------------------
//	osd_pause
//---------------------------------------------------------------------
void osd_pause( int paused )
{
}

/* Raised by the pause menu's "Exit Game" row (usrintrf.c), consumed by the
   main loop in nx_maintest.cpp. */
int nx_exit_to_menu = 0;

//---------------------------------------------------------------------
//	pause_updatescreen - void(void) adapter for updatescreen()'s
//	int(void) signature, so it fits the pause_action function pointer
//---------------------------------------------------------------------
static void pause_updatescreen( void )
{
	updatescreen();
}

//---------------------------------------------------------------------
//	mame_pause - drives usrintrf.c's in-game config/pause menu
//	(was defined in the now-removed libretro glue; core-facing only,
//	no libretro dependency - cpuexec.c's frame loop checks pause_action
//	every tick and calls it instead of running CPUs while set)
//---------------------------------------------------------------------
void mame_pause( bool pause )
{
	if (pause)
	{
		pause_action = pause_updatescreen;
		osd_pause(1);
	}
	else
	{
		toggle_showgfx = false;
		pause_action = 0;
		osd_pause(0);
	}
}

//---------------------------------------------------------------------
//	logerror
//---------------------------------------------------------------------
void logerror( const char *fmt, ... )
{
 
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );

	//PRINTMSG(( T_ERROR, buf ));
 

//debugload("error = %s\n",buf);
}

//---------------------------------------------------------------------
//	osd_print_error
//---------------------------------------------------------------------
void osd_print_error( const char *fmt, ... )
{  
  char buf[1024] = {0};

  va_list arg;
  va_start( arg, fmt );
  vsnprintf( buf, 1023, fmt, arg );
  va_end( arg );
 
   
  RenderMessage(buf);
 
}

//---------------------------------------------------------------------
//	osd_autobootsavestate
//---------------------------------------------------------------------
void osd_autobootsavestate( const char *gameName )
{
 
}

//---------------------------------------------------------------------
//	frontend_message_cb - on-screen warning/status toast (missing
//	ROMs/samples, XML dat progress...). No RA overlay here, route to
//	the log so these don't silently vanish (e.g. why a ROM won't launch).
//---------------------------------------------------------------------
void frontend_message_cb( const char *message_string, unsigned frames_to_display )
{
	if (log_cb)
		log_cb(NX_LOG_WARN, "%s\n", message_string);
}

/* osd_die: log.h #defines this to fatalerror (its own static inline,
   log_cb-backed implementation) - a real osd_die() here would get
   textually renamed to fatalerror by that macro and collide with it. */

double atan2(double x, double y)
{
  return 0.0f;
}