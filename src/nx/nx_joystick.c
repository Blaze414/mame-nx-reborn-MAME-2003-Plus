 
#include <stdio.h>

#include "osd_cpu.h"
#include "osdepend.h"
#include "mame2003.h"
#include "inptport.h"
#include "nx_joystick.h"

#include "nx_mame.h" 
#include <switch.h> 
 
#define ANALOG_AS_DIGITAL_DEADZONE	4096
 
static struct JoystickInfo		g_joystickInfo[128] = {0,0,0};
static UINT32                   g_calibrationStep = 0;
static UINT32                   g_calibrationJoynum = 0;
 
UINT32							g_numOSDInputKeywords;

/* libnx's old HidControllerID/hidJoystickRead/JoystickPosition API was
   removed upstream in favor of PadState/padGetButtons/padGetStickPos -
   one PadState per supported player (stickIndex 0-3, matching the loop
   below). Player 0 also accepts Handheld, same as padInitializeDefault(). */
static PadState g_pads[4];

/*---------------------------------------------------------------------
	Per-frame input latch

	osd_is_joy_pressed() used to call padUpdate() itself, which meant one HID
	read per queried code rather than per frame. update_input_ports() walks
	every port and every element of every sequence, so a two-player driver asks
	upward of a hundred times a frame - and each of those reads a *different*
	snapshot of a shared-memory block the system is writing to asynchronously.

	The cost was the smaller half of it. The real problem is that a frame's
	inputs were never read as one moment: a button pressed a microsecond into
	the walk is seen by the ports after it and not by the ports before it. On a
	fighter that is two simultaneous presses landing on different frames -
	block-and-low arriving as block-then-low, a motion input losing its last
	direction - which reads as the emulator dropping inputs.

	Latched once instead, at the first query of each frame, which VBLANK puts
	right at the start of the emulated frame and immediately after the pacer's
	wait. So it is both the whole truth and the freshest one available.
---------------------------------------------------------------------*/
static u64  g_padButtons[4];
static HidAnalogStickState g_padLeft[4], g_padRight[4];
static int  g_padLatched = 0;

static void nx_joystick_latch( void )
{
	int i;

	if( g_padLatched ) return;

	for( i = 0; i < 4; ++i )
	{
		padUpdate( &g_pads[i] );
		g_padButtons[i] = padGetButtons( &g_pads[i] );
		g_padLeft[i]    = padGetStickPos( &g_pads[i], 0 );
		g_padRight[i]   = padGetStickPos( &g_pads[i], 1 );
	}
	g_padLatched = 1;
}

/* Called once a frame from osd_update_video_and_audio(), which runs on every
   frame including skipped ones - so the latch can never go stale for longer
   than the frame it was taken in. */
void nx_joystick_frame( void )
{
	g_padLatched = 0;
}

void nxInitializeJoystick( void )
{
	BEGINENTRYMAP();
	INT32 stickIndex = 0;

	/* Configured once in main() - see the note there. */
	padInitialize( &g_pads[0], HidNpadIdType_No1, HidNpadIdType_Handheld );
	padInitialize( &g_pads[1], HidNpadIdType_No2 );
	padInitialize( &g_pads[2], HidNpadIdType_No3 );
	padInitialize( &g_pads[3], HidNpadIdType_No4 );

	for( ; stickIndex < 4; ++stickIndex )
	{
    char name[32];
 
      // DPad
    ADDENTRY( "DPAD UP",      JOYCODE( stickIndex, JT_DPAD_UP, 0 ),        STDCODE( UP ) );
    ADDENTRY( "DPAD RIGHT",   JOYCODE( stickIndex, JT_DPAD_RIGHT , 0 ),    STDCODE( RIGHT ) );
    ADDENTRY( "DPAD DOWN",    JOYCODE( stickIndex, JT_DPAD_DOWN, 0 ),      STDCODE( DOWN ) );
    ADDENTRY( "DPAD LEFT",    JOYCODE( stickIndex, JT_DPAD_LEFT , 0 ),     STDCODE( LEFT ) );

      // Left analog
    AXISCODE( stickIndex, JT_LSTICK_UP );
    AXISCODE( stickIndex, JT_LSTICK_RIGHT );
    AXISCODE( stickIndex, JT_LSTICK_DOWN );
    AXISCODE( stickIndex, JT_LSTICK_LEFT );
    ADDENTRY( "LA UP",        JOYCODE( stickIndex, JT_LSTICK_UP, 0 ),     CODE_OTHER );
    ADDENTRY( "LA RIGHT",     JOYCODE( stickIndex, JT_LSTICK_RIGHT , 0 ), CODE_OTHER );
    ADDENTRY( "LA DOWN",      JOYCODE( stickIndex, JT_LSTICK_DOWN, 0 ),   CODE_OTHER );
    ADDENTRY( "LA LEFT",      JOYCODE( stickIndex, JT_LSTICK_LEFT , 0 ),  CODE_OTHER );

      // Right analog
    AXISCODE( stickIndex, JT_RSTICK_UP );
    AXISCODE( stickIndex, JT_RSTICK_RIGHT );
    AXISCODE( stickIndex, JT_RSTICK_DOWN );
    AXISCODE( stickIndex, JT_RSTICK_LEFT );
    ADDENTRY( "RA UP",        JOYCODE( stickIndex, JT_RSTICK_UP, 0 ),     CODE_OTHER );
    ADDENTRY( "RA RIGHT",     JOYCODE( stickIndex, JT_RSTICK_RIGHT , 0 ), CODE_OTHER );
    ADDENTRY( "RA DOWN",      JOYCODE( stickIndex, JT_RSTICK_DOWN, 0 ),   CODE_OTHER );
    ADDENTRY( "RA LEFT",      JOYCODE( stickIndex, JT_RSTICK_LEFT , 0 ),  CODE_OTHER );

      // Buttons
 
    BUTTONCODE( stickIndex, BUTTON_ZL );
    BUTTONCODE( stickIndex, BUTTON_ZR );
    ADDENTRY( "A",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_A ),              STDCODE( BUTTON1 ) );
    ADDENTRY( "B",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_B ),              STDCODE( BUTTON2 ) );
    ADDENTRY( "X",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_X ),              STDCODE( BUTTON3 ) );
    ADDENTRY( "Y",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_Y ),              STDCODE( BUTTON4 ) );
    ADDENTRY( "LTrig",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_LEFT_TRIGGER ),   STDCODE( BUTTON5 ) );
    ADDENTRY( "RTrig",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_RIGHT_TRIGGER ),  STDCODE( BUTTON6 ) );
    ADDENTRY( "Plus",         JOYCODE( stickIndex, JT_BUTTON, BUTTON_PLUS ),           STDCODE( START ) );
    ADDENTRY( "Minus",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_MINUS ),          STDCODE( SELECT ) );     
    /* ZL/ZR carry BUTTON9/BUTTON10, matching mame-nx, so six-button games plus
       the two extra arcade buttons are all reachable. nx_input.c moves
       IPT_UI_CONFIGURE off BUTTON9 onto an R-stick click, so these stay pure
       game buttons. */
    ADDENTRY( "ZL",           JOYCODE( stickIndex, JT_BUTTON, BUTTON_ZL ),             STDCODE( BUTTON9 ) );
    ADDENTRY( "ZR",           JOYCODE( stickIndex, JT_BUTTON, BUTTON_ZR ),             STDCODE( BUTTON10 ) );
    ADDENTRY( "L Stick",      JOYCODE( stickIndex, JT_BUTTON, BUTTON_LA_STICK ),       CODE_OTHER );
    ADDENTRY( "R Stick",      JOYCODE( stickIndex, JT_BUTTON, BUTTON_RA_STICK ),       CODE_OTHER );


  }
}
 
 
//---------------------------------------------------------------------
//	osd_get_joy_list
//---------------------------------------------------------------------
const struct JoystickInfo *osd_get_joy_list( void )
{  
	 
	
	
	return g_joystickInfo;
}

