#include <stdio.h>
#include "osd_cpu.h"
#include "osdepend.h"
#include "mame2003.h"

#include "nx_joystick.h"
 
  //! Macros for redefining input sequences
  //  NOTE: SEQ_MAX is 16 (src/input.h) - REMAP_SEQ_17..20 would overflow
  //  InputSeq and are not defined. SEQ_DEF_8..16 aren't provided upstream
  //  (input.h only goes up to SEQ_DEF_7), so they're filled in below,
  //  following the exact same CODE_NONE-padding pattern input.h itself uses.
#define SEQ_DEF_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) { a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p }
#define SEQ_DEF_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) SEQ_DEF_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,CODE_NONE)
#define SEQ_DEF_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) SEQ_DEF_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,CODE_NONE)
#define SEQ_DEF_13(a,b,c,d,e,f,g,h,i,j,k,l,m) SEQ_DEF_14(a,b,c,d,e,f,g,h,i,j,k,l,m,CODE_NONE)
#define SEQ_DEF_12(a,b,c,d,e,f,g,h,i,j,k,l) SEQ_DEF_13(a,b,c,d,e,f,g,h,i,j,k,l,CODE_NONE)
#define SEQ_DEF_11(a,b,c,d,e,f,g,h,i,j,k) SEQ_DEF_12(a,b,c,d,e,f,g,h,i,j,k,CODE_NONE)
#define SEQ_DEF_10(a,b,c,d,e,f,g,h,i,j) SEQ_DEF_11(a,b,c,d,e,f,g,h,i,j,CODE_NONE)
#define SEQ_DEF_9(a,b,c,d,e,f,g,h,i) SEQ_DEF_10(a,b,c,d,e,f,g,h,i,CODE_NONE)
#define SEQ_DEF_8(a,b,c,d,e,f,g,h) SEQ_DEF_9(a,b,c,d,e,f,g,h,CODE_NONE)
#define REMAP_SEQ_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)           { InputSeq newSeq = SEQ_DEF_15((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n)             { InputSeq newSeq = SEQ_DEF_14((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_13(a,b,c,d,e,f,g,h,i,j,k,l,m)               { InputSeq newSeq = SEQ_DEF_13((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_12(a,b,c,d,e,f,g,h,i,j,k,l)                 { InputSeq newSeq = SEQ_DEF_12((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_11(a,b,c,d,e,f,g,h,i,j,k)                   { InputSeq newSeq = SEQ_DEF_11((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_10(a,b,c,d,e,f,g,h,i,j)                     { InputSeq newSeq = SEQ_DEF_10((a),(b),(c),(d),(e),(f),(g),(h),(i),(j)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_9(a,b,c,d,e,f,g,h,i)                        { InputSeq newSeq = SEQ_DEF_9((a),(b),(c),(d),(e),(f),(g),(h),(i)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_8(a,b,c,d,e,f,g,h)                          { InputSeq newSeq = SEQ_DEF_8((a),(b),(c),(d),(e),(f),(g),(h)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_7(a,b,c,d,e,f,g)                            { InputSeq newSeq = SEQ_DEF_7((a),(b),(c),(d),(e),(f),(g)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_6(a,b,c,d,e,f)                              { InputSeq newSeq = SEQ_DEF_6((a),(b),(c),(d),(e),(f)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_5(a,b,c,d,e)                                { InputSeq newSeq = SEQ_DEF_5((a),(b),(c),(d),(e)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_4(a,b,c,d)                                  { InputSeq newSeq = SEQ_DEF_4((a),(b),(c),(d)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_3(a,b,c)                                    { InputSeq newSeq = SEQ_DEF_3((a),(b),(c)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_2(a,b)                                      { InputSeq newSeq = SEQ_DEF_2((a),(b)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_1(a)                                        { InputSeq newSeq = SEQ_DEF_1((a)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_0()				                          { InputSeq newSeq = SEQ_DEF_0; memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
 
static int g_systemInitialized = 0;
 
const struct KeyboardInfo nxKeys[] =
{
	{0, 0, 0}
};
 
//---------------------------------------------------------------------
//	osd_customize_inputport_defaults
//---------------------------------------------------------------------
void osd_customize_inputport_defaults( struct ipd *defaults )
{

  if( g_systemInitialized == 0)
  {
    nxInitializeJoystick();
    g_systemInitialized = 1;
  }	 
  
  nxCustomizeInputPortDefaults( defaults );
}

 
int osd_is_key_pressed( int keycode )
{
	return 0;
}

int osd_readkey_unicode( int flush )
{
 
	return 0;
}

//---------------------------------------------------------------------
//	osd_get_key_list
//---------------------------------------------------------------------
const struct KeyboardInfo *osd_get_key_list( void )
{
	return nxKeys;
}

void nxCustomizeInputPortDefaults( struct ipd *defaults )
{

	UINT32 i = 0;

	for( ; defaults[i].type != IPT_END; ++i )
	{
		struct ipd *entry = &defaults[i];

		switch( entry->type )
		{
		/* Upstream puts UI Select on BUTTON2 and UI Cancel on BUTTON1
		   (inptport.c:172-173). Those are PC-pad positions; on a Switch pad
		   BUTTON1 is A and BUTTON2 is B, so out of the box A cancelled and B
		   confirmed - backwards everywhere on this console, and the reason
		   MAME's own menus felt like a different device from the frontend.
		   Bind them explicitly to the physical buttons instead of relying on
		   an ordering that means something else here.

		   input_ui_pressed() for these two is only ever called from
		   usrintrf.c's menu handlers, so A and B keep working as arcade
		   Buttons 1 and 2 during play. */
		case IPT_UI_SELECT:
			REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_A ) );
			break;
		case IPT_UI_CANCEL:
			/* Every submenu backs out with this, so B is "back" at any depth,
			   not only on the top-level page. */
			REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_B ) );
			break;
		case IPT_UI_CONFIGURE:
			/* R Stick click opens (and closes) MAME's own config menu. Plain
			   binding now that the stick chords are gone: the frontend's pause
			   overlay lives on Plus+Minus and no longer claims either stick. */
			REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_RA_STICK ) );
			break;
		case IPT_UI_ON_SCREEN_DISPLAY:
			/* Left blank on purpose - the frontend overlay covers pausing. */
			REMAP_SEQ_0();
			break;


		case IPT_UI_UP:
		  REMAP_SEQ_15( JOYCODE_1_UP, CODE_OR, AXISCODE( 0, JT_LSTICK_UP ), CODE_OR,
						JOYCODE_2_UP, CODE_OR, AXISCODE( 1, JT_LSTICK_UP ), CODE_OR,
						JOYCODE_3_UP, CODE_OR, AXISCODE( 2, JT_LSTICK_UP ), CODE_OR,
						JOYCODE_4_UP, CODE_OR, AXISCODE( 3, JT_LSTICK_UP ));
		  break;

				// *** IPT_UI_LEFT *** //
		case IPT_UI_LEFT:
		  REMAP_SEQ_15( JOYCODE_1_LEFT, CODE_OR, AXISCODE( 0, JT_LSTICK_LEFT ), CODE_OR,
						JOYCODE_2_LEFT, CODE_OR, AXISCODE( 1, JT_LSTICK_LEFT ), CODE_OR,
						JOYCODE_3_LEFT, CODE_OR, AXISCODE( 2, JT_LSTICK_LEFT ), CODE_OR,
						JOYCODE_4_LEFT, CODE_OR, AXISCODE( 3, JT_LSTICK_LEFT ) );
		  break;

				// *** IPT_UI_DOWN *** //
		case IPT_UI_DOWN:
		  REMAP_SEQ_15( JOYCODE_1_DOWN, CODE_OR, AXISCODE( 0, JT_LSTICK_DOWN ), CODE_OR,
						JOYCODE_2_DOWN, CODE_OR, AXISCODE( 1, JT_LSTICK_DOWN ), CODE_OR,
						JOYCODE_3_DOWN, CODE_OR, AXISCODE( 2, JT_LSTICK_DOWN ), CODE_OR,
						JOYCODE_4_DOWN, CODE_OR, AXISCODE( 3, JT_LSTICK_DOWN ) );
		  break;

				// *** IPT_UI_RIGHT *** //
		case IPT_UI_RIGHT:
		  REMAP_SEQ_15( JOYCODE_1_RIGHT, CODE_OR, AXISCODE( 0, JT_LSTICK_RIGHT ), CODE_OR,
						JOYCODE_2_RIGHT, CODE_OR, AXISCODE( 1, JT_LSTICK_RIGHT ), CODE_OR,
						JOYCODE_3_RIGHT, CODE_OR, AXISCODE( 2, JT_LSTICK_RIGHT ), CODE_OR,
						JOYCODE_4_RIGHT, CODE_OR, AXISCODE( 3, JT_LSTICK_RIGHT ) );
		break;
		}
	}
}