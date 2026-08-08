 

#include "osd_cpu.h"
#include "osdepend.h"
#include "mame2003.h"
#include <switch.h>
#include <time.h>
#include <sys/time.h>

 

//-------------------------------------------------------------
//	osd_cycles
//-------------------------------------------------------------
cycles_t osd_cycles( void )
{
	/* The system tick counter is monotonic, needs no syscall, and runs at a
	   fixed 19.2MHz on every Switch - a better clock source here than
	   gettimeofday(), which can step. */
	return (cycles_t)armGetSystemTick();
}


//-------------------------------------------------------------
//	osd_cycles_per_second
//-------------------------------------------------------------
cycles_t osd_cycles_per_second( void )
{
	return (cycles_t)armGetSystemTickFreq();
}

//-------------------------------------------------------------
//	osd_profiling_ticks
//-------------------------------------------------------------
cycles_t osd_profiling_ticks( void )
{
	return osd_cycles();
}