//---------------------------------------------------------------------
//	osd_is_joy_pressed
//---------------------------------------------------------------------
int osd_is_joy_pressed( int joycode )
{
	int32_t joyindex = JOYINDEX(joycode);
	int32_t joytype = JT(joycode);
	int32_t joynum = JOYNUM(joycode);

	nx_joystick_latch();

	u64 buttons = g_padButtons[joynum];
	HidAnalogStickState pos_left  = g_padLeft[joynum];
	HidAnalogStickState pos_right = g_padRight[joynum];

	switch( joytype )
	{
		case JT_BUTTON:
			switch( joyindex )
			{
				case BUTTON_A:
					return (buttons & HidNpadButton_A);
				case BUTTON_X:
					return (buttons & HidNpadButton_X);
				case BUTTON_B:
					return (buttons & HidNpadButton_B);
				case BUTTON_Y:
					return (buttons & HidNpadButton_Y);
				case BUTTON_LEFT_TRIGGER:
					return (buttons & HidNpadButton_L);
				case BUTTON_RIGHT_TRIGGER:
					return (buttons & HidNpadButton_R);
				case BUTTON_PLUS:
					return (buttons & HidNpadButton_Plus);
				case BUTTON_MINUS:
					return (buttons & HidNpadButton_Minus);
				case BUTTON_LA_STICK:
					return (buttons & HidNpadButton_StickL);
				case BUTTON_RA_STICK:
					return (buttons & HidNpadButton_StickR);
			}
			break;

		case JT_LSTICK_UP:
			return (pos_left.y > ANALOG_AS_DIGITAL_DEADZONE );
		case JT_LSTICK_DOWN:
			return (pos_left.y < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_LSTICK_LEFT:
			return (pos_left.x < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_LSTICK_RIGHT:
			return (pos_left.x > ANALOG_AS_DIGITAL_DEADZONE );

		case JT_RSTICK_UP:
			return (pos_right.y  > ANALOG_AS_DIGITAL_DEADZONE );
		case JT_RSTICK_DOWN:
			return (pos_right.y  < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_RSTICK_LEFT:
			return (pos_right.x  < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_RSTICK_RIGHT:
			return (pos_right.x  > ANALOG_AS_DIGITAL_DEADZONE );

		case JT_DPAD_UP:
			return (buttons & HidNpadButton_Up) || (pos_left.y > ANALOG_AS_DIGITAL_DEADZONE );
		case JT_DPAD_DOWN:
			return (buttons & HidNpadButton_Down) ||  (pos_left.y < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_DPAD_LEFT:
			return (buttons & HidNpadButton_Left) || (pos_left.x < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_DPAD_RIGHT:
			return (buttons & HidNpadButton_Right) || (pos_left.x > ANALOG_AS_DIGITAL_DEADZONE );


	}

	return 0;

}


//---------------------------------------------------------------------
//	osd_is_joystick_axis_code
//---------------------------------------------------------------------
int osd_is_joystick_axis_code( int joycode )
{
	
	
 
	return 0;
}

//---------------------------------------------------------------------
//	osd_joystick_needs_calibration
//---------------------------------------------------------------------
int osd_joystick_needs_calibration( void )
{
 
  return 0;
}


//---------------------------------------------------------------------
//	osd_joystick_start_calibration
//---------------------------------------------------------------------
void osd_joystick_start_calibration( void )
{
 
}

//---------------------------------------------------------------------
//	osd_joystick_calibrate_next
//---------------------------------------------------------------------
const char *osd_joystick_calibrate_next( void )
{
 
	return NULL;
}

//---------------------------------------------------------------------
//	osd_joystick_calibrate
//---------------------------------------------------------------------
void osd_joystick_calibrate( void )
{
 
}

//---------------------------------------------------------------------
//	osd_joystick_end_calibration
//---------------------------------------------------------------------
void osd_joystick_end_calibration( void )
{
/* Postprocessing (e.g. saving joystick data to config) */
}

//---------------------------------------------------------------------
//	osd_xy_device_read - relative/absolute X-Y pointer devices (mouse,
//	trackball, lightgun via inptport.c). No such hardware on Switch;
//	report no movement rather than leaving deltax/deltay uninitialized.
//---------------------------------------------------------------------
void osd_xy_device_read(int player, int *deltax, int *deltay, const char *type)
{
	*deltax = 0;
	*deltay = 0;
}


//---------------------------------------------------------------------
//	osd_analogjoy_read
//---------------------------------------------------------------------
void osd_analogjoy_read(	int player,
													int analog_axis[MAX_ANALOG_AXES], 
													InputCode analogjoy_input[MAX_ANALOG_AXES] )
{
 
}
 
 
void nxAddEntry( const char *name, INT32 code, INT32 standardCode, UINT32 *joycount )
{
	struct JoystickInfo *ji = NULL;
	struct ik *inputkeywords;
  
	ji = &g_joystickInfo[*joycount];

	ji->name = strdup( name );
	if( !ji->name )
	{	 
		osd_print_error( "Out of memory!" );
		return;
	}

    // Convert spaces in ji->name to '_'
	{
		char *cur = ji->name;
		while( *cur )
		{
			if( *cur == ' ' )
			*cur = '_';
		++cur;
		}
	}

	ji->code = code;
	ji->standardcode = standardCode;

	++(*joycount);
}
  