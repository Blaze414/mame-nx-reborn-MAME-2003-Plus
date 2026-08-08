/******************************************************************************

  driver.c

  The list of all available drivers. Drivers have to be included here to be
  recognized by the executable.

  To save some typing, we use a hack here. This file is recursively #included
  twice, with different definitions of the DRIVER() macro. The first one
  declares external references to the drivers; the second one builds an array
  storing all the drivers.

******************************************************************************/

#include "driver.h"


#ifndef DRIVER_RECURSIVE

/* The "root" driver, defined so we can have &driver_##NAME in macros. */
struct GameDriver driver_0 =
{
	__FILE__,
	0,
	"",
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	NOT_A_DRIVER
};

#endif

#ifndef DRIVER_RECURSIVE

#define DRIVER_RECURSIVE

/* step 1: declare all external references */
#define DRIVER(NAME) extern struct GameDriver driver_##NAME;
#define TESTDRIVER(NAME) extern struct GameDriver driver_##NAME;
#include "driver.c"

/* step 2: define the drivers[] array */
#undef DRIVER
#undef TESTDRIVER
#define DRIVER(NAME) &driver_##NAME,
#define TESTDRIVER(NAME)
const struct GameDriver *drivers[] =
{
#include "driver.c"
	0	/* end of array */
};

const int total_drivers = sizeof(drivers)/sizeof(drivers[0]);

/* step 3: define the test_drivers[] array */
#undef DRIVER
#undef TESTDRIVER
#define DRIVER(NAME)
#define TESTDRIVER(NAME) &driver_##NAME,

const struct GameDriver *test_drivers[] =
{
#include "driver.c"
	0	/* end of array */
};

#else	/* DRIVER_RECURSIVE */


	/* "Pacman hardware" games */

	/* S2650 Pacman Kits */

	/* Epos games */
	DRIVER( catapult )	/* (c) 1982 */
	DRIVER( dealer )	/* (c) 198? */

	/* "Galaxian hardware" games */

	/* Has some similarities with Moon Cresta but Board is very different */

	/* "Scramble hardware" (and variations) games */
	DRIVER( flyboy )	/* (c) 1982 Kaneko */
	DRIVER( flyboyb )	/* bootleg */
	DRIVER( fastfred )	/* (c) 1982 Atari */
	DRIVER( jumpcoas )	/* (c) 1983 Kaneko */
	DRIVER( boggy84 )	/* bootleg, original is (c)1983 Taito/Kaneko */
	DRIVER( redrobin )	/* (c) 1986 Elettronolo */
	DRIVER( imago ) 	/* cocktail set */
	DRIVER( imagoa )	/* no cocktail set */

	/* "Crazy Climber hardware" games */
	DRIVER( cclimber )	/* (c) 1980 Nichibutsu */
	DRIVER( cclimbrj )	/* (c) 1980 Nichibutsu */
	DRIVER( ccboot )	/* bootleg */
	DRIVER( ccboot2 )	/* bootleg */
	DRIVER( ckong )		/* (c) 1981 Falcon */
	DRIVER( ckonga )	/* (c) 1981 Falcon */
	DRIVER( ckongjeu )	/* bootleg */
	DRIVER( ckongo )	/* bootleg */
	DRIVER( ckongalc )	/* bootleg */
	DRIVER( monkeyd )	/* bootleg */
	DRIVER( rpatrolb )	/* bootleg */
	DRIVER( silvland )	/* Falcon */
	DRIVER( swimmer )	/* (c) 1982 Tehkan */
	DRIVER( swimmera )	/* (c) 1982 Tehkan */
	DRIVER( swimmerb )	/* (c) 1982 Tehkan */
	DRIVER( guzzler )	/* (c) 1983 Tehkan */
	DRIVER( cannonb )	/* (c) 1985 Soft */

	/* Nichibutsu games */
	DRIVER( gomoku )	/* (c) 1981 */
	DRIVER( wiping )	/* (c) 1982 */
	DRIVER( rugrats )	/* (c) 1983 */
	DRIVER( friskyt )	/* (c) 1981 */
	DRIVER( friskyta )	/* (c) 1981 */
	DRIVER( radrad )	/* (c) 1982 Nichibutsu USA */
	DRIVER( seicross )	/* (c) 1984 + Alice */
	DRIVER( sectrzon )	/* (c) 1984 + Alice */
	DRIVER( firebatl )	/* (c) 1984 Taito */
	DRIVER( magmax )	/* (c) 1985 */
	DRIVER( cop01 )		/* (c) 1985 */
	DRIVER( cop01a )	/* (c) 1985 */
	DRIVER( mightguy )	/* (c) 1986 */
	DRIVER( terracre )	/* (c) 1985 */
	DRIVER( terracrb )	/* (c) 1985 */
	DRIVER( terracra )	/* (c) 1985 */
	DRIVER( amazon )	/* (c) 1986 */
	DRIVER( amatelas )	/* (c) 1986 */
	DRIVER( horekid )	/* (c) 1987 */
	DRIVER( horekidb )	/* bootleg */
	DRIVER( galivan )	/* (c) 1985 */
	DRIVER( galivan2 )	/* (c) 1985 */
	DRIVER( dangar )	/* (c) 1986 */
	DRIVER( dangar2 )	/* (c) 1986 */
	DRIVER( dangarb )	/* bootleg */
	DRIVER( ninjemak )	/* (c) 1986 (US?) */
	DRIVER( youma )		/* (c) 1986 (Japan) */

	/* Nichibutsu Mahjong games */
	DRIVER( hyhoo )		/* (c) 1987 */
	DRIVER( hyhoo2 )	/* (c) 1987 */

	DRIVER( pastelgl )	/* (c) 1985 */

	DRIVER( crystalg )	/* (c) 1986 */
	DRIVER( crystal2 )	/* (c) 1986 */
	DRIVER( citylove )	/* (c) 1986 */
	DRIVER( apparel )	/* (c) 1986 Central Denshi */
	DRIVER( secolove )	/* (c) 1986 */
	DRIVER( housemnq )	/* (c) 1987 */
	DRIVER( housemn2 )	/* (c) 1987 */
	DRIVER( seiha )		/* (c) 1987 */
	DRIVER( seiham )	/* (c) 1987 */
	DRIVER( bijokkoy )	/* (c) 1987 */
	DRIVER( iemoto )	/* (c) 1987 */
	DRIVER( ojousan )	/* (c) 1987 */
	DRIVER( bijokkog )	/* (c) 1988 */
	DRIVER( orangec )	/* (c) 1988 Daiichi Denshi */
	DRIVER( vipclub )	/* (c) 1988 Daiichi Denshi */
	DRIVER( korinai )	/* (c) 1988 */
	DRIVER( kaguya )	/* (c) 1988 MIKI SYOUJI */
	DRIVER( otonano )	/* (c) 1988 Apple */
	DRIVER( kanatuen )	/* (c) 1988 Panac */
	DRIVER( mjsikaku )	/* (c) 1988 */
	DRIVER( mjsikakb )	/* (c) 1988 */
	DRIVER( mjcamera )	/* (c) 1988 MIKI SYOUJI */
	DRIVER( mmcamera )	/* (c) 1988 MIKI SYOUJI */
	DRIVER( idhimitu )	/* (c) 1989 Digital Soft */

	DRIVER( msjiken )	/* (c) 1988 */
	DRIVER( hanamomo )	/* (c) 1988 */
	DRIVER( telmahjn )	/* (c) 1988 */
	DRIVER( gionbana )	/* (c) 1989 */
	DRIVER( mjfocus )	/* (c) 1989 */
	DRIVER( mjfocusm )	/* (c) 1989 */
	DRIVER( peepshow )	/* (c) 1989 AC */
	DRIVER( scandal )	/* (c) 1989 */
	DRIVER( scandalm )	/* (c) 1989 */
	DRIVER( mgmen89 )	/* (c) 1989 */
	DRIVER( mjnanpas )	/* (c) 1989 BROOKS */
	DRIVER( mjnanpaa )	/* (c) 1989 BROOKS */
	DRIVER( mjnanpau )	/* (c) 1989 BROOKS */
	DRIVER( pairsten )	/* (c) 1989 System Ten */
	DRIVER( bananadr )	/* (c) 1989 DIGITAL SOFT */
	DRIVER( mladyhtr )	/* (c) 1990 */
	DRIVER( chinmoku )	/* (c) 1990 */
	DRIVER( maiko )		/* (c) 1990 */
	DRIVER( club90s )	/* (c) 1990 */
	DRIVER( club90sa )	/* (c) 1990 */
	DRIVER( hanaoji )	/* (c) 1991 */

	DRIVER( pstadium )	/* (c) 1990 */
	DRIVER( triplew1 )	/* (c) 1989 */
	DRIVER( triplew2 )	/* (c) 1990 */
	DRIVER( ntopstar )	/* (c) 1990 */
	DRIVER( mjlstory )	/* (c) 1991 */
	DRIVER( vanilla )	/* (c) 1991 */
	DRIVER( finalbny )	/* (c) 1991 */
	DRIVER( qmhayaku )	/* (c) 1991 */
	DRIVER( galkoku )	/* (c) 1989 Nichibutsu/T.R.TEC */
	DRIVER( hyouban )	/* (c) 1989 Nichibutsu/T.R.TEC */
	DRIVER( galkaika )	/* (c) 1989 Nichibutsu/T.R.TEC */
	DRIVER( tokyogal )	/* (c) 1989 */
	DRIVER( tokimbsj )	/* (c) 1989 */
	DRIVER( mcontest )	/* (c) 1989 */
	DRIVER( uchuuai )	/* (c) 1989 */
	DRIVER( av2mj1bb )	/* (c) 1991 MIKI SYOUJI/AV JAPAN */
	DRIVER( av2mj2rg )	/* (c) 1991 MIKI SYOUJI/AV JAPAN */

	DRIVER( mjuraden )	/* (c) 1992 Nichibutsu/Yubis */
	DRIVER( koinomp )	/* (c) 1992 */
	DRIVER( patimono )	/* (c) 1992 */
	DRIVER( mjanbari )	/* (c) 1992 Nichibutsu/Yubis/AV JAPAN */
	DRIVER( ultramhm )	/* (c) 1993 Apple */
	DRIVER( gal10ren )	/* (c) 1993 FUJIC */
	DRIVER( renaiclb )	/* (c) 1993 FUJIC */
	DRIVER( mjlaman )	/* (c) 1993 Nichibutsu/AV JAPAN */
	DRIVER( mkeibaou )	/* (c) 1993 */
	DRIVER( pachiten )	/* (c) 1993 Nichibutsu/MIKI SYOUJI/AV JAPAN */
	DRIVER( sailorws )	/* (c) 1993 */
	DRIVER( sailorwr )	/* (c) 1993 */
	DRIVER( psailor1 )	/* (c) 1994 SPHINX */
	DRIVER( psailor2 )	/* (c) 1994 SPHINX */
	DRIVER( otatidai )	/* (c) 1995 SPHINX */
	DRIVER( ngpgal )	/* (c) 1991 */
	DRIVER( mjgottsu )	/* (c) 1991 */
	DRIVER( bakuhatu )	/* (c) 1991 */
	DRIVER( cmehyou )	/* (c) 1992 Nichibutsu/Kawakusu */
	DRIVER( mmehyou )	/* (c) 1992 Nichibutsu/Kawakusu */
	DRIVER( mjkoiura )	/* (c) 1992 */
	DRIVER( imekura )	/* (c) 1994 SPHINX/AV JAPAN */
	DRIVER( mscoutm )	/* (c) 1994 SPHINX/AV JAPAN */
	DRIVER( mjegolf )	/* (c) 1994 FUJIC/AV JAPAN */


	/* "Phoenix hardware" (and variations) games */
	DRIVER( safarir )	/* Shin Nihon Kikaku (SNK) */
	DRIVER( safarirj )	/* Shin Nihon Kikaku (SNK) Taito License */
	DRIVER( pleiads )	/* (c) 1981 Tehkan */
	DRIVER( pleiadbl )	/* bootleg */
	DRIVER( pleiadce )	/* (c) 1981 Centuri + Tehkan */
	DRIVER( capitol )	/* bootleg? */
	DRIVER( survival )	/* (c) 1982 Rock-ola */

	/* Namco games (plus some intruders on similar hardware) */
	DRIVER( bombbee )	/* [1979] Namco */
	DRIVER( cutieq )	/* (c) 1979 Namco */
	DRIVER( navalone )	/* (c) 1980 Namco */
	DRIVER( sos )		/* [1980] Namco */
	DRIVER( tankbatt )	/* (c) 1980 Namco */
	DRIVER( warpwarp )	/* (c) 1981 Namco */
	DRIVER( warpwarr )	/* (c) 1981 Rock-ola - the high score table says "NAMCO" */
	DRIVER( warpwar2 )	/* (c) 1981 Rock-ola - the high score table says "NAMCO" */
	DRIVER( rallyx )	/* (c) 1980 Namco */
	DRIVER( rallyxm )	/* (c) 1980 Midway */
	DRIVER( nrallyx )	/* (c) 1981 Namco */
	DRIVER( jungler )	/* GX327 (c) 1981 Konami */
	DRIVER( junglers )	/* GX327 (c) 1981 Stern */
	DRIVER( tactcian )	/* GX335 (c) 1982 Sega */
	DRIVER( tactcan2 )	/* GX335 (c) 1981 Sega */
	DRIVER( locomotn )	/* GX359 (c) 1982 Konami + Centuri license */
	DRIVER( gutangtn )	/* GX359 (c) 1982 Konami + Sega license */
	DRIVER( cottong )	/* bootleg */
	DRIVER( commsega )	/* (c) 1983 Sega */
	/* the following ones all have a custom I/O chip */
	/* Libble Rabble board (first Japanese game using a 68000) */
	/* Z8000 games */

	/* no custom I/O in the following, HD63701 (or compatible) microcontroller instead */
	DRIVER( drgnbstr )	/* (c) 1984 */
	DRIVER( skykid )	/* (c) 1985 */
	DRIVER( skykido )	/* (c) 1985 */
	DRIVER( skykidd )	/* (c) 1985 */
	DRIVER( baraduke )	/* (c) 1985 */
	DRIVER( baraduka )	/* (c) 1985 */
	DRIVER( metrocrs )	/* (c) 1985 */
	DRIVER( metrocra )	/* (c) 1985 */

	/* Namco System 86 games */

	/* Thunder Ceptor HW */

	/* Namco System 1 games */
	DRIVER( shadowld )	/* (c) 1987 */
	DRIVER( youkaidk )	/* (c) 1987 (Japan new version) */
	DRIVER( yokaidko )	/* (c) 1987 (Japan old version) */
	DRIVER( dspirit )	/* (c) 1987 new version */
	DRIVER( dspirito )	/* (c) 1987 old version */
	DRIVER( blazer )	/* (c) 1987 (Japan) */
	DRIVER( quester )	/* (c) 1987 (Japan) */
	DRIVER( pacmania )	/* (c) 1987 */
	DRIVER( pacmanij )	/* (c) 1987 (Japan) */
	DRIVER( galaga88 )	/* (c) 1987 */
	DRIVER( galag88b )	/* (c) 1987 */
	DRIVER( galag88j )	/* (c) 1987 (Japan) */
	DRIVER( ws )		/* (c) 1988 (Japan) */
	DRIVER( berabohm )	/* (c) 1988 (Japan) */
	DRIVER( beraboho )	/* (c) 1988 (Japan) */
	DRIVER( mmaze )		/* (c) 1988 (Japan) */
	DRIVER( bakutotu )	/* (c) 1988 (Japan) */
	DRIVER( wldcourt )	/* (c) 1988 (Japan) */
	DRIVER( splatter )	/* (c) 1988 (Japan) */
	DRIVER( faceoff )	/* (c) 1988 (Japan) */
	DRIVER( rompers )	/* (c) 1989 (Japan) */
	DRIVER( romperso )	/* (c) 1989 (Japan) */
	DRIVER( blastoff )	/* (c) 1989 (Japan) */
	DRIVER( ws89 )		/* (c) 1989 (Japan) */
	DRIVER( dangseed )	/* (c) 1989 (Japan) */
	DRIVER( ws90 )		/* (c) 1990 (Japan) */
	DRIVER( pistoldm )	/* (c) 1990 (Japan) */
	DRIVER( boxyboy )	/* (c) 1990 (US) */
	DRIVER( soukobdx )	/* (c) 1990 (Japan) */
	DRIVER( puzlclub )	/* (c) 1990 (Japan) */
	DRIVER( tankfrce )	/* (c) 1991 (US) */
	DRIVER( tankfrcj )	/* (c) 1991 (Japan) */

	/* Namco System 2 games */
	/* 91.9  Super World Stadium */
	/* 92.8  Bubble Trouble */
	/* 93.6  Super World Stadium '93 */

	/* Namco NA-1 / NA-2 System games */
	DRIVER( bkrtmaq )	/* (c) 1992 (Japan) */
	DRIVER( cgangpzl )	/* (c) 1992 (US) */
	DRIVER( cgangpzj )	/* (c) 1992 (Japan) */
	DRIVER( exvania )	/* (c) 1992 (Japan) */
	DRIVER( fghtatck )	/* (c) 1992 (US) */
	DRIVER( fa )		/* (c) 1992 (Japan) */
	DRIVER( knckhead )	/* (c) 1992 (World) */
	DRIVER( knckhedj )	/* (c) 1992 (Japan) */
	DRIVER( swcourt )	/* (c) 1992 (Japan) */
	DRIVER( emeralda )	/* (c) 1993 (Japan) */
	DRIVER( emerldaa )	/* (c) 1993 (Japan) */
	DRIVER( numanath )	/* (c) 1993 (World) */
	DRIVER( numanatj )	/* (c) 1993 (Japan) */
	DRIVER( quiztou )	/* (c) 1993 (Japan) */
	DRIVER( tinklpit )	/* (c) 1993 (Japan) */
	DRIVER( xday2 )		/* (c) 1995 (Japan) */

	/* Namco NB-1 / NB-2 System games */

	/* Namco ND-1 games */
	DRIVER( ncv1 )		/* (c) 1995 */
	DRIVER( ncv1j )		/* (c) 1995 (Japan) */
	DRIVER( ncv1j2 )	/* (c) 1995 (Japan) */
	DRIVER( ncv2 )		/* (c) 1996 */
	DRIVER( ncv2j )		/* (c) 1996 (Japan) */

	/* Namco System 21 games */
	/* 1988, Winning Run */
	/* 1989, Winning Run Suzuka Grand Prix */
/* 199?, Driver's Eyes */
/* 1992, ShimDrive */

	/* Namco System 22 games */


	/* Universal games */
	DRIVER( cosmicg )	/* 7907 (c) 1979 */
	DRIVER( cosmica )	/* 7910 (c) [1979] */
	DRIVER( cosmica2 )	/* 7910 (c) 1979 */
	DRIVER( panic )		/* (c) 1980 */
	DRIVER( panic2 )	/* (c) 1980 */
	DRIVER( panic3 )	/* (c) 1980 */
	DRIVER( panich )	/* (c) 1980 */
	DRIVER( panicger )	/* (c) 1980 */
	DRIVER( zerohour )	/* 8011 (c) Universal */
	DRIVER( redclash )	/* (c) 1981 Tehkan */
	DRIVER( redclask )	/* (c) Kaneko (bootleg?) */
	DRIVER( magspot )	/* ???? (c) [1980] */
	DRIVER( magspot2 )	/* 8013 (c) [1980] */
	DRIVER( devzone )	/* 8022 (c) [1980] */
	DRIVER( devzone2 )	/* 8022 (c) [1980] */
	DRIVER( nomnlnd )	/* (c) [1980?] */
	DRIVER( nomnlndg )	/* (c) [1980?] + Gottlieb */
	DRIVER( cheekyms )	/* (c) [1980?] */
	DRIVER( mrdo )		/* (c) 1982 */
	DRIVER( mrdoy )		/* (c) 1982 */
	DRIVER( mrdot )		/* (c) 1982 + Taito license */
	DRIVER( mrdofix )	/* (c) 1982 + Taito license */
	DRIVER( mrlo )		/* bootleg */
	DRIVER( mrdu )		/* bootleg */
	DRIVER( yankeedo )	/* bootleg */
	DRIVER( docastle )	/* (c) 1983 */
	DRIVER( docastl2 )	/* (c) 1983 */
	DRIVER( docastlo )	/* (c) 1983 */
	DRIVER( douni )		/* (c) 1983 */
	DRIVER( dorunrun )	/* (c) 1984 */
	DRIVER( dorunru2 )	/* (c) 1984 */
	DRIVER( dorunruc )	/* (c) 1984 */
	DRIVER( spiero )	/* (c) 1987 */
	DRIVER( dowild )	/* (c) 1984 */
	DRIVER( jjack )		/* (c) 1984 */
	DRIVER( kickridr )	/* (c) 1984 */
	DRIVER( idsoccer )	/* (c) 1985 */

	/* Nintendo games */
	DRIVER( mario )		/* (c) 1983 Nintendo of America */
	DRIVER( marioe )	/* (c) 1983 Nintendo of America */
	DRIVER( mariof )	/* (c) 1983 Nintendo of America */
	DRIVER( marioj )	/* (c) 1983 Nintendo */
	DRIVER( masao )		/* bootleg */
	DRIVER( skyskipr )	/* (c) 1981 */
	DRIVER( popeye )	/* (c) 1982 */
	DRIVER( popeyeu )	/* (c) 1982 */
	DRIVER( popeyef )	/* (c) 1982 */
	DRIVER( popeyebl )	/* bootleg */
	DRIVER( punchout )	/* (c) 1984 */
	DRIVER( spnchout )	/* (c) 1984 */
	DRIVER( spnchotj )	/* (c) 1984 (Japan) */
	DRIVER( armwrest )	/* (c) 1985 */

	/* Nintendo Playchoice 10 games */

	/* Nintendo VS games */

	/* Nintendo Super System Games */

	/* Midway 8080 b/w games */
	/* 603 - Top Gun [1976] */
	/* 640 - Space Walk [1978] */
	/* 870 - Space Invaders Deluxe cocktail */
	DRIVER( m79amb )



	/* Similar but with a M6800 instead of an 8080 */

	/* Meadows S2650 games */
	DRIVER( lazercmd )	/* [1976?] */
	DRIVER( bbonk )		/* [1976?] */
	DRIVER( deadeye )	/* [1978?] */
	DRIVER( gypsyjug )	/* [1978?] */
	DRIVER( minferno )	/* [1978?] */
	DRIVER( medlanes )	/* [1977?] */

	/* CVS games */

	/* Midway "Astrocade" games */
	DRIVER( seawolf2 )
	DRIVER( spacezap )	/* (c) 1980 */
	DRIVER( ebases )
	DRIVER( wow )		/* (c) 1980 */
	DRIVER( gorf )		/* (c) 1981 */
	DRIVER( gorfpgm1 )	/* (c) 1981 */
	DRIVER( robby )		/* (c) 1981 Bally Midway */
	DRIVER( profpac )	/* (c) 1983 Bally Midway */

	/* Bally Midway MCR games */
	/* MCR1 */
	DRIVER( solarfox )	/* (c) 1981 */
	DRIVER( kick )		/* (c) 1981 */
	DRIVER( kicka )		/* bootleg? */
	/* MCR2 */
	DRIVER( shollow )	/* (c) 1981 */
	DRIVER( shollow2 )	/* (c) 1981 */
	DRIVER( tron )		/* (c) 1982 */
	DRIVER( tron2 )		/* (c) 1982 */
	DRIVER( tronfp )	/* (c) custom free rom */
	DRIVER( kroozr )	/* (c) 1982 */
	DRIVER( domino )	/* (c) 1982 */
	DRIVER( wacko )		/* (c) 1982 */
	DRIVER( twotiger )	/* (c) 1984 */
	DRIVER( twotigra )	/* (c) 1984 */
	/* MCR2 + MCR3 sprites */
	DRIVER( journey )	/* (c) 1983 */
	/* MCR3 */
	DRIVER( tapper )	/* (c) 1983 */
	DRIVER( tappera )	/* (c) 1983 */
	DRIVER( sutapper )	/* (c) 1983 */
	DRIVER( rbtapper )	/* (c) 1984 */
	DRIVER( timber )	/* (c) 1984 */
	DRIVER( dotron )	/* (c) 1983 */
	DRIVER( dotrona )	/* (c) 1983 */
	DRIVER( dotrone )	/* (c) 1983 */
	DRIVER( demoderb )	/* (c) 1984 */
	DRIVER( demoderm )	/* (c) 1984 */
	DRIVER( sarge )		/* (c) 1985 */
	DRIVER( rampage )	/* (c) 1986 */
	DRIVER( rampage2 )	/* (c) 1986 */
	DRIVER( powerdrv )	/* (c) 1986 */
	DRIVER( stargrds )	/* (c) 1987 */
	DRIVER( maxrpm )	/* (c) 1986 */
	DRIVER( spyhunt )	/* (c) 1983 */
	DRIVER( turbotag )	/* (c) 1985 */
	DRIVER( crater )	/* (c) 1984 */
	/* MCR 68000 */
	DRIVER( zwackery )	/* (c) 1984 */
	DRIVER( xenophob )	/* (c) 1987 */
	DRIVER( spyhunt2 )	/* (c) 1987 */
	DRIVER( spyhnt2a )	/* (c) 1987 */
	DRIVER( blasted )	/* (c) 1988 */
	DRIVER( archrivl )	/* (c) 1989 */
	DRIVER( archriv2 )	/* (c) 1989 */
	DRIVER( trisport )	/* (c) 1989 */
	DRIVER( pigskin )	/* (c) 1990 */

	/* Bally / Sente games */
	DRIVER( gridlee )	/* [1983 Videa] prototype - no copyright notice */

	/* Irem games */
	/* trivia: IREM means "International Rental Electronics Machines" */
	DRIVER( andromed )
	DRIVER( ipminvad )	/* M10 no copyright notice */
	DRIVER( skychut )	/* Irem [1980] */
	DRIVER( spacbeam )	/* M15 no copyright notice */
	DRIVER( greenber )	/* Irem */

	DRIVER( olibochu )	/* M47 (c) 1981 + "GDI presents" */
	DRIVER( mpatrol )	/* M52 (c) 1982 */
	DRIVER( mpatrolw )	/* M52 (c) 1982 + Williams license */
	DRIVER( troangel )	/* (c) 1983 */
	DRIVER( yard )		/* (c) 1983 */
	DRIVER( vsyard )	/* (c) 1983/1984 */
	DRIVER( vsyard2 )	/* (c) 1983/1984 */
	DRIVER( travrusa )	/* (c) 1983 */
	DRIVER( motorace )	/* (c) 1983 Williams license */
	DRIVER( shtrider )	/* (c) 1984 Seibu Kaihatsu */
	DRIVER( wilytowr )	/* M63 (c) 1984 */
	DRIVER( atomboy )	/* M63 (c) 1985 Irem + Memetron license */
	/* M62 */
	DRIVER( kungfum )	/* (c) 1984 */
	DRIVER( kungfud )	/* (c) 1984 + Data East license */
	DRIVER( spartanx )	/* (c) 1984 (Japan) */
	DRIVER( kungfub )	/* bootleg */
	DRIVER( kungfub2 )	/* bootleg */
	DRIVER( battroad )	/* (c) 1984 */
	DRIVER( ldrun )		/* (c) 1984 licensed from Broderbund */
	DRIVER( ldruna )	/* (c) 1984 licensed from Broderbund */
	DRIVER( ldrun2 )	/* (c) 1984 licensed from Broderbund */
	DRIVER( ldrun3 )	/* (c) 1985 licensed from Broderbund */
	DRIVER( ldrun3jp )	/* (c) 1985 licensed from Broderbund */
	DRIVER( ldrun4 )	/* (c) 1986 licensed from Broderbund */
	DRIVER( lotlot )	/* (c) 1985 licensed from Tokuma Shoten */
	DRIVER( kidniki )	/* (c) 1986 + Data East USA license */
	DRIVER( yanchamr )	/* (c) 1986 (Japan) */
	DRIVER( spelunkr )	/* (c) 1985 licensed from Broderbund */
	DRIVER( spelnkrj )	/* (c) 1985 licensed from Broderbund */
	DRIVER( spelunk2 )	/* (c) 1986 licensed from Broderbund */
	DRIVER( horizon )	/* (c) 1985 */
	DRIVER( youjyudn )	/* (c) 1986 (Japan) */

	/* M72 (and derivatives) */
	/* not M72, but same sound hardware */
	/* M90 */
	/* M97 */
	/* M92 */
	/* M107 */

	/* Gottlieb/Mylstar games (Gottlieb became Mylstar in 1983) */
	DRIVER( reactor )	/* GV-100 (c) 1982 Gottlieb */
	DRIVER( mplanets )	/* GV-102 (c) 1983 Gottlieb */
	DRIVER( mplanuk )	/* GV-102 (c) 1983 Gottlieb */
	DRIVER( insector )	/* GV-??? (c) 1982 Gottlieb - never released */
	DRIVER( tylz )	/* GV-??? (c) 1982 Gottlieb - never released */
	DRIVER( argusg )	/* GV-??? (c) 1982 Gottlieb - never released */
	DRIVER( krull )		/* GV-105 (c) 1983 Gottlieb */
	DRIVER( kngtmare )	/* GV-??? (c) 1983 Gottlieb - never released */
	DRIVER( mach3 )		/* GV-109 (c) 1983 Mylstar */
	DRIVER( usvsthem )	/* GV-??? (c) 198? Mylstar */
	DRIVER( 3stooges )	/* GV-113 (c) 1984 Mylstar */
	DRIVER( screwloo )	/* GV-123 (c) 1983 Mylstar - never released */
	DRIVER( curvebal )	/* GV-134 (c) 1984 Mylstar */
	DRIVER( wizwarz )	/* GV-??? (c) 1984 Mylstar - never released */

	/* Taito "Qix hardware" games */
	DRIVER( sdungeon )	/* SD  (c) 1981 Taito America Corporation */
	DRIVER( elecyoyo )	/* YY  (c) 1982 Taito America Corporation */
	DRIVER( elecyoy2 )	/* YY  (c) 1982 Taito America Corporation */
	DRIVER( kram )		/* KS  (c) 1982 Taito America Corporation */
	DRIVER( kram2 )		/* KS  (c) 1982 Taito America Corporation */
	DRIVER( kram3 )
	DRIVER( slither )	/* (c) 1982 Century II */
	DRIVER( slithera )	/* (c) 1982 Century II */
	DRIVER( complexx )	/* ??  (c) 1984 Taito America Corporation */

	/* Taito SJ System games */

	/* other Taito games */
	DRIVER( crbaloon )	/* CL  (c) 1980 Taito Corporation */
	DRIVER( crbalon2 )	/* CL  (c) 1980 Taito Corporation */
	DRIVER( grchamp )	/* GM  (c) 1981 Taito Corporation */
	DRIVER( bking )		/* DM  (c) 1982 Taito Corporation */
	DRIVER( bking2 )	/* AD6 (c) 1983 Taito Corporation */
	DRIVER( bking3 )	/* A24 (c) 1984 Taito Corporation */
	DRIVER( josvolly )	/* ??? (c) 1983 Taito Corporation */
	DRIVER( gsword )	/* ??? (c) 1984 Taito Corporation */
	DRIVER( lkage )		/* A54 (c) 1984 Taito Corporation */
	DRIVER( lkageb )	/* bootleg */
	DRIVER( lkageb2 )	/* bootleg */
	DRIVER( lkageb3 )	/* bootleg */
	DRIVER( msisaac )	/* A34 (c) 1985 Taito Corporation */
	DRIVER( fightrol )	/* (c) 1983 Taito */
	DRIVER( rollace )	/* (c) 1983 Williams */
	DRIVER( rollace2 )	/* (c) 1983 Williams */
	DRIVER( vsgongf )	/* (c) 1984 Kaneko */
	DRIVER( ringfgt )	/* (c) 1984 Taito */
	DRIVER( ringfgt2 )	/* (c) 1984 Taito */
	DRIVER( 40love )	/* A30 (c) 1984 Taito */
	DRIVER( tsamurai )	/* A35 (c) 1985 Taito */
	DRIVER( tsamura2 )	/* A35 (c) 1985 Taito */
	DRIVER( nunchaku )	/* ??? (c) 1985 Taito */
	DRIVER( yamagchi )	/* A38 (c) 1985 Taito */
	DRIVER( m660 )      /* ??? (c) 1986 Taito America Corporation */
	DRIVER( m660j )     /* ??? (c) 1986 Taito Corporation (Japan) */
	DRIVER( m660b )     /* bootleg */
	DRIVER( alphaxz )   /* ??? (c) 1986 Ed/Wood Place */
	DRIVER( buggychl )	/* A22 (c) 1984 Taito Corporation */
	DRIVER( buggycht )	/* A22 (c) 1984 Taito Corporation + Tefri license */
	DRIVER( ssrj )		/* A40 (c) 1985 Taito Corporation */
	DRIVER( bigevglf )	/* A67 (c) 1986 Taito America Corporation (US) */
	DRIVER( nycaptor )	/* A50 (c) 1985 Taito Corporation */
	DRIVER( cyclshtg )	/* A97 (c) 1986 Taito Corporation */
	DRIVER( bronx )	    /* 1986 bootleg */
	DRIVER( benberob )	/* A26 */
	DRIVER( halleys )	/* A62 (c) 1986 Taito America Corporation + Coin It (US) */
	DRIVER( halleysc )	/* A62 (c) 1986 Taito Corporation (Japan) */
	DRIVER( halleycj )	/* A62 (c) 1986 Taito Corporation (Japan) */
	DRIVER( tokio )		/* A71 1986 */
	DRIVER( tokiob )	/* bootleg */
	DRIVER( bublbobl )	/* A78 (c) 1986 Taito Corporation */
	DRIVER( bublbob1 )	/* A78 (c) 1986 Taito Corporation */
	DRIVER( bublbobr )	/* A78 (c) 1986 Taito America Corporation + Romstar license */
	DRIVER( bubbobr1 )	/* A78 (c) 1986 Taito America Corporation + Romstar license */
	DRIVER( bublboblp )     /* Prototype (c) 1986 Taito Corporation */
	DRIVER( boblbobl )	/* bootleg */
	DRIVER( sboblbob )	/* bootleg */
	DRIVER( bublboblu )	/* hack */
	DRIVER( bublcave )	/* hack */
	DRIVER( missb2 )	/* bootleg on enhanced hardware */
	DRIVER( darius )	/* A96 (c) 1986 Taito Corporation Japan (World) */
	DRIVER( dariusj )	/* A96 (c) 1986 Taito Corporation (Japan) */
	DRIVER( dariuso )	/* A96 (c) 1986 Taito Corporation (Japan) */
	DRIVER( dariuse )	/* A96 (c) 1986 Taito Corporation (Japan) */
	DRIVER( rastan )	/* B04 (c) 1987 Taito Corporation Japan (World) */
	DRIVER( rastanu )	/* B04 (c) 1987 Taito America Corporation (US) */
	DRIVER( rastanu2 )	/* B04 (c) 1987 Taito America Corporation (US) */
	DRIVER( rastsaga )	/* B04 (c) 1987 Taito Corporation (Japan)*/
	DRIVER( topspeed )	/* B14 (c) 1987 Taito Corporation Japan (World) */
	DRIVER( topspedu )	/* B14 (c) 1987 Taito America Corporation (US) */
	DRIVER( fullthrl )	/* B14 (c) 1987 Taito Corporation (Japan) */
	DRIVER( othunder )	/* B67 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( othundu )	/* B67 (c) 1988 Taito America Corporation (US) */
	DRIVER( rainbow )	/* B22 (c) 1987 Taito Corporation */
	DRIVER( rainbowo )	/* B22 (c) 1987 Taito Corporation */
	DRIVER( rainbowe )	/* B39 (c) 1988 Taito Corporation */
	DRIVER( jumping )	/* bootleg */
	DRIVER( arkanoid )	/* A75 (c) 1986 Taito Corporation Japan (World) */
	DRIVER( arknoidu )	/* A75 (c) 1986 Taito America Corporation + Romstar license (US) */
	DRIVER( arknoiuo )	/* A75 (c) 1986 Taito America Corporation + Romstar license (US) */
	DRIVER( arknoidj )	/* A75 (c) 1986 Taito Corporation (Japan) */
	DRIVER( arkbl2 )	/* bootleg */
	DRIVER( arkbl3 )	/* bootleg */
	DRIVER( paddle2 )	/* bootleg */
	DRIVER( arkatayt )	/* bootleg */
	DRIVER( arkblock )	/* bootleg */
	DRIVER( arkbloc2 )	/* bootleg */
	DRIVER( arkangc )	/* bootleg */
	DRIVER( arkatour )	/* ??? (c) 1987 Taito America Corporation + Romstar license (US) */
	DRIVER( exzisus )	/* B23 (c) 1987 Taito Corporation (Japan) */
	DRIVER( minivadr )	/* D26 cabinet test board */
	DRIVER( bonzeadv )	/* B41 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( bonzeadu )	/* B41 (c) 1988 Taito America Corporation (US) */
	DRIVER( jigkmgri )	/* B41 (c) 1988 Taito Corporation (Japan)*/
	DRIVER( asuka )		/* ??? (c) 1988 Taito Corporation (Japan) */
	DRIVER( mofflott )	/* C17 (c) 1989 Taito Corporation (Japan) */
	DRIVER( cadash )	/* C21 (c) 1989 Taito Corporation Japan */
	DRIVER( cadashj )	/* C21 (c) 1989 Taito Corporation */
	DRIVER( cadashu )	/* C21 (c) 1989 Taito America Corporation */
	DRIVER( cadashi )	/* C21 (c) 1989 Taito Corporation Japan */
	DRIVER( cadashf )	/* C21 (c) 1989 Taito Corporation Japan */
	DRIVER( galmedes )	/* (c) 1992 Visco (Japan) */
	DRIVER( earthjkr )	/* (c) 1993 Visco (Japan) */
	DRIVER( eto )		/* (c) 1994 Visco (Japan) */
	DRIVER( wgp )		/* C32 (c) 1989 Taito America Corporation (US) */
	DRIVER( wgpj )		/* C32 (c) 1989 Taito Corporation (Japan) */
	DRIVER( wgpjoy )	/* C32 (c) 1989 Taito Corporation (Japan) */
	DRIVER( wgpjoya )	/* C32 (c) 1989 Taito Corporation (Japan) */
	DRIVER( wgp2 )		/* C73 (c) 1990 Taito Corporation (Japan) */
	DRIVER( slapshot )	/* D71 (c) 1994 Taito Corporation (Japan) */
	DRIVER( opwolf3 )	/* D74 (c) 1994 Taito */
	DRIVER( opwolf3u )	/* D74 (c) 1994 Taito */

	/* Taito multi-screen games */
	DRIVER( ninjaw )	/* B31 (c) 1987 Taito Corporation Japan (World) */
	DRIVER( ninjawj )	/* B31 (c) 1987 Taito Corporation (Japan) */
	DRIVER( darius2 )	/* C07 (c) 1989 Taito Corporation (Japan) */
	DRIVER( darius2d )	/* C07 (c) 1989 Taito Corporation (Japan) */
	DRIVER( drius2do )	/* C07 (c) 1989 Taito Corporation (Japan) */
	DRIVER( warriorb )	/* D24 (c) 1991 Taito Corporation (Japan) */

	/* Taito "X"-system games */

	/* Taito "tnzs" hardware */

	/* Taito L-System games */
	DRIVER( raimais )	/* B36 (c) 1988 Taito Corporation (Japan) */
	DRIVER( kurikint )	/* B42 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( kurikinu )	/* B42 (c) 1988 Taito America Corporation (US) */
	DRIVER( kurikinj )	/* B42 (c) 1988 Taito Corporation (Japan) */
	DRIVER( kurikina )	/* B42 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( evilston )	/* C67 (c) 1990 Spacy Industrial, Ltd */
	DRIVER( fhawk )		/* B70 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( fhawkj )	/* B70 (c) 1988 Taito Corporation (Japan) */
	DRIVER( plotting )	/* B96 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( champwr )	/* C01 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( champwru )	/* C01 (c) 1989 Taito America Corporation (US) */
	DRIVER( champwrj )	/* C01 (c) 1989 Taito Corporation (Japan) */
	DRIVER( puzznic )	/* C20 (c) 1989 Taito Corporation (Japan) */
	DRIVER( horshoes )	/* C47 (c) 1990 Taito America Corporation (US) */
	DRIVER( palamed )	/* C63 (c) 1990 Taito Corporation (Japan) */
	DRIVER( cachat )	/* ??? (c) 1993 Taito Corporation (Japan) */
	DRIVER( tubeit )	/* ??? no copyright message */
	DRIVER( cubybop )	/* ??? no copyright message */
	DRIVER( plgirls )	/* (c) 1992 Hot-B. */
	DRIVER( plgirls2 )	/* (c) 1993 Hot-B. */

	/* Taito H-System games */
	DRIVER( syvalion )	/* B51 (c) 1988 Taito Corporation (Japan) */
	DRIVER( recordbr )	/* B56 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( dleague )	/* C02 (c) 1990 Taito Corporation (Japan) */

	/* Taito B-System games */
	DRIVER( masterw )	/* B72 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( nastar )	/* B81 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( nastarw )	/* B81 (c) 1988 Taito America Corporation (US) */
	DRIVER( rastsag2 )	/* B81 (c) 1988 Taito Corporation (Japan) */
	DRIVER( rambo3 )	/* B93 (c) 1989 Taito Europe Corporation (Europe) */
	DRIVER( rambo3ae )	/* B93 (c) 1989 Taito Europe Corporation (Europe) */
	DRIVER( rambo3a )	/* B93 (c) 1989 Taito America Corporation (US) */
	DRIVER( crimec )	/* B99 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( crimecu )	/* B99 (c) 1989 Taito America Corporation (US) */
	DRIVER( crimecj )	/* B99 (c) 1989 Taito Corporation (Japan) */
	DRIVER( tetrist )	/* C12 (c) 1989 Sega Enterprises,Ltd. (Japan) */
	DRIVER( viofight )	/* C16 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( ashura )	/* C43 (c) 1990 Taito Corporation (Japan) */
	DRIVER( ashurau )	/* C43 (c) 1990 Taito America Corporation (US) */
	DRIVER( hitice )	/* C59 (c) 1990 Williams (US) */
	DRIVER( sbm )		/* C69 (c) 1990 Taito Corporation (Japan) */
	DRIVER( selfeena )	/* ??? (c) 1991 East Technology */
	DRIVER( silentd )	/* ??? (c) 1992 Taito Corporation Japan (World) */
	DRIVER( silentdj )	/* ??? (c) 1992 Taito Corporation (Japan) */
	DRIVER( ryujin )	/* ??? (c) 1993 Taito Corporation (Japan) */
	DRIVER( qzshowby )	/* D72 (c) 1993 Taito Corporation (Japan) */
	DRIVER( pbobble )	/* ??? (c) 1994 Taito Corporation (Japan) */
	DRIVER( bublbust )	/* ??? (c) 1994 Taito Corporation (US) */
	DRIVER( spacedx )	/* D89 (c) 1994 Taito Corporation (US) */
	DRIVER( spacedxj )	/* D89 (c) 1994 Taito Corporation (Japan) */
	DRIVER( spacedxo )	/* D89 (c) 1994 Taito Corporation (Japan) */

	/* Taito Z-System games */
	DRIVER( contcirc )	/* B33 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( contcrcu )	/* B33 (c) 1987 Taito America Corporation (US) */
	DRIVER( chasehq )	/* B52 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( chasehqj )	/* B52 (c) 1988 Taito Corporation (Japan) */
	DRIVER( enforce )	/* B58 (c) 1988 Taito Corporation (Japan) */
	DRIVER( nightstr )	/* B91 (c) 1989 Taito America Corporation (US) */
	DRIVER( sci )		/* C09 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( scia )		/* C09 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( sciu )		/* C09 (c) 1989 Taito America Corporation (US) */
	DRIVER( bshark )	/* C34 (c) 1989 Taito America Corporation (US) */
	DRIVER( bsharkj )	/* C34 (c) 1989 Taito Corporation (Japan) */
	DRIVER( bsharkjjs )	/* C34 (c) 1989 Taito Corporation (Japan) */
	DRIVER( aquajack )	/* B77 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( aquajckj )	/* B77 (c) 1990 Taito Corporation (Japan) */
	DRIVER( dblaxle )	/* C78 (c) 1991 Taito America Corporation (US) */
	DRIVER( pwheelsj )	/* C78 (c) 1991 Taito Corporation (Japan) */
	DRIVER( racingb )	/* C84 (c) 1991 Taito Coropration (Japan) */

	/* Taito Air System games */
	DRIVER( topland )	/* B62 (c) 1988 Taito Coporation Japan (World) */
	DRIVER( ainferno )	/* C45 (c) 1990 Taito America Corporation (US) */

	/* enhanced Z-System hardware games */
	DRIVER( gunbustr )	/* D27 (c) 1992 Taito Corporation (Japan) */
	DRIVER( groundfx )	/* D51 (c) 1992 Taito Coporation */
	DRIVER( undrfire )	/* D67 (c) 1993 Taito Coporation Japan (World) */
	DRIVER( undrfiru )	/* D67 (c) 1993  Taito America Corporation (US) */
	DRIVER( undrfirj )	/* D67 (c) 1993 Taito Coporation (Japan) */
	DRIVER( cbombers )	/* D?? (c) 1994 Taito Coporation Japan (World) */
	DRIVER( cbombersj )	/* D?? (c) 1994 Taito Coporation (Japan) */
	DRIVER( cbombersp )	/* D?? (c) 1994 Taito Coporation (Japan Prototype) */

	/* Taito F2 games */
	DRIVER( finalb )	/* B82 (c) 1988 Taito Corporation Japan (World) */
	DRIVER( finalbj )	/* B82 (c) 1988 Taito Corporation (Japan) */
	DRIVER( dondokod )	/* B95 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( dondokdu )	/* B95 (c) 1989 Taito America Corporation (US) */
	DRIVER( dondokdj )	/* B95 (c) 1989 Taito Corporation (Japan) */
	DRIVER( megab )		/* C11 (c) 1989 Taito Corporation Japan (World) */
	DRIVER( megabj )	/* C11 (c) 1989 Taito Corporation (Japan) */
	DRIVER( thundfox )	/* C28 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( thndfoxu )	/* C28 (c) 1990 Taito America Corporation (US) */
	DRIVER( thndfoxj )	/* C28 (c) 1990 Taito Corporation (Japan) */
	DRIVER( cameltry )	/* C38 (c) 1989 Taito America Corporation (US) */
	DRIVER( camltrua )	/* C38 (c) 1989 Taito America Corporation (US) */
	DRIVER( cameltrj )	/* C38 (c) 1989 Taito Corporation (Japan) */
	DRIVER( qtorimon )	/* C41 (c) 1990 Taito Corporation (Japan) */
	DRIVER( liquidk )	/* C49 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( liquidku )	/* C49 (c) 1990 Taito America Corporation (US) */
	DRIVER( mizubaku )	/* C49 (c) 1990 Taito Corporation (Japan) */
	DRIVER( quizhq )	/* C53 (c) 1990 Taito Corporation (Japan) */
	DRIVER( ssi )		/* C64 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( majest12 )	/* C64 (c) 1990 Taito Corporation (Japan) */
	DRIVER( gunfront )	/* C71 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( gunfronj )	/* C71 (c) 1990 Taito Corporation (Japan) */
	DRIVER( growl )		/* C74 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( growlu )	/* C74 (c) 1990 Taito America Corporation (US) */
	DRIVER( runark )	/* C74 (c) 1990 Taito Corporation (Japan) */
	DRIVER( mjnquest )	/* C77 (c) 1990 Taito Corporation (Japan) */
	DRIVER( mjnquesb )	/* C77 (c) 1990 Taito Corporation (Japan) */
	DRIVER( footchmp )	/* C80 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( hthero )	/* C80 (c) 1990 Taito Corporation (Japan) */
	DRIVER( euroch92 )	/*     (c) 1992 Taito Corporation Japan (World) */
	DRIVER( koshien )	/* C81 (c) 1990 Taito Corporation (Japan) */
	DRIVER( yuyugogo )	/* C83 (c) 1990 Taito Corporation (Japan) */
	DRIVER( ninjak )	/* C85 (c) 1990 Taito Corporation Japan (World) */
	DRIVER( ninjakj )	/* C85 (c) 1990 Taito Corporation (Japan) */
	DRIVER( solfigtr )	/* C91 (c) 1991 Taito Corporation Japan (World) */
	DRIVER( qzquest )	/* C92 (c) 1991 Taito Corporation (Japan) */
	DRIVER( pulirula )	/* C98 (c) 1991 Taito Corporation Japan (World) */
	DRIVER( pulirulj )	/* C98 (c) 1991 Taito Corporation (Japan) */
	DRIVER( metalb )	/* D16? (c) 1991 Taito Corporation Japan (World) */
	DRIVER( metalbj )	/* D12 (c) 1991 Taito Corporation (Japan) */
	DRIVER( qzchikyu )	/* D19 (c) 1991 Taito Corporation (Japan) */
	DRIVER( yesnoj )	/* D20 (c) 1992 Taito Corporation (Japan) */
	DRIVER( deadconx )	/* D28 (c) 1992 Taito Corporation Japan (World) */
	DRIVER( deadconj )	/* D28 (c) 1992 Taito Corporation (Japan) */
	DRIVER( dinorex )	/* D39 (c) 1992 Taito Corporation Japan (World) */
	DRIVER( dinorexj )	/* D39 (c) 1992 Taito Corporation (Japan) */
	DRIVER( dinorexu )	/* D39 (c) 1992 Taito America Corporation (US) */
	DRIVER( qjinsei )	/* D48 (c) 1992 Taito Corporation (Japan) */
	DRIVER( qcrayon )	/* D55 (c) 1993 Taito Corporation (Japan) */
	DRIVER( qcrayon2 )	/* D63 (c) 1993 Taito Corporation (Japan) */
	DRIVER( driftout )	/* (c) 1991 Visco */
	DRIVER( driveout )	/* bootleg */

	/* Taito F3 games */

	/* Toaplan games */
	DRIVER( mjsister )	/* (c) 1986 Toaplan */

	DRIVER( fshark )	/* TP-007 (c) 1987 Taito Corporation (World) */
	DRIVER( skyshark )	/* TP-007 (c) 1987 Taito America Corporation + Romstar license (US) */
	DRIVER( hishouza )	/* TP-007 (c) 1987 Taito Corporation (Japan) */
	DRIVER( fsharkbt )	/* bootleg */
	DRIVER( wardner )	/* TP-009 (c) 1987 Taito Corporation Japan (World) */
	DRIVER( pyros )		/* TP-009 (c) 1987 Taito America Corporation (US) */
	DRIVER( wardnerj )	/* TP-009 (c) 1987 Taito Corporation (Japan) */
	DRIVER( twincobr )	/* TP-011 (c) 1987 Taito Corporation (World) */
	DRIVER( twincobu )	/* TP-011 (c) 1987 Taito America Corporation + Romstar license (US) */
	DRIVER( ktiger )	/* TP-011 (c) 1987 Taito Corporation (Japan) */
  DRIVER( ktiger2p )	/* (c) 2021 M2 Co Ltd */
	DRIVER( gulfwar2 )	/* (c) 1991 Comad */

	DRIVER( snowbros )	/* MIN16-02 (c) 1990 Toaplan + Romstar license */
	DRIVER( snowbroa )	/* MIN16-02 (c) 1990 Toaplan + Romstar license */
	DRIVER( snowbrob )	/* MIN16-02 (c) 1990 Toaplan + Romstar license */
	DRIVER( snowbroj )	/* MIN16-02 (c) 1990 Toaplan */
	DRIVER( wintbob )	/* bootleg */

	/* SemiCom games */
	DRIVER( hyperpac )	/* (c) 1995 SemiCom */
	DRIVER( hyperpcb )	/* bootleg */
	DRIVER( cookbib2 )  /* (c) 1996 SemiCom */
	DRIVER( toppyrap )	/* (c) 1996 SemiCom */
	DRIVER( cookbib3 )  /* (c) 1997 SemiCom */
	DRIVER( pzlbreak )	/* (c) 1997 SemiCom */
	DRIVER( twinkle )	/* (c) 1997 SemiCom */
	DRIVER( 3in1semi )  /* (c) 1998 SemiCom */
	DRIVER( moremore )  /* (c) 1999 SemiCom / Exit */
	DRIVER( moremorp )  /* (c) 1999 SemiCom / Exit */

	/* Cloned snow bros hardware */
	DRIVER( finalttr )  /* (c) 1993 Jeil */
	DRIVER( toto )	    /* (c) 1996 SoftClub */
	DRIVER( 4in1boot )	/* (c) 1999 K1 Soft */
	DRIVER( snowbro3 )  /* (c) 2002 Syrmex */


	/* More Toaplan Games */
	/* see http://www.vsa-ag.ch/r8zing/ for a list of Raizing/8ing games */

/*
Toa Plan's board list
(translated from http://www.aianet.ne.jp/~eisetu/rom/rom_toha.html)

Title              ROMno.   Remark(1)   Remark(2)
--------------------------------------------------
Tiger Heli           A47      GX-551
Hishouzame           B02      TP-007
Kyukyoku Tiger       B30      TP-011
Dash Yarou           B45      TP-012
Tatsujin             B65      TP-013B   M6100649A
Zero Wing            O15      TP-015
Horror Story         O16      TP-016
Same!Same!Same!      O17      TP-017
Out Zone                      TP-018
Vimana                        TP-019
Teki Paki            O20      TP-020
Ghox               TP-21      TP-021
Dogyuun                       TP-022
Tatsujin Oh                   TP-024    *1
Fixeight                      TP-026
V-V                           TP-027

*1 There is a doubt this game uses TP-024 board and TP-025 romsets.

   86 Mahjong Sisters                                 Kit 2P 8W+2B     HC    Mahjong TP-
   88 Dash                                            Kit 2P 8W+2B                   TP-
   89 Fire Shark                                      Kit 2P 8W+2B     VC    Shooter TP-017
   89 Twin Hawk                                       Kit 2P 8W+2B     VC    Shooter TP-
   91 Whoopie                                         Kit 2P 8W+2B     HC    Action
   92 Teki Paki                                       Kit 2P                         TP-020
   92 Ghox                                            Kit 2P Paddle+1B VC    Action  TP-021
10/92 Dogyuun                                         Kit 2P 8W+2B     VC    Shooter TP-022
92/93 Knuckle Bash                 Atari Games        Kit 2P 8W+2B     HC    Action  TP-023
10/92 Tatsujin II/Truxton II       Taito              Kit 2P 8W+2B     VC    Shooter TP-024
10/92 Truxton II/Tatsujin II       Taito              Kit 2P 8W+2B     VC    Shooter TP-024
      Pipi & Bipi                                                                    TP-025
   92 Fix Eight                                       Kit 2P 8W+2B     VC    Action  TP-026
12/92 V  -  V (5)/Grind Stormer                       Kit 2P 8W+2B     VC    Shooter TP-027
 1/93 Grind Stormer/V - V (Five)                      Kit 2P 8W+2B     VC    Shooter TP-027
 2/94 Batsugun                                        Kit 2P 8W+2B     VC            TP-
 4/94 Snow Bros. 2                                    Kit 2P 8W+2B     HC    Action  TP-
*/

	/* Cave games */
	/* Cave was formed in 1994 from the ruins of Toaplan, like Raizing was. */
	DRIVER( pwrinst2 )	/* (c) 1994 Atlus */
	DRIVER( plegends )	/* (c) 1995 Atlus */
	DRIVER( mazinger )	/* (c) 1994 Banpresto (country is in EEPROM) */
	DRIVER( donpachi )	/* (c) 1995 Atlus/Cave */
	DRIVER( donpachj )	/* (c) 1995 Atlus/Cave */
	DRIVER( donpachk )	/* (c) 1995 Atlus/Cave */
	DRIVER( metmqstr )	/* (c) 1995 Banpresto / Pandorabox */
	DRIVER( nmaster )	/* (c) 1995 Banpresto / Pandorabox */
	DRIVER( sailormn )	/* (c) 1995 Banpresto (country is in EEPROM) */
	DRIVER( sailormo )	/* (c) 1995 Banpresto (country is in EEPROM) */
	DRIVER( agallet )	/* (c) 1996 Banpresto / Gazelle (country is in EEPROM) */
	DRIVER( hotdogst )	/* (c) 1996 Marble */
	DRIVER( ddonpach )	/* (c) 1997 Atlus/Cave */
	DRIVER( ddonpchj )	/* (c) 1997 Atlus/Cave */
	DRIVER( ddonpacha )	/* (c) 2012 Trap15 Hack */
	DRIVER( dfeveron )	/* (c) 1998 Cave + Nihon System license */
	DRIVER( feversos )	/* (c) 1998 Cave + Nihon System license */
	DRIVER( esprade )	/* (c) 1998 Atlus/Cave */
	DRIVER( espradej )	/* (c) 1998 Atlus/Cave (Japan) */
	DRIVER( espradeo )	/* (c) 1998 Atlus/Cave (Japan) */
	DRIVER( uopoko )	/* (c) 1998 Cave + Jaleco license */
	DRIVER( guwange )	/* (c) 1999 Atlus/Cave */
	DRIVER( guwanges )	/* (c) 2000 Atlus/Cave */
	DRIVER( gaia )		/* (c) 1999 Noise Factory */
	DRIVER( theroes )	/* (c) 2001 Primetek Investmants */

	/* Kyugo games */
	/* Kyugo only made four games: Repulse, Flash Gal, SRD Mission and Air Wolf. */
	/* Gyrodine was made by Crux. Crux was antecedent of Toa Plan, and spin-off from Orca. */
	DRIVER( gyrodine )	/* (c) 1984 Taito Corporation */
	DRIVER( sonofphx )	/* (c) 1985 Associated Overseas MFR */
	DRIVER( repulse )	/* (c) 1985 Sega */
	DRIVER( 99lstwar )	/* (c) 1985 Proma */
	DRIVER( 99lstwra )	/* (c) 1985 Proma */
	DRIVER( flashgal )	/* (c) 1985 Sega */
	DRIVER( srdmissn )	/* (c) 1986 Taito Corporation */
	DRIVER( legend )	/* no copyright notice [1986 Sega/Coreland?] */
	DRIVER( airwolf )	/* (c) 1987 Kyugo */
	DRIVER( skywolf )	/* bootleg */
	DRIVER( skywolf2 )	/* bootleg */

	/* Williams games */
	DRIVER( defndjeu )	/* bootleg */
	DRIVER( mayday )
	DRIVER( maydaya )
	DRIVER( maydayb )
	DRIVER( colony7 )	/* (c) 1981 Taito */
	DRIVER( colony7a )	/* (c) 1981 Taito */
	DRIVER( stargate )	/* (c) 1981 */
	DRIVER( bubbles )	/* (c) 1982 */
	DRIVER( bubblesr )	/* (c) 1982 */
	DRIVER( bubblesp )	/* (c) 1982 */
	DRIVER( splat )		/* (c) 1982 */
	DRIVER( playball )	/* (c) 1983 */
	DRIVER( blaster )	/* (c) 1983 */
	DRIVER( blastkit )	/* (c) 1983 */
	DRIVER( spdball )	/* (c) 1985 */
	DRIVER( alienar )   /* (c) 1985 */
	DRIVER( alienaru )  /* (c) 1985 */
	DRIVER( mysticm )	/* (c) 1983 */
	DRIVER( tshoot )	/* (c) 1984 */
	DRIVER( lottofun )	/* (c) 1987 H.A.R. Management */

	/* Capcom games */
	/* The following is a COMPLETE list of the Capcom games up to 1997, as shown on */
	/* their web site. The list is sorted by production date.                       */
	/* A comprehensive list of Capcom games with board info can be found here:      */
	/* http://www.arcadeflyers.com/strider/capcom_list.html                         */
	DRIVER( vulgus )	/*  5/1984 (c) 1984 */
	DRIVER( vulgus2 )	/*  5/1984 (c) 1984 */
	DRIVER( vulgusj )	/*  5/1984 (c) 1984 */
	DRIVER( sonson )	/*  7/1984 (c) 1984 */
	DRIVER( sonsonj )	/*  7/1984 (c) 1984 (Japan) */
	DRIVER( higemaru )	/*  9/1984 (c) 1984 */
	DRIVER( 1942 )		/* 12/1984 (c) 1984 */
	DRIVER( 1942a )		/* 12/1984 (c) 1984 */
	DRIVER( 1942b )		/* 12/1984 (c) 1984 */
	DRIVER( exedexes )	/*  2/1985 (c) 1985 */
	DRIVER( savgbees )	/*  2/1985 (c) 1985 + Memetron license */
	DRIVER( commando )	/*  5/1985 (c) 1985 (World) */
	DRIVER( commandu )	/*  5/1985 (c) 1985 + Data East license (US) */
	DRIVER( commandj )	/*  5/1985 (c) 1985 (Japan) */
	DRIVER( sinvasn )	/* Europe original? */
	DRIVER( sinvasnb )	/* bootleg */
	DRIVER( gng )		/*  9/1985 (c) 1985 */
	DRIVER( gnga )		/*  9/1985 (c) 1985 */
	DRIVER( gngt )		/*  9/1985 (c) 1985 */
	DRIVER( makaimur )	/*  9/1985 (c) 1985 */
	DRIVER( makaimuc )	/*  9/1985 (c) 1985 */
	DRIVER( makaimug )	/*  9/1985 (c) 1985 */
	DRIVER( diamond )	/* (c) 1989 KH Video (NOT A CAPCOM GAME but runs on GnG hardware) */
	DRIVER( gngknight )		/*  Hack */
	DRIVER( gngknighta )		/*  Hack */
	DRIVER( gngknightb )		/*  Hack */
	DRIVER( gngknightc )		/*  Hack */
	DRIVER( gngknightt )		/*  Hack */
	DRIVER( gngmaiden )		/*  Hack */
	DRIVER( gngmaidena )		/*  Hack */
	DRIVER( gngmaidenb )		/*  Hack */
	DRIVER( gngmaidenc )		/*  Hack */
	DRIVER( gngmaident )		/*  Hack */
	DRIVER( makknight )		/*  Hack */
	DRIVER( makknightb )		/*  Hack */
	DRIVER( makknightc )		/*  Hack */
	DRIVER( makknightg )		/*  Hack */
	DRIVER( makmaiden )		/*  Hack */
	DRIVER( makmaidenb )		/*  Hack */
	DRIVER( makmaidenc )		/*  Hack */
	DRIVER( makmaideng )		/*  Hack */
	DRIVER( gunsmoke )	/* 11/1985 (c) 1985 (World) */
	DRIVER( gunsmoku )	/* 11/1985 (c) 1985 + Romstar (US) */
	DRIVER( gunsmoka )	/* 11/1985 (c) 1985 (US) */
	DRIVER( gunsmokj )	/* 11/1985 (c) 1985 (Japan) */
	DRIVER( sectionz )	/* 12/1985 (c) 1985 */
	DRIVER( sctionza )	/* 12/1985 (c) 1985 */
	DRIVER( srumbler )	/*  9/1986 (c) 1986 */
	DRIVER( srumblr2 )	/*  9/1986 (c) 1986 */
	DRIVER( rushcrsh )	/*  9/1986 (c) 1986 */
	DRIVER( lwings )	/* 11/1986 (c) 1986 */
	DRIVER( lwings2 )	/* 11/1986 (c) 1986 */
	DRIVER( lwingsjp )	/* 11/1986 (c) 1986 */
	DRIVER( sidearms )	/* 12/1986 (c) 1986 (World) */
	DRIVER( sidearmr )	/* 12/1986 (c) 1986 + Romstar license (US) */
	DRIVER( sidearjp )	/* 12/1986 (c) 1986 (Japan) */
	DRIVER( turtship )	/* (c) 1988 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware) */
	DRIVER( dyger )		/* (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware) */
	DRIVER( dygera )	/* (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware) */
	DRIVER( whizz )		/* (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware) */
	DRIVER( avengers )	/*  2/1987 (c) 1987 (US) */
	DRIVER( avenger2 )	/*  2/1987 (c) 1987 (US) */
	DRIVER( buraiken )	/*  2/1987 (c) 1987 (Japan) */
	DRIVER( bionicc )	/*  3/1987 (c) 1987 (US) */
	DRIVER( bionicc2 )	/*  3/1987 (c) 1987 (US) */
	DRIVER( topsecrt )	/*  3/1987 (c) 1987 (Japan) */
	DRIVER( 1943 )		/*  6/1987 (c) 1987 (US) */
	DRIVER( 1943j )		/*  6/1987 (c) 1987 (Japan) */
	DRIVER( 1943mii )       /*  6/1987 (c) 1987 (US) */
	DRIVER( blktiger )	/*  8/1987 (c) 1987 (US) */
	DRIVER( bktigerb )	/* bootleg */
	DRIVER( blkdrgon )	/*  8/1987 (c) 1987 (Japan) */
	DRIVER( blkdrgonb )	/* bootleg, hacked to say Black Tiger */
	DRIVER( 1943kai )	/*  6/1988 (c) 1987 (Japan) */
	DRIVER( lastduel )	/*  7/1988 (c) 1988 (US) */
	DRIVER( lstduela )	/*  7/1988 (c) 1988 (US) */
	DRIVER( lstduelb )	/* bootleg */
	DRIVER( madgear )	/*  2/1989 (c) 1989 (US) */
	DRIVER( madgearj )	/*  2/1989 (c) 1989 (Japan) */
	DRIVER( ledstorm )	/*  2/1989 (c) 1989 (US) */
	DRIVER( leds2011 )  /*  5/1988 (c) 1988 (World) */
	/*  3/1989 Dokaben (baseball) - see below among "Mitchell" games */
	/*  8/1989 Dokaben 2 (baseball) - see below among "Mitchell" games */
	/* 10/1989 Capcom Baseball - see below among "Mitchell" games */
	/* 11/1989 Capcom World - see below among "Mitchell" games */
	/*  3/1990 Adventure Quiz 2 Hatena no Dai-Bouken - see below among "Mitchell" games */
	/*  1/1991 Quiz Tonosama no Yabou - see below among "Mitchell" games */
	/*  4/1991 Ashita Tenki ni Naare (golf) - see below among "Mitchell" games */
	/*  5/1991 Ataxx - see below among "Leland" games */
	/*  6/1991 Quiz Sangokushi - see below among "Mitchell" games */
	/* 10/1991 Block Block - see below among "Mitchell" games */
	/*  6/1995 Street Fighter - the Movie - see below among "Incredible Technologies" games */


	/* Capcom CPS1 games */
	DRIVER( forgottn )	/*  7/1988 (c) 1988 (US) */
	DRIVER( lostwrld )	/*  7/1988 (c) 1988 (Japan) */
	DRIVER( ghouls )	/* 12/1988 (c) 1988 (World) */
	DRIVER( ghoulsu )	/* 12/1988 (c) 1988 (US) */
	DRIVER( daimakai )	/* 12/1988 (c) 1988 (Japan) */
	DRIVER( strider )	/*  3/1989 (c) 1989 (not explicitly stated but should be US) */
	DRIVER( stridrua )	/*  3/1989 (c) 1989 (not explicitly stated but should be US) */
	DRIVER( striderj )	/*  3/1989 (c) 1989 */
	DRIVER( stridrja )	/*  3/1989 (c) 1989 */
	DRIVER( dw )		/*  4/1989 (c) 1989 (World) */
	DRIVER( dwj )		/*  4/1989 (c) 1989 (Japan) */
	DRIVER( willow )	/*  6/1989 (c) 1989 (US) */
	DRIVER( willowj )	/*  6/1989 (c) 1989 (Japan) */
	DRIVER( willowje )	/*  6/1989 (c) 1989 (Japan) */
	DRIVER( unsquad )	/*  8/1989 (c) 1989 */
	DRIVER( area88 )	/*  8/1989 (c) 1989 */
	DRIVER( ffight )	/* 12/1989 (c) (World) */
	DRIVER( ffightu )	/* 12/1989 (c) (US)    */
	DRIVER( ffightj )	/* 12/1989 (c) (Japan) */
	DRIVER( ffightj1 )	/* 12/1989 (c) (Japan) */
	DRIVER( ffightae )  /* 12/1989 (c) 1989 (World) 3P */
	DRIVER( 1941 )		/*  2/1990 (c) 1990 (World) */
	DRIVER( 1941j )		/*  2/1990 (c) 1990 (Japan) */
	DRIVER( mercs )		/* 02/03/1990 (c) 1990 (World) */
	DRIVER( mercsu )	/* 02/03/1990 (c) 1990 (US)    */
	DRIVER( mercsua )	/* 08/06/1990 (c) 1990 (US)    */
	DRIVER( mercsj )	/* 02/03/1990 (c) 1990 (Japan) */
	DRIVER( mtwins )	/* 19/06/1990 (c) 1990 (World) */
	DRIVER( chikij )	/* 19/06/1990 (c) 1990 (Japan) */
	DRIVER( msword )	/* 25/07/1990 (c) 1990 (World) */
	DRIVER( mswordr1 )	/* 23/06/1990 (c) 1990 (World) */
	DRIVER( mswordu )	/* 25/07/1990 (c) 1990 (US)    */
	DRIVER( mswordj )	/* 23/06/1990 (c) 1990 (Japan) */
	DRIVER( cawing )	/* 12/10/1990 (c) 1990 (World) */
	DRIVER( cawingu )	/* 12/10/1990 (c) 1990 (US) */
	DRIVER( cawingj )	/* 12/10/1990 (c) 1990 (Japan) */
	DRIVER( nemo )		/* 30/11/1990 (c) 1990 (World) */
	DRIVER( nemoj )		/* 20/11/1990 (c) 1990 (Japan) */
	DRIVER( 3wonders )	/* 20/05/1991 (c) 1991 (World) */
	DRIVER( 3wonderu )	/* 20/05/1991 (c) 1991 (US)    */
	DRIVER( wonder3 )	/* 20/05/1991 (c) 1991 (Japan) */
	DRIVER( kod )		/* 11/07/1991 (c) 1991 (World) */
	DRIVER( kodu )		/* 10/09/1991 (c) 1991 (US)    */
	DRIVER( kodj )		/* 05/08/1991 (c) 1991 (Japan) */
	DRIVER( kodb )		/* bootleg */
	DRIVER( captcomm )	/* 14/10/1991 (c) 1991 (World) */
	DRIVER( captcomu )	/* 28/ 9/1991 (c) 1991 (US)    */
	DRIVER( captcomj )	/* 02/12/1991 (c) 1991 (Japan) */
	DRIVER( knights )	/* 27/11/1991 (c) 1991 (World) */
	DRIVER( knightsu )	/* 27/11/1991 (c) 1991 (US)    */
	DRIVER( knightsj )	/* 27/11/1991 (c) 1991 (Japan) */
	DRIVER( varth )		/* 12/06/1992 (c) 1992 (World) */
	DRIVER( varthu )	/* 12/06/1992 (c) 1992 (US) */
	DRIVER( varthj )	/* 14/07/1992 (c) 1992 (Japan) */
	DRIVER( cworld2j )	/* 11/06/1992 (QUIZ 5) (c) 1992 (Japan) */
	DRIVER( wof )		/* 02/10/1992 (c) 1992 (World) (CPS1 + QSound) */
	DRIVER( wofa )		/* 05/10/1992 (c) 1992 (Asia)  (CPS1 + QSound) */
	DRIVER( wofu )		/* 31/10/1992 (c) 1992 (US) (CPS1 + QSound) */
	DRIVER( wofj )		/* 31/10/1992 (c) 1992 (Japan) (CPS1 + QSound) */
	DRIVER( sf2t )		/* 09/12/1992 (c) 1992 (US)    */
	DRIVER( sf2tj )		/* 09/12/1992 (c) 1992 (Japan) */
	DRIVER( dino )		/* 01/02/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( dinou )		/* 01/02/1993 (c) 1993 (US)    (CPS1 + QSound) */
	DRIVER( dinoj )		/* 01/02/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( punisher )	/* 22/04/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( punishru )	/* 22/04/1993 (c) 1993 (US)    (CPS1 + QSound) */
	DRIVER( punishrj )	/* 22/04/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( slammast )	/* 13/07/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( slammasu )	/* 13/07/1993 (c) 1993 (US)    (CPS1 + QSound) */
	DRIVER( mbomberj )	/* 13/07/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( mbombrd )	/* 06/12/1993 (c) 1993 (World) (CPS1 + QSound) */
	DRIVER( mbombrdj )	/* 06/12/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	DRIVER( pnickj )	/* 08/06/1994 (c) 1994 Compile + Capcom license (Japan) not listed on Capcom's site */
	DRIVER( qad )		/* 01/07/1992 (c) 1992 (US)    */
	DRIVER( qadj )		/* 21/09/1994 (c) 1994 (Japan) */
	DRIVER( qtono2 )	/* 23/01/1995 (c) 1995 (Japan) */
	DRIVER( pang3 )		/* 11/05/1995 (c) 1995 Mitchell (Euro) not listed on Capcom's site */
	DRIVER( pang3j )	/* 11/05/1995 (c) 1995 Mitchell (Japan) not listed on Capcom's site */
	DRIVER( megaman )	/* 06/10/1995 (c) 1995 (Asia)  */
	DRIVER( rockmanj )	/* 22/09/1995 (c) 1995 (Japan) */

  /* CPS Prototype */
	DRIVER( gulunpa )   /* (c) 1993 (prototype) */

  /* Capcom CPS Changer */
	DRIVER( wofch )		/* 31/10/1992 (c) 1992 (Japan) (CPS1 + QSound) */
	/* Capcom CPS2 games */

	DRIVER( ssf2 )       /* Capcom, Super Street Fighter II: The New Challengers (World 931005) */
	DRIVER( ssf2r1 )     /* Capcom, Super Street Fighter II: The New Challengers (World 930911) */
	DRIVER( ssf2u )      /* Capcom, Super Street Fighter II: The New Challengers (USA 930911) */
	DRIVER( ssf2us2 )    /* bootleg,Super Street Fighter II: The New Challengers Super 2 (USA 930911) */
	DRIVER( ssf2a )      /* Capcom, Super Street Fighter II: The New Challengers (Asia 931005) */
	DRIVER( ssf2ar1 )    /* Capcom, Super Street Fighter II: The New Challengers (Asia 930914) */
	DRIVER( ssf2j )      /* Capcom, Super Street Fighter II: The New Challengers (Japan 931005) */
	DRIVER( ssf2jr1 )    /* Capcom, Super Street Fighter II: The New Challengers (Japan 930911) */
	DRIVER( ssf2jr2 )    /* Capcom, Super Street Fighter II: The New Challengers (Japan 930910) */
	DRIVER( ssf2h )      /* Capcom, Super Street Fighter II: The New Challengers (Hispanic 930911) */
	DRIVER( ssf2tb )     /* Capcom, Super Street Fighter II: The Tournament Battle (World 931119) */
	DRIVER( ssf2tbr1 )   /* Capcom, Super Street Fighter II: The Tournament Battle (World 930911) */
	DRIVER( ssf2tbu )    /* Capcom, Super Street Fighter II: The Tournament Battle (USA 930911) */
	DRIVER( ssf2tbj )    /* Capcom, Super Street Fighter II: The Tournament Battle (Japan 931005) */
	DRIVER( ssf2tbj1 )   /* Capcom, Super Street Fighter II: The Tournament Battle (Japan 930911) */
	DRIVER( ssf2tba )    /* Capcom, Super Street Fighter II: The Tournament Battle (Asia 931005) */
	DRIVER( ssf2tbh )    /* Capcom, Super Street Fighter II: The Tournament Battle (Hispanic 931005) */
	DRIVER( ecofghtr )   /* Capcom, Eco Fighters (World 931203) */
	DRIVER( ecofghtru )  /* Capcom, Eco Fighters (USA 940215) */
	DRIVER( ecofghtru1 ) /* Capcom, Eco Fighters (USA 931203) */
	DRIVER( uecology )   /* Capcom, Ultimate Ecology (Japan 931203) */
	DRIVER( ecofghtra )  /* Capcom, Eco Fighters (Asia 931203) */
	DRIVER( ecofghtrh )  /* Capcom, Eco Fighters (Hispanic 931203) */
	DRIVER( ddtod )      /* Capcom, Dungeons & Dragons: Tower of Doom (Euro 940412) */
	DRIVER( ddtodr1 )    /* Capcom, Dungeons & Dragons: Tower of Doom (Euro 940113) */
	DRIVER( ddtodu )     /* Capcom, Dungeons & Dragons: Tower of Doom (USA 940125) */
	DRIVER( ddtodur1 )   /* Capcom, Dungeons & Dragons: Tower of Doom (USA 940113) */
	DRIVER( ddtodj )     /* Capcom, Dungeons & Dragons: Tower of Doom (Japan 940412) */
	DRIVER( ddtodjr1 )   /* Capcom, Dungeons & Dragons: Tower of Doom (Japan 940125) */
	DRIVER( ddtodjr2 )   /* Capcom, Dungeons & Dragons: Tower of Doom (Japan 940113) */
	DRIVER( ddtoda )     /* Capcom, Dungeons & Dragons: Tower of Doom (Asia 940412) */
	DRIVER( ddtodar1 )   /* Capcom, Dungeons & Dragons: Tower of Doom (Asia 940113) */
	DRIVER( ddtodh )     /* Capcom, Dungeons & Dragons: Tower of Doom (Hispanic 940412) */
	DRIVER( ddtodhr1 )   /* Capcom, Dungeons & Dragons: Tower of Doom (Hispanic 940125) */
	DRIVER( ddtodhr2 )   /* Capcom, Dungeons & Dragons: Tower of Doom (Hispanic 940113) */
	DRIVER( ssf2t )      /* Capcom, Super Street Fighter II Turbo (World 940223) */
	DRIVER( ssf2ta )     /* Capcom, Super Street Fighter II Turbo (Asia 940223) */
	DRIVER( ssf2th )     /* Capcom, Super Street Fighter II Turbo (Hispanic 940223) */
	DRIVER( ssf2tu )     /* Capcom, Super Street Fighter II Turbo (USA 940323) */
	DRIVER( ssf2tur1 )   /* Capcom, Super Street Fighter II Turbo (USA 940223) */
	DRIVER( ssf2xj )     /* Capcom, Super Street Fighter II X: Grand Master Challenge (Japan 940311) */
	DRIVER( ssf2xjr1 )   /* Capcom, Super Street Fighter II X: Grand Master Challenge (Japan 940223) */
	DRIVER( ssf2xjr1r )  /* Capcom, Super Street Fighter II X: Grand Master Challenge (Japan 940223 rent version) */
	DRIVER( avsp )       /* Capcom, Alien vs. Predator (Euro 940520) */
	DRIVER( avspu )      /* Capcom, Alien vs. Predator (USA 940520) */
	DRIVER( avspj )      /* Capcom, Alien vs. Predator (Japan 940520) */
	DRIVER( avspa )      /* Capcom, Alien vs. Predator (Asia 940520) */
	DRIVER( avsph )      /* Capcom, Alien vs. Predator (Hispanic 940520) */
	DRIVER( dstlk )      /* Capcom, Darkstalkers: The Night Warriors (Euro 940705) */
	DRIVER( dstlku )     /* Capcom, Darkstalkers: The Night Warriors (USA 940818) */
	DRIVER( dstlkur1 )   /* Capcom, Darkstalkers: The Night Warriors (USA 940705) */
	DRIVER( dstlka )     /* Capcom, Darkstalkers: The Night Warriors (Asia 940705) */
	DRIVER( dstlkh )     /* Capcom, Darkstalkers: The Night Warriors (Hispanic 940818) */
	DRIVER( vampj )      /* Capcom, Vampire: The Night Warriors (Japan 940705) */
	DRIVER( vampja )     /* Capcom, Vampire: The Night Warriors (Japan 940705 alt) */
	DRIVER( vampjr1 )    /* Capcom, Vampire: The Night Warriors (Japan 940630) */
	DRIVER( ringdest )   /* Capcom, Ring of Destruction: Slammasters II (Euro 940902) */
	DRIVER( ringdesta )  /* Capcom, Ring of Destruction: Slammasters II (Asia 940831) */
	DRIVER( ringdesth )  /* Capcom, Ring of Destruction: Slammasters II (Hispanic 940902) */
	DRIVER( ringdestb )  /* Capcom, Ring of Destruction: Slammasters II (Brazil 940902) */
	DRIVER( smbomb )     /* Capcom, Super Muscle Bomber: The International Blowout (Japan 940831) */
	DRIVER( smbombr1 )   /* Capcom, Super Muscle Bomber: The International Blowout (Japan 940808) */
	DRIVER( armwar )     /* Capcom, Armored Warriors (Euro 941024) */
	DRIVER( armwarr1 )   /* Capcom, Armored Warriors (Euro 941011) */
	DRIVER( armwaru )    /* Capcom, Armored Warriors (USA 941024) */
	DRIVER( armwaru1 )   /* Capcom, Armored Warriors (USA 940920) */
	DRIVER( armwarb )    /* Capcom, Armored Warriors (Brazil 941024) */
	DRIVER( pgear )      /* Capcom, Powered Gear: Strategic Variant Armor Equipment (Japan 941024) */
	DRIVER( pgearr1 )    /* Capcom, Powered Gear: Strategic Variant Armor Equipment (Japan 940916) */
	DRIVER( armwara )    /* Capcom, Armored Warriors (Asia 941024) */
	DRIVER( armwarar1 )  /* Capcom, Armored Warriors (Asia 940920) */
	DRIVER( xmcota )     /* Capcom, X-Men: Children of the Atom (Euro 950331) */
	DRIVER( xmcotar1 )   /* Capcom, X-Men: Children of the Atom (Euro 950105) */
	DRIVER( xmcotau )    /* Capcom, X-Men: Children of the Atom (USA 950105) */
	DRIVER( xmcotab )    /* Capcom, X-Men: Children of the Atom (Brazil 950331) */
	DRIVER( xmcotah )    /* Capcom, X-Men: Children of the Atom (Hispanic 950331) */
	DRIVER( xmcotahr1 )  /* Capcom, X-Men: Children of the Atom (Hispanic 950105) */
	DRIVER( xmcotaj )    /* Capcom, X-Men: Children of the Atom (Japan 950105) */
	DRIVER( xmcotaj1 )   /* Capcom, X-Men: Children of the Atom (Japan 941222) */
	DRIVER( xmcotaj2 )   /* Capcom, X-Men: Children of the Atom (Japan 941219) */
	DRIVER( xmcotaj3 )   /* Capcom, X-Men: Children of the Atom (Japan 941217) */
	DRIVER( xmcotajr )   /* Capcom, X-Men: Children of the Atom (Japan 941208 rent version) */
	DRIVER( xmcotaa )    /* Capcom, X-Men: Children of the Atom (Asia 950105) */
	DRIVER( xmcotaar1 )  /* Capcom, X-Men: Children of the Atom (Asia 941219) */
	DRIVER( xmcotaar2 )  /* Capcom, X-Men: Children of the Atom (Asia 941217) */
	DRIVER( nwarr )      /* Capcom, Night Warriors: Darkstalkers' Revenge (Euro 950316) */
	DRIVER( nwarru )     /* Capcom, Night Warriors: Darkstalkers' Revenge (USA 950406) */
	DRIVER( nwarrh )     /* Capcom, Night Warriors: Darkstalkers' Revenge (Hispanic 950403) */
	DRIVER( nwarrb )     /* Capcom, Night Warriors: Darkstalkers' Revenge (Brazil 950403) */
	DRIVER( nwarra )     /* Capcom, Night Warriors: Darkstalkers' Revenge (Asia 950302) */
	DRIVER( vhuntj )     /* Capcom, Vampire Hunter: Darkstalkers' Revenge (Japan 950316) */
	DRIVER( vhuntjr1s )  /* Capcom, Vampire Hunter: Darkstalkers' Revenge (Japan 950307 stop version) */
	DRIVER( vhuntjr1 )   /* Capcom, Vampire Hunter: Darkstalkers' Revenge (Japan 950307) */
	DRIVER( vhuntjr2 )   /* Capcom, Vampire Hunter: Darkstalkers' Revenge (Japan 950302) */
	DRIVER( cybots )     /* Capcom, Cyberbots: Fullmetal Madness (Euro 950424) */
	DRIVER( cybotsu )    /* Capcom, Cyberbots: Fullmetal Madness (USA 950424) */
	DRIVER( cybotsj )    /* Capcom, Cyberbots: Fullmetal Madness (Japan 950420) */
	DRIVER( sfa )        /* Capcom, Street Fighter Alpha: Warriors' Dreams (Euro 950727) */
	DRIVER( sfar1 )      /* Capcom, Street Fighter Alpha: Warriors' Dreams (Euro 950718) */
	DRIVER( sfar2 )      /* Capcom, Street Fighter Alpha: Warriors' Dreams (Euro 950627) */
	DRIVER( sfar3 )      /* Capcom, Street Fighter Alpha: Warriors' Dreams (Euro 950605) */
	DRIVER( sfau )       /* Capcom, Street Fighter Alpha: Warriors' Dreams (USA 950627) */
	DRIVER( sfza )       /* Capcom, Street Fighter Zero (Asia 950627) */
	DRIVER( sfzar1 )     /* Capcom, Street Fighter Zero (Asia 950605) */
	DRIVER( sfzj )       /* Capcom, Street Fighter Zero (Japan 950727) */
	DRIVER( sfzjr1 )     /* Capcom, Street Fighter Zero (Japan 950627) */
	DRIVER( sfzjr2 )     /* Capcom, Street Fighter Zero (Japan 950605) */
	DRIVER( sfzh )       /* Capcom, Street Fighter Zero (Hispanic 950718) */
	DRIVER( sfzhr1 )     /* Capcom, Street Fighter Zero (Hispanic 950627) */
	DRIVER( sfzb )       /* Capcom, Street Fighter Zero (Brazil 951109) */
	DRIVER( sfzbr1 )     /* Capcom, Street Fighter Zero (Brazil 950727) */
	DRIVER( mmancp2u )   /* Capcom, Mega Man: The Power Battle (CPS2 */
	DRIVER( mmancp2ur1 ) /* Capcom, Mega Man: The Power Battle (CPS2 */
	DRIVER( mmancp2ur2 ) /* Capcom, Mega Man: The Power Battle (CPS2 */
	DRIVER( rmancp2j )   /* Capcom, Rockman: The Power Battle (CPS2 */
	DRIVER( msh )        /* Capcom, Marvel Super Heroes (Euro 951024) */
	DRIVER( mshu )       /* Capcom, Marvel Super Heroes (USA 951024) */
	DRIVER( mshj )       /* Capcom, Marvel Super Heroes (Japan 951117) */
	DRIVER( mshjr1 )     /* Capcom, Marvel Super Heroes (Japan 951024) */
	DRIVER( msha )       /* Capcom, Marvel Super Heroes (Asia 951024) */
	DRIVER( mshh )       /* Capcom, Marvel Super Heroes (Hispanic 951117) */
	DRIVER( mshb )       /* Capcom, Marvel Super Heroes (Brazil 951117) */
	DRIVER( mshbr1 )     /* Capcom, Marvel Super Heroes (Brazil 951024) */
	DRIVER( 19xx )       /* Capcom, 19XX: The War Against Destiny (Euro 960104) */
	DRIVER( 19xxu )      /* Capcom, 19XX: The War Against Destiny (USA 951207) */
	DRIVER( 19xxa )      /* Capcom, 19XX: The War Against Destiny (Asia 960104) */
	DRIVER( 19xxar1 )    /* Capcom, 19XX: The War Against Destiny (Asia 951207) */
	DRIVER( 19xxj )      /* Capcom, 19XX: The War Against Destiny (Japan 960104 */
	DRIVER( 19xxjr1 )    /* Capcom, 19XX: The War Against Destiny (Japan 951225) */
	DRIVER( 19xxjr2 )    /* Capcom, 19XX: The War Against Destiny (Japan 951207) */
	DRIVER( 19xxh )      /* Capcom, 19XX: The War Against Destiny (Hispanic 951218) */
	DRIVER( 19xxb )      /* Capcom, 19XX: The War Against Destiny (Brazil 951218) */
	DRIVER( ddsomh )     /* Capcom, Dungeons & Dragons: Shadow over Mystara (Hispanic 960223) */
	DRIVER( ddsomb )     /* Capcom, Dungeons & Dragons: Shadow over Mystara (Brazil 960223) */
	DRIVER( sfa2 )       /* Capcom, Street Fighter Alpha 2 (Euro 960229) */
	DRIVER( sfa2u )      /* Capcom, Street Fighter Alpha 2 (USA 960430) */
	DRIVER( sfa2ur1 )    /* Capcom, Street Fighter Alpha 2 (USA 960306) */
	DRIVER( sfz2j )      /* Capcom, Street Fighter Zero 2 (Japan 960430) */
	DRIVER( sfz2jr1 )    /* Capcom, Street Fighter Zero 2 (Japan 960227) */
	DRIVER( sfz2a )      /* Capcom, Street Fighter Zero 2 (Asia 960227) */
	DRIVER( sfz2b )      /* Capcom, Street Fighter Zero 2 (Brazil 960531) */
	DRIVER( sfz2br1 )    /* Capcom, Street Fighter Zero 2 (Brazil 960304) */
	DRIVER( sfz2h )      /* Capcom, Street Fighter Zero 2 (Hispanic 960304) */
	DRIVER( sfz2n )      /* Capcom, Street Fighter Zero 2 (Oceania 960229) */
	DRIVER( sfz2al )     /* Capcom, Street Fighter Zero 2 Alpha (Asia 960826) */
	DRIVER( sfz2alr1 )   /* Capcom, Street Fighter Zero 2 Alpha (Asia 960805) */
	DRIVER( sfz2alj )    /* Capcom, Street Fighter Zero 2 Alpha (Japan 960805) */
	DRIVER( sfz2alh )    /* Capcom, Street Fighter Zero 2 Alpha (Hispanic 960813) */
	DRIVER( sfz2alb )    /* Capcom, Street Fighter Zero 2 Alpha (Brazil 960813) */
	DRIVER( spf2t )      /* Capcom, Super Puzzle Fighter II Turbo (Euro 960529) */
	DRIVER( spf2tu )     /* Capcom, Super Puzzle Fighter II Turbo (USA 960620) */
	DRIVER( spf2xj )     /* Capcom, Super Puzzle Fighter II X (Japan 960531) */
	DRIVER( spf2ta )     /* Capcom, Super Puzzle Fighter II Turbo (Asia 960529) */
	DRIVER( spf2th )     /* Capcom, Super Puzzle Fighter II Turbo (Hispanic 960531) */
	DRIVER( megaman2 )   /* Capcom, Mega Man 2: The Power Fighters (USA 960708) */
	DRIVER( megaman2a )  /* Capcom, Mega Man 2: The Power Fighters (Asia 960708) */
	DRIVER( rockman2j )  /* Capcom, Rockman 2: The Power Fighters (Japan 960708) */
	DRIVER( megaman2h )  /* Capcom, Mega Man 2: The Power Fighters (Hispanic 960712) */
	DRIVER( qndream )    /* Capcom, Quiz Nanairo Dreams: Nijiirochou no Kiseki (Japan 96086) */
	DRIVER( xmvsf )      /* Capcom, X-Men Vs. Street Fighter (Euro 961004) */
	DRIVER( xmvsfr1 )    /* Capcom, X-Men Vs. Street Fighter (Euro 960910) */
	DRIVER( xmvsfu )     /* Capcom, X-Men Vs. Street Fighter (USA 961023) */
	DRIVER( xmvsfur1 )   /* Capcom, X-Men Vs. Street Fighter (USA 961004) */
	DRIVER( xmvsfur2 )   /* Capcom, X-Men Vs. Street Fighter (USA 960910) */
	DRIVER( xmvsfj )     /* Capcom, X-Men Vs. Street Fighter (Japan 961023) */
	DRIVER( xmvsfjr1 )   /* Capcom, X-Men Vs. Street Fighter (Japan 961004) */
	DRIVER( xmvsfjr2 )   /* Capcom, X-Men Vs. Street Fighter (Japan 960910) */
	DRIVER( xmvsfjr3 )   /* Capcom, X-Men Vs. Street Fighter (Japan 960909) */
	DRIVER( xmvsfa )     /* Capcom, X-Men Vs. Street Fighter (Asia 961023) */
	DRIVER( xmvsfar1 )   /* Capcom, X-Men Vs. Street Fighter (Asia 961004) */
	DRIVER( xmvsfar2 )   /* Capcom, X-Men Vs. Street Fighter (Asia 960919) */
	DRIVER( xmvsfar3 )   /* Capcom, X-Men Vs. Street Fighter (Asia 960910) */
	DRIVER( xmvsfh )     /* Capcom, X-Men Vs. Street Fighter (Hispanic 961004) */
	DRIVER( xmvsfb )     /* Capcom, X-Men Vs. Street Fighter (Brazil 961023) */
	DRIVER( batcir )     /* Capcom, Battle Circuit (Euro 970319) */
	DRIVER( batcira )    /* Capcom, Battle Circuit (Asia 970319) */
	DRIVER( batcirj )    /* Capcom, Battle Circuit (Japan 970319) */
	DRIVER( vsav )       /* Capcom, Vampire Savior: The Lord of Vampire (Euro 970519) */
	DRIVER( vsavu )      /* Capcom, Vampire Savior: The Lord of Vampire (USA 970519) */
	DRIVER( vsavj )      /* Capcom, Vampire Savior: The Lord of Vampire (Japan 970519) */
	DRIVER( vsava )      /* Capcom, Vampire Savior: The Lord of Vampire (Asia 970519) */
	DRIVER( vsavh )      /* Capcom, Vampire Savior: The Lord of Vampire (Hispanic 970519) */
	DRIVER( vsavb )      /* Capcom, Vampire Savior: The Lord of Vampire (Brazil 970519) */
	DRIVER( mshvsf )     /* Capcom, Marvel Super Heroes Vs. Street Fighter (Euro 970625) */
	DRIVER( mshvsfu )    /* Capcom, Marvel Super Heroes Vs. Street Fighter (USA 970827) */
	DRIVER( mshvsfu1 )   /* Capcom, Marvel Super Heroes Vs. Street Fighter (USA 970625) */
	DRIVER( mshvsfj )    /* Capcom, Marvel Super Heroes Vs. Street Fighter (Japan 970707) */
	DRIVER( mshvsfj1 )   /* Capcom, Marvel Super Heroes Vs. Street Fighter (Japan 970702) */
	DRIVER( mshvsfj2 )   /* Capcom, Marvel Super Heroes Vs. Street Fighter (Japan 970625) */
	DRIVER( mshvsfh )    /* Capcom, Marvel Super Heroes Vs. Street Fighter (Hispanic 970625) */
	DRIVER( mshvsfa )    /* Capcom, Marvel Super Heroes Vs. Street Fighter (Asia 970625) */
	DRIVER( mshvsfa1 )   /* Capcom, Marvel Super Heroes Vs. Street Fighter (Asia 970620) */
	DRIVER( mshvsfb )    /* Capcom, Marvel Super Heroes Vs. Street Fighter (Brazil 970827) */
	DRIVER( mshvsfb1 )   /* Capcom, Marvel Super Heroes Vs. Street Fighter (Brazil 970625) */
	DRIVER( csclub )     /* Capcom, Capcom Sports Club (Euro 971017) */
	DRIVER( csclub1 )    /* Capcom, Capcom Sports Club (Euro 970722) */
	DRIVER( cscluba )    /* Capcom, Capcom Sports Club (Asia 970722) */
	DRIVER( csclubj )    /* Capcom, Capcom Sports Club (Japan 970722) */
	DRIVER( csclubjy )   /* Capcom, Capcom Sports Club (Japan 970722 */
	DRIVER( csclubh )    /* Capcom, Capcom Sports Club (Hispanic 970722) */
	DRIVER( sgemf )      /* Capcom, Super Gem Fighter Mini Mix (USA 970904) */
	DRIVER( pfghtj )     /* Capcom, Pocket Fighter (Japan 970904) */
	DRIVER( sgemfa )     /* Capcom, Super Gem Fighter: Mini Mix (Asia 970904) */
	DRIVER( sgemfh )     /* Capcom, Super Gem Fighter: Mini Mix (Hispanic 970904) */
	DRIVER( vhunt2 )     /* Capcom, Vampire Hunter 2: Darkstalkers Revenge (Japan 970929) */
	DRIVER( vhunt2r1 )   /* Capcom, Vampire Hunter 2: Darkstalkers Revenge (Japan 970913) */
	DRIVER( vsav2 )      /* Capcom, Vampire Savior 2: The Lord of Vampire (Japan 970913) */
	DRIVER( mvsc )       /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Euro 980123) */
	DRIVER( mvscr1 )     /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Euro 980112) */
	DRIVER( mvscu )      /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (USA 980123) */
	DRIVER( mvscur1 )    /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (USA 971222) */
	DRIVER( mvscj )      /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Japan 980123) */
	DRIVER( mvscjr1 )    /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Japan 980112) */
	DRIVER( mvscjsing )  /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Japan 980123) (Single PCB) */
	DRIVER( mvsca )      /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Asia 980123) */
	DRIVER( mvscar1 )    /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Asia 980112) */
	DRIVER( mvsch )      /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Hispanic 980123) */
	DRIVER( mvscb )      /* Capcom, Marvel Vs. Capcom: Clash of Super Heroes (Brazil 980123) */
	DRIVER( sfa3 )       /* Capcom, Street Fighter Alpha 3 (Euro 980904) */
	DRIVER( sfa3u )      /* Capcom, Street Fighter Alpha 3 (USA 980904) */
	DRIVER( sfa3ur1 )    /* Capcom, Street Fighter Alpha 3 (USA 980629) */
	DRIVER( sfa3us )     /* Capcom, Street Fighter Alpha 3 (USA 980616 */
	DRIVER( sfa3h )      /* Capcom, Street Fighter Alpha 3 (Hispanic 980904) */
	DRIVER( sfa3hr1 )    /* Capcom, Street Fighter Alpha 3 (Hispanic 980629) */
	DRIVER( sfa3b )      /* Capcom, Street Fighter Alpha 3 (Brazil 980629) */
	DRIVER( sfz3j )      /* Capcom, Street Fighter Zero 3 (Japan 980904) */
	DRIVER( sfz3jr1 )    /* Capcom, Street Fighter Zero 3 (Japan 980727) */
	DRIVER( sfz3jr2 )    /* Capcom, Street Fighter Zero 3 (Japan 980629) */
	DRIVER( sfz3a )      /* Capcom, Street Fighter Zero 3 (Asia 980904) */
	DRIVER( sfz3ar1 )    /* Capcom, Street Fighter Zero 3 (Asia 980701) */
	DRIVER( jyangoku )   /* Capcom, Jyangokushi: Haoh no Saihai (Japan 990527) */
	DRIVER( gigawing )   /* Takumi (Capcom license), Giga Wing (USA 990222) */
	DRIVER( gigawingj )  /* Takumi (Capcom license), Giga Wing (Japan 990223) */
	DRIVER( gigawinga )  /* Takumi (Capcom license), Giga Wing (Asia 990222) */
	DRIVER( gigawingh )  /* Takumi (Capcom license), Giga Wing (Hispanic 990222) */
	DRIVER( gigawingb )  /* Takumi (Capcom license), Giga Wing (Brazil 990222) */
	DRIVER( mmatrix )    /* Takumi (Capcom license), Mars Matrix: Hyper Solid Shooting (USA 000412) */
	DRIVER( mmatrixa )   /* Takumi (Capcom license), Mars Matrix: Hyper Solid Shooting (Asia 000412) */
	DRIVER( mmatrixj )   /* Takumi (Capcom license), Mars Matrix: Hyper Solid Shooting (Japan 000412) */
	DRIVER( mpang )      /* Mitchell (Capcom license), Mighty! Pang (Euro 001010) */
	DRIVER( mpangr1 )    /* Mitchell (Capcom license), Mighty! Pang (Euro 000925) */
	DRIVER( mpangu )     /* Mitchell (Capcom license), Mighty! Pang (USA 001010) */
	DRIVER( mpangj )     /* Mitchell (Capcom license), Mighty! Pang (Japan 001011) */
	DRIVER( mpanga )     /* Mitchell (Capcom license), Mighty! Pang (Asia 001010) */
	DRIVER( pzloop2 )    /* Mitchell (Capcom license), Puzz Loop 2 (Euro 010302) */
	DRIVER( pzloop2j )   /* Mitchell (Capcom license), Puzz Loop 2 (Japan 010226) */
	DRIVER( pzloop2jr1 ) /* Mitchell (Capcom license), Puzz Loop 2 (Japan 010205) */
	DRIVER( choko )      /* Mitchell (Capcom license), Janpai Puzzle Choukou (Japan 010820) */
	DRIVER( dimahoo )    /* Eighting / Raizing (Capcom license), Dimahoo (Euro 000121) */
	DRIVER( dimahoou )   /* Eighting / Raizing (Capcom license), Dimahoo (USA 000121) */
	DRIVER( gmahou )     /* Eighting / Raizing (Capcom license), Great Mahou Daisakusen (Japan 000121) */
	DRIVER( 1944 )       /* Eighting / Raizing (Capcom license), 1944: The Loop Master (Euro 000620) */
	DRIVER( 1944j )      /* Eighting / Raizing (Capcom license), 1944: The Loop Master (Japan 000620) */
	DRIVER( 1944u )      /* Eighting / Raizing (Capcom license), 1944: The Loop Master (USA 000620) */
	DRIVER( progear )    /* Cave (Capcom license), Progear (USA 010117) */
	DRIVER( progearj )   /* Cave (Capcom license), Progear no Arashi (Japan 010117) */
	DRIVER( progeara )   /* Cave (Capcom license), Progear (Asia 010117) */
	#if defined(CPS2_BOOTLEG)
	DRIVER( ddtodd )     /* bootleg, Dungeons & Dragons: Tower of Doom (Euro 940412 Phoenix Edition) (bootleg) */
	DRIVER( ecofghtrd )  /* bootleg, Eco Fighters (World 931203 Phoenix Edition) (bootleg) */
	DRIVER( ssf2ud )     /* bootleg, Super Street Fighter II: The New Challengers (USA 930911 Phoenix Edition) (bootleg) */
	DRIVER( ssf2tbd )    /* bootleg, Super Street Fighter II: The Tournament Battle (World 931119 Phoenix Edition) (bootleg) */
	DRIVER( armwar1d )   /* bootleg, Armored Warriors (Euro 941011 Phoenix Edition) (bootleg) */
	DRIVER( avspd )      /* bootleg, Alien vs. Predator (Euro 940520 Phoenix Edition) (bootleg) */
	DRIVER( dstlku1d )   /* bootleg, Darkstalkers: The Night Warriors (USA 940705 Phoenix Edition) (bootleg) */
	DRIVER( ringdstd )   /* bootleg, Ring of Destruction: Slammasters II (Euro 940902 Phoenix Edition) (bootleg) */
	DRIVER( ssf2tad )    /* bootleg, Super Street Fighter II Turbo (Asia 940223 Phoenix Edition) (bootleg) */
	DRIVER( ssf2xjr1d )  /* bootleg, Super Street Fighter II X: Grand Master Challenge (Japan 940223 Phoenix Edition) (bootleg) */
	DRIVER( xmcotar1d )  /* bootleg, X-Men: Children of the Atom (Euro 950105 Phoenix Edition) (bootleg) */
	DRIVER( mshud )      /* bootleg, Marvel Super Heroes (US 951024 Phoenix Edition) (bootleg) */
	DRIVER( cybotsud )   /* bootleg, Cyberbots: Fullmetal Madness (USA 950424 Phoenix Edition) (bootleg) */
	DRIVER( cybotsjd )   /* bootleg, Cyberbots: Fullmetal Madness (Japan 950424) (decrypted bootleg) */
	DRIVER( nwarrud )    /* bootleg, Night Warriors: Darkstalkers' Revenge (USA 950406 Phoenix Edition) (bootleg) */
	DRIVER( sfad )       /* bootleg, Street Fighter Alpha: Warriors' Dreams (Euro 950727 Phoenix Edition) (bootleg) */
	DRIVER( 19xxd )      /* bootleg, 19XX: The War Against Destiny (USA 951207 Phoenix Edition) (bootleg) */
	DRIVER( ddsomud )    /* bootleg, Dungeons & Dragons: Shadow over Mystara (USA 960619 Phoenix Edition) (bootleg) */
	DRIVER( gigaman2 )   /* bootleg, Giga Man 2: The Power Fighters (bootleg of Mega Man 2: The Power Fighters) */
	DRIVER( megamn2d )   /* bootleg, Mega Man 2: The Power Fighters (USA 960708 Phoenix Edition) (bootleg) */
	DRIVER( sfz2ad )     /* bootleg, Street Fighter Zero 2 (Asia 960227 Phoenix Edition) (bootleg) */
	DRIVER( sfz2jd )     /* bootleg, Street Fighter Zero 2 (Japan 960227 Phoenix Edition) (bootleg) */
	DRIVER( spf2td )     /* bootleg, Super Puzzle Fighter II Turbo (USA 960620 Phoenix Edition) (bootleg) */
	DRIVER( spf2xjd )    /* bootleg, Super Puzzle Fighter II X (Japan 960531 Phoenix Edition) (bootleg) */
	DRIVER( sfz2ald )    /* bootleg, Street Fighter Zero 2 Alpha (Asia 960826 Phoenix Edition) (bootleg) */
	DRIVER( xmvsfu1d )   /* bootleg, X-Men Vs. Street Fighter (USA 961004 Phoenix Edition) (bootleg) */
	DRIVER( batcird )    /* bootleg, Battle Circuit (Euro 970319 Phoenix Edition) (bootleg) */
	DRIVER( csclub1d )   /* bootleg, Capcom Sports Club (Euro 970722 Phoenix Edition) (bootleg) */
	DRIVER( mshvsfu1d )  /* bootleg, Marvel Super Heroes Vs. Street Fighter (USA 970625 Phoenix Edition) (bootleg) */
	DRIVER( sgemfd )     /* bootleg, Super Gem Fighter Mini Mix (USA 970904 Phoenix Edition) (bootleg) */
	DRIVER( vsavd )      /* bootleg, Vampire Savior: The Lord of Vampire (Euro 970519 Phoenix Edition) (bootleg) */
	DRIVER( vhunt2d )    /* bootleg, Vampire Hunter 2: Darkstalkers Revenge (Japan 970913 Phoenix Edition) (bootleg) */
	DRIVER( vsav2d )     /* bootleg, Vampire Savior 2: The Lord of Vampire (Japan 970913 Phoenix Edition) (bootleg) */
	DRIVER( mvscud )     /* bootleg, Marvel Vs. Capcom: Clash of Super Heroes (USA 980123 Phoenix Edition) (bootleg) */
	DRIVER( sfa3ud )     /* bootleg, Street Fighter Alpha 3 (USA 980904 Phoenix Edition) (bootleg) */
	DRIVER( sfz3jr2d )   /* bootleg, Street Fighter Zero 3 (Japan 980629 Phoenix Edition) (bootleg) */
	DRIVER( gigawingd )  /* bootleg, Giga Wing (USA 990222 Phoenix Edition) (bootleg) */
	DRIVER( gigawingjd ) /* bootleg, Giga Wing (Japan 990223 Phoenix Edition) (bootleg) */
	DRIVER( 1944d )      /* bootleg, 1944: The Loop Master (USA 000620 Phoenix Edition) (bootleg) */
	DRIVER( dimahoud )   /* bootleg, Dimahoo (USA 000121 Phoenix Edition) (bootleg) */
	DRIVER( mmatrixd )   /* bootleg, Mars Matrix: Hyper Solid Shooting (USA 000412 Phoenix Edition) (bootleg) */
	DRIVER( progearud )  /* bootleg, Progear (USA 010117 Phoenix Edition) (bootleg) */
	DRIVER( progearjd )  /* bootleg, Progear no Arashi (Japan 010117 Phoenix Edition) (bootleg) */
	DRIVER( progearjbl ) /* bootleg, Progear no Arashi (Japan 010117) (decrypted bootleg) */
	DRIVER( pzloop2jd )  /* bootleg, Puzz Loop 2 (Japan 010226 Phoenix Edition) (bootleg) */
	DRIVER( hsf2d )      /* bootleg, Hyper Street Fighter II: The Anniversary Edition (Asia 040202 Phoenix Edition) (bootleg)  */
	#endif
	/* Capcom CPS3 games */
	/* 10/1996 Warzard */
	/*  2/1997 Street Fighter III - New Generation */
	/* ???? Jojo's Bizarre Adventure */
	/* ???? Street Fighter 3: Second Impact ~giant attack~ */
	/* ???? Street Fighter 3: Third Strike ~fight to the finish~ */

	/* Capcom ZN1 */

	/* Capcom ZN2 */
						/* Rival Schools 2 */

	/* Tecmo ZN1 */

	/* PS Arcade 95 */

	/* Atari PSX */

	/* Acclaim PSX */

	/* Video System ZN1 */

	/* Taito FX1a */

	/* Taito FX1b */

	/* Namco System 11 */

	/* Namco System 12 */

	/* Namco System 10 */

	/* Konami GQ */

	/* Mitchell games */
	/*  8/1989 Dokaben 2 (baseball) */
	/*  4/1991 Ashita Tenki ni Naare (golf) */

	/* Incredible Technologies games */
	/* http://www.itsgames.com */
	DRIVER( timekill )	/* (c) 1992 Strata/Incredible Technologies */
	DRIVER( timek131 )	/* (c) 1992 Strata/Incredible Technologies */
	DRIVER( hardyard )	/* (c) 1993 Strata/Incredible Technologies */
	DRIVER( hardyd10 )	/* (c) 1993 Strata/Incredible Technologies */
	DRIVER( bloodstm )	/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( bloods22 )	/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( bloods21 )	/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( bloods11 )	/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( pairs )		/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( pairsa )	/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( drivedge )	/* (c) 1994 Strata/Incredible Technologies */
	DRIVER( wcbowl )	/* (c) 1995 Incredible Technologies */
	DRIVER( wcbwl165 )	/* (c) 1995 Incredible Technologies */
	DRIVER( wcbwl161 )	/* (c) 1995 Incredible Technologies */
	DRIVER( wcbwl12 )	/* (c) 1995 Incredible Technologies */
	DRIVER( sftm )		/* (c) 1995 Capcom/Incredible Technologies */
	DRIVER( sftm110 )	/* (c) 1995 Capcom/Incredible Technologies */
	DRIVER( sftm111 )	/* (c) 1995 Capcom/Incredible Technologies */
	DRIVER( sftmj )		/* (c) 1995 Capcom/Incredible Technologies */
	DRIVER( pubball )	/* (c) 1996 Midway/Incredible Technologies (prototype)*/
	DRIVER( shufshot )	/* (c) Strata/Incredible Technologies */
	DRIVER( sshot137 )	/* (c) Strata/Incredible Technologies */
	DRIVER( gt3d )      /* (c) 1995 Incredible Technologies */
	DRIVER( gt97 )      /* (c) 1997 Incredible Technologies */
	DRIVER( gt98 )      /* (c) 1998 Incredible Technologies */
	DRIVER( gt99 )      /* (c) 1999 Incredible Technologies */
	DRIVER( gt2k )      /* (c) 2000 Incredible Technologies */
	DRIVER( gtclassc )  /* (c) 2001 Incredible Technologies */

	/* Leland games */
	DRIVER( cerberus )	/* (c) 1985 Cinematronics */
	DRIVER( mayhem )	/* (c) 1985 Cinematronics */
	DRIVER( powrplay )	/* (c) 1985 Cinematronics */
	DRIVER( wseries )	/* (c) 1985 Cinematronics */
	DRIVER( alleymas )	/* (c) 1986 Cinematronics */
	DRIVER( dangerz )	/* (c) 1986 Cinematronics USA */
	DRIVER( basebal2 )	/* (c) 1987 Cinematronics */
	DRIVER( dblplay )	/* (c) 1987 Tradewest / Leland */
	DRIVER( strkzone )	/* (c) 1988 Leland */
	DRIVER( redlin2p )	/* (c) 1987 Cinematronics + Tradewest license */
	DRIVER( quarterb )	/* (c) 1987 Leland */
	DRIVER( quartrba )	/* (c) 1987 Leland */
	DRIVER( viper )		/* (c) 1988 Leland */
	DRIVER( teamqb )	/* (c) 1988 Leland */
	DRIVER( teamqb2 )	/* (c) 1988 Leland */
	DRIVER( aafb )		/* (c) 1989 Leland */
	DRIVER( aafbd2p )	/* (c) 1989 Leland */
	DRIVER( aafbc )		/* (c) 1989 Leland */
	DRIVER( aafbb )		/* (c) 1989 Leland */
	DRIVER( offroad )	/* (c) 1989 Leland */
	DRIVER( offroadt )	/* (c) 1989 Leland */
	DRIVER( pigout )	/* (c) 1990 Leland */
	DRIVER( pigouta )	/* (c) 1990 Leland */
	DRIVER( ataxx )		/* (c) 1990 Leland */
	DRIVER( ataxxa )	/* (c) 1990 Leland */
	DRIVER( ataxxj )	/* (c) 1990 Leland */
	DRIVER( wsf )		/* (c) 1990 Leland */
	DRIVER( indyheat )	/* (c) 1991 Leland */
	DRIVER( brutforc )	/* (c) 1991 Leland */
	DRIVER( asylum )	/* (c) 1991 Leland */

	/* Gremlin 8080 games */
	/* the numbers listed are the range of ROM part numbers */
	DRIVER( blockade )	/* 1-4 [1977 Gremlin] */
	DRIVER( comotion )	/* 5-7 [1977 Gremlin] */
	DRIVER( hustle )	/* 16-21 [1977 Gremlin] */
	DRIVER( blasto )	/* [1978 Gremlin] */
	DRIVER( mineswpr )	/* [1977 Amutech] */

	/* Gremlin/Sega "VIC dual game board" games */
	/* the numbers listed are the range of ROM part numbers */
	DRIVER( depthch )	/* 50-55 [1977 Gremlin?] */
	DRIVER( depthv1 )	/*   ?   [1977 Gremlin?] */
	DRIVER( subhunt )	/*   ?   [1977 Taito] */
	DRIVER( safari )	/* 57-66 [1977 Gremlin?] */
	DRIVER( frogs )		/* 112-119 [1978 Gremlin?] */
	DRIVER( sspaceat )	/* 155-162 (c) */
	DRIVER( sspacat2 )
	DRIVER( sspacatc )	/* 139-146 (c) */
	DRIVER( sspacaho )	/* ? epr00001.bin - epr00008.bin */
	DRIVER( headon )	/* 163-167/192-193 (c) Gremlin */
	DRIVER( headonb )	/* 163-167/192-193 (c) Gremlin */
	DRIVER( headon2 )	/* ???-??? (c) 1979 Sega */
	/* ???-??? Fortress */
	/* ???-??? Gee Bee */
	/* 255-270  Head On 2 / Deep Scan */
	DRIVER( invho2 )	/* 271-286 (c) 1979 Sega */
	DRIVER( samurai )	/* 289-302 + upgrades (c) 1980 Sega */
	DRIVER( invinco )	/* 310-318 (c) 1979 Sega */
	DRIVER( invds )		/* 367-382 (c) 1979 Sega */
	DRIVER( tranqgun )	/* 413-428 (c) 1980 Sega */
	/* 450-465  Tranquilizer Gun (different version?) */
	/* ???-??? Car Hunt / Deep Scan */
	DRIVER( spacetrk )	/* 630-645 (c) 1980 Sega */
	DRIVER( sptrekct )	/* (c) 1980 Sega */
	DRIVER( carnival )	/* 651-666 (c) 1980 Sega */
	DRIVER( carnvckt )	/* 501-516 (c) 1980 Sega */
	DRIVER( brdrlinb )	/* bootleg */
	DRIVER( digger )	/* 684-691 no copyright notice */
	DRIVER( pulsar )	/* 790-805 (c) 1981 Sega */
	DRIVER( heiankyo )	/* (c) [1979?] Denki Onkyo */
	DRIVER( alphaho )	/* Data East */

	/* Sega G-80 vector games */
	DRIVER( spacfury )	/* (c) 1981 */
	DRIVER( spacfura )	/* no copyright notice */
	DRIVER( zektor )	/* (c) 1982 */
	DRIVER( tacscan )	/* (c) */
	DRIVER( elim2 )		/* (c) 1981 Gremlin */
	DRIVER( elim2a )	/* (c) 1981 Gremlin */
	DRIVER( elim4 )		/* (c) 1981 Gremlin */
	DRIVER( startrek )	/* (c) 1982 */

	/* Sega G-80 raster games */
	DRIVER( astrob )	/* (c) 1981 */
	DRIVER( astrob2 )	/* (c) 1981 */
	DRIVER( astrob1 )	/* (c) 1981 */
	DRIVER( 005 )		/* (c) 1981 */
	DRIVER( monsterb )	/* (c) 1982 */
	DRIVER( spaceod )	/* (c) 1981 */
	DRIVER( pignewt )	/* (c) 1983 */
	DRIVER( pignewta )	/* (c) 1983 */
	DRIVER( sindbadm )	/* 834-5244 (c) 1983 Sega */

	/* Sega "Zaxxon hardware" games */

	/* Sega SG1000 based games */

	/* Sega System 1 / System 2 games */
/*
other System 1 / System 2 games:
WarBall
Sanrin Sanchan
DokiDoki Penguin Land *not confirmed
*/

	/* Sega System E games (Master System hardware) */

	/* other Sega 8-bit games */
	DRIVER( turbo )		/* (c) 1981 Sega */
	DRIVER( turboa )	/* (c) 1981 Sega */
	DRIVER( turbob )	/* (c) 1981 Sega */
	DRIVER( suprloco )	/* (c) 1982 Sega */
	DRIVER( dotrikun )	/* cabinet test board */
	DRIVER( dotriku2 )	/* cabinet test board */
	DRIVER( dotriman )
	DRIVER( spcpostn )	/* (c) 1986 Sega / Nasco" */
	DRIVER( angelkds )	/* 833-6599 (c) 1988 Sega / Nasco? */

	/* Sega System 16 games */
	/* Not working */


	/* Working */
/*
Sega System 24 game list
Apr.1988 Hot Rod
Oct.1988 Scramble Spirits
Nov.1988 Gain Ground
Apr.1989 Crack Down
Aug.1989 Jumbo Ozaki Super Masters
Jun.1990 Bonanza Bros.
Dec.1990 Rough Racer
Feb.1991 Quiz Syukudai wo Wasuremashita
Jul.1991 Dynamic C.C.
Dec.1991 Quiz Rouka ni Tattenasai
Dec.1992 Tokorosan no MahMahjan
May.1993 Quiz Mekurumeku Story
May.1994 Tokorosan no MahMahjan 2
Sep.1994 Quiz Ghost Hunter
*/
	/* playable */
	/* not working */

	/* Sega System 32 games */
	DRIVER( holo )		/* (c) 1992 (US) */
	DRIVER( svf )		/* (c) 1994 */
	DRIVER( svs )		/* (c) 1994 */
	DRIVER( jleague )	/* (c) 1994 (Japan) */
	DRIVER( brival )	/* (c) 1992 (Japan) */
	DRIVER( radm )    /* (c) 1991 */
	DRIVER( radr )		/* (c) 1991 */
	DRIVER( f1en )    /* (c) 1991 */
	DRIVER( alien3 )	/* (c) 1993 */
	DRIVER( sonic )		/* (c) 1992 (Japan) */
	DRIVER( sonicp )	/* (c) 1992 (Japan) */
	DRIVER( jpark )		/* (c) 1994 */
	DRIVER( ga2 )		  /* (c) 1992 */
	DRIVER( ga2j )		/* (c) 1992 */
	DRIVER( spidey )	/* (c) 1991 */
	DRIVER( spideyj )	/* (c) 1991 (Japan) */
	DRIVER( arabfgt )	/* (c) 1991 */
	DRIVER( arescue )	/* (c) 1992 */
	DRIVER( f1lap )   /* (c) 1993 (World) */
	DRIVER( dbzvrvs ) /* (c) 1994 */
	DRIVER( darkedge ) /* (c) 1992 */
	DRIVER( slipstrm ) /* (c) 1995 (Brazil) Capcom */

	/* Sega Multi System 32 games */	/* (c) 1992 (US) */	/* (c) 1994 (World) */    /* (c) 1992 */	/* (c) 1992 (World) */

	/* Sega ST-V games */



	/* Deniam games */
	/* they run on Sega System 16 video hardware */
	DRIVER( logicpro )	/* (c) 1996 Deniam */
	DRIVER( croquis )	/* (c) 1996 Deniam (Germany) */
	DRIVER( karianx )	/* (c) 1996 Deniam */
	DRIVER( logicpr2 )	/* (c) 1997 Deniam (Japan) */
/*
Deniam is a Korean company (http://deniam.co.kr).

Game list:
Title            System     Date
---------------- ---------- ----------
GO!GO!           deniam-16b 1995/10/11
Logic Pro        deniam-16b 1996/10/20
Karian Cross     deniam-16b 1997/04/17
LOTTERY GAME     deniam-16c 1997/05/21
Logic Pro 2      deniam-16c 1997/06/20
Propose          deniam-16c 1997/06/21
BOMULEUL CHAJARA SEGA ST-V  1997/04/11
*/

	/* System C games */

	/* System C-2 games */

	/* Atlus Print Club 'Games' C-2 board */

	/* Genie Hardware (uses Genesis VDP) also has 'Sun Mixing Co' put into tile ram */

	/* Sega MegaTech, the number shown ia on the label of the instruction rom */
						/* 06 */
						/* 07 */
						/* 09 */
						/* 12 */
						/* 14 */
						/* 15 */
						/* 16 */
						/* 17 */
						/* 18 */
						/* 19 */
						/* 24 */
						/* 26 */
						/* 30 */
						/* 32 */
						/* 33 */
						/* 34 */
						/* 36 */
						/* 37 */
						/* 41 */
						/* 42 */
						/* 43 */
						/* 44 */
						/* 45 */
						/* 46 */
						/* 47 */
						/* 48 */
						/* 50 */
						/* 51 */
						/* 54 */
						/* 55 */
						/* 56 */
						/* 58 */
						/* 59 */

	/* Sega MegaPlay */

	/* Data East "Burger Time hardware" games */
	DRIVER( lnc )		/* (c) 1981 */
	DRIVER( zoar )		/* (c) 1982 */
	DRIVER( btime )		/* (c) 1982 */
	DRIVER( btime2 )	/* (c) 1982 */
	DRIVER( btimem )	/* (c) 1982 + Midway */
	DRIVER( cookrace )	/* bootleg */
	DRIVER( wtennis )	/* bootleg 1982 */
	DRIVER( brubber )	/* (c) 1982 */
	DRIVER( bnj )		/* (c) 1982 + Midway */
	DRIVER( caractn )	/* bootleg */
	DRIVER( disco )		/* (c) 1982 */
	DRIVER( discof )	/* (c) 1982 */
	DRIVER( sdtennis )	/* (c) 1983 */
	DRIVER( mmonkey )	/* (c) 1982 Technos Japan + Roller Tron */
	/* cassette system, parent is decocass */
  DRIVER( chwy )		  /* 02 (c) 1980 */
	DRIVER( ctisland )	/* 16 (c) 1981 */
	DRIVER( ctislnd2 )	/* 16 (c) 1981 */
	DRIVER( ctislnd3 )	/* 16? (c) 1981 */
	DRIVER( cexplore )	/* ?? (c) 1982 */
	DRIVER( cgraplp2 )	/* 28? (c) 1983 */
  DRIVER( csdtenis )  /* 34 (c) 1983 */
	/* the following don't work at all */
	DRIVER( cflyball ) /* ?? (c) 198? */

	/* other Data East games */
	DRIVER( astrof )	/* (c) [1980?] */
	DRIVER( astrof2 )	/* (c) [1980?] */
	DRIVER( astrof3 )	/* (c) [1980?] */
	DRIVER( tomahawk )	/* (c) [1980?] */
	DRIVER( tomahaw5 )	/* (c) [1980?] */
	DRIVER( prosoccr )	/* (c) 1983 */
	DRIVER( prosport )	/* (c) 1983 */
	DRIVER( boomrang )	/* (c) 1983 */
	DRIVER( kamikcab )	/* (c) 1984 */
	DRIVER( liberate )	/* (c) 1984 */
	DRIVER( liberatb )	/* bootleg */
	DRIVER( batwings )	/* (c) 1984 */
	DRIVER( kchamp )	/* (c) 1984 Data East USA (US) */
	DRIVER( karatedo )	/* (c) 1984 Data East Corporation (Japan) */
	DRIVER( kchampvs )	/* (c) 1984 Data East USA (US) */
	DRIVER( karatevs )	/* (c) 1984 Data East Corporation (Japan) */
	DRIVER( firetrap )	/* (c) 1986 Data East USA (US) */
	DRIVER( firetpbl )	/* bootleg */
	DRIVER( metlclsh )	/* (c) 1985 Data East */
	DRIVER( brkthru )	/* (c) 1986 Data East USA (US) */
	DRIVER( brkthruj )	/* (c) 1986 Data East Corporation (Japan) */
	DRIVER( darwin )	/* (c) 1986 Data East Corporation (Japan) */
	DRIVER( shootout )	/* (c) 1985 Data East USA (US) */
	DRIVER( shootouj )	/* (c) 1985 Data East USA (Japan) */
	DRIVER( shootoub )	/* bootleg */
	DRIVER( sidepckt )	/* (c) 1986 Data East Corporation */
	DRIVER( sidepctj )	/* (c) 1986 Data East Corporation */
	DRIVER( sidepctb )	/* bootleg */
	DRIVER( exprraid )	/* (c) 1986 Data East USA (US) */
	DRIVER( wexpress )	/* (c) 1986 Data East Corporation (World?) */
	DRIVER( wexpresb )	/* bootleg */
	DRIVER( wexpresc )	/* bootleg */
	DRIVER( pcktgal )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( pcktgalb )	/* bootleg */
	DRIVER( pcktgal2 )	/* (c) 1989 Data East Corporation (World?) */
	DRIVER( spool3 )	/* (c) 1989 Data East Corporation (World?) */
	DRIVER( spool3i )	/* (c) 1990 Data East Corporation + I-Vics license */
	DRIVER( actfancr )	/* (c) 1989 Data East Corporation (World) */
	DRIVER( actfanc1 )	/* (c) 1989 Data East Corporation (World) */
	DRIVER( actfancj )	/* (c) 1989 Data East Corporation (Japan) */
	DRIVER( triothep )	/* (c) 1989 Data East Corporation (Japan) */

	/* Data East 8-bit games */
	DRIVER( lastmisn )	/* (c) 1986 Data East USA (US) */
	DRIVER( lastmsno )	/* (c) 1986 Data East USA (US) */
	DRIVER( lastmsnj )	/* (c) 1986 Data East Corporation (Japan) */
	DRIVER( shackled )	/* (c) 1986 Data East USA (US) */
	DRIVER( breywood )	/* (c) 1986 Data East Corporation (Japan) */
	DRIVER( csilver )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( ghostb )	/* (c) 1987 Data East USA (US) */
	DRIVER( ghostb3 )	/* (c) 1987 Data East USA (US) */
	DRIVER( meikyuh )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( srdarwin )	/* (c) 1987 Data East Corporation (World) */
	DRIVER( srdarwnj )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( gondo )		/* (c) 1987 Data East USA (US) */
	DRIVER( makyosen )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( garyoret )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( cobracom )	/* (c) 1988 Data East Corporation (World) */
	DRIVER( cobracmj )	/* (c) 1988 Data East Corporation (Japan) */
	DRIVER( oscar )		/* (c) 1988 Data East USA (US) */
	DRIVER( oscarj )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( oscarj1 )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( oscarj0 )	/* (c) 1987 Data East Corporation (Japan) */

	/* Data East 16-bit games */
	DRIVER( karnov )	/* (c) 1987 Data East USA (US) */
	DRIVER( karnovj )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( wndrplnt )	/* (c) 1987 Data East Corporation (Japan) */
	DRIVER( chelnov )	/* (c) 1988 Data East USA (World) */
	DRIVER( chelnovu )	/* (c) 1988 Data East USA (US) */
	DRIVER( chelnovj )	/* (c) 1988 Data East Corporation (Japan) */
/* the following ones all run on similar hardware */
/* end of similar hardware */
	DRIVER( stadhero )	/* (c) 1988 Data East Corporation (Japan) */
	DRIVER( madmotor )	/* (c) [1989] Mitchell */
	/* All these games have a unique code stamped on the mask roms */
	DRIVER( vaportra )	/* MAA (c) 1989 Data East Corporation (World) */
	DRIVER( vaportru )	/* MAA (c) 1989 Data East Corporation (US) */
	DRIVER( kuhga )		/* MAA (c) 1989 Data East Corporation (Japan) */
	DRIVER( cbuster )	/* MAB (c) 1990 Data East Corporation (World) */
	DRIVER( cbusterw )	/* MAB (c) 1990 Data East Corporation (World) */
	DRIVER( cbusterj )	/* MAB (c) 1990 Data East Corporation (Japan) */
	DRIVER( twocrude )	/* MAB (c) 1990 Data East USA (US) */
	DRIVER( darkseal )	/* MAC (c) 1990 Data East Corporation (World) */
	DRIVER( darksea1 )	/* MAC (c) 1990 Data East Corporation (World) */
	DRIVER( darkseaj )	/* MAC (c) 1990 Data East Corporation (Japan) */
	DRIVER( gatedoom )	/* MAC (c) 1990 Data East Corporation (US) */
	DRIVER( gatedom1 )	/* MAC (c) 1990 Data East Corporation (US) */
	DRIVER( edrandy )	/* MAD (c) 1990 Data East Corporation (World) */
	DRIVER( edrandy1 )	/* MAD (c) 1990 Data East Corporation (World) */
	DRIVER( edrandyj )	/* MAD (c) 1990 Data East Corporation (Japan) */
	DRIVER( supbtime )	/* MAE (c) 1990 Data East Corporation (World) */
	DRIVER( supbtimj )	/* MAE (c) 1990 Data East Corporation (Japan) */
	DRIVER( mutantf )	/* MAF (c) 1992 Data East Corporation (World) */
	DRIVER( mutantfa )	/* MAF (c) 1992 Data East Corporation (World) */
	DRIVER( deathbrd )	/* MAF (c) 1992 Data East Corporation (Japan) */
	DRIVER( cninja )	/* MAG (c) 1991 Data East Corporation (World) */
	DRIVER( cninja0 )	/* MAG (c) 1991 Data East Corporation (World) */
	DRIVER( cninjau )	/* MAG (c) 1991 Data East Corporation (US) */
	DRIVER( joemac )	/* MAG (c) 1991 Data East Corporation (Japan) */
	DRIVER( stoneage )	/* bootleg */
	DRIVER( robocop2 )	/* MAH (c) 1991 Data East Corporation (World) */
	DRIVER( robocp2u )	/* MAH (c) 1991 Data East Corporation (US) */
	DRIVER( robocp2j )	/* MAH (c) 1991 Data East Corporation (Japan) */
	DRIVER( thndzone )	/* MAJ (c) 1991 Data East Corporation (World) */
	DRIVER( dassault )	/* MAJ (c) 1991 Data East Corporation (US) */
	DRIVER( dassaul4 )	/* MAJ (c) 1991 Data East Corporation (US) */
	DRIVER( chinatwn )	/* MAK (c) 1991 Data East Corporation (Japan) */
	DRIVER( captaven )	/* MAN (c) 1991 Data East Corporation (Asia) */
	DRIVER( captavna )	/* MAN (c) 1991 Data East Corporation (Asia) */
	DRIVER( captavne )	/* MAN (c) 1991 Data East Corporation (UK) */
	DRIVER( captavnu )	/* MAN (c) 1991 Data East Corporation (US) */
	DRIVER( captavuu )	/* MAN (c) 1991 Data East Corporation (US) */
	DRIVER( captavnj )	/* MAN (c) 1991 Data East Corporation (Japan) */
	/* MAO ?? */
	/* MAQ ?? */
	DRIVER( dragngun )	/* MAR (c) 1992 Data East Corporation (US) */
	DRIVER( funkyjet )	/* MAT (c) 1992 Mitchell */
	/* MAU ?? */
	/* MAW ?? */
	/* MAX ?? */
	DRIVER( dietgo )	/* MAY (c) 1992 Data East Corporation (Euro) */
	DRIVER( dietgoe )	/* MAY (c) 1992 Data East Corporation (Euro) */
	DRIVER( dietgou )	/* MAY (c) 1992 Data East Corporation (US) */
	DRIVER( dietgoj )	/* MAY (c) 1992 Data East Corporation (Japan) */
	DRIVER( pktgaldx )      /* MAZ (c) 1992 Data East Corporation (Euro) */
	DRIVER( pktgaldj )      /* MAZ (c) 1993 Nihon System */
	/* MBA ?? */
	/* MBB ?? */
	/* MBC ?? */
	DRIVER( boogwing )	/* MBD (c) 1993 Data East Corporation (Euro) */
	DRIVER( boogwinga )	/* MBD (c) 1993 Data East Corporation (Asia) */
	DRIVER( ragtime )	/* MBD (c) 1993 Data East Corporation (Japan)*/
	DRIVER( ragtimea )	/* MBD (c) 1993 Data East Corporation (Japan)*/
	DRIVER( fghthist )	/* MBF (c) 1993 Data East Corporation (World) */
	DRIVER( fghthistu )	/* MBF (c) 1993 Data East Corporation (US) */
	DRIVER( fghthista )	/* MBF (c) 1993 Data East Corporation (US) */
	DRIVER( fghthistj )	/* MBF (c) 1993 Data East Corporation (Japan) */
	DRIVER( nslasher )	/* MBH (c) 1994 Data East Corporation  */
	DRIVER( nslasherj )	/* MBH (c) 1994 Data East Corporation  */
  DRIVER( nslasheru )	/* MBH (c) 1994 Data East Corporation  */
	/* MBI ?? */
	/* MBJ ?? */
	/* MBK ?? */
	/* MBL ?? */
	DRIVER( lockload )	/* MBM (c) 1994 Data East Corporation (US) */
	DRIVER( joemacr )	/* MBN (c) 1994 Data East Corporation  */
	DRIVER( joemacra )	/* MBN (c) 1994 Data East Corporation  */
	/* MBO ?? */
	/* MBP ?? */
	/* MBQ ?? */
	DRIVER( tattass )	/* prototype (c) 1994 Data East Pinball (US) */
	DRIVER( tattassa )	/* prototype (c) 1994 Data East Pinball (Asia) */
	/* MBS ?? */
	/* MBT ?? */
	/* MBU ?? */
	/* MBV ?? */
	/* MBW ?? */
	/* MBX ?? */
	/* MBY ?? */
	/* MCA ?? */
	/* Ganbare! Gonta!! 2/Lady Killer Part 2 - Party Time  MCB (c) 1995 Mitchell */
	DRIVER( chainrec )	/* MCC (c) 1994 Data East Corporation */
	/* MCD ?? */
	/* Dunk Dream 95/Hoops MCE (c) 1995 */
	/* MCF ?? */
	DRIVER( sotsugyo )	/* (c) 1995 Mitchell (Atlus license) */
	DRIVER( sshangha )	/* (c) 1992 Hot-B */
	DRIVER( sshanghb )	/* bootleg */

	/* Other Data East games not yet identified */
	DRIVER( candance )	/* (c) 1996 Mitchell */
	DRIVER( magdrop )	/* (c) 1995 Data East Corporation */
	DRIVER( magdropp )	/* (c) 1995 Data East Corporation */
	DRIVER( osman )		/* (c) 1996 Mitchell */
	DRIVER( gangonta )	/* (c) 1995 Mitchell */
	DRIVER( prtytime )	/* (c) 1995 Mitchell */

	/* Data East MLC Games */

	/* Tehkan / Tecmo games (Tehkan became Tecmo in 1986) */
	DRIVER( senjyo )	/* (c) 1983 Tehkan */
	DRIVER( starforc )	/* (c) 1984 Tehkan */
	DRIVER( starfore )	/* (c) 1984 Tehkan */
	DRIVER( megaforc )	/* (c) 1985 Tehkan + Video Ware license */
	DRIVER( baluba )	/* (c) 1986 Able Corp. */
	DRIVER( bombjack )	/* (c) 1984 Tehkan */
	DRIVER( bombjac2 )	/* (c) 1984 Tehkan */
	DRIVER( pbaction )	/* (c) 1985 Tehkan */
	DRIVER( pbactio2 )	/* (c) 1985 Tehkan */
	/* 6009 Tank Busters */
	/* 6011 Pontoon (c) 1985 Tehkan is a gambling game - removed */
	DRIVER( tehkanwc )	/* (c) 1985 Tehkan */
	DRIVER( gridiron )	/* (c) 1985 Tehkan */
	DRIVER( teedoff )	/* 6102 - (c) 1986 Tecmo */
	DRIVER( solomon )	/* (c) 1986 Tecmo */
	DRIVER( rygar )		/* 6002 - (c) 1986 Tecmo */
	DRIVER( rygar2 )	/* 6002 - (c) 1986 Tecmo */
	DRIVER( rygarj )	/* 6002 - (c) 1986 Tecmo */
	DRIVER( gemini )	/* (c) 1987 Tecmo */
  DRIVER( backfirt )	/* 6217 - (c) 1988 Tecmo */
	DRIVER( silkworm )	/* 6217 - (c) 1988 Tecmo */
	DRIVER( silkwrm2 )	/* 6217 - (c) 1988 Tecmo */
	DRIVER( tbowl )		/* 6206 - (c) 1987 Tecmo */
	DRIVER( tbowlj )	/* 6206 - (c) 1987 Tecmo */
	DRIVER( wc90 )		/* (c) 1989 Tecmo */
	DRIVER( wc90a )		/* (c) 1989 Tecmo */
	DRIVER( wc90t )		/* (c) 1989 Tecmo */
	DRIVER( wc90b )		/* bootleg */
	DRIVER( spbactn )	/* 9002 - (c) 1991 Tecmo */
	DRIVER( spbactnj )	/* 9002 - (c) 1991 Tecmo */
	DRIVER( fstarfrc )	/* (c) 1992 Tecmo */
	DRIVER( ginkun )	/* (c) 1995 Tecmo */
	DRIVER( riot )	        /* (c) 1992 NMK */
	DRIVER( deroon )    /* (c) 1996 Tecmo */
	DRIVER( deroon2 )   /* (c) 1996 Tecmo */
	DRIVER( tkdensho )  /* (c) 1996 Tecmo */
	DRIVER( tkdensha )  /* (c) 1996 Tecmo */

	/* Konami bitmap games */
	DRIVER( tutankhm )	/* GX350 (c) 1982 Konami */
	DRIVER( tutankst )	/* GX350 (c) 1982 Stern */
	DRIVER( junofrst )	/* GX310 (c) 1983 Konami */
	DRIVER( junofstg )	/* GX310 (c) 1983 Konami + Gottlieb license */

	/* Konami games */
	DRIVER( pooyan )	/* GX320 (c) 1982 */
	DRIVER( pooyans )	/* GX320 (c) 1982 Stern */
	DRIVER( pootan )	/* bootleg */
	DRIVER( timeplt )	/* GX393 (c) 1982 */
	DRIVER( timepltc )	/* GX393 (c) 1982 + Centuri license*/
	DRIVER( spaceplt )	/* bootleg */
	DRIVER( psurge )	/* (c) 1988 unknown (NOT Konami) */
	DRIVER( megazone )	/* GX319 (c) 1983 */
	DRIVER( megaznik )	/* GX319 (c) 1983 + Interlogic / Kosuka */
	DRIVER( pandoras )	/* GX328 (c) 1984 + Interlogic */
	DRIVER( gyruss )	/* GX347 (c) 1983 */
	DRIVER( gyrussce )	/* GX347 (c) 1983 + Centuri license */
	DRIVER( venus )		/* bootleg */
	DRIVER( trackfld )	/* GX361 (c) 1983 */
	DRIVER( trackflc )	/* GX361 (c) 1983 + Centuri license */
	DRIVER( hyprolym )	/* GX361 (c) 1983 */
	DRIVER( hyprolyb )	/* bootleg */
	DRIVER( whizquiz )	/* (c) 1985 Zilec-Zenitone */
	DRIVER( mastkin )	/* (c) 1988 Du Tech */
	DRIVER( rocnrope )	/* GX364 (c) 1983 */
	DRIVER( rocnropk )	/* GX364 (c) 1983 + Kosuka */
	DRIVER( circusc )	/* GX380 (c) 1984 */
	DRIVER( circusc2 )	/* GX380 (c) 1984 */
	DRIVER( circuscc )	/* GX380 (c) 1984 + Centuri license */
	DRIVER( circusce )	/* GX380 (c) 1984 + Centuri license */
	DRIVER( tp84 )		/* GX388 (c) 1984 */
	DRIVER( tp84a )		/* GX388 (c) 1984 */
	DRIVER( hyperspt )	/* GX330 (c) 1984 + Centuri */
	DRIVER( hpolym84 )	/* GX330 (c) 1984 */
	DRIVER( sbasketb )	/* GX405 (c) 1984 */
	DRIVER( sbasketo )	/* GX405 (c) 1984 */
	DRIVER( sbasketu )	/* GX405 (c) 1984 */
	DRIVER( mikie )		/* GX469 (c) 1984 */
	DRIVER( mikiej )	/* GX469 (c) 1984 */
	DRIVER( mikiehs )	/* GX469 (c) 1984 */
	DRIVER( roadf )		/* GX461 (c) 1984 */
	DRIVER( roadf2 )	/* GX461 (c) 1984 */
	DRIVER( yiear )		/* GX407 (c) 1985 */
	DRIVER( yiear2 )	/* GX407 (c) 1985 */
	DRIVER( kicker )	/* GX477 (c) 1985 */
	DRIVER( shaolins )	/* GX477 (c) 1985 */
	DRIVER( pingpong )	/* GX555 (c) 1985 */
	DRIVER( gberet )	/* GX577 (c) 1985 */
	DRIVER( rushatck )	/* GX577 (c) 1985 */
	DRIVER( gberetb )	/* bootleg on different hardware */
	DRIVER( mrgoemon )	/* GX621 (c) 1986 (Japan) */
	DRIVER( finalizr )	/* GX523 (c) 1985 */
	DRIVER( finalizb )	/* bootleg */
	DRIVER( ironhors )	/* GX560 (c) 1986 */
	DRIVER( dairesya )	/* GX560 (c) 1986 (Japan) */
	DRIVER( farwest )
	DRIVER( jackal )	/* GX631 (c) 1986 (World) */
	DRIVER( topgunr )	/* GX631 (c) 1986 (US) */
	DRIVER( jackalj )	/* GX631 (c) 1986 (Japan) */
	DRIVER( topgunbl )	/* bootleg */
	DRIVER( ddribble )	/* GX690 (c) 1986 */
	DRIVER( hcastle )	/* GX768 (c) 1988 */
	DRIVER( hcastleo )	/* GX768 (c) 1988 */
	DRIVER( hcastlej )	/* GX768 (c) 1988 (Japan) */
	DRIVER( hcastljo )	/* GX768 (c) 1988 (Japan) */

	/* Konami "Nemesis hardware" games */
	DRIVER( nemesis )	/* GX456 (c) 1985 */
	DRIVER( nemesuk )	/* GX456 (c) 1985 */
	DRIVER( konamigt )	/* GX561 (c) 1985 */
	DRIVER( salamand )	/* GX587 (c) 1986 */
	DRIVER( salamanj )	/* GX587 (c) 1986 */
	DRIVER( lifefrce )	/* GX587 (c) 1986 (US) */
	DRIVER( lifefrcj )	/* GX587 (c) 1986 (Japan) */
	DRIVER( blkpnthr )	/* GX604 (c) 1987 (Japan) */
	DRIVER( citybomb )	/* GX787 (c) 1987 (World) */
	DRIVER( citybmrj )	/* GX787 (c) 1987 (Japan) */
	DRIVER( kittenk )	/* GX712 (c) 1988 */
	DRIVER( nyanpani )	/* GX712 (c) 1988 (Japan) */
	DRIVER( hcrash )        /* GX790 (c) 1987 */
	DRIVER( hcrashc )       /* GX790 (c) 1987 */

	/* GX400 BIOS based games */
	DRIVER( rf2 )		/* GX561 (c) 1985 */
	DRIVER( twinbee )	/* GX412 (c) 1985 */
	DRIVER( gradius )	/* GX456 (c) 1985 */
	DRIVER( gwarrior )	/* GX578 (c) 1985 */

	/* Konami "Twin 16" games */

	/* (some) Konami 68000 games */

	/* Konami dual 68000 games */

	/* Konami 68020 games */

	/* Konami System GX games */

	/* GX Type 1 */

	/* GX Type 2 */

	/* GX Type 3 */

	/* GX Type 4 */

	/* DJ Main */

	/* Exidy games */
	DRIVER( carpolo )	/* (c) 1977 */
	DRIVER( sidetrac )	/* (c) 1979 */
	DRIVER( targ )		/* (c) 1980 */
	DRIVER( targc )		/* (c) 1980 */
	DRIVER( spectar )	/* (c) 1980 */
	DRIVER( spectar1 )	/* (c) 1980 */
	DRIVER( rallys )	/* (c) 1980 Novar (bootleg?) */
	DRIVER( phantoma )	/* (c) 1980 Jeutel (bootleg?) */
	DRIVER( venture )	/* (c) 1981 */
	DRIVER( venture2 )	/* (c) 1981 */
	DRIVER( venture4 )	/* (c) 1981 */
	DRIVER( mtrap )		/* (c) 1981 */
	DRIVER( mtrap3 )	/* (c) 1981 */
	DRIVER( mtrap4 )	/* (c) 1981 */
	DRIVER( pepper2 )	/* (c) 1982 */
	DRIVER( hardhat )	/* (c) 1982 */
	DRIVER( fax )		/* (c) 1983 */
	DRIVER( circus )	/* no copyright notice [1977?] */
	DRIVER( robotbwl )	/* no copyright notice */
	DRIVER( crash )		/* Exidy [1979?] */
	DRIVER( ripcord )	/* Exidy [1977?] */
	DRIVER( starfire )	/* Exidy [1979?] */
	DRIVER( starfira )	/* Exidy [1979?] */
	DRIVER( fireone )	/* (c) 1979 Exidy */
	DRIVER( starfir2 )	/* (c) 1979 Exidy */
	DRIVER( victory )	/* (c) 1982 */
	DRIVER( victorba )	/* (c) 1982 */
	DRIVER( teetert )	/* (c) 1982 Exidy */

	/* Exidy 440 games */
	DRIVER( crossbow )	/* (c) 1983 */
	DRIVER( cheyenne )	/* (c) 1984 */
	DRIVER( combat )	/* (c) 1985 */
	DRIVER( catch22 )	/* (c) 1985 */
	DRIVER( cracksht )	/* (c) 1985 */
	DRIVER( claypign )	/* (c) 1986 */
	DRIVER( chiller )	/* (c) 1986 */
	DRIVER( topsecex )	/* (c) 1986 */
	DRIVER( hitnmiss )	/* (c) 1987 */
	DRIVER( hitnmis2 )	/* (c) 1987 */
	DRIVER( whodunit )	/* (c) 1988 */
	DRIVER( showdown )	/* (c) 1988 */

	/* Atari b/w games */
	/* Tank 8 */  		/* ??????			1976/04 [6800] */
	DRIVER( copsnrob )	/* 005625			1976/07 [6502] */
	DRIVER( flyball )	/* 005629			1976/07 [6502] */
	DRIVER( sprint2 )	/* 005922			1976/11 [6502] */
	DRIVER( sprint2a )	/* 005922			1976/11 [6502] */
	DRIVER( sprint4 )	/* 008716			1977/12 [6502] */
	DRIVER( sprint4a )	/* 008716			1977/12 [6502] */
	DRIVER( nitedrvr )	/* 006321			1976/10 [6502] */
	DRIVER( dominos )	/* 007305			1977/01 [6502] */
	DRIVER( dragrace )	/* 008505-008521	1977/06 [6800] */
	DRIVER( poolshrk )	/* 006281			1977/06 [6800] */
	DRIVER( starshp1 )	/* 007513-007531	1977/07 [6502] */
	DRIVER( starshpp )	/* 007513-007531	1977/07 [6502] */
	DRIVER( destroyr )	/* 030131-030136	1977/10 [6800] */
	DRIVER( sprint1 )	/* 006443			1978/01 [6502] */
	DRIVER( ultratnk )	/* 009801			1978/02 [6502] */
	DRIVER( skyraid )	/* 009709			1978/03 [6502] */
	DRIVER( tourtabl )	/* 030170			1978/03 [6507] */
	DRIVER( tourtab2 )	/* 030170			1978/03 [6507] */
	DRIVER( avalnche )	/* 030574			1978/04 [6502] */
	/* Smokey Joe */	/* 030926			1978/07 [6502] */
	DRIVER( sbrkout )	/* 033442-033455	1978/09 [6502] */
	DRIVER( orbit )		/* 033689-033702	1978/11 [6800] */
	DRIVER( boxer )		/* ?????? prototype 1978    [6502] */
	DRIVER( wolfpack )	/* ?????? prototype         [6502] */
	DRIVER( subs )		/* 033714			1979/05 [6502] */
	DRIVER( bsktball )	/* 034756-034766	1979/05 [6502] */

	/* Atari "Missile Command hardware" games */
	DRIVER( missile )	/* 035820-035825	(c) 1980 */
	DRIVER( missile2 )	/* 035820-035825	(c) 1980 */
	DRIVER( suprmatk )	/* 					(c) 1980 + (c) 1981 Gencomp */

	/* Atari vector games */
	DRIVER( spacduel )	/* 136006			(c) 1980 */
	DRIVER( gravitar )	/* 136010			(c) 1982 */
	DRIVER( gravitr2 )	/* 136010			(c) 1982 */
	DRIVER( gravp )		/* (proto)			(c) 1982 */
	DRIVER( lunarbat )	/* (proto)			(c) 1982 */
	DRIVER( lunarba1 )	/* (proto)			(c) 1982 */
	DRIVER( quantum )	/* 136016			(c) 1982 */	/* made by Gencomp */
	DRIVER( quantum1 )	/* 136016			(c) 1982 */	/* made by Gencomp */
	DRIVER( quantump )	/* 136016			(c) 1982 */	/* made by Gencomp */
	DRIVER( bwidow )	/* 136017			(c) 1982 */
	DRIVER( starwars )	/* 136021			(c) 1983 */
	DRIVER( starwar1 )	/* 136021			(c) 1983 */
	DRIVER( mhavoc )	/* 136025			(c) 1983 */
	DRIVER( mhavoc2 )	/* 136025			(c) 1983 */
	DRIVER( mhavocp )	/* 136025			(c) 1983 */
	DRIVER( mhavocrv )	/* (hack) */
	DRIVER( alphaone )	/* (proto)          (c) 1983 */
	DRIVER( alphaona )	/* (proto)          (c) 1983 */
	DRIVER( esb )		/* 136031			(c) 1985 */

	/* Atari "Centipede hardware" games */

	/* misc Atari games */
	DRIVER( ccastles )	/* 136022			(c) 1983 */
	DRIVER( ccastle3 )	/* 136022			(c) 1983 */
	DRIVER( ccastle2 )	/* 136022			(c) 1983 */
	DRIVER( ccastlej )	/* 136022			(c) 1983 */
	DRIVER( cloak )		/* 136023			(c) 1983 */
	DRIVER( cloaksp )	/* 136023			(c) 1983 */
	DRIVER( cloakfr )	/* 136023			(c) 1983 */
	DRIVER( cloakgr )	/* 136023			(c) 1983 */
	DRIVER( cloud9 )	/* (proto)			(c) 1983 */
	DRIVER( jedi )		/* 136030			(c) 1984 */

	/* Atari System 1 games */
	DRIVER( peterpak )	/* 136028			(c) 1984 */
	/* Marble Madness */
	DRIVER( marble )	/* 136033			(c) 1984 */
	DRIVER( marble2 )	/* 136033			(c) 1984 */
	DRIVER( marble3 )	/* 136033			(c) 1984 */
	DRIVER( marble4 )	/* 136033			(c) 1984 */
	/* Indiana Jones and the Temple of Doom */
	DRIVER( indytemp )	/* 136036			(c) 1985 */
	DRIVER( indytem2 )	/* 136036			(c) 1985 */
	DRIVER( indytem3 )	/* 136036			(c) 1985 */
	DRIVER( indytem4 )	/* 136036			(c) 1985 */
	DRIVER( indytemd )	/* 136036           (c) 1985 */
	/* Road Runner */
	DRIVER( roadrunn )	/* 136040			(c) 1985 */
	DRIVER( roadrun2 )	/* 136040			(c) 1985 */
	DRIVER( roadrun1 )	/* 136040			(c) 1985 */
	/* Road Blasters */
	DRIVER( roadblst )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadblsg )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadbls3 )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadblg2 )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadbls2 )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadblg1 )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadbls1 )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadblsc )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadblcg )	/* 136048			(c) 1986, 1987 */
	DRIVER( roadblc1 )	/* 136048			(c) 1986, 1987 */

	/* Atari System 2 games */
	/* Paperboy */
	/* Super Sprint */
	/* Championship Sprint */
	/* 720 Degrees */
	/* APB. */

	/* Atari polygon games */
	DRIVER( irobot )	/* 136029			(c) 1983 */
	/* Hard Drivin' */
	/* Hard Drivin' Compact */
	/* Stun Runner */
	/* Race Drivin' */
	/* Race Drivin' Compact */
	/* Steel Talons */
	/* Hard Drivin' Airbourne */

	/* later Atari games */

	/* Gauntlet Hardware */
	/* Gauntlet */
	DRIVER( gauntlet )	/* 136037			(c) 1985 */
	DRIVER( gaunts )	/* 136037			(c) 1985 */
	DRIVER( gauntj )	/* 136037			(c) 1985 */
	DRIVER( gauntg )	/* 136037			(c) 1985 */
	DRIVER( gauntj12 )	/* 136037			(c) 1985 */
	DRIVER( gauntr9 )	/* 136037			(c) 1985 */
	DRIVER( gauntgr8 )	/* 136037			(c) 1985 */
	DRIVER( gauntr7 )	/* 136037			(c) 1985 */
	DRIVER( gauntgr6 )	/* 136037			(c) 1985 */
	DRIVER( gauntr5 )	/* 136037			(c) 1985 */
	DRIVER( gauntr4 )	/* 136037			(c) 1985 */
	DRIVER( gauntgr3 )	/* 136037			(c) 1985 */
	DRIVER( gauntr2 )	/* 136037			(c) 1985 */
	DRIVER( gauntr1 )	/* 136037			(c) 1985 */
	/* Gauntlet - 2 Player */
	DRIVER( gaunt2p )	/* 136037			(c) 1985 */
	DRIVER( gaunt2pj )	/* 136037			(c) 1985 */
	DRIVER( gaunt2pg )	/* 136037			(c) 1985 */
	DRIVER( gaun2pr3 )	/* 136037			(c) 1985 */
	DRIVER( gaun2pj2 )	/* 136037			(c) 1985 */
	DRIVER( gaun2pg1 )	/* 136037			(c) 1985 */
	/* Gauntlet 2 */
	DRIVER( gaunt2 )	/* 136043			(c) 1986 */
	DRIVER( gaunt2g )	/* 136043			(c) 1986 */
	/* Gauntlet 2 - 2 Player */
	DRIVER( gaunt22p )	/* 136043			(c) 1986 */
	DRIVER( gaun22p1 )	/* 136043			(c) 1986 */
	DRIVER( gaun22pg )	/* 136043			(c) 1986 */
	/* Vindicators Part II */
	DRIVER( vindctr2 )	/*     ??			(c) 1988 */
	DRIVER( vindc2r2 )	/*     ??			(c) 1988 */
	DRIVER( vindc2r1 )	/*     ??			(c) 1988 */

	/* Other Hardware */

	/* Xybots */
	DRIVER( xybots )	/* 136054			(c) 1987 */
	DRIVER( xybotsg )	/* 136054			(c) 1987 */
	DRIVER( xybotsf )	/* 136054			(c) 1987 */
	DRIVER( xybots1 )	/* 136054			(c) 1987 */
	DRIVER( xybots0 )	/* 136054			(c) 1987 */
	/* Blasteroids */
	DRIVER( blstroid )	/* 136057			(c) 1987 */
	DRIVER( blstroi3 )	/* 136057			(c) 1987 */
	DRIVER( blstroi2 )	/* 136057			(c) 1987 */
	DRIVER( blstroig )	/* 136057			(c) 1987 */
	DRIVER( blsthead )	/* (proto)			(c) 1987 */
	/* Vindicators */
	DRIVER( vindictr )	/* 136059			(c) 1988 */
	DRIVER( vindicte )	/* 136059			(c) 1988 */
	DRIVER( vindictg )	/* 136059			(c) 1988 */
	DRIVER( vindice4 )	/* 136059			(c) 1988 */
	DRIVER( vindict4 )	/* 136059			(c) 1988 */
	DRIVER( vindice3 )	/* 136059			(c) 1988 */
	DRIVER( vindict2 )	/* 136059			(c) 1988 */
	DRIVER( vindict1 )	/* 136059			(c) 1988 */
	/* Toobin */
	DRIVER( toobin )	/* 136061			(c) 1988 */
	DRIVER( toobine )	/* 136061			(c) 1988 */
	DRIVER( toobing )	/* 136061			(c) 1988 */
	DRIVER( toobin2 )	/* 136061			(c) 1988 */
	DRIVER( toobin2e )	/* 136061			(c) 1988 */
	DRIVER( toobin1 )	/* 136061			(c) 1988 */
	/* Cyberball */
	/* Atari Tetris */
	DRIVER( atetcktl )	/* 136066			(c) 1989 */
	DRIVER( atetckt2 )	/* 136066			(c) 1989 */
	DRIVER( atetris )	/* 136066			(c) 1988 */
	DRIVER( atetrisa )	/* 136066			(c) 1988 */
	DRIVER( atetrisb )	/* (bootleg) */
	/* Escape from the Planet of Robot Monsters */
	DRIVER( eprom )		/* 136069			(c) 1989 */
	DRIVER( eprom2 )	/* 136069			(c) 1989 */
  DRIVER( guts )	  /* prototype */
	/* Skull and Crossbones */
	DRIVER( skullxbo )	/* 136072			(c) 1989 */
	DRIVER( skullxb4 )	/* 136072			(c) 1989 */
	DRIVER( skullxb3 )	/* 136072			(c) 1989 */
	DRIVER( skullxb2 )	/* 136072			(c) 1989 */
	DRIVER( skullxb1 )	/* 136072			(c) 1989 */
	/* Cyberball Tournament */

	DRIVER( badlands )	/* 136074			(c) 1989 */
	DRIVER( klax )		/* 136075			(c) 1989 */
	DRIVER( klax2 )		/* 136075			(c) 1989 */
	DRIVER( klax3 )		/* 136075			(c) 1989 */
	DRIVER( klaxj )		/* 136075			(c) 1989 (Japan) */
	DRIVER( klaxd )		/* 136075			(c) 1989 (Germany) */
	DRIVER( klaxp1 )	/* prototype */
	DRIVER( klaxp2 )	/* prototype */
	DRIVER( thunderj )	/* 136076			(c) 1990 */

	/* Cyberball 2 Player */

	DRIVER( rampart )	/* 136082			(c) 1990 */
	DRIVER( ramprt2p )	/* 136082			(c) 1990 */
	DRIVER( rampartj )	/* 136082			(c) 1990 (Japan) */
	DRIVER( shuuz )		/* 136083			(c) 1990 */
	DRIVER( shuuz2 )	/* 136083			(c) 1990 */
	DRIVER( batman )	/* 136085			(c) 1991 */
	DRIVER( marblmd2 )
	DRIVER( offtwall )	/* 136090			(c) 1991 */
	DRIVER( offtwalc )	/* 136090			(c) 1991 */
	DRIVER( relief )	/* 136093			(c) 1992 */
	DRIVER( relief2 )	/* 136093			(c) 1992 */
	DRIVER( arcadecl )	/* (proto)			(c) 1992 */
	DRIVER( sparkz )	/* (proto)			(c) 1992 */
	DRIVER( beathead )	/* (proto)			(c) 1993 */
	DRIVER( area51 )	/* 136105			(c) 1995 */
	DRIVER( area51mx )	/* 136105			(c) 1998 */
	DRIVER( a51mxr3k )	/*     ??           (c) 1998 */
	DRIVER( maxforce )	/*     ??			(c) 1996 */
	DRIVER( maxf_102 )	/*     ??			(c) 1996 */
	DRIVER( vcircle )	/*     ??			(c) 1996 */

	/* SNK / Rock-ola games */
	DRIVER( sasuke )	/* [1980] Shin Nihon Kikaku (SNK) */
	DRIVER( satansat )	/* (c) 1981 SNK */
	DRIVER( zarzon )	/* (c) 1981 Taito, gameplay says SNK */
	DRIVER( vanguard )	/* (c) 1981 SNK */
	DRIVER( vangrdce )	/* (c) 1981 SNK + Centuri */
	DRIVER( fantasy )	/* (c) 1981 Rock-ola */
	DRIVER( fantasyj )	/* (c) 1981 SNK */
	DRIVER( pballoon )	/* (c) 1982 SNK */
	DRIVER( nibbler )	/* (c) 1982 Rock-ola */
	DRIVER( nibblera )	/* (c) 1982 Rock-ola */

	/* later SNK games, each game can be identified by PCB code and ROM
	code, the ROM code is the same between versions, and usually based
	upon the Japanese title. */
	DRIVER( lasso )		/*       'WM' (c) 1982 */
	DRIVER( chameleo )	/* (c) 1983 Jaleco */
	DRIVER( wwjgtin )	/* (c) 1984 Jaleco / Casio */
	DRIVER( pinbo )		/* (c) 1984 Jaleco */
	DRIVER( pinbos )	/* (c) 1985 Strike */
	DRIVER( joyfulr )	/* A2001      (c) 1983 */
	DRIVER( mnchmobl )	/* A2001      (c) 1983 + Centuri license */
	DRIVER( marvins )	/* A2003      (c) 1983 */
	DRIVER( madcrash )	/* A2005      (c) 1984 */
	DRIVER( vangrd2 )	/*            (c) 1984 */
	DRIVER( sgladiat )	/* A3006      (c) 1984 */
	DRIVER( hal21 )		/*            (c) 1985 */
	DRIVER( hal21j )	/*            (c) 1985 (Japan) */
	DRIVER( aso )		/*            (c) 1985 */
	DRIVER( tnk3 )		/* A5001      (c) 1985 */
	DRIVER( tnk3j )		/* A5001      (c) 1985 */
	DRIVER( tnk3b )		/* bootleg    (c) 1985 */
	DRIVER( athena )	/*       'UP' (c) 1986 */
	DRIVER( fitegolf )	/*       'GU' (c) 1988 */
	DRIVER( fitegol2 )	/*       'GU' (c) 1988 */
	DRIVER( countryc )	/*       'CC' (c) 1988 */
	DRIVER( ikari )		/* A5004 'IW' (c) 1986 */
	DRIVER( ikarijp )	/* A5004 'IW' (c) 1986 (Japan) */
	DRIVER( ikarijpb )	/* bootleg */
	DRIVER( victroad )	/*            (c) 1986 */
	DRIVER( dogosoke )	/*            (c) 1986 */
	DRIVER( dogosokj )      /* bootleg */
	DRIVER( gwar )		/* A7003 'GV' (c) 1987 */
	DRIVER( gwarj )		/* A7003 'GV' (c) 1987 (Japan) */
	DRIVER( gwara )		/* A7003 'GV' (c) 1987 */
	DRIVER( gwarb )		/* bootleg */
	DRIVER( bermudat )	/* A6003 'WW' (c) 1987 */
	DRIVER( bermudao )	/* A6003 'WW' (c) 1987 */
	DRIVER( bermudaa )	/* A6003 'WW' (c) 1987 */
	DRIVER( worldwar )	/* A6003 'WW' (c) 1987 */
	DRIVER( psychos )	/*       'PS' (c) 1987 */
	DRIVER( psychosj )	/*       'PS' (c) 1987 (Japan) */
	DRIVER( chopper )	/* A7003 'KK' (c) 1988 */
	DRIVER( choppera )	/* A7003 'KK' (c) 1988 */
	DRIVER( chopperb )	/* A7003 'KK' (c) 1988 */
	DRIVER( legofair )	/* A7003 'KK' (c) 1988 */
	DRIVER( ftsoccer )	/*            (c) 1988 */
	DRIVER( tdfever )	/* A6006 'TD' (c) 1987 */
	DRIVER( tdfeverj )	/* A6006 'TD' (c) 1987 */
	DRIVER( tdfever2 )	/* A6006 'TD'?(c) 1988 */
	DRIVER( ikari3 )	/* A7007 'IK3'(c) 1989 */
	DRIVER( ikari3nr )	/* A7007 'IK3'(c) 1989 */
	DRIVER( pow )		/* A7008 'DG' (c) 1988 */
	DRIVER( powj )		/* A7008 'DG' (c) 1988 */
	DRIVER( searchar )	/* A8007 'BH' (c) 1989 */
	DRIVER( sercharu )	/* A8007 'BH' (c) 1989 */
	DRIVER( streetsm )	/* A8007 'S2' (c) 1989 */
	DRIVER( streets1 )	/* A7008 'S2' (c) 1989 */
	DRIVER( streetsw )	/*            (c) 1989 */
	DRIVER( streetsj )	/* A8007 'S2' (c) 1989 */
	DRIVER( mechatt )	/* A8002 'MA' (c) 1989 */
	DRIVER( bbusters )	/* A9003 'BB' (c) 1989 */

	/* Alpha Denshi games */
	DRIVER( champbas )	/* (c) 1983 Sega */
	DRIVER( champbbj )	/* (c) 1983 Alpha Denshi Co. */
	DRIVER( champbja )	/* (c) 1983 Alpha Denshi Co. */
	DRIVER( champbb2 )	/* (c) 1983 Sega */
	DRIVER( exctsccr )	/* (c) 1983 Alpha Denshi Co. */
	DRIVER( exctscca )	/* (c) 1983 Alpha Denshi Co. */
	DRIVER( exctsccb )	/* bootleg */
	DRIVER( exctscc2 )	/* (c) 1984 Alpha Denshi Co. */
	DRIVER( equites )	/* (c) 1984 Alpha Denshi Co. */
	DRIVER( equitess )	/* (c) 1984 Alpha Denshi Co./Sega */
	DRIVER( bullfgtr )	/* (c) 1984 Alpha Denshi Co./Sega */
	DRIVER( kouyakyu )	/* (c) 1985 Alpha Denshi Co. */
	DRIVER( splndrbt )	/* (c) 1985 Alpha Denshi Co. */
	DRIVER( hvoltage )	/* (c) 1985 Alpha Denshi Co. */

	/* SNK / Alpha 68K games */

	/* Technos games */
	DRIVER( scregg )	/* TA-0001 (c) 1983 */
	DRIVER( eggs )		/* TA-0002 (c) 1983 Universal USA */
	DRIVER( dommy )		/* TA-00?? (c) */
	DRIVER( bigprowr )	/* TA-0007 (c) 1983 */
	DRIVER( tagteam )	/* TA-0007 (c) 1983 + Data East license */
	DRIVER( ssozumo )	/* TA-0008 (c) 1984 */
	DRIVER( dogfgt )	/* TA-0011 (c) 1984 */
	DRIVER( dogfgtj )	/* TA-0011 (c) 1984 */
	DRIVER( bogeyman )	/* -0204-0 (Data East part number) (c) [1985?] */
	DRIVER( xsleena )	/* TA-0019 (c) 1986 */
	DRIVER( xsleenab )	/* bootleg */
	DRIVER( solarwar )	/* TA-0019 (c) 1986 Taito + Memetron license */
	DRIVER( battlane )	/* -0215, -0216 (Data East part number) (c) 1986 + Taito license */
	DRIVER( battlan2 )	/* -0215, -0216 (Data East part number) (c) 1986 + Taito license */
	DRIVER( battlan3 )	/* -0215, -0216 (Data East part number) (c) 1986 + Taito license */
	DRIVER( spdodgeb )	/* TA-0022 (c) 1987 */
	DRIVER( nkdodgeb )	/* TA-0022 (c) 1987 (Japan) */
	DRIVER( saiyugb1 )	/* bootleg */
	DRIVER( wwfsstar )	/* TA-0024 (c) 1989 (US) */
	DRIVER( vball )		/* TA-0025 (c) 1988 */
	DRIVER( vball2pj )	/* TA-0025 (c) 1988 (Japan) */
	DRIVER( toffy )		/* (c) 1993 Midas */
	DRIVER( stoffy )	/* (c) 1994 Midas + Unico */
	DRIVER( ddungeon )	/* Game Room */
	DRIVER( darktowr )	/* Game Room */
	DRIVER( tstrike )	/* Game Room */
	DRIVER( blockout )	/* TA-0029 (c) 1989 + California Dreams */
	DRIVER( blckout2 )	/* TA-0029 (c) 1989 + California Dreams */
	DRIVER( blckoutj )	/* TA-0029 (c) 1989 + California Dreams (Japan) */
	DRIVER( wwfwfest )	/* TA-0031 (c) 1991 (US) */
	DRIVER( wwfwfsta )	/* TA-0031 (c) 1991 + Tecmo license (US) */
	DRIVER( wwfwfstj )	/* TA-0031 (c) 1991 (Japan) */
	DRIVER( shadfrce )	/* TA-0032 (c) 1993 (US) */

	/* Stern "Berzerk hardware" games */
	DRIVER( berzerk )	/* (c) 1980 */
	DRIVER( berzerk1 )	/* (c) 1980 */
	DRIVER( frenzy )	/* (c) 1982 */

	/* Stern 'Mazer Blazer hardware' games */

	/* Other Stern */
	DRIVER( supdrapo )

	/* GamePlan games */
	DRIVER( toratora )	/* (c) 1980 Game Plan */
	DRIVER( megatack )	/* (c) 1980 Centuri */
	DRIVER( killcom )	/* (c) 1980 Centuri */
	DRIVER( challeng )	/* (c) 1981 Centuri */
	DRIVER( kaos )		/* (c) 1981 */

	/* Zaccaria games */
	DRIVER( sia2650 )	/* (c) 1978 */
	DRIVER( tinv2650 )	/* (c) 1978 */
	DRIVER( dodgem )	/* (c) 1979 */
	DRIVER( monymony )	/* (c) 1983 */
	DRIVER( jackrabt )	/* (c) 1984 */
	DRIVER( jackrab2 )	/* (c) 1984 */
	DRIVER( jackrabs )	/* (c) 1984 */

	/* UPL games */
	DRIVER( mouser )	/* UPL-83001 (c) 1983 */
	DRIVER( mouserc )	/* UPL-83001 (c) 1983 */
	DRIVER( nova2001 )	/* UPL-83005 (c) 1983 */
	DRIVER( nov2001u )	/* UPL-83005 (c) [1983] + Universal license */
	DRIVER( ninjakun )	/* UPL-84003 (c) 1984 Taito Corporation */
	DRIVER( raiders5 )	/* UPL-85004 (c) 1985 */
	DRIVER( raidrs5t )
	DRIVER( pkunwar )	/* UPL-????? [1985?] */
	DRIVER( pkunwarj )	/* UPL-????? [1985?] */
	DRIVER( xxmissio )	/* UPL-86001 [1986] */
	DRIVER( mnight )	/* UPL-????? (c) 1987 distributed by Kawakus */
	DRIVER( arkarea )	/* UPL-87007 (c) [1988?] */

	/* UPL/NMK/Banpresto games */

	/* Jaleco Mahjong Games, Similar Hardware to the NMK ones above? */
	DRIVER( daireika )
	DRIVER( mjzoomin )
	DRIVER( kakumei )
	DRIVER( kakumei2 )

	/* Based on nmk16? */

	/* don't know what hardare Banpresto used for these games */
	DRIVER( macrossp )	/* (c) 1996 Banpresto */
	DRIVER( quizmoon )	/* (c) 1997 Banpresto */

	/* Face/NMK games */
	DRIVER( gakupara )	/* (c) 1991 NMK */
	DRIVER( quizdna )	/* (c) 1992 Face */
	DRIVER( gekiretu )	/* (c) 1992 Face */

	/* Williams/Midway games */
	DRIVER( saurnfrnt )	/* (c) 1991 Williams */
	DRIVER( mk )		/* (c) 1992 Midway */
	DRIVER( mkr4 )		/* (c) 1992 Midway */
	DRIVER( jdreddp )	/* (c) 1993 Midway */

	/* Midway / Atari  Voodoo based Hardware (Seattle, Flagstaff) */

	/* Cinematronics raster games */
	DRIVER( tripool )	/* (c) 1981 Noma (Casino Tech license) */
	DRIVER( tripoola )	/* (c) 1981 Noma (Costal Games license) */
	DRIVER( jack )		/* (c) 1982 Cinematronics */
	DRIVER( jack2 )		/* (c) 1982 Cinematronics */
	DRIVER( jack3 )		/* (c) 1982 Cinematronics */
	DRIVER( treahunt )	/* (c) 1982 Hara Ind. */
	DRIVER( zzyzzyxx )	/* (c) 1982 Cinematronics + Advanced Microcomputer Systems */
	DRIVER( zzyzzyx2 )	/* (c) 1982 Cinematronics + Advanced Microcomputer Systems */
	DRIVER( brix )		/* (c) 1982 Cinematronics + Advanced Microcomputer Systems */
	DRIVER( freeze )	/* Cinematronics */
	DRIVER( sucasino )	/* (c) 1982 Data Amusement */

	/* Cinematronics vector games */
	/* this one uses 68000+Z80 instead of the Cinematronics CPU */

	/* "The Pit hardware" games */
	DRIVER( roundup )	/* (c) 1981 Amenip/Centuri */
	DRIVER( fitter )	/* (c) 1981 Taito */
	DRIVER( thepit )	/* (c) 1982 Centuri */
	DRIVER( dockman )	/* (c) 1982 Taito Corp. */
	DRIVER( portman )	/* (c) 1982 Nova Games Ltd. */
	DRIVER( funnymou )	/* (c) 1982 Chuo Co. Ltd */
	DRIVER( suprmous )	/* (c) 1982 Taito */
	DRIVER( machomou )	/* (c) 1982 Techstar */
	DRIVER( intrepid )	/* (c) 1983 Nova Games Ltd. */
	DRIVER( intrepi2 )	/* (c) 1983 Nova Games Ltd. */
	DRIVER( zaryavos )	/* (c) 1983 Nova Games of Canada (prototype) */

	/* Valadon Automation games */
	DRIVER( tankbust )	/* (c) 1985 */

	/* Seibu Denshi / Seibu Kaihatsu games */
	DRIVER( stinger )	/* (c) 1983 Seibu Denshi */
	DRIVER( stinger2 )	/* (c) 1983 Seibu Denshi */
	DRIVER( scion )		/* (c) 1984 Seibu Denshi */
	DRIVER( scionc )	/* (c) 1984 Seibu Denshi + Cinematronics license */
	DRIVER( kungfut )	/* (c) 1984 Seibu Kaihatsu */
	DRIVER( wiz )		/* (c) 1985 Seibu Kaihatsu */
	DRIVER( wizt )		/* (c) 1985 Taito Corporation */
	DRIVER( empcity )	/* (c) 1986 Seibu Kaihatsu (bootleg?) */
	DRIVER( empcityj )	/* (c) 1986 Taito Corporation (Japan) */
	DRIVER( stfight )	/* (c) 1986 Seibu Kaihatsu (Germany) (bootleg?) */
	DRIVER( cshooter )	/* (c) 1987 Taito */
	DRIVER( cshootre )
	DRIVER( airraid )
	DRIVER( deadang )	/* (c) 1988 Seibu Kaihatsu */
	DRIVER( ghunter )	/* (c) 1988 Seibu Kaihatsu + Segasa/Sonic license */
	DRIVER( dynduke )	/* (c) 1989 Seibu Kaihatsu + Fabtek license */
	DRIVER( dbldyn )	/* (c) 1989 Seibu Kaihatsu + Fabtek license */
	DRIVER( raiden )	/* (c) 1990 Seibu Kaihatsu */
	DRIVER( raidena )	/* (c) 1990 Seibu Kaihatsu */
	DRIVER( raidenk )	/* (c) 1990 Seibu Kaihatsu + IBL Corporation license */
	DRIVER( raident )	/* (c) 1990 Seibu Kaihatsu + Liang HWA Electronics license */
	DRIVER( sdgndmps )	/* (c) Banpresto / Bandai (Japan) */
	DRIVER( dcon )		/* (c) 1992 Success */
	DRIVER( sengokmj )	/* (c) 1991 Sigma */


	DRIVER( raiden2 )

/* Seibu STI System games:
	Viper: Phase 1 					(c) 1995
	Viper: Phase 1 (New version)	(c) 1996
	Battle Balls					(c) 1996
	Raiden Fighters					(c) 1996
	Raiden Fighters 2 				(c) 1997
	Senku							(c) 1997
*/

	/* Tad games (Tad games run on Seibu hardware) */
	DRIVER( cabal )	        /* (c) 1988 Tad (World) */
	DRIVER( cabala )	/* (c) 1988 Tad + Alpha Trading license */
	DRIVER( cabalus )	/* (c) 1988 Tad + Fabtek license */
	DRIVER( cabalus2 )	/* (c) 1988 Tad + Fabtek license */
	DRIVER( cabalbl )	/*  bootleg  */
	DRIVER( toki )		/* (c) 1989 Tad (World) */
	DRIVER( tokia )		/* (c) 1989 Tad (World) */
	DRIVER( tokij )		/* (c) 1989 Tad (Japan) */
	DRIVER( tokiu )		/* (c) 1989 Tad + Fabtek license (US) */
	DRIVER( tokib )		/* bootleg */
	DRIVER( bloodbro )	/* (c) 1990 Tad */
	DRIVER( weststry )	/* bootleg */
	DRIVER( skysmash )	/* (c) 1990 Nihon System Inc. */
	DRIVER( legionna )	/* (c) 1992 Tad (World) */
	DRIVER( legionnu )	/* (c) 1992 Tad + Fabtek license (US) */
	DRIVER( heatbrl )	/* (c) 1992 Tad (World) */
	DRIVER( heatbrlo )	/* (c) 1992 Tad (World) */
	DRIVER( heatbrlu )	/* (c) 1992 Tad (US) */
	DRIVER( godzilla )	/* (c) 1993 Banpresto */
	DRIVER( sdgndmrb )	/* (c) 1993 Banpresto */
	DRIVER( cupsoc )	/* (c) 1992 Seibu */
	DRIVER( cupsoc2 )	/* (c) 1992 Seibu */
	DRIVER( olysoc92 )	/* (c) 1992 Seibu */

	/* Jaleco games */
	DRIVER( exerion )	/* (c) 1983 Jaleco */
	DRIVER( exeriont )	/* (c) 1983 Jaleco + Taito America license */
	DRIVER( exerionb )	/* bootleg */
	DRIVER( fcombat )	/* (c) 1985 Jaleco */
	DRIVER( citycon )	/* (c) 1985 Jaleco */
	DRIVER( citycona )	/* (c) 1985 Jaleco */
	DRIVER( cruisin )	/* (c) 1985 Jaleco/Kitkorp */
	DRIVER( momoko )	/* (c) 1986 Jaleco */
	DRIVER( argus )		/* (c) 1986 Jaleco */
	DRIVER( valtric )	/* (c) 1986 Jaleco */
	DRIVER( butasan )	/* (c) 1987 Jaleco */
	DRIVER( ginganin )	/* (c) 1987 Jaleco */
	DRIVER( skyfox )	/* (c) 1987 Jaleco + Nichibutsu USA license */
	DRIVER( exerizrb )	/* bootleg */
	DRIVER( bigrun )	/* (c) 1989 Jaleco */
	DRIVER( cischeat )	/* (c) 1990 Jaleco */
	DRIVER( f1gpstar )	/* (c) 1991 Jaleco */
	DRIVER( f1gpstr2 )	/* (c) 1993 Jaleco */
	DRIVER( scudhamm )	/* (c) 1994 Jaleco */
	DRIVER( tetrisp2 )	/* (c) 1997 Jaleco */
	DRIVER( teplus2j )	/* (c) 1997 Jaleco */
	DRIVER( rockn )		/* (c) 1999 Jaleco */
	DRIVER( rockna )        /* (c) 1999 Jaleco */
	DRIVER( rockn2 )        /* (c) 1999 Jaleco */
	DRIVER( rockn3 )        /* (c) 1999 Jaleco */
	DRIVER( rockn4 )        /* (c) 2000 Jaleco */

	/* Jaleco Mega System 1 games */
	DRIVER( lomakai )	/* (c) 1988 (World) */
	DRIVER( makaiden )	/* (c) 1988 (Japan) */
	DRIVER( p47 )		/* (c) 1988 */
	DRIVER( p47j )		/* (c) 1988 (Japan) */
	DRIVER( kickoff )	/* (c) 1988 (Japan) */
	DRIVER( tshingen )	/* (c) 1988 (Japan) */
	DRIVER( tshingna )	/* (c) 1988 (Japan) */
  DRIVER( kazan )	    /* (c) 1988 (World) */
	DRIVER( iganinju )	/* (c) 1988 (Japan) */
	DRIVER( astyanax )	/* (c) 1989 */
	DRIVER( lordofk )	/* (c) 1989 (Japan) */
	DRIVER( hachoo )	/* (c) 1989 */
	DRIVER( jitsupro )	/* (c) 1989 (Japan) */
	DRIVER( plusalph )	/* (c) 1989 */
	DRIVER( stdragon )	/* (c) 1989 */
	DRIVER( rodland )	/* (c) 1990 */
	DRIVER( rodlandj )	/* (c) 1990 (Japan) */
	DRIVER( rodlndjb )	/* bootleg */
	DRIVER( avspirit )	/* (c) 1991 */
	DRIVER( inyourfa )  /* (c) 1991 (US, prototype) */
	DRIVER( phantasm )	/* (c) 1991 (Japan) */
	DRIVER( edf )		    /* (c) 1991 */
  DRIVER( edfu )		  /* (c) 1991 (North America) */
	DRIVER( 64street )	/* (c) 1991 */
	DRIVER( 64streej )	/* (c) 1991 (Japan) */
	DRIVER( soldamj )	/* (c) 1992 (Japan) */
	DRIVER( bigstrik )	/* (c) 1992 */
	DRIVER( bigstrkb )	/* bootleg on different hardware */
	DRIVER( chimerab )	/* (c) 1993 */
	DRIVER( cybattlr )	/* (c) 1993 */
	DRIVER( peekaboo )	/* (c) 1993 */

	/* Jaleco Mega System 1 games */

	/* Video System Co. games */
	DRIVER( rabiolep )	/* (c) 1987 V-System Co. (Japan) */
	DRIVER( rpunch )	/* (c) 1987 V-System Co. + Bally/Midway/Sente license (US) */
	DRIVER( svolley )	/* (c) 1989 V-System Co. (Japan) */
	DRIVER( svolleyk )	/* (c) 1989 V-System Co. (Korea) */
	DRIVER( svolleyu )	/* (c) 1989 V-System Co. + Data East license (US) */
	DRIVER( nekkyoku )	/* [1988] Video System Co. (Japan) */
	DRIVER( idolmj )	/* [1988] (c) System Service (Japan) */
	DRIVER( mjnatsu )	/* [1989] Video System presents (Japan) */
	DRIVER( natsuiro )	/* [1989] Video System presents (Japan) */
	DRIVER( mfunclub )	/* [1989] V-System (Japan) */
	DRIVER( daiyogen )	/* [1990] Video System Co. (Japan) */
	DRIVER( nmsengen )	/* (c) 1991 Video System (Japan) */
	DRIVER( fromance )	/* (c) 1991 Video System Co. (Japan) */
	DRIVER( pipedrm )	/* (c) 1990 Video System Co. (US) */
	DRIVER( pipedrmj )	/* (c) 1990 Video System Co. (Japan) */
	DRIVER( hatris )	/* (c) 1990 Video System Co. (Japan) */
	DRIVER( spinlbrk )	/* (c) 1990 V-System Co. (World) */
	DRIVER( spinlbru )	/* (c) 1990 V-System Co. (US) */
	DRIVER( spinlbrj )	/* (c) 1990 V-System Co. (Japan) */
	DRIVER( pspikes )	/* (c) 1991 Video System Co. (World) */
	DRIVER( pspikesk )	/* (c) 1991 Video System Co. (Korea) */
	DRIVER( svolly91 )	/* (c) 1991 Video System Co. (Japan) */
	DRIVER( karatblz )	/* (c) 1991 Video System Co. */
	DRIVER( karatblu )	/* (c) 1991 Video System Co. (US) */
	DRIVER( turbofrc )	/* (c) 1991 Video System Co. */
	DRIVER( aerofgt )	/* (c) 1992 Video System Co. */
	DRIVER( aerofgtb )	/* (c) 1992 Video System Co. */
	DRIVER( aerofgtc )	/* (c) 1992 Video System Co. */
	DRIVER( sonicwi )	/* (c) 1992 Video System Co. (Japan) */
	DRIVER( taotaido )	/* (c) 1993 Video System Co. */
	DRIVER( taotaida )	/* (c) 1993 Video System Co. */

	/* Psikyo games */
	DRIVER( s1945ii )	/* (c) 1997 */
	DRIVER( soldivid )	/* (c) 1997 */
	DRIVER( s1945iii )	/* (c) 1999 */
	DRIVER( dragnblz )	/* (c) 2000 */
	DRIVER( hotgmck )	/* (c) 1997 */
	DRIVER( hgkairak )	/* (c) 1998 */
	DRIVER( hotgmck3 )	/* (c) 1999 */
	DRIVER( gnbarich )	/* (c) 2001 */
	DRIVER( tgm2 )	        /* (c) 2000 */
	DRIVER( tgm2p )	        /* (c) 2000 */

	/* Orca games */
	DRIVER( funkybee )	/* (c) 1982 Orca */
	DRIVER( skylancr )	/* (c) 1983 Orca + Esco Trading Co license */
	DRIVER( zodiack )	/* (c) 1983 Orca + Esco Trading Co license */
	DRIVER( dogfight )	/* (c) 1983 Thunderbolt */
	DRIVER( moguchan )	/* (c) 1982 Orca + Eastern Commerce Inc. license (doesn't appear on screen) */
	DRIVER( percuss )	/* (c) 1981 Orca */
	DRIVER( bounty )	/* (c) 1982 Orca */
	DRIVER( espial )	/* (c) 1983 Thunderbolt, Orca logo is hidden in title screen */
	DRIVER( espiale )	/* (c) 1983 Thunderbolt, Orca logo is hidden in title screen */
	DRIVER( netwars )	/* (c) 1983 Orca + Esco Trading Co license */
	/* Vastar was made by Orca, but when it was finished, Orca had already bankrupted. */
	/* So they sold this game as "Made by Sesame Japan" because they couldn't use */
	/* the name "Orca" */
	DRIVER( vastar )	/* (c) 1983 Sesame Japan */
	DRIVER( vastar2 )	/* (c) 1983 Sesame Japan */
	DRIVER( pprobe )  /* (c) 1985 Crux / Kyugo */
/*
   other Orca games:
   82 Battle Cross                         Kit 2P
   82 River Patrol Empire Mfg/Kerstens Ind Ded 2P        HC Action
   82 Slalom                               Kit 2P        HC Action
   83 Net Wars                                 2P
   83 Super Crush                          Kit 2P           Action
*/

	/* Gaelco 2D games */
	/* Master Boy */	/* (c) 1987 - No Ref on the PCB */
	DRIVER( xorworld )	/* (c) 1990 - prototype */
	DRIVER( bigkarnk )	/* (c) 1991 - Ref 901112-1 */
	/* Master Boy 2 */	/* (c) 1991 - Ref ??? */
	DRIVER( splash )	/* (c) 1992 - Ref 922804 */
	DRIVER( thoop )	        /* (c) 1992 - Ref 922804/1 */
	DRIVER( squash )	/* (c) 1992 - Ref 922804/2 */
	DRIVER( wrally )	/* (c) 1993 - Ref 930705 */
	DRIVER( wrallya )	/* (c) 1993 - Ref 930705 */
	DRIVER( glass )		/* (c) 1993 - Ref 931021 */
	DRIVER( glasskr )	/* (c) 1994 - Ref 931021 Anime girls, unprotected */
	DRIVER( targeth )	/* (c) 1994 - Ref 940531 */
	DRIVER( quickshts )	/* (c) 1994 - Ref 940531 */
	DRIVER( thoop2 )	/* (c) 1994 - Ref ??? */
	DRIVER( aligator )	/* (c) 1994 - Ref 940411 */
	DRIVER( aligatun )	/* (c) 1994 - Ref 940411 (unprotected) */
	DRIVER( biomtoy )	/* (c) 1995 - Ref 922804/2 - (unprotected) */
	DRIVER( touchgo )	/* (c) 1995 - Ref 950510-1 */
	DRIVER( touchgok )/* (c) 1995 - Ref 950510-1 - (unprotected) */
	DRIVER( wrally2 )	/* (c) 1995 - Ref 950510 */
	DRIVER( maniacsp )	/* (c) 1996 - Ref 922804/2 - (prototype) */
	DRIVER( maniacsq )	/* (c) 1996 - Ref ??? - (unprotected) */
	DRIVER( snowboar )	/* (c) 1996 - Ref 960419/1 */
	DRIVER( snowbalt )	/* (c) 1996 - Ref 960419/1 */
	DRIVER( bang )		/* (c) 1998 - Ref ??? */
	DRIVER( bangj )		/* (c) 1999 - Ref ??? */

	/*
	Remaining Gaelco Games:
	=======================
	1996: Speed Up
	1997: Surf Planet (Ref 971223)
	1998: Radikal Bikers
	1999: Rolling Extreme
	2000: Football Power
	2001: Smashing Drive
	2002: ATV Track
	*/

	/* Kaneko games */
  DRIVER( djboy )		  /* (c) 1989 Kaneko */
	DRIVER( djboyj )	  /* (c) 1989 Kaneko */
	DRIVER( airbustr )	/* (c) 1990 Kaneko + Namco */
	DRIVER( airbustj )	/* (c) 1990 Kaneko + Namco (Japan) */
	DRIVER( galpanic )	/* (c) 1990 Kaneko */
	DRIVER( galpanib )	/* (c) 1990 Kaneko */
	DRIVER( galpani2 )	/* (c) 1993 Kaneko */

	/* Kaneko "AX System" games */
	DRIVER( berlwall )	/* (c) 1991 Kaneko */
	DRIVER( berlwalt )	/* (c) 1991 Kaneko */
	DRIVER( mgcrystl )	/* (c) 1991 Kaneko (World) */
	DRIVER( mgcrystj )	/* (c) 1991 Kaneko + distributed by Atlus (Japan) */
	DRIVER( blazeon )	/* (c) 1992 Atlus */
	DRIVER( sandscrp )	/* (c) 1992 Face */
	DRIVER( bakubrkr )      /* (c) 1992 Kaneko */
	DRIVER( shogwarr )      /* (c) 1992 Kaneko */
	DRIVER( fjbuster )      /* (c) 1992 Kaneko (Japan) */
	DRIVER( brapboys )      /* (c) 1992 Kaneko (World) */
	DRIVER( brapboysj )     /* (c) 1992 Kaneko (Japan) */
	DRIVER( bloodwar )      /* (c) 1994 Kaneko */
	DRIVER( bonkadv )       /* (c) 1994 Kaneko */
	DRIVER( gtmr )		/* (c) 1994 Kaneko */
	DRIVER( gtmre )		/* (c) 1994 Kaneko */
	DRIVER( gtmrusa )	/* (c) 1994 Kaneko (US) */
	DRIVER( gtmr2 )		/* (c) 1995 Kaneko */
	DRIVER( wingforc )      /* (c) 1993 Atlus */
	DRIVER( packbang )      /* (c) 1994 Kaneko */

	/* Kaneko "Super Nova System" games */
	DRIVER( galpani4 )	/* (c) 1996 Kaneko (Japan) */
	DRIVER( galpandx )	/* (c) 2001 Kaneko (Asia) */
	DRIVER( galpanis )	/* (c) 1997 Kaneko (Japan) */
	DRIVER( sengekis )	/* (c) 1997 Kaneko / Warashi (Asia) */
	DRIVER( sengekij )	/* (c) 1997 Kaneko / Warashi (Japan) */
	DRIVER( vblokbrk )	/* (c) 1997 Kaneko / Mediaworks (Asia) */
	DRIVER( sarukani )	/* (c) 1997 Kaneko / Mediaworks (Japan) */
	DRIVER( cyvern )	/* (c) 1998 Kaneko (Japan) */
	DRIVER( galpans2 )	/* (c) 1999 Kaneko (Japan) */
	DRIVER( panicstr )	/* (c) 1999 Kaneko (Japan) */
	DRIVER( senknow )	/* (c) 1999 Kaneko / Kouyousha (Japan) */
	DRIVER( gutsn )		/* (c) 2000 Kaneko / Kouyousha (Japan) */
	DRIVER( puzzloop )	/* (c) 1998 Mitchell (Europe) */
	DRIVER( puzloopj )	/* (c) 1998 Mitchell (Japan) */
	DRIVER( puzloopu )	/* (c) 1998 Mitchell (USA) */
	DRIVER( jjparads )	/* (c) 1996 Electro Design Co. (Japan) */
	DRIVER( jjparad2 )	/* (c) 1997 Electro Design Co. (Japan) */
	DRIVER( ryouran )	/* (c) 1998 Electro Design Co. (Japan) */
	DRIVER( teljan )	/* (c) 1999 Electro Design Co. (Japan) */

	/* Seta games */
	DRIVER( hanaawas )	/* (c) SetaKikaku */


	/* SSV System (Seta, Sammy, Visco) games */

	/* Atlus games */
	DRIVER( powerins )	/* (c) 1993 Atlus (Japan) */
	DRIVER( powerina )	/* (c) 1993 Atlus (Japan) */
	DRIVER( ohmygod )	/* (c) 1993 Atlus (Japan) */
	DRIVER( naname )	/* (c) 1994 Atlus (Japan) */
	DRIVER( blmbycar )	/* (c) 1994 ABM & Gecas - uses same gfx chip as powerins? */
	DRIVER( blmbycau )	/* (c) 1994 ABM & Gecas - uses same gfx chip as powerins? */

	/* Sun Electronics / SunSoft games */
	DRIVER( speakres )	/* [Sun Electronics] */
	DRIVER( stratvox )	/* [1980 Sun Electronics] Taito */
	DRIVER( spacecho )	/* bootleg */
	DRIVER( route16 )	/* (c) 1981 Tehkan/Sun + Centuri license */
	DRIVER( route16a )	/* (c) 1981 Tehkan/Sun + Centuri license */
	DRIVER( route16b )	/* bootleg */
	DRIVER( routex )	/* bootleg */
	DRIVER( ttmahjng )	/* Taito */
	DRIVER( fnkyfish )	/* (c) 1981 Sun Electronics */
	DRIVER( kangaroo )	/* (c) 1982 Sun Electronics */
	DRIVER( kangaroa )	/* 136008			(c) 1982 Atari */
	DRIVER( kangarob )	/* (bootleg) */
	DRIVER( arabian )	/* TVG13 (c) 1983 Sun Electronics */
	DRIVER( arabiana )	/* 136019			(c) 1983 Atari */
	DRIVER( markham )	/* TVG14 (c) 1983 Sun Electronics */
	DRIVER( strnskil )	/* TVG15 (c) 1984 Sun Electronics */
	DRIVER( guiness )	/* TVG15 (c) 1984 Sun Electronics */
	DRIVER( pettanp )	/* TVG16 (c) 1984 Sun Electronics (Japan) */
	DRIVER( ikki )		/* TVG17 (c) 1985 Sun Electronics (Japan) */
	DRIVER( shanghai )	/* (c) 1988 Sunsoft (Sun Electronics) */
	DRIVER( shangha2 )	/* (c) 1989 Sunsoft (Sun Electronics) */
	DRIVER( shangha3 )	/* (c) 1993 Sunsoft */
	DRIVER( heberpop )	/* (c) 1994 Sunsoft / Atlus */
	DRIVER( blocken )	/* (c) 1994 KID / Visco */
/*
Other Sun games
1978 (GT)Block Perfect
1978 (GT)Block Challenger
1979 Galaxy Force
1979 Run Away
1979 Dai San Wakusei (The Third Planet)
1979 Warp 1
1980 Cosmo Police (Cosmopolis?)
1985 Ikki
1993 Saikyou Battler Retsuden
1995 Shanghai Banri no Choujou (ST-V)
1996 Karaoke Quiz Intro DonDon (ST-V)
1998 Astra Super Stars (ST-V)
1998 Shanghai Mateki Buyuu (TPS)
*/

	/* Suna games */
	DRIVER( goindol )	/* (c) 1987 Sun a Electronics */
	DRIVER( goindolu )	/* (c) 1987 Sun a Electronics */
	DRIVER( goindolj )	/* (c) 1987 Sun a Electronics */
	DRIVER( bssoccer )	/* (c) 1996 SunA */
	DRIVER( uballoon )	/* (c) 1996 SunA */

	/* Dooyong games */
	DRIVER( gundealr )	/* (c) 1990 Dooyong */
	DRIVER( gundeala )	/* (c) Dooyong */
	DRIVER( gundealt )	/* (c) 1990 Tecmo */
	DRIVER( yamyam )	/* (c) 1990 Dooyong */
	DRIVER( wiseguy )	/* (c) 1990 Dooyong */

	/* Tong Electronic games */
	DRIVER( leprechn )	/* (c) 1982 */
	DRIVER( potogold )	/* (c) 1982 */
	DRIVER( piratetr )  /* (c) 1982 */
	DRIVER( beezer )	/* (c) 1982 */
	DRIVER( beezer1 )	/* (c) 1982 */

	/* Comad games */
	DRIVER( pushman )	/* (c) 1990 Comad + American Sammy license */
	DRIVER( bballs )	/* (c) 1991 Comad */
	DRIVER( zerozone )	/* (c) 1993 Comad */
	DRIVER( lvgirl94 )	/* (c) 1994 Comad */
	DRIVER( hotpinbl )	/* (c) 1995 Comad & New Japan System */
	DRIVER( galspnbl )	/* (c) 1996 Comad */
	/* the following ones run on modified Gals Panic hardware */
	DRIVER( fantasia )	/* (c) 1994 Comad & New Japan System */
	DRIVER( newfant )	/* (c) 1995 Comad & New Japan System */
	DRIVER( fantsy95 )	/* (c) 1995 Hi-max Technology Inc. */
	DRIVER( missw96 )	/* (c) 1996 Comad */
	DRIVER( fantsia2 )	/* (c) 1997 Comad */
	DRIVER( galhustl )	/* (c) 1997 ACE International */

	/* Playmark games */
	DRIVER( sslam )		/* (c) 1993 */
	DRIVER( sslama )	/* (c) 1993 */

	/* Pacific Novelty games */
	DRIVER( sharkatt )	/* (c) [1980] */
	DRIVER( thief )		/* (c) 1981 */
	DRIVER( natodef )	/* (c) 1982 */
	DRIVER( natodefa )	/* (c) 1982 */
	DRIVER( mrflea )	/* (c) 1982 */

	/* Tecfri games */
	DRIVER( holeland )	/* (c) 1984 */
	DRIVER( crzrally )	/* (c) 1985 */
	DRIVER( speedbal )	/* (c) 1987 */
	DRIVER( sauro )		/* (c) 1987 */

	/* Metro games */

	/* Venture Line games */
	DRIVER( spcforce )	/* (c) 1980 Venture Line */
	DRIVER( spcforc2 )	/* bootleg */
	DRIVER( meteor )	/* (c) 1981 Venture Line */
	DRIVER( looping )	/* (c) 1982 Venture Line + licensed from Video Games */
	DRIVER( loopinga )	/* (c) 1982 Venture Line + licensed from Video Games */
	DRIVER( skybump )	/* (c) 1982 Venture Line */

	/* Yun Sung games */
	DRIVER( cannball )	/* (c) 1995 Yun Sung / Soft Visio */
	DRIVER( magix )		/* (c) 1995 Yun Sung */
	DRIVER( magicbub )	/* (c) Yun Sung */
	DRIVER( shocking )	/* (c) 1997 Yun Sung */
	DRIVER( bombkick )	/* (c) 1998 Yun Sung */

	/* Zilec games */
	DRIVER( blueprnt )	/* (c) 1982 Bally Midway (Zilec in ROM 3U, and the programmer names) */
	DRIVER( blueprnj )	/* (c) 1982 Jaleco (Zilec in ROM 3U, and the programmer names) */
	DRIVER( saturn )	/* (c) 1983 Jaleco (Zilec in ROM R6, and the programmer names) */

	/* Fuuki FG-2 games */
	DRIVER( gogomile )	/* (c) 1995 */
	DRIVER( gogomilj )	/* (c) 1995 (Japan) */
	DRIVER( pbancho )	/* (c) 1996 (Japan) */

	/* Fuuki FG-3 games */
	DRIVER( asurabld )	/* (c) 1998 (Japan) */
	DRIVER( asurabus )	/* (c) 2000 (Japan) */

	/* Unico games */
	DRIVER( drgnmst )	/* (c) 1994 */
  DRIVER( mastfury )	/* (c) 1996 */
	DRIVER( burglarx )	/* (c) 1997 */
	DRIVER( zeropnt )	/* (c) 1998 */
	DRIVER( zeropnta )	/* (c) 1998 */
	DRIVER( zeropnt2 )	/* (c) 1999 */
	DRIVER( silkroad )	/* (c) 1999 */

	/* Afega games */
	DRIVER( bubl2000 )	/* (c) 1998 Tuning */

	/* ESD games */
	/* http://www.esdgame.co.kr/english/ */

	/* Dyna Electronics / Dynax / Nakanihon games */
	DRIVER( royalmah )	/* (c) 1982 Falcon */
	DRIVER( suzume )	/*  ??  (c) 1986 Dyna Electronics */
	DRIVER( hnayayoi )	/* "02" (c) 1987 Dyna Electronics */
	DRIVER( dondenmj )	/* "03" (c) 1986 Dyna Electronics */
	DRIVER( hnfubuki )	/* "06" (c) 1987 Dynax */
	DRIVER( mjdiplob )	/* "07" (c) 1987 Dynax */
	DRIVER( untoucha )	/* "08" (c) 1987 Dynax */
	DRIVER( tontonb )	/* "09" (c) 1987 Dynax */
	DRIVER( majs101b )	/* "17" (c) [1988] Dynax */
	DRIVER( mjderngr )	/* "22" (c) 1989 Dynax */
						/* "27" Jantouki 1989 Dynax */
	DRIVER( mjifb )		/* "29" 1990 Dynax */
						/* "45" Neruton Haikujiradan 1990 Dynax */
	DRIVER( mmpanic )	/* "70" 1992 Nakanihon/Taito */
	DRIVER( quizchq )	/* "73" (c) 1993 Nakanihon */
	DRIVER( quizchql )	/* "73" (c) 1993 Laxan */
	DRIVER( quiz365 )	/* "78" (c) 1994 Nakanihon */
	DRIVER( rongrong )	/* "80" (c) 1994 Nakanihon */
	DRIVER( nettoqc )	/* "??" (c) 1995 Nakanihon */
	DRIVER( ddenlovr )	/* "113" (c) 1996 Dynax */
	DRIVER( hanakanz )	/* "507" 1996 Dynax */
						/* "510" Hana Kagerou 1996 Nakanihon */
	DRIVER( realbrk )	/* "523" Billiard Academy Real Break 1998 */
						/* "526" Mahjong Reach Ippatsu 1998 Nihon System/Dynax */

	/* Sigma games */
	DRIVER( nyny )		/* (c) 1980 Sigma Ent. Inc. */
	DRIVER( nynyg )		/* (c) 1980 Sigma Ent. Inc. + Gottlieb */
	DRIVER( arcadia )	/* (c) 1982 Sigma Ent. Inc. */
	DRIVER( r2dtank )	/* (c) 1980 Sigma Ent. Inc. */
	DRIVER( spiders )	/* (c) 1981 Sigma Ent. Inc. */
	DRIVER( spiders2 )	/* (c) 1981 Sigma Ent. Inc. */

	/* IGS games ( www.igs.com.tw ) */
	DRIVER( iqblock )	/* (c) 1993 */
	DRIVER( grndtour )	/* (c) 1993 */
	DRIVER( cabaret )
	DRIVER( drgnwrld )	/* (c) 1997 */
	DRIVER( vbowl )	        /* (c) 1996 */
	DRIVER( vbowlj )	/* (c) 1996 */
	DRIVER( grtwall )

	/* RamTek games */
	DRIVER( hitme )		/* [1976 Ramtek] */
	DRIVER( mblkjack )	/* [197? Mirco] */
	DRIVER( barricad )	/* [1976 Ramtek] */
	DRIVER( brickyrd )	/* [1976 Ramtek] */
	DRIVER( starcrus )	/* [1977 Ramtek] */

	/* Omori games */
	DRIVER( carjmbre )	/* (c) 1983 Omori Electric Co., Ltd. */
	DRIVER( popper )	/* (c) 1983 Omori Electric Co., Ltd. */

	/* TCH games */
	DRIVER( speedspn )	/* (c) 1994 */

	/* U.S. Games games */
	DRIVER( usg32 )
	DRIVER( usg82 )
	DRIVER( usg83 )
	DRIVER( usg83x )
	DRIVER( usg185 )
	DRIVER( usg252 )

	/* Sanritsu games */
	DRIVER( drmicro )	/* (c) 1983 Sanritsu */
	DRIVER( appoooh )	/* (c) 1984 Sega */	/* made by Sanritsu */
	DRIVER( bankp )		/* (c) 1984 Sega */	/* made by Sanritsu */
	DRIVER( combh )		/* (c) 1987 Sega */	/* made by Sanritsu */
	DRIVER( mjkjidai )	/* (c) 1986 Sanritsu */

	/* Rare games */
	DRIVER( btoads )	/* (c) 1994 Rare */

	/* Nihon System games */
	DRIVER( gigasb )
	DRIVER( gigasm2b )
	DRIVER( oigas )
	DRIVER( pbillrd )	/* (c) 1987 Nihon System */
	DRIVER( pbillrds )
	DRIVER( freekick )
	DRIVER( freekckb )	/* (c) 1987 Nihon System (+ optional Sega) */
	DRIVER( countrun )
	DRIVER( countrnb )
	DRIVER( countrb2 )
  DRIVER( countrunb3 )

	/* Alba games */
	DRIVER( rmhaihai )	/* (c) 1985 Alba */
	DRIVER( rmhaihib )	/* (c) 1985 Alba */
	DRIVER( rmhaijin )	/* (c) 1986 Alba */
	DRIVER( rmhaisei )	/* (c) 1986 Visco */
	DRIVER( themj )		/* (c) 1987 Visco */
	DRIVER( hanaroku )	/* (c) 1988 Alba */

	/* Home Data games */
	DRIVER( hourouki ) 	/* (c) 1987 Home Data */
	DRIVER( mhgaiden ) 	/* (c) 1987 Home Data */
	DRIVER( mjhokite ) 	/* (c) 1988 Home Data */
	DRIVER( mjclinic ) 	/* (c) 1988 Home Data */
	DRIVER( mrokumei ) 	/* (c) 1988 Home Data */
	DRIVER( reikaids ) 	/* (c) 1988 Home Data */
	DRIVER( mjkojink ) 	/* (c) 1989 Home Data */
	DRIVER( vitaminc ) 	/* (c) 1989 Home Data */
	DRIVER( mjyougo ) 	/* (c) 1989 Home Data */
	DRIVER( lemnangl ) 	/* (c) 1990 Home Data */
	DRIVER( mjkinjas ) 	/* (c) 1991 Home Data */
	DRIVER( battlcry ) 	/* (c) 1991 Home Data */
	DRIVER( jogakuen )	/* Windom corporation */
	DRIVER( mjikaga )	/* Mitchell */

	/* Art & Magic games */

	/* Taiyo games */
	DRIVER( dynamski )	/* (c) 1984 Taiyo */
	DRIVER( shangkid )	/* (c) 1985 Taiyo + Data East license */
	DRIVER( hiryuken )	/* (c) 1985 Taito */

	DRIVER( spaceint )	/* [1980] Shoei */
	DRIVER( spacefb )	/* (c) [1980?] Nintendo */
	DRIVER( spacefbg )	/* 834-0031 (c) 1980 Gremlin */
	DRIVER( spacefbb )	/* bootleg */
	DRIVER( spacebrd )	/* bootleg */
	DRIVER( spacedem )	/* (c) 1980 Fortrek + made by Nintendo */
	DRIVER( omegrace )	/* (c) 1981 Midway */
	DRIVER( deltrace )	/* bootleg */
	DRIVER( dday )		/* (c) 1982 Olympia */
	DRIVER( ddayc )		/* (c) 1982 Olympia + Centuri license */
	DRIVER( hexa )		/* D. R. Korea */
	DRIVER( stactics )	/* [1981 Sega] */
	DRIVER( exterm )	/* (c) 1989 Premier Technology - a Gottlieb game */
	DRIVER( ambush )	/* (c) 1983 Nippon Amuse Co-Ltd */
	DRIVER( ambusht )	/* (c) 1983 Tecfri */
	DRIVER( homo )		/* bootleg */
	DRIVER( dlair )
	DRIVER( aztarac )	/* (c) 1983 Centuri (vector game) */
	DRIVER( mole )		/* (c) 1982 Yachiyo Electronics, Ltd. */
	DRIVER( thehand )	/* (c) 1981 T.I.C. */
	DRIVER( gotya )		/* (c) 1981 Game-A-Tron */
	DRIVER( mrjong )	/* (c) 1983 Kiwako */
	DRIVER( crazyblk )	/* (c) 1983 Kiwako + ECI license */
	DRIVER( blkbustr )	/* (c) 1983 Kiwako + ECI license */
	DRIVER( polyplay )
	DRIVER( amspdwy )	/* no copyright notice, but (c) 1987 Enerdyne Technologies, Inc. */
	DRIVER( amspdwya )	/* no copyright notice, but (c) 1987 Enerdyne Technologies, Inc. */
	DRIVER( mosaic )	/* (c) 1990 Space */
	DRIVER( mosaica )	/* (c) 1990 Space + Fuuki license */
	DRIVER( gfire2 )	/* (c) 1992 Topis Corp */
	DRIVER( spdbuggy )
	DRIVER( sprcros2 )	/* (c) 1986 GM Shoji */
	DRIVER( mugsmash )	/* (c) Electronic Devices (Italy) / 3D Games (England) */
	DRIVER( fantland )	/* (c) 1987 Electronic Devices Italy */
	DRIVER( galaxygn )	/* (c) 1989 Electronic Devices Italy */
	DRIVER( borntofi )	/* (c) 1987 International Games */
	DRIVER( wheelrun )	/* (c) 1987 International Games */
	DRIVER( gcpinbal )	/* (c) 1994 Excellent System */
	DRIVER( aquarium )	/* (c) 1996 Excellent System */
	DRIVER( policetr )	/* (c) 1996 P&P Marketing */
	DRIVER( policeto )	/* (c) 1996 P&P Marketing */
	DRIVER( plctr13b )  /* (c) 1996 P&P Marketing */
	DRIVER( sshooter )	/* (c) 1998 P&P Marketing */
	DRIVER( pass )		/* (c) 1992 Oksan */
	DRIVER( news )		/* "Virus"??? ((c) 1993 Poby in ROM VIRUS.4) */
	DRIVER( taxidrvr )	/* [1984 Graphic Techno] */
	DRIVER( xyonix )	/* [1989 Philko] */
	DRIVER( findout )	/* (c) 1987 [Elettronolo] */
	DRIVER( dribling )	/* (c) 1983 Model Racing */
	DRIVER( driblino )	/* (c) 1983 Olympia */
	DRIVER( ace )		/* [1976 Allied Leisure] */
	DRIVER( clayshoo )	/* [1979 Allied Leisure] */
	DRIVER( pirates )	/* (c) 1994 NIX */
	DRIVER( genix )		/* (c) 199? NIX */
	DRIVER( fitfight )	/* bootleg of Art of Fighting */
	DRIVER( histryma )	/* bootleg of Fighter's History */
	DRIVER( bbprot )
	DRIVER( diverboy )	/* (c) 1992 Electronic Devices */
	DRIVER( beaminv )	/* Tekunon Kougyou */
	DRIVER( mcatadv )	/* (c) 1993 Wintechno */
	DRIVER( mcatadvj )	/* (c) 1993 Wintechno */
	DRIVER( nost )		/* (c) 1993 Face */
	DRIVER( nostj )		/* (c) 1993 Face */
	DRIVER( nostk )		/* (c) 1993 Face */
	DRIVER( 4enraya )	/* (c) 1990 IDSA */
	DRIVER( oneshot )	/* no copyright notice */
	DRIVER( maddonnb )	/* (c) 1995 Tuning */
	DRIVER( tugboat )	/* (c) 1982 ETM */
	DRIVER( amerdart )	/* (c) 1989 Ameri Corporation */
	DRIVER( coolpool )	/* (c) 1992 Catalina Games */
	DRIVER( 9ballsht )	/* (c) 1993 E-Scape EnterMedia + "marketed by Bundra Games" */
	DRIVER( 9ballsh2 )	/* (c) 1993 E-Scape EnterMedia + "marketed by Bundra Games" */
	DRIVER( 9ballsh3 )	/* (c) 1993 E-Scape EnterMedia + "marketed by Bundra Games" */
	DRIVER( gumbo )		/* (c) 1994 Min Corp. */
	DRIVER( trivquiz )	/* (c) 1984 Status Games */
	DRIVER( statriv2 )	/* (c) 1984 Status Games */
	DRIVER( supertr2 )	/* (c) 1986 Status Games */
	DRIVER( tickee )	/* (c) 1994 Raster Elite */
	DRIVER( crgolf )	/* (c) 1984 Nasco Japan */
	DRIVER( crgolfa )	/* (c) 1984 Nasco Japan */
	DRIVER( crgolfb )	/* (c) 1984 Nasco Japan */
	DRIVER( crgolfc )	/* (c) 1984 Nasco Japan */
	DRIVER( truco )		/* (c) 198? Playtronic SRL */
	DRIVER( thedeep )	/* (c) 1987 Woodplace */
	DRIVER( rundeep )	/* (c) 1988 Cream (bootleg?) */
	DRIVER( skyarmy )	/* (c) 1982 Shoei */
	DRIVER( lethalj )	/* (c) 1996 The Game Room */
	DRIVER( eggventr )	/* (c) 1997 The Game Room */
	DRIVER( eggvntdx )	/* (c) 1997 The Game Room */
	DRIVER( rotaryf )
	DRIVER( sbugger )	/* (c) 1981 Game-A-Tron */
	DRIVER( portrait )	/* (c) 1983 Olympia */
	DRIVER( enigma2 )	/* (c) 1981 Game Plan (Zilec Electronics license) */
	DRIVER( enigma2a )	/* (c) 1984 Zilec Electronics (bootleg?) */
	DRIVER( ltcasino )	/* (c) 1982 Digital Controls Inc */
	DRIVER( ltcasin2 )	/* (c) 1984 Digital Controls Inc */

	/* Neo Geo games */
	/* the four digits number is the game ID stored at address 0x0108 of the program ROM */
	/* info on prototypes taken from http://www.members.tripod.com/fresa/proto/puzzle.htm */
	DRIVER( nam1975 )	/* 0001 (c) 1990 SNK */
	DRIVER( bstars )	/* 0002 (c) 1990 SNK */
	DRIVER( tpgolf )	/* 0003 (c) 1990 SNK */
	DRIVER( mahretsu )	/* 0004 (c) 1990 SNK */
	DRIVER( maglord )	/* 0005 (c) 1990 Alpha Denshi Co. */
	DRIVER( maglordh )	/* 0005 (c) 1990 Alpha Denshi Co. */
	DRIVER( ridhero )	/* 0006 (c) 1990 SNK */
	DRIVER( ridheroh )	/* 0006 (c) 1990 SNK */
	DRIVER( alpham2 )	/* 0007 (c) 1991 SNK */
	/* 0008 Sunshine (prototype) 1990 SNK */
	DRIVER( ncombat )	/* 0009 (c) 1990 Alpha Denshi Co. */
	DRIVER( cyberlip )	/* 0010 (c) 1990 SNK */
	DRIVER( superspy )	/* 0011 (c) 1990 SNK */
	/* 0012 */
	/* 0013 */
	DRIVER( mutnat )	/* 0014 (c) 1992 SNK */
	/* 0015 */
	DRIVER( kotm )		/* 0016 (c) 1991 SNK */
	DRIVER( kotmh )		/* 0016 (c) 1991 SNK */
	DRIVER( sengoku )	/* 0017 (c) 1991 SNK */
	DRIVER( sengokh )	/* 0017 (c) 1991 SNK */
	DRIVER( burningf )	/* 0018 (c) 1991 SNK */
	DRIVER( burningh )	/* 0018 (c) 1991 SNK */
	DRIVER( lbowling )	/* 0019 (c) 1990 SNK */
	DRIVER( gpilots )	/* 0020 (c) 1991 SNK */
	DRIVER( joyjoy )	/* 0021 (c) 1990 SNK */
	DRIVER( bjourney )	/* 0022 (c) 1990 Alpha Denshi Co. */
	DRIVER( quizdais )	/* 0023 (c) 1991 SNK */
	DRIVER( lresort )	/* 0024 (c) 1992 SNK */
	DRIVER( eightman )	/* 0025 (c) 1991 SNK / Pallas */
	/* 0026 Fun Fun Brothers (prototype) 1991 Alpha */
	DRIVER( minasan )	/* 0027 (c) 1990 Monolith Corp. */
	/* 0028 Dunk Star (prototype) Sammy */
	DRIVER( legendos )	/* 0029 (c) 1991 SNK */
	DRIVER( 2020bb )	/* 0030 (c) 1991 SNK / Pallas */
	DRIVER( 2020bbh )	/* 0030 (c) 1991 SNK / Pallas */
	DRIVER( socbrawl )	/* 0031 (c) 1991 SNK */
	DRIVER( roboarmy )	/* 0032 (c) 1991 SNK */
	DRIVER( fatfury1 )	/* 0033 (c) 1991 SNK */
	DRIVER( fbfrenzy )	/* 0034 (c) 1992 SNK */
	/* 0035 Mystic Wand (prototype) 1991 Alpha */
	DRIVER( bakatono )	/* 0036 (c) 1991 Monolith Corp. */
	DRIVER( crsword )	/* 0037 (c) 1991 Alpha Denshi Co. */
	DRIVER( trally )	/* 0038 (c) 1991 Alpha Denshi Co. */
	DRIVER( kotm2 )		/* 0039 (c) 1992 SNK */
	DRIVER( sengoku2 )	/* 0040 (c) 1993 SNK */
	DRIVER( bstars2 )	/* 0041 (c) 1992 SNK */
	DRIVER( quizdai2 )	/* 0042 (c) 1992 SNK */
	DRIVER( 3countb )	/* 0043 (c) 1993 SNK */
	DRIVER( aof )		/* 0044 (c) 1992 SNK */
	DRIVER( samsho )	/* 0045 (c) 1993 SNK */
	DRIVER( tophuntr )	/* 0046 (c) 1994 SNK */
	DRIVER( fatfury2 )	/* 0047 (c) 1992 SNK */
	DRIVER( janshin )	/* 0048 (c) 1994 Aicom */
	DRIVER( androdun )	/* 0049 (c) 1992 Visco */
	DRIVER( ncommand )	/* 0050 (c) 1992 Alpha Denshi Co. */
	DRIVER( viewpoin )	/* 0051 (c) 1992 Sammy */
	DRIVER( ssideki )	/* 0052 (c) 1992 SNK */
	DRIVER( wh1 )		/* 0053 (c) 1992 Alpha Denshi Co. */
	DRIVER( wh1h )		/* 0053 (c) 1992 Alpha Denshi Co. */
	DRIVER( crswd2bl )  /* 0054 Crossed Swords 2  (CD only? not confirmed, MVS might exist) */
	DRIVER( kof94 )		/* 0055 (c) 1994 SNK */
	DRIVER( aof2 )		/* 0056 (c) 1994 SNK */
	DRIVER( wh2 )		/* 0057 (c) 1993 ADK */
	DRIVER( fatfursp )	/* 0058 (c) 1993 SNK */
	DRIVER( fatfursa )	/* 0058 (c) 1993 SNK */
	DRIVER( savagere )	/* 0059 (c) 1995 SNK */
	DRIVER( fightfev )	/* 0060 (c) 1994 Viccom */
	DRIVER( fightfva )	/* 0060 (c) 1994 Viccom */
	DRIVER( ssideki2 )	/* 0061 (c) 1994 SNK */
	DRIVER( spinmast )	/* 0062 (c) 1993 Data East Corporation */
	DRIVER( samsho2 )	/* 0063 (c) 1994 SNK */
	DRIVER( wh2j )		/* 0064 (c) 1994 ADK / SNK */
	DRIVER( wjammers )	/* 0065 (c) 1994 Data East Corporation */
	DRIVER( karnovr )	/* 0066 (c) 1994 Data East Corporation */
	DRIVER( gururin )	/* 0067 (c) 1994 Face */
	DRIVER( pspikes2 )	/* 0068 (c) 1994 Video System Co. */
	DRIVER( fatfury3 )	/* 0069 (c) 1995 SNK */
	DRIVER( zupapa )	/* 0070 (c) SNK */
	DRIVER( b2b )     /* 0071 Bang Bang Busters 1994 Visco */
	/* 0072 Last Odyssey Pinball Fantasia (prototype) 1995 Monolith */
	DRIVER( panicbom )	/* 0073 (c) 1994 Eighting / Hudson */
	DRIVER( aodk )		/* 0074 (c) 1994 ADK / SNK */
	DRIVER( sonicwi2 )	/* 0075 (c) 1994 Video System Co. */
	DRIVER( zedblade )	/* 0076 (c) 1994 NMK */
	/* 0077 The Warlocks of the Fates (prototype) 1995 Astec */
	DRIVER( galaxyfg )	/* 0078 (c) 1995 Sunsoft */
	DRIVER( strhoop )	/* 0079 (c) 1994 Data East Corporation */
	DRIVER( quizkof )	/* 0080 (c) 1995 Saurus */
	DRIVER( ssideki3 )	/* 0081 (c) 1995 SNK */
	DRIVER( doubledr )	/* 0082 (c) 1995 Technos */
	DRIVER( pbobblen )	/* 0083 (c) 1994 Taito */
	DRIVER( pbobblna )	/* 0083 (c) 1994 Taito */
	DRIVER( kof95 )		/* 0084 (c) 1995 SNK */
	DRIVER( kof95a )	/* 0084 (c) 1995 SNK */
	/* 0085 Shinsetsu Samurai Spirits Bushidoretsuden / Samurai Shodown RPG (CD only) */
	DRIVER( tws96 )		/* 0086 (c) 1996 Tecmo */
	DRIVER( samsho3 )	/* 0087 (c) 1995 SNK */
	DRIVER( stakwin )	/* 0088 (c) 1995 Saurus */
	DRIVER( pulstar )	/* 0089 (c) 1995 Aicom */
	DRIVER( whp )		/* 0090 (c) 1995 ADK / SNK */
	/* 0091 */
	DRIVER( kabukikl )	/* 0092 (c) 1995 Hudson */
	DRIVER( neobombe )	/* 0093 (c) 1997 Hudson */
	DRIVER( gowcaizr )	/* 0094 (c) 1995 Technos */
	DRIVER( rbff1 )		/* 0095 (c) 1995 SNK */
	DRIVER( aof3 )		/* 0096 (c) 1996 SNK */
	DRIVER( sonicwi3 )	/* 0097 (c) 1995 Video System Co. */
	/* 0098 Idol Mahjong - final romance 2 (CD only? not confirmed, MVS might exist) */
	/* 0099 Neo Pool Masters */
	DRIVER( turfmast )	/* 0200 (c) 1996 Nazca */
	DRIVER( mslug )		/* 0201 (c) 1996 Nazca */
	DRIVER( puzzledp )	/* 0202 (c) 1995 Taito (Visco license) */
	DRIVER( mosyougi )	/* 0203 (c) 1995 ADK / SNK */
	/* 0204 QP (prototype) */
	/* 0205 Neo-Geo CD Special (CD only) */
	DRIVER( marukodq )	/* 0206 (c) 1995 Takara */
	DRIVER( neomrdo )	/* 0207 (c) 1996 Visco */
	DRIVER( sdodgeb )	/* 0208 (c) 1996 Technos */
	DRIVER( goalx3 )	/* 0209 (c) 1995 Visco */
	/* 0210 Karate Ninja Sho (prototype) 1995 Yumekobo */
	DRIVER( zintrckb )	/* 0211 hack - this is not a genuine MVS proto, its a bootleg made from the CD version */
	DRIVER( overtop )	/* 0212 (c) 1996 ADK */
	DRIVER( neodrift )	/* 0213 (c) 1996 Visco */
	DRIVER( kof96 )		/* 0214 (c) 1996 SNK */
	DRIVER( kof96h )	/* 0214 (c) 1996 SNK */
	DRIVER( ssideki4 )	/* 0215 (c) 1996 SNK */
	DRIVER( kizuna )	/* 0216 (c) 1996 SNK */
	DRIVER( ninjamas )	/* 0217 (c) 1996 ADK / SNK */
	DRIVER( ragnagrd )	/* 0218 (c) 1996 Saurus */
	DRIVER( pgoal )		/* 0219 (c) 1996 Saurus */
	DRIVER( ironclad )  /* 0220 Choutetsu Brikin'ger - iron clad (prototype) 1996 Saurus */
	DRIVER( magdrop2 )	/* 0221 (c) 1996 Data East Corporation */
	DRIVER( samsho4 )	/* 0222 (c) 1996 SNK */
	DRIVER( rbffspec )	/* 0223 (c) 1996 SNK */
	DRIVER( twinspri )	/* 0224 (c) 1996 ADK */
	DRIVER( wakuwak7 )	/* 0225 (c) 1996 Sunsoft */
	/* 0226 Pair Pair Wars (prototype) 1996 Sunsoft? */
	DRIVER( stakwin2 )	/* 0227 (c) 1996 Saurus */
	DRIVER( ghostlop )	/* 0228 GhostLop (prototype) 1996? Data East */
	/* 0229 King of Fighters '96 CD Collection (CD only) */
	DRIVER( breakers )	/* 0230 (c) 1996 Visco */
	DRIVER( miexchng )	/* 0231 (c) 1997 Face */
	DRIVER( kof97 )		/* 0232 (c) 1997 SNK */
	DRIVER( kof97a )	/* 0232 (c) 1997 SNK */
	DRIVER( kof97pls )	/* bootleg of kof97 */
	DRIVER( kog )	    /* bootleg of kof97 */
	DRIVER( magdrop3 )	/* 0233 (c) 1997 Data East Corporation */
	DRIVER( lastblad )	/* 0234 (c) 1997 SNK */
	DRIVER( lastblda )	/* 0234 (c) 1997 SNK */
	DRIVER( puzzldpr )	/* 0235 (c) 1997 Taito (Visco license) */
	DRIVER( irrmaze )	/* 0236 (c) 1997 SNK / Saurus */
	DRIVER( popbounc )	/* 0237 (c) 1997 Video System Co. */
	DRIVER( shocktro )	/* 0238 (c) 1997 Saurus */
	DRIVER( shocktra )	/* 0238 (c) 1997 Saurus */
	DRIVER( blazstar )	/* 0239 (c) 1998 Yumekobo */
	DRIVER( rbff2 )		/* 0240 (c) 1998 SNK */
	DRIVER( rbff2a )	/* 0240 (c) 1998 SNK */
	DRIVER( mslug2 )	/* 0241 (c) 1998 SNK */
	DRIVER( mslug2t )	/* 0941 (c) 2015 Trap15 Hack */
	DRIVER( kof98 )		/* 0242 (c) 1998 SNK */
	DRIVER( kof98n )	/* 0242 (c) 1998 SNK */
	DRIVER( kof98k )	/* 0242 (c) 1998 SNK */
	DRIVER( lastbld2 )	/* 0243 (c) 1998 SNK */
	DRIVER( neocup98 )	/* 0244 (c) 1998 SNK */
	DRIVER( breakrev )	/* 0245 (c) 1998 Visco */
	DRIVER( shocktr2 )	/* 0246 (c) 1998 Saurus */
	DRIVER( flipshot )	/* 0247 (c) 1998 Visco */
	DRIVER( pbobbl2n )	/* 0248 (c) 1999 Taito (SNK license) */
	DRIVER( ctomaday )	/* 0249 (c) 1999 Visco */
	DRIVER( mslugx )	/* 0250 (c) 1999 SNK */
	DRIVER( kof99 )		/* 0251 (c) 1999 SNK */
	DRIVER( kof99a )	/* 0251 (c) 1999 SNK */
	DRIVER( kof99e )	/* 0251 (c) 1999 SNK */
	DRIVER( kof99n )	/* 0251 (c) 1999 SNK */
	DRIVER( kof99p )	/* 0251 (c) 1999 SNK */
	DRIVER( ganryu )	/* 0252 (c) 1999 Visco */
	DRIVER( garou )		/* 0253 (c) 1999 SNK */
	DRIVER( garouo )	/* 0253 (c) 1999 SNK */
	DRIVER( garoup )	/* 0253 (c) 1999 SNK */
	DRIVER( s1945p )	/* 0254 (c) 1999 Psikyo */
	DRIVER( preisle2 )	/* 0255 (c) 1999 Yumekobo */
	DRIVER( mslug3 )	/* 0256 (c) 2000 SNK */
	DRIVER( mslug3n )	/* 0256 (c) 2000 SNK */
	DRIVER( kof2000 )	/* 0257 (c) 2000 SNK */
	DRIVER( kof2000n )	/* 0257 (c) 2000 SNK */
	DRIVER( bangbead )	/* 0259 (c) 2000 Visco */
	DRIVER( nitd )		/* 0260 (c) 2000 Eleven / Gavaking */
	DRIVER( sengoku3 )	/* 0261 (c) 2001 SNK */
	DRIVER( kof2001 )	/* 0262 (c) 2001 Eolith */
	DRIVER( kof2001h )	/* 0262 (c) 2001 Eolith */
	DRIVER( cthd2003 )	/* 0262 (c) 2001 Eolith */
	DRIVER( ct2k3sp )	/* 0262 (c) 2001 Eolith */
	DRIVER( mslug4 )	/* 0263 (c) 2001 Mega Enterprises */
	DRIVER( rotd )		/* 0264 (c) 2002 Evoga Entertainment */
	DRIVER( kof2002 )	/* 0265 (c) 2002 Eolith */
	DRIVER( kf2k2pls )	/* bootleg */
	DRIVER( kf2k2pla )	/* bootleg */
	DRIVER( kf2k2mp )	/* bootleg */
	DRIVER( kf2k2mp2 )	/* bootleg */
	DRIVER( kof2k4se )  /* bootleg of kof2002 */
	DRIVER( matrim )	/* 0266 (c) 2002 Atlus / Noise Factory */
	DRIVER( pnyaa )		/* 0267 (c) 2003 Aiky / Taito */
	DRIVER( mslug5 )	/* 0268 (c) 2003 SNK Playmore */
	DRIVER( svc )		/* 0269 (c) 2003 Playmore / Capcom */
	DRIVER( svcboot )	/* 0269 (c) 2003 Bootleg */
	DRIVER( svcplus )	/* bootleg */
	DRIVER( svcplusa )	/* bootleg */
	DRIVER( svcsplus )	/* bootleg */
	DRIVER( samsho5 )	/* 0270 (c) 2003 Yuki Enterprises */
	DRIVER( samsh5sp )	/* 0270 (c) 2003 Yuki Enterprises */
	DRIVER( kof2003 )	/* 0271 (c) 2003 SNK Playmore */
	DRIVER( kof2003d)	/* 0271 (c) 2003 SNK Playmore */
	DRIVER( kf2k3bl)    /* bootleg */
	DRIVER( kf2k3bla)   /* bootleg */
	DRIVER( kf2k3pl)    /* bootleg */
	DRIVER( kf2k3upl)   /* bootleg */
	DRIVER( kof10th )       /* 2005 SNK Playmore */
	DRIVER( kf10thep)       /* bootleg of kof2002 */
	DRIVER( kf2k5uni)       /* bootleg of kof2002 */
	DRIVER( lasthope )      /* (c) 2005 NG:DEV.TEAM */
  DRIVER( knightsch )     /* (c) 2014 Neobitz */
	DRIVER( xeno )          /* (c) 2019 Bitmap Bureau */
	DRIVER( hypernoid )     /* 2021 M.Priewe */
  DRIVER( teot)           /* 2022 Ozzy Ouzo */
	DRIVER( cyborgforce )   /* (c) 2023 Neo Byte Force Ltd */
	DRIVER( captainbarrel ) /* (c) 2024 Neo Byte Force Ltd */
  DRIVER( gladmort )      /* (c) 2024 Pixelheart / ChipsOnSteroids */
  DRIVER( doubled1 )      /* (c) 2025 La Casa De Ruivo / Neo Byte Force */
  DRIVER( doubled1a )      /* (c) 2025 La Casa De Ruivo / Neo Byte Force */
  DRIVER( doubled1b )      /* (c) 2025 La Casa De Ruivo / Neo Byte Force */
  DRIVER( doubled1c )      /* (c) 2025 La Casa De Ruivo / Neo Byte Force */
  DRIVER( doubled1d )      /* (c) 2025 La Casa De Ruivo / Neo Byte Force */
  DRIVER( doubled1e )      /* (c) 2025 La Casa De Ruivo / Neo Byte Force */
	
	/* SemiCom 68020 based hardware */
	DRIVER( baryon )    /* (c) 1997 SemiCom / Tirano */
	DRIVER( cutefght )  /* (c) 1998 SemiCom */
	DRIVER( dreamwld )  /* (c) 2000 SemiCom */
	DRIVER( gaialast )  /* (c) 1999 SemiCom / XESS */
	DRIVER( rolcrush )  /* (c) 1999 Trust / SemiCom */

	/* Seibu Hardware */
	DRIVER( denjinmk )  /* (c) 1994 Banpresto */

	/* Mighty Warriors  */

	/* The Lost Castle In Darkmist */

	/* Varia Metal */

	/* Cave PGM System Games */

	/* Sega Model 1 Hardware */

  /* Oriental Soft */

  /* Current Tech */


	/* ---------------------------------------------------------------- */
	/* Games whose driver source is already compiled into the build but
	   which were dropped from the list above during the link-error triage.
	   Re-registered here; grouped by source file.                          */
	/* ---------------------------------------------------------------- */

	/* 40love.c */
	DRIVER( fieldday )
	DRIVER( undoukai )

	/* 8080bw.c */
	DRIVER( alieninv )
	DRIVER( ballbomb )
	DRIVER( bandido )
	DRIVER( cosmicmo )
	DRIVER( desterth )
	DRIVER( earthinv )
	DRIVER( galxwar2 )
	DRIVER( galxwars )
	DRIVER( galxwart )
	DRIVER( grescue )
	DRIVER( helifira )
	DRIVER( helifire )
	DRIVER( invaderl )
	DRIVER( invadpt2 )
	DRIVER( invrvnga )
	DRIVER( invrvnge )
	DRIVER( jspecter )
	DRIVER( jspectr2 )
	DRIVER( laser )
	DRIVER( lrescue )
	DRIVER( lupin3 )
	DRIVER( moonbase )
	DRIVER( ozmawar2 )
	DRIVER( ozmawars )
	DRIVER( polaris )
	DRIVER( polarisa )
	DRIVER( rollingc )
	DRIVER( schaser )
	DRIVER( schasrcv )
	DRIVER( sflush )
	DRIVER( sheriff )
	DRIVER( sicv )
	DRIVER( sinvemag )
	DRIVER( sinvzen )
	DRIVER( sisv )
	DRIVER( sisv2 )
	DRIVER( sitv )
	DRIVER( solfight )
	DRIVER( spaceat2 )
	DRIVER( spaceatt )
	DRIVER( spacefev )
	DRIVER( spaceph )
	DRIVER( spacewr3 )
	DRIVER( spceking )
	DRIVER( spcewarl )
	DRIVER( spcewars )
	DRIVER( spclaser )
	DRIVER( sstrangr )
	DRIVER( sstrngr2 )
	DRIVER( starw )
	DRIVER( superinv )
	DRIVER( yosakdon )

	/* 88games.c */
	DRIVER( 88games )
	DRIVER( hypsptsp )
	DRIVER( konami88 )

	/* aburner.c */
	DRIVER( aburner )
	DRIVER( aburner2 )

	/* aeroboto.c */
	DRIVER( aeroboto )
	DRIVER( formatz )

	/* afega.c */
	DRIVER( firehawk )
	DRIVER( grdnstrm )
	DRIVER( redhawk )
	DRIVER( spec2k )
	DRIVER( stagger1 )

	/* ajax.c */
	DRIVER( ajax )
	DRIVER( ajaxj )
	DRIVER( typhoon )

	/* aliens.c */
	DRIVER( aliens )
	DRIVER( aliens2 )
	DRIVER( aliensj )
	DRIVER( aliensu )

	/* alpha68k.c */
	DRIVER( btlfield )
	DRIVER( gangwarb )
	DRIVER( gangwars )
	DRIVER( goldmedb )
	DRIVER( goldmedl )
	DRIVER( kyros )
	DRIVER( kyrosj )
	DRIVER( paddlema )
	DRIVER( sbasebal )
	DRIVER( skyadvnj )
	DRIVER( skyadvnt )
	DRIVER( skyadvnu )
	DRIVER( skysoldr )
	DRIVER( sstingry )
	DRIVER( timesol1 )
	DRIVER( timesold )
	DRIVER( tnexspce )

	/* amidar.c */
	DRIVER( 600 )
	DRIVER( amidar )
	DRIVER( amidaro )
	DRIVER( amidaru )
	DRIVER( amigo )
	DRIVER( turpin )
	DRIVER( turtles )

	/* armedf.c */
	DRIVER( armedf )
	DRIVER( cclimbr2 )
	DRIVER( kodure )
	DRIVER( legion )
	DRIVER( legiono )
	DRIVER( terraf )
	DRIVER( terrafu )

	/* artmagic.c */
	DRIVER( cheesech )
	DRIVER( ultennis )

	/* asterix.c */
	DRIVER( asterix )
	DRIVER( astrxeaa )
	DRIVER( astrxeac )

	/* asteroid.c */
	DRIVER( astdelu1 )
	DRIVER( astdelux )
	DRIVER( asterock )
	DRIVER( asteroi1 )
	DRIVER( asteroib )
	DRIVER( asteroid )
	DRIVER( llander )
	DRIVER( llander1 )

	/* astinvad.c */
	DRIVER( astinvad )
	DRIVER( kamikaze )
	DRIVER( spcking2 )

	/* atarifb.c */
	DRIVER( abaseb )
	DRIVER( abaseb2 )
	DRIVER( atarifb )
	DRIVER( atarifb1 )
	DRIVER( atarifb4 )
	DRIVER( soccer )

	/* atarig1.c */
	DRIVER( hydra )
	DRIVER( hydrap )
	DRIVER( hydrap2 )
	DRIVER( pitfigh3 )
	DRIVER( pitfighb )
	DRIVER( pitfighj )
	DRIVER( pitfight )

	/* atarig42.c */
	DRIVER( guardian )
	DRIVER( roadriot )

	/* atarigt.c */
	DRIVER( primraga )
	DRIVER( primrage )
	DRIVER( tmek )
	DRIVER( tmekprot )

	/* atarigx2.c */
	DRIVER( motofren )
	DRIVER( rrreveng )
	DRIVER( rrrevenp )
	DRIVER( spclorda )
	DRIVER( spclords )

	/* atarisy2.c */
	DRIVER( 720 )
	DRIVER( 720g )
	DRIVER( 720gr1 )
	DRIVER( 720r1 )
	DRIVER( 720r2 )
	DRIVER( 720r3 )
	DRIVER( apb )
	DRIVER( apb1 )
	DRIVER( apb2 )
	DRIVER( apb3 )
	DRIVER( apb4 )
	DRIVER( apb5 )
	DRIVER( apb6 )
	DRIVER( apbf )
	DRIVER( apbg )
	DRIVER( cspring1 )
	DRIVER( csprins1 )
	DRIVER( csprint )
	DRIVER( csprint1 )
	DRIVER( csprint2 )
	DRIVER( csprintf )
	DRIVER( csprintg )
	DRIVER( csprints )
	DRIVER( paperboy )
	DRIVER( paperbr1 )
	DRIVER( paperbr2 )
	DRIVER( sspring1 )
	DRIVER( ssprint )
	DRIVER( ssprint1 )
	DRIVER( ssprint3 )
	DRIVER( ssprintf )
	DRIVER( ssprintg )
	DRIVER( ssprints )

	/* avengrgs.c */
	DRIVER( avengrgs )

	/* bagman.c */
	DRIVER( bagman )
	DRIVER( bagmans )
	DRIVER( bagmans2 )
	DRIVER( bagnard )
	DRIVER( botanic )
	DRIVER( botanicf )
	DRIVER( pickin )
	DRIVER( sbagman )
	DRIVER( sbagmans )

	/* balsente.c */
	DRIVER( cshift )
	DRIVER( gghost )
	DRIVER( gimeabrk )
	DRIVER( grudge )
	DRIVER( hattrick )
	DRIVER( minigol2 )
	DRIVER( minigolf )
	DRIVER( nametune )
	DRIVER( nstocker )
	DRIVER( otwalls )
	DRIVER( rescraid )
	DRIVER( rescrdsa )
	DRIVER( sentetst )
	DRIVER( sfootbal )
	DRIVER( shrike )
	DRIVER( snakepit )
	DRIVER( snakjack )
	DRIVER( spiker )
	DRIVER( stocker )
	DRIVER( stompin )
	DRIVER( toggle )
	DRIVER( triviabb )
	DRIVER( triviag1 )
	DRIVER( triviag2 )
	DRIVER( triviasp )
	DRIVER( triviayp )

	/* battlera.c */
	DRIVER( battlera )
	DRIVER( bldwolf )

	/* battlex.c */
	DRIVER( battlex )
	DRIVER( dodgeman )

	/* battlnts.c */
	DRIVER( battlntj )
	DRIVER( battlnts )
	DRIVER( rackemup )
	DRIVER( thehustj )
	DRIVER( thehustl )

	/* bladestl.c */
	DRIVER( bladestl )
	DRIVER( bladstle )

	/* blockhl.c */
	DRIVER( blockhl )
	DRIVER( quarth )

	/* bosco.c */
	DRIVER( bosco )
	DRIVER( boscomd )
	DRIVER( boscomdo )
	DRIVER( boscoo )
	DRIVER( boscoo2 )

	/* bottom9.c */
	DRIVER( bottom9 )
	DRIVER( bottom9n )
	DRIVER( mstadium )

	/* bwing.c */
	DRIVER( bwing )
	DRIVER( bwings )
	DRIVER( zaviga )
	DRIVER( zavigaj )

	/* bzone.c */
	DRIVER( bradley )
	DRIVER( bzone )
	DRIVER( bzone2 )
	DRIVER( bzonec )
	DRIVER( redbaron )

	/* canyon.c */
	DRIVER( canyon )
	DRIVER( canyonp )

	/* capbowl.c */
	DRIVER( bowlrama )
	DRIVER( capbowl )
	DRIVER( capbowl2 )
	DRIVER( clbowl )

	/* cbasebal.c */
	DRIVER( cbasebal )

	/* cchasm.c */
	DRIVER( cchasm )
	DRIVER( cchasm1 )

	/* centiped.c */
	DRIVER( centipb2 )
	DRIVER( centipd2 )
	DRIVER( centipdb )
	DRIVER( centiped )
	DRIVER( centtime )
	DRIVER( magworm )
	DRIVER( mazeinv )
	DRIVER( milliped )
	DRIVER( millpac )
	DRIVER( warlords )

	/* chaknpop.c */
	DRIVER( chaknpop )

	/* chinagat.c */
	DRIVER( chinagat )
	DRIVER( saiyugb2 )
	DRIVER( saiyugou )

	/* chqflag.c */
	DRIVER( chqflag )
	DRIVER( chqflagj )

	/* cinemat.c */
	DRIVER( armora )
	DRIVER( armorap )
	DRIVER( armorar )
	DRIVER( barrier )
	DRIVER( boxingb )
	DRIVER( demon )
	DRIVER( ripoff )
	DRIVER( speedfrk )
	DRIVER( starcas )
	DRIVER( starcas1 )
	DRIVER( starcase )
	DRIVER( starcasp )
	DRIVER( starhawk )
	DRIVER( stellcas )
	DRIVER( sundance )
	DRIVER( tailg )
	DRIVER( wotw )

	/* clshroad.c */
	DRIVER( clshroad )

	/* combatsc.c */
	DRIVER( bootcamp )
	DRIVER( combasc )
	DRIVER( combascb )
	DRIVER( combascj )
	DRIVER( combasct )

	/* congo.c */
	DRIVER( congo )
	DRIVER( tiptop )

	/* contra.c */
	DRIVER( contra )
	DRIVER( contrab )
	DRIVER( contraj )
	DRIVER( contrajb )
	DRIVER( gryzor )

	/* crimfght.c */
	DRIVER( crimfght )
	DRIVER( crimfgt2 )
	DRIVER( crimfgtj )

	/* crshrace.c */
	DRIVER( crshrac2 )
	DRIVER( crshrace )

	/* cyberbal.c */
	DRIVER( cyberb21 )
	DRIVER( cyberb22 )
	DRIVER( cyberb23 )
	DRIVER( cyberb2p )
	DRIVER( cyberba2 )
	DRIVER( cyberbal )
	DRIVER( cyberbap )
	DRIVER( cyberbt )
	DRIVER( cyberbt1 )

	/* dbz2.c */
	DRIVER( dbz )
	DRIVER( dbz2 )

	/* ddragon3.c */
	DRIVER( ctribe )
	DRIVER( ctribeb )

	/* dec0.c */
	DRIVER( baddudes )
	DRIVER( birdtry )
	DRIVER( bouldash )
	DRIVER( bouldshj )
	DRIVER( drgninja )
	DRIVER( ffantasa )
	DRIVER( ffantasy )
	DRIVER( hbarrel )
	DRIVER( hbarrelw )
	DRIVER( hippodrm )
	DRIVER( midres )
	DRIVER( midresbj )
	DRIVER( midresj )
	DRIVER( midresu )
	DRIVER( robocop )
	DRIVER( robocopb )
	DRIVER( robocopj )
	DRIVER( robocopu )
	DRIVER( robocopw )
	DRIVER( robocpu0 )
	DRIVER( secretab )
	DRIVER( secretag )
	DRIVER( slyspy )
	DRIVER( slyspy2 )

	/* decocass.c */
	DRIVER( cadanglr )
	DRIVER( castfant )
	DRIVER( cbdash )
	DRIVER( cbnj )
	DRIVER( cbtime )
	DRIVER( cburnrb2 )
	DRIVER( cburnrub )
	DRIVER( cdiscon1 )
	DRIVER( cfghtice )
	DRIVER( cfishing )
	DRIVER( cgraplop )
	DRIVER( clapapa )
	DRIVER( clapapa2 )
	DRIVER( clocknch )
	DRIVER( cluckypo )
	DRIVER( cmanhat )
	DRIVER( cmissnx )
	DRIVER( cnights2 )
	DRIVER( cnightst )
	DRIVER( coozumou )
	DRIVER( cppicf )
	DRIVER( cppicf2 )
	DRIVER( cprobowl )
	DRIVER( cprogolf )
	DRIVER( cprosocc )
	DRIVER( cptennis )
	DRIVER( cscrtry )
	DRIVER( cscrtry2 )
	DRIVER( cskater )
	DRIVER( csuperas )
	DRIVER( csweetht )
	DRIVER( cterrani )
	DRIVER( ctornado )
	DRIVER( ctsttape )
	DRIVER( czeroize )
	DRIVER( decocass )

	/* digdug.c */
	DRIVER( digdug )
	DRIVER( digdugat )
	DRIVER( digdugb )
	DRIVER( dzigzag )

	/* dkong.c */
	DRIVER( 8ballact )
	DRIVER( 8ballat2 )
	DRIVER( dkchrmx )
	DRIVER( dkngjnrb )
	DRIVER( dkngjnrj )
	DRIVER( dkong )
	DRIVER( dkong3 )
	DRIVER( dkong3b )
	DRIVER( dkong3j )
	DRIVER( dkongjo )
	DRIVER( dkongjo1 )
	DRIVER( dkongjp )
	DRIVER( dkongjr )
	DRIVER( dkongjrb )
	DRIVER( dkongjrj )
	DRIVER( dkongo )
	DRIVER( dkongpe )
	DRIVER( dkongx )
	DRIVER( dkremix )
	DRIVER( drakton )
	DRIVER( herbiedk )
	DRIVER( herodk )
	DRIVER( herodku )
	DRIVER( hunchbkd )
	DRIVER( jammin )
	DRIVER( pestplce )
	DRIVER( radarscp )
	DRIVER( radarscp1 )
	DRIVER( radarscpc )
	DRIVER( sbdk )
	DRIVER( shootgal )
	DRIVER( spcfrcii )
	DRIVER( spclforc )
	DRIVER( strtheat )

	/* dooyong.c */
	DRIVER( bluehawk )
	DRIVER( bluehawn )
	DRIVER( flytiger )
	DRIVER( gulfstr2 )
	DRIVER( gulfstrm )
	DRIVER( gundl94 )
	DRIVER( lastday )
	DRIVER( lastdaya )
	DRIVER( pollux )
	DRIVER( polluxa )
	DRIVER( primella )
	DRIVER( rshark )
	DRIVER( sadari )
	DRIVER( superx )
	DRIVER( superxm )

	/* dynax.c */
	DRIVER( 7jigen )
	DRIVER( drgpunch )
	DRIVER( hanamai )
	DRIVER( hnkochou )
	DRIVER( hnoridur )
	DRIVER( inca )
	DRIVER( maya )
	DRIVER( mcnpshnt )
	DRIVER( mjangels )
	DRIVER( mjdialq2 )
	DRIVER( mjfriday )
	DRIVER( quiztvqq )
	DRIVER( roldfrga )
	DRIVER( roldfrog )
	DRIVER( sprtmtch )
	DRIVER( yarunara )

	/* epos.c */
	DRIVER( eeekk )
	DRIVER( igmo )
	DRIVER( megadon )
	DRIVER( suprglob )
	DRIVER( theglob )
	DRIVER( theglob2 )
	DRIVER( theglob3 )

	/* esd16.c */
	DRIVER( deluxe5 )
	DRIVER( hedpanic )
	DRIVER( hedpanicf )
	DRIVER( hedpanico )
	DRIVER( mchampdx )
	DRIVER( mchampdxa )
	DRIVER( multchmp )
	DRIVER( multchmpk )
	DRIVER( swatpolc )
	DRIVER( tangtang )

	/* f1gp.c */
	DRIVER( f1gp )
	DRIVER( f1gp2 )

	/* fastlane.c */
	DRIVER( fastlane )

	/* firetrk.c */
	DRIVER( firetrk )
	DRIVER( montecar )
	DRIVER( superbug )

	/* flkatck.c */
	DRIVER( flkatck )
	DRIVER( mx5000 )

	/* flower.c */
	DRIVER( flower )
	DRIVER( flowerbl )

	/* flstory.c */
	DRIVER( flstory )
	DRIVER( flstoryj )
	DRIVER( onna34ra )
	DRIVER( onna34ro )
	DRIVER( rumba )
	DRIVER( victnine )

	/* frogger.c */
	DRIVER( frogger )
	DRIVER( frogseg1 )
	DRIVER( frogseg2 )

	/* fromanc2.c */
	DRIVER( fromanc2 )
	DRIVER( fromanc4 )
	DRIVER( fromancr )

	/* gaiden.c */
	DRIVER( drgnbowl )
	DRIVER( gaiden )
	DRIVER( raiga )
	DRIVER( ryukendn )
	DRIVER( shadoww )
	DRIVER( shadowwa )
	DRIVER( stratof )
	DRIVER( tknight )
	DRIVER( wildfang )

	/* galaxian.c */
	DRIVER( 4in1 )
	DRIVER( bagmanmc )
	DRIVER( batman2 )
	DRIVER( checkmaj )
	DRIVER( checkman )
	DRIVER( devilfsg )
	DRIVER( dingo )
	DRIVER( dkongjrm )
	DRIVER( drivfrcg )
	DRIVER( frogg )
	DRIVER( froggrmc )
	DRIVER( gmgalax )
	DRIVER( jumpbug )
	DRIVER( jumpbugb )
	DRIVER( ladybugg )
	DRIVER( levers )
	DRIVER( mshuttle )
	DRIVER( mshuttlj )
	DRIVER( omega )
	DRIVER( ozon1 )
	DRIVER( pacmanbl )
	DRIVER( rockclim )
	DRIVER( scorpnmc )
	DRIVER( scramblb )
	DRIVER( vpool )
	DRIVER( zigzag )
	DRIVER( zigzag2 )

	/* gaplus.c */
	DRIVER( galaga3 )
	DRIVER( galaga3a )
	DRIVER( galaga3m )
	DRIVER( gaplus )
	DRIVER( gaplusa )
	DRIVER( gapluso )

	/* gbusters.c */
	DRIVER( crazycop )
	DRIVER( gbusters )

	/* geebee.c */
	DRIVER( geebee )
	DRIVER( geebeeg )
	DRIVER( kaitei )
	DRIVER( kaitein )

	/* gijoe.c */
	DRIVER( gijoe )
	DRIVER( gijoej )
	DRIVER( gijoeu )

	/* gladiatr.c */
	DRIVER( gladiatr )
	DRIVER( ogonsiro )

	/* gotcha.c */
	DRIVER( gotcha )

	/* gradius3.c */
	DRIVER( gradius3 )
	DRIVER( grdius3a )
	DRIVER( grdius3e )

	/* grobda.c */
	DRIVER( grobda )
	DRIVER( grobda2 )
	DRIVER( grobda3 )

	/* gstriker.c */
	DRIVER( gstriker )
	DRIVER( vgoalsca )
	DRIVER( vgoalsoc )
	DRIVER( worldc94 )

	/* harddriv.c */
	DRIVER( harddrb5 )
	DRIVER( harddrb6 )
	DRIVER( harddrc1 )
	DRIVER( harddrcb )
	DRIVER( harddrcg )
	DRIVER( harddrg4 )
	DRIVER( harddriv )
	DRIVER( harddrj6 )
	DRIVER( harddrv1 )
	DRIVER( harddrv2 )
	DRIVER( harddrv3 )
	DRIVER( harddrvb )
	DRIVER( harddrvc )
	DRIVER( harddrvg )
	DRIVER( harddrvj )
	DRIVER( hdrivaip )
	DRIVER( hdrivair )
	DRIVER( racedcb4 )
	DRIVER( racedcg4 )
	DRIVER( racedrb1 )
	DRIVER( racedrb4 )
	DRIVER( racedrc1 )
	DRIVER( racedrc2 )
	DRIVER( racedrc4 )
	DRIVER( racedrcb )
	DRIVER( racedrcg )
	DRIVER( racedrg1 )
	DRIVER( racedrg4 )
	DRIVER( racedriv )
	DRIVER( racedrv1 )
	DRIVER( racedrv2 )
	DRIVER( racedrv3 )
	DRIVER( racedrv4 )
	DRIVER( racedrvb )
	DRIVER( racedrvc )
	DRIVER( racedrvg )
	DRIVER( steelta1 )
	DRIVER( steeltag )
	DRIVER( steeltal )
	DRIVER( steeltap )
	DRIVER( stunrn2e )
	DRIVER( stunrn3e )
	DRIVER( stunrun )
	DRIVER( stunrun0 )
	DRIVER( stunrun2 )
	DRIVER( stunrun3 )
	DRIVER( stunrun4 )
	DRIVER( stunrun5 )
	DRIVER( stunrune )
	DRIVER( stunrunj )
	DRIVER( stunrunp )

	/* hexion.c */
	DRIVER( hexion )

	/* hyprduel.c */
	DRIVER( hyprdelj )
	DRIVER( hyprduel )

	/* itech8.c */
	DRIVER( arlingtn )
	DRIVER( dynobop )
	DRIVER( gpgolf )
	DRIVER( gtg )
	DRIVER( gtg2 )
	DRIVER( gtg2j )
	DRIVER( gtg2t )
	DRIVER( hstenn10 )
	DRIVER( hstennis )
	DRIVER( neckneck )
	DRIVER( ninclown )
	DRIVER( peggle )
	DRIVER( pegglet )
	DRIVER( rimrck12 )
	DRIVER( rimrck16 )
	DRIVER( rimrck20 )
	DRIVER( rimrockn )
	DRIVER( sliksh17 )
	DRIVER( slikshot )
	DRIVER( sstrike )
	DRIVER( stratab )
	DRIVER( stratab1 )
	DRIVER( wfortuna )
	DRIVER( wfortune )

	/* jailbrek.c */
	DRIVER( jailbrek )
	DRIVER( manhatan )

	/* jrpacman.c */
	DRIVER( jrpacman )
	DRIVER( jrpacmnf )

	/* kickgoal.c */
	DRIVER( actionhw )
	DRIVER( kickgoal )
	DRIVER( topdrive )

	/* kingobox.c */
	DRIVER( kingofb )
	DRIVER( ringkin2 )
	DRIVER( ringkin3 )
	DRIVER( ringking )

	/* kinst.c */
	DRIVER( kinst )
	DRIVER( kinst2 )

	/* kncljoe.c */
	DRIVER( bcrusher )
	DRIVER( kncljoe )
	DRIVER( kncljoea )

	/* kopunch.c */
	DRIVER( kopunch )

	/* labyrunr.c */
	DRIVER( labyrunk )
	DRIVER( labyrunr )
	DRIVER( tricktrp )

	/* ladybug.c */
	DRIVER( cavenger )
	DRIVER( dorodon )
	DRIVER( dorodon2 )
	DRIVER( ladybug )
	DRIVER( ladybugb )
	DRIVER( mrsdyna )
	DRIVER( snapjack )
	DRIVER( sraider )

	/* lemmings.c */
	DRIVER( lemmings )

	/* liberatr.c */
	DRIVER( liberat2 )
	DRIVER( liberatr )

	/* lsasquad.c */
	DRIVER( daikaiju )
	DRIVER( lsasquad )
	DRIVER( storming )

	/* m107.c */
	DRIVER( dsoccr94 )
	DRIVER( firebarr )
	DRIVER( wpksoc )

	/* m72.c */
	DRIVER( airduel )
	DRIVER( bchopper )
	DRIVER( cosmccop )
	DRIVER( dkgenm72 )
	DRIVER( dkgensan )
	DRIVER( gallop )
	DRIVER( hharry )
	DRIVER( hharryu )
	DRIVER( imgfight )
	DRIVER( kengo )
	DRIVER( loht )
	DRIVER( majtitle )
	DRIVER( mrheli )
	DRIVER( nspirit )
	DRIVER( nspiritj )
	DRIVER( poundfor )
	DRIVER( poundfou )
	DRIVER( rtype )
	DRIVER( rtype2 )
	DRIVER( rtype2j )
	DRIVER( rtypepj )
	DRIVER( rtypeu )
	DRIVER( xmultipl )

	/* m90.c */
	DRIVER( atompunk )
	DRIVER( bbmanw )
	DRIVER( bbmanwj )
	DRIVER( bombrman )
	DRIVER( dicegame )
	DRIVER( dynablsb )
	DRIVER( dynablst )
	DRIVER( gussun )
	DRIVER( hasamu )
	DRIVER( matchit2 )
	DRIVER( quizf1 )
	DRIVER( riskchal )
	DRIVER( shisen2 )

	/* m92.c */
	DRIVER( bmaster )
	DRIVER( dsccr94j )
	DRIVER( geostorm )
	DRIVER( gunforc2 )
	DRIVER( gunforce )
	DRIVER( gunforcj )
	DRIVER( gunforcu )
	DRIVER( gunhohki )
	DRIVER( hook )
	DRIVER( hookj )
	DRIVER( hooku )
	DRIVER( inthunt )
	DRIVER( inthuntu )
	DRIVER( kaiteids )
	DRIVER( leaguemn )
	DRIVER( lethalth )
	DRIVER( majtitl2 )
	DRIVER( mysticri )
	DRIVER( nbbatman )
	DRIVER( psoldier )
	DRIVER( rtypelej )
	DRIVER( rtypeleo )
	DRIVER( skingam2 )
	DRIVER( skingame )
	DRIVER( ssoldier )
	DRIVER( thndblst )
	DRIVER( uccops )
	DRIVER( uccopsar )
	DRIVER( uccopsj )

	/* mainevt.c */
	DRIVER( devstor2 )
	DRIVER( devstor3 )
	DRIVER( devstors )
	DRIVER( garuka )
	DRIVER( mainev2p )
	DRIVER( mainevt )
	DRIVER( mainevto )
	DRIVER( ringohja )

	/* mainsnk.c */
	DRIVER( mainsnk )

	/* mappy.c */
	DRIVER( digdug2 )
	DRIVER( digdug2o )
	DRIVER( mappy )
	DRIVER( mappyj )
	DRIVER( motos )
	DRIVER( pacnchmp )
	DRIVER( pacnpal )
	DRIVER( pacnpal2 )
	DRIVER( phozon )
	DRIVER( superpac )
	DRIVER( superpcm )
	DRIVER( todruaga )
	DRIVER( todruago )

	/* marineb.c */
	DRIVER( bcruzm12 )
	DRIVER( changes )
	DRIVER( hoccer )
	DRIVER( hoccer2 )
	DRIVER( hopprobo )
	DRIVER( looper )
	DRIVER( marineb )
	DRIVER( springer )
	DRIVER( wanted )

	/* matmania.c */
	DRIVER( excthour )
	DRIVER( maniach )
	DRIVER( maniach2 )
	DRIVER( matmania )

	/* mayumi.c */
	DRIVER( mayumi )

	/* mazerbla.c */
	DRIVER( greatgun )
	DRIVER( mazerbla )

	/* mermaid.c */
	DRIVER( mermaid )
	DRIVER( rougien )

	/* metro.c */
	DRIVER( 3kokushi )
	DRIVER( balcube )
	DRIVER( bangball )
	DRIVER( blzntrnd )
	DRIVER( daitorid )
	DRIVER( dharma )
	DRIVER( dokyusei )
	DRIVER( dokyusp )
	DRIVER( gakusai )
	DRIVER( gakusai2 )
	DRIVER( gstrik2 )
	DRIVER( gunmast )
	DRIVER( karatour )
	DRIVER( ladykill )
	DRIVER( lastfero )
	DRIVER( lastfort )
	DRIVER( moegonta )
	DRIVER( mouja )
	DRIVER( msgogo )
	DRIVER( pangpomm )
	DRIVER( pangpoms )
	DRIVER( poitto )
	DRIVER( pururun )
	DRIVER( puzzli )
	DRIVER( skyalert )
	DRIVER( toride2g )

	/* mexico86.c */
	DRIVER( kicknrun )
	DRIVER( kikikai )
	DRIVER( mexico86 )

	/* midvunit.c */
	DRIVER( crusnw13 )
	DRIVER( crusnw20 )
	DRIVER( crusnwld )
	DRIVER( wargods )

	/* midwunit.c */
	DRIVER( mk3 )
	DRIVER( mk3r10 )
	DRIVER( mk3r20 )

	/* midyunit.c */
	DRIVER( mkla1 )
	DRIVER( mkla2 )
	DRIVER( mkla3 )
	DRIVER( mkla4 )
	DRIVER( mkprot9 )
	DRIVER( strkforc )

	/* mitchell.c */
	DRIVER( 7toitsu )
	DRIVER( bbros )
	DRIVER( block )
	DRIVER( blocka )
	DRIVER( blockbl )
	DRIVER( blockj )
	DRIVER( cworld )
	DRIVER( dokaben )
	DRIVER( hatena )
	DRIVER( marukin )
	DRIVER( mgakuen )
	DRIVER( mgakuen2 )
	DRIVER( mstworld )
	DRIVER( pang )
	DRIVER( pangb )
	DRIVER( pkladiel )
	DRIVER( pkladies )
	DRIVER( pompingw )
	DRIVER( qsangoku )
	DRIVER( qtono1 )
	DRIVER( sbbros )
	DRIVER( spang )

	/* moo.c */
	DRIVER( bucky )
	DRIVER( buckyua )
	DRIVER( moo )
	DRIVER( moobl )
	DRIVER( mooua )

	/* ms32.c */
	DRIVER( 47pie2 )
	DRIVER( 47pie2o )
	DRIVER( akiss )
	DRIVER( bbbxing )
	DRIVER( desertwr )
	DRIVER( f1superb )
	DRIVER( gametngk )
	DRIVER( gratia )
	DRIVER( gratiaa )
	DRIVER( hayaosi1 )
	DRIVER( kirarast )
	DRIVER( p47aces )
	DRIVER( tetrisp )
	DRIVER( tp2m32 )

	/* mustache.c */
	DRIVER( mustache )

	/* mystston.c */
	DRIVER( myststno )
	DRIVER( mystston )

	/* namconb1.c */
	DRIVER( gslgr94u )
	DRIVER( gunbulet )
	DRIVER( machbrkr )
	DRIVER( nebulray )
	DRIVER( nebulryj )
	DRIVER( outfxesj )
	DRIVER( outfxies )
	DRIVER( ptblank )
	DRIVER( sws95 )
	DRIVER( sws96 )
	DRIVER( sws97 )
	DRIVER( vshoot )

	/* namcos2.c */
	DRIVER( assault )
	DRIVER( assaultj )
	DRIVER( assaultp )
	DRIVER( burnforc )
	DRIVER( cosmogng )
	DRIVER( cosmognj )
	DRIVER( dirtfoxj )
	DRIVER( dsaber )
	DRIVER( dsaberj )
	DRIVER( finalap2 )
	DRIVER( finalap3 )
	DRIVER( finalapc )
	DRIVER( finalapd )
	DRIVER( finallap )
	DRIVER( finalp2j )
	DRIVER( finalp3a )
	DRIVER( finehour )
	DRIVER( finlapjb )
	DRIVER( finlapjc )
	DRIVER( fourtrax )
	DRIVER( kyukaidk )
	DRIVER( kyukaido )
	DRIVER( marvland )
	DRIVER( marvlanj )
	DRIVER( metlhawk )
	DRIVER( mirninja )
	DRIVER( ordyne )
	DRIVER( phelios )
	DRIVER( rthun2 )
	DRIVER( rthun2j )
	DRIVER( suzuk8h2 )
	DRIVER( suzuk8hj )
	DRIVER( suzuka8h )
	DRIVER( sws92 )
	DRIVER( sws92g )
	DRIVER( sws93 )
	DRIVER( valkyrie )

	/* namcos21.c */
	DRIVER( aircombj )
	DRIVER( aircombu )
	DRIVER( cybsled )
	DRIVER( solvalou )
	DRIVER( starblad )
	DRIVER( winrun91 )

	/* namcos22.c */
	DRIVER( acedrvrw )
	DRIVER( airco22b )
	DRIVER( alpinerc )
	DRIVER( alpinerd )
	DRIVER( cybrcomm )
	DRIVER( cybrcycc )
	DRIVER( propcycl )
	DRIVER( raveracw )
	DRIVER( ridger2j )
	DRIVER( ridgeraj )
	DRIVER( timecrsa )
	DRIVER( victlapw )

	/* namcos86.c */
	DRIVER( genpeitd )
	DRIVER( hopmappy )
	DRIVER( roishtar )
	DRIVER( rthunder )
	DRIVER( rthundro )
	DRIVER( skykiddo )
	DRIVER( skykiddx )
	DRIVER( wndrmomo )

	/* naughtyb.c */
	DRIVER( naughtya )
	DRIVER( naughtyb )
	DRIVER( naughtyc )
	DRIVER( popflama )
	DRIVER( popflamb )
	DRIVER( popflame )

	/* neogeo.c */
	DRIVER( kf2k3pcb )
	DRIVER( svcpcb )

	/* ninjakd2.c */
	DRIVER( ninjak2a )
	DRIVER( ninjak2b )
	DRIVER( ninjakd2 )
	DRIVER( rdaction )

	/* niyanpai.c */
	DRIVER( niyanpai )

	/* nmk16.c */
	DRIVER( acrobatm )
	DRIVER( airattck )
	DRIVER( airattcka )
	DRIVER( bigbang )
	DRIVER( bioship )
	DRIVER( bjtwin )
	DRIVER( blkhearj )
	DRIVER( blkheart )
	DRIVER( dolmen )
	DRIVER( gunnail )
	DRIVER( gunnailp )
	DRIVER( hachamf )
	DRIVER( hachamfb )
	DRIVER( macross )
	DRIVER( macross2 )
	DRIVER( manybloc )
	DRIVER( mustang )
	DRIVER( mustangb )
	DRIVER( mustangs )
	DRIVER( nouryoku )
	DRIVER( raphero )
	DRIVER( sabotenb )
	DRIVER( sabotnba )
	DRIVER( ssmissin )
	DRIVER( strahl )
	DRIVER( strahla )
	DRIVER( tdragon )
	DRIVER( tdragon2 )
	DRIVER( tdragonb )
	DRIVER( tharierj )
	DRIVER( tharrier )
	DRIVER( twinactn )
	DRIVER( vandyjal )
	DRIVER( vandyke )

	/* ojankohs.c */
	DRIVER( ccasino )
	DRIVER( ojanko2 )
	DRIVER( ojankoc )
	DRIVER( ojankohs )
	DRIVER( ojankoy )

	/* omegaf.c */
	DRIVER( omegaf )
	DRIVER( omegafs )
	DRIVER( robokid )
	DRIVER( robokidj )

	/* oneshot.c */
	DRIVER( maddonna )

	/* opwolf.c */
	DRIVER( opwolf )
	DRIVER( opwolfb )

	/* othldrby.c */
	DRIVER( othldrby )

	/* overdriv.c */
	DRIVER( overdriv )

	/* pacland.c */
	DRIVER( pacland )
	DRIVER( pacland2 )
	DRIVER( pacland3 )
	DRIVER( paclandm )

	/* pacman.c */
	DRIVER( 8bpm )
	DRIVER( acitya )
	DRIVER( alibaba )
	DRIVER( alibabab )
	DRIVER( beastf )
	DRIVER( bigbucks )
	DRIVER( bwcasino )
	DRIVER( crush )
	DRIVER( crush2 )
	DRIVER( crush3 )
	DRIVER( dremshpr )
	DRIVER( drivfrcp )
	DRIVER( eggor )
	DRIVER( eyes )
	DRIVER( eyes2 )
	DRIVER( gorkans )
	DRIVER( hangly )
	DRIVER( hangly2 )
	DRIVER( hangly3 )
	DRIVER( joyman )
	DRIVER( jumpshot )
	DRIVER( korosuke )
	DRIVER( lizwiz )
	DRIVER( maketrax )
	DRIVER( maketrxb )
	DRIVER( mbrush )
	DRIVER( mrtnt )
	DRIVER( mschamp )
	DRIVER( mspacmab )
	DRIVER( mspacman )
	DRIVER( mspacmat )
	DRIVER( mspacmnf )
	DRIVER( mspacpls )
	DRIVER( mspactwin )
	DRIVER( newpuc2 )
	DRIVER( newpuc2b )
	DRIVER( newpuckx )
	DRIVER( nmouse )
	DRIVER( nmouseb )
	DRIVER( pacgal )
	DRIVER( pacheart )
	DRIVER( pacman )
	DRIVER( pacmanf )
	DRIVER( pacmod )
	DRIVER( pacplus )
	DRIVER( paintrlr )
	DRIVER( piranha )
	DRIVER( piranhah )
	DRIVER( piranhao )
	DRIVER( ponpoko )
	DRIVER( ponpokov )
	DRIVER( porky )
	DRIVER( puckman )
	DRIVER( puckmana )
	DRIVER( puckmod )
	DRIVER( shootbul )
	DRIVER( theglobp )
	DRIVER( vanvan )
	DRIVER( vanvank )
	DRIVER( woodpeca )
	DRIVER( woodpeck )

	/* paradise.c */
	DRIVER( madball )
	DRIVER( madballn )
	DRIVER( paradise )
	DRIVER( penky )
	DRIVER( tgtball )
	DRIVER( tgtballa )
	DRIVER( torus )

	/* parodius.c */
	DRIVER( parodisj )
	DRIVER( parodius )

	/* pbillian.c */
	DRIVER( hotsmash )
	DRIVER( pbillian )

	/* pengo.c */
	DRIVER( pengo )
	DRIVER( pengoa )
	DRIVER( pengob )
	DRIVER( pengoc )
	DRIVER( pengoj )
	DRIVER( pengoja )
	DRIVER( pengojb )
	DRIVER( pengojbl )
	DRIVER( penta )

	/* pgm.c */
	DRIVER( ddp2 )
	DRIVER( ddp3 )
	DRIVER( ddp3a )
	DRIVER( ddp3b )
	DRIVER( ddp3blk )
	DRIVER( dragwld2 )
	DRIVER( espgal )
	DRIVER( ket )
	DRIVER( keta )
	DRIVER( ketb )
	DRIVER( ketikd )
	DRIVER( killbld )
	DRIVER( kov )
	DRIVER( kov115 )
	DRIVER( kovplus )
	DRIVER( kovsh )
	DRIVER( olds )
	DRIVER( olds103t )
	DRIVER( orlegend )
	DRIVER( orlegndc )
	DRIVER( orlegnde )
	DRIVER( pgm )
	DRIVER( photoy2k )
	DRIVER( puzlstar )
	DRIVER( puzzli2 )
	DRIVER( puzzli2s )
	DRIVER( py2k2 )

	/* phoenix.c */
	DRIVER( condor )
	DRIVER( falcon )
	DRIVER( phoenix )
	DRIVER( phoenix3 )
	DRIVER( phoenixa )
	DRIVER( phoenixc )
	DRIVER( phoenixt )
	DRIVER( vautour )

	/* pitnrun.c */
	DRIVER( jumpkun )
	DRIVER( pitnrun )

	/* playch10.c */
	DRIVER( pc_1942 )
	DRIVER( pc_bball )
	DRIVER( pc_bfght )
	DRIVER( pc_bstar )
	DRIVER( pc_cntra )
	DRIVER( pc_cshwk )
	DRIVER( pc_cvnia )
	DRIVER( pc_dbldr )
	DRIVER( pc_ddrgn )
	DRIVER( pc_drmro )
	DRIVER( pc_duckh )
	DRIVER( pc_ebike )
	DRIVER( pc_ftqst )
	DRIVER( pc_gntlt )
	DRIVER( pc_golf )
	DRIVER( pc_goons )
	DRIVER( pc_grdue )
	DRIVER( pc_grdus )
	DRIVER( pc_hgaly )
	DRIVER( pc_kngfu )
	DRIVER( pc_mario )
	DRIVER( pc_miket )
	DRIVER( pc_mman3 )
	DRIVER( pc_moglf )
	DRIVER( pc_mtoid )
	DRIVER( pc_ngai2 )
	DRIVER( pc_ngai3 )
	DRIVER( pc_ngaid )
	DRIVER( pc_pinbt )
	DRIVER( pc_pwbld )
	DRIVER( pc_pwrst )
	DRIVER( pc_radr2 )
	DRIVER( pc_radrc )
	DRIVER( pc_rcpam )
	DRIVER( pc_rkats )
	DRIVER( pc_rnatk )
	DRIVER( pc_rrngr )
	DRIVER( pc_rygar )
	DRIVER( pc_sjetm )
	DRIVER( pc_smb )
	DRIVER( pc_smb2 )
	DRIVER( pc_smb3 )
	DRIVER( pc_suprc )
	DRIVER( pc_tbowl )
	DRIVER( pc_tenis )
	DRIVER( pc_tkfld )
	DRIVER( pc_tmnt )
	DRIVER( pc_tmnt2 )
	DRIVER( pc_trjan )
	DRIVER( pc_vball )
	DRIVER( pc_wcup )
	DRIVER( pc_wgnmn )
	DRIVER( pc_ynoid )
	DRIVER( playch10 )

	/* playmark.c */
	DRIVER( bigtwin )
	DRIVER( hrdtimes )
	DRIVER( powerbal )
	DRIVER( wbeachvl )

	/* plygonet.c */
	DRIVER( plygonet )

	/* polepos.c */
	DRIVER( polepos )
	DRIVER( polepos2 )
	DRIVER( polepos2a )
	DRIVER( polepos2b )
	DRIVER( poleposa1 )
	DRIVER( poleposa1n )
	DRIVER( poleposa2 )
	DRIVER( poleposj )
	DRIVER( ppspeed )
	DRIVER( topracer )
	DRIVER( topracera )

	/* prehisle.c */
	DRIVER( gensitou )
	DRIVER( prehisle )
	DRIVER( prehislu )

	/* psikyo.c */
	DRIVER( btlkroad )
	DRIVER( gunbird )
	DRIVER( gunbirdj )
	DRIVER( gunbirdk )
	DRIVER( s1945 )
	DRIVER( s1945j )
	DRIVER( s1945jn )
	DRIVER( samuraia )
	DRIVER( sngkace )
	DRIVER( tengai )

	/* psikyo4.c */
	DRIVER( hotdebut )
	DRIVER( loderdfa )
	DRIVER( loderndf )

	/* psikyosh.c */
	DRIVER( daraku )
	DRIVER( gunbird2 )
	DRIVER( sbomberb )

	/* psychic5.c */
	DRIVER( psychic5 )

	/* redalert.c */
	DRIVER( demoneye )
	DRIVER( redalert )

	/* renegade.c */
	DRIVER( kuniokub )
	DRIVER( kuniokun )
	DRIVER( renegade )
	DRIVER( renegadeb )

	/* retofinv.c */
	DRIVER( retofin1 )
	DRIVER( retofin2 )
	DRIVER( retofinv )

	/* rockrage.c */
	DRIVER( rockrage )
	DRIVER( rockragj )

	/* rohga.c */
	DRIVER( darksel2 )
	DRIVER( hangzo )
	DRIVER( nitrobal )
	DRIVER( rohga )
	DRIVER( rohgah )
	DRIVER( rohgau )
	DRIVER( wizdfire )
	DRIVER( wolffang )

	/* rollerg.c */
	DRIVER( rollerg )
	DRIVER( rollergj )

	/* rungun.c */
	DRIVER( slmdunkj )

	/* sauro.c */
	DRIVER( trckydoc )

	/* sbowling.c */
	DRIVER( sbowling )

	/* scobra.c */
	DRIVER( anteater )
	DRIVER( aponow )
	DRIVER( armorca2 )
	DRIVER( armorcar )
	DRIVER( billiard )
	DRIVER( calipso )
	DRIVER( darkplnt )
	DRIVER( hustler )
	DRIVER( hustlerb )
	DRIVER( losttmbh )
	DRIVER( losttomb )
	DRIVER( mimonkey )
	DRIVER( mimonsco )
	DRIVER( minefld )
	DRIVER( moonwar )
	DRIVER( moonwara )
	DRIVER( rescue )
	DRIVER( scobra )
	DRIVER( scobrab )
	DRIVER( scobras )
	DRIVER( scobrase )
	DRIVER( spdcoin )
	DRIVER( stratgys )
	DRIVER( stratgyx )
	DRIVER( superbon )
	DRIVER( tazmani2 )
	DRIVER( tazmania )

	/* scramble.c */
	DRIVER( 800fath )
	DRIVER( amidars )
	DRIVER( atlantis )
	DRIVER( atlants2 )
	DRIVER( cavelon )
	DRIVER( ckongs )
	DRIVER( devilfsh )
	DRIVER( explorer )
	DRIVER( froggers )
	DRIVER( hotshock )
	DRIVER( hunchbks )
	DRIVER( knockout )
	DRIVER( mariner )
	DRIVER( mars )
	DRIVER( mimonscr )
	DRIVER( mrkougar )
	DRIVER( mrkougb )
	DRIVER( mrkougr2 )
	DRIVER( newsin7 )
	DRIVER( scramble )
	DRIVER( scrambls )
	DRIVER( sfx )
	DRIVER( theend )
	DRIVER( theends )
	DRIVER( triplep )

	/* segac2.c */
	DRIVER( aladmdb )
	DRIVER( barek2ch )
	DRIVER( barek3mb )
	DRIVER( bloxeedc )
	DRIVER( borench )
	DRIVER( columns )
	DRIVER( columns2 )
	DRIVER( columnsj )
	DRIVER( headonch )
	DRIVER( ichir )
	DRIVER( ichirj )
	DRIVER( ichirjbl )
	DRIVER( ichirk )
	DRIVER( jparkmb )
	DRIVER( jzth )
	DRIVER( ooparts )
	DRIVER( pclubj )
	DRIVER( pclubjv2 )
	DRIVER( pclubjv4 )
	DRIVER( pclubjv5 )
	DRIVER( potopoto )
	DRIVER( puckpkmn )
	DRIVER( puyopuy2 )
	DRIVER( puyopuya )
	DRIVER( puyopuyb )
	DRIVER( puyopuyo )
	DRIVER( ribbit )
	DRIVER( sbubsm )
	DRIVER( shuifeng )
	DRIVER( songjang )
	DRIVER( sonic2mb )
	DRIVER( sonic3mb )
	DRIVER( ssonicbr )
	DRIVER( stkclmns )
	DRIVER( tantr )
	DRIVER( tantrbl )
	DRIVER( tantrbl2 )
	DRIVER( tantrkor )
	DRIVER( tfrceac )
	DRIVER( tfrceacb )
	DRIVER( tfrceacj )
	DRIVER( tfrceacjpb )
	DRIVER( twinktmb )
	DRIVER( zunkyou )

	/* segasyse.c */
	DRIVER( astrofl )
	DRIVER( fantzn2 )
	DRIVER( hangonjr )
	DRIVER( megrescu )
	DRIVER( opaopa )
	DRIVER( opaopan )
	DRIVER( ridleofp )
	DRIVER( slapshtr )
	DRIVER( tetrisse )
	DRIVER( transfrm )

	/* seta.c */
	DRIVER( arbalest )
	DRIVER( atehate )
	DRIVER( blandia )
	DRIVER( blandiap )
	DRIVER( blockcar )
	DRIVER( calibr50 )
	DRIVER( daioh )
	DRIVER( downtowj )
	DRIVER( downtown )
	DRIVER( drgnunit )
	DRIVER( eightfrc )
	DRIVER( extdwnhl )
	DRIVER( gundhara )
	DRIVER( jjsquawk )
	DRIVER( kamenrid )
	DRIVER( kiwame )
	DRIVER( krzybowl )
	DRIVER( madshark )
	DRIVER( metafox )
	DRIVER( msgunda1 )
	DRIVER( msgundam )
	DRIVER( neobattl )
	DRIVER( oisipuzl )
	DRIVER( qzkklgy2 )
	DRIVER( qzkklogy )
	DRIVER( rezon )
	DRIVER( sokonuke )
	DRIVER( stg )
	DRIVER( thunderl )
	DRIVER( tndrcade )
	DRIVER( tndrcadj )
	DRIVER( triplfun )
	DRIVER( twineagl )
	DRIVER( umanclub )
	DRIVER( usclssic )
	DRIVER( utoukond )
	DRIVER( wits )
	DRIVER( wrofaero )
	DRIVER( zingzip )
	DRIVER( zombraid )

	/* seta2.c */
	DRIVER( deerhunt )
	DRIVER( deerhunta )
	DRIVER( deerhuntb )
	DRIVER( grdians )
	DRIVER( gundamex )
	DRIVER( mj4simai )
	DRIVER( myangel )
	DRIVER( myangel2 )
	DRIVER( penbros )
	DRIVER( pzlbowl )
	DRIVER( trophyh )
	DRIVER( turkhunt )
	DRIVER( wschamp )
	DRIVER( wschampa )

	/* sf1.c */
	DRIVER( sf1 )
	DRIVER( sf1jp )
	DRIVER( sf1p )
	DRIVER( sf1us )

	/* sg1000a.c */
	DRIVER( chboxing )
	DRIVER( chwrestl )
	DRIVER( dokidoki )

	/* shangkid.c */
	DRIVER( chinher2 )
	DRIVER( chinhero )

	/* sharrier.c */
	DRIVER( endurob2 )
	DRIVER( endurobl )
	DRIVER( enduror )
	DRIVER( hangon )
	DRIVER( sharrier )

	/* shisen.c */
	DRIVER( matchit )
	DRIVER( shisen )
	DRIVER( sichuan2 )
	DRIVER( sichuana )

	/* shougi.c */
	DRIVER( shougi )
	DRIVER( shougi2 )

	/* simpsons.c */
	DRIVER( simps2pa )
	DRIVER( simps2pj )
	DRIVER( simpsn2p )
	DRIVER( simpsons )

	/* slapfght.c */
	DRIVER( alcon )
	DRIVER( getstar )
	DRIVER( getstarb )
	DRIVER( getstarj )
	DRIVER( perfrman )
	DRIVER( perfrmau )
	DRIVER( slapbtjp )
	DRIVER( slapbtuk )
	DRIVER( slapfigh )
	DRIVER( tigerh )
	DRIVER( tigerh2 )
	DRIVER( tigerhb1 )
	DRIVER( tigerhb2 )
	DRIVER( tigerhj )

	/* spy.c */
	DRIVER( spy )
	DRIVER( spyu )

	/* srmp2.c */
	DRIVER( mjyuugi )
	DRIVER( mjyuugia )
	DRIVER( ponchin )
	DRIVER( ponchina )
	DRIVER( srmp2 )
	DRIVER( srmp3 )

	/* ssv.c */
	DRIVER( cairblad )
	DRIVER( drifto94 )
	DRIVER( dynagear )
	DRIVER( eaglshot )
	DRIVER( eaglshta )
	DRIVER( gdfs )
	DRIVER( hypreac2 )
	DRIVER( hypreact )
	DRIVER( janjans1 )
	DRIVER( jsk )
	DRIVER( keithlcy )
	DRIVER( koikois2 )
	DRIVER( meosism )
	DRIVER( mslider )
	DRIVER( ryorioh )
	DRIVER( srmp4 )
	DRIVER( srmp4o )
	DRIVER( srmp7 )
	DRIVER( stmblade )
	DRIVER( survarts )
	DRIVER( sxyreact )
	DRIVER( twineag2 )
	DRIVER( ultrax )
	DRIVER( vasara )
	DRIVER( vasara2 )
	DRIVER( vasara2a )

	/* stlforce.c */
	DRIVER( mortalr )
	DRIVER( stlforce )

	/* suna8.c */
	DRIVER( brickzn )
	DRIVER( brickzn3 )
	DRIVER( hardhea2 )
	DRIVER( hardhead )
	DRIVER( hardhedb )
	DRIVER( rranger )
	DRIVER( sparkman )
	DRIVER( sranger )
	DRIVER( srangerb )
	DRIVER( srangerw )
	DRIVER( starfigh )

	/* superchs.c */
	DRIVER( superchs )

	/* superqix.c */
	DRIVER( perestro )
	DRIVER( sqixbl )
	DRIVER( superqix )

	/* suprnova.c */
	DRIVER( skns )

	/* suprslam.c */
	DRIVER( suprslam )

	/* surpratk.c */
	DRIVER( surpratk )

	/* system1.c */
	DRIVER( 4dwarrio )
	DRIVER( blckgalb )
	DRIVER( blockgal )
	DRIVER( brain )
	DRIVER( bullfgt )
	DRIVER( chplft )
	DRIVER( chplftb )
	DRIVER( chplftbl )
	DRIVER( dakkochn )
	DRIVER( flicky )
	DRIVER( flickyo )
	DRIVER( gardia )
	DRIVER( gardiab )
	DRIVER( hvymetal )
	DRIVER( imsorry )
	DRIVER( imsorryj )
	DRIVER( mrviking )
	DRIVER( mrvikngj )
	DRIVER( myhero )
	DRIVER( myherok )
	DRIVER( noboranb )
	DRIVER( nprinces )
	DRIVER( nprincsb )
	DRIVER( nprincso )
	DRIVER( nprincsu )
	DRIVER( pitfall2 )
	DRIVER( pitfallu )
	DRIVER( raflesia )
	DRIVER( regulus )
	DRIVER( reguluso )
	DRIVER( regulusu )
	DRIVER( seganinj )
	DRIVER( seganinu )
	DRIVER( shtngmst )
	DRIVER( spatter )
	DRIVER( ssanchan )
	DRIVER( sscandal )
	DRIVER( starjack )
	DRIVER( starjacs )
	DRIVER( swat )
	DRIVER( teddybb )
	DRIVER( teddybbo )
	DRIVER( thetogyu )
	DRIVER( tokisens )
	DRIVER( ufosensb )
	DRIVER( ufosensi )
	DRIVER( upndown )
	DRIVER( upndownu )
	DRIVER( wbdeluxe )
	DRIVER( wbml )
	DRIVER( wbmlb )
	DRIVER( wbmljb )
	DRIVER( wbmljo )
	DRIVER( wbmlvc )
	DRIVER( wboy )
	DRIVER( wboy2 )
	DRIVER( wboy2u )
	DRIVER( wboy3 )
	DRIVER( wboyo )
	DRIVER( wboyu )
	DRIVER( wmatch )

	/* system16.c */
	DRIVER( aceattac )
	DRIVER( afighter )
	DRIVER( afightera )
	DRIVER( alexkidd )
	DRIVER( alexkidd1 )
	DRIVER( aliensya )
	DRIVER( aliensyb )
	DRIVER( aliensyj )
	DRIVER( aliensyn )
	DRIVER( altbeas2 )
	DRIVER( altbeast )
	DRIVER( atomicp )
	DRIVER( bayrouta )
	DRIVER( bayroute )
	DRIVER( bayrtbl1 )
	DRIVER( bayrtbl2 )
	DRIVER( bodyslam )
	DRIVER( cotton )
	DRIVER( cottona )
	DRIVER( dduxbl )
	DRIVER( dumpmtmt )
	DRIVER( eswat )
	DRIVER( eswatbl )
	DRIVER( exctleag )
	DRIVER( fantzone )
	DRIVER( fantzono )
	DRIVER( fpoint )
	DRIVER( fpointbj )
	DRIVER( fpointbl )
	DRIVER( goldnabl )
	DRIVER( goldnaxa )
	DRIVER( goldnaxb )
	DRIVER( goldnaxc )
	DRIVER( goldnaxe )
	DRIVER( goldnaxj )
	DRIVER( hwchamp )
	DRIVER( jyuohki )
	DRIVER( mjleague )
	DRIVER( mvp )
	DRIVER( passht4b )
	DRIVER( passsht )
	DRIVER( passshtb )
	DRIVER( quartet )
	DRIVER( quartet2 )
	DRIVER( quartetj )
	DRIVER( quartt2j )
	DRIVER( riotcity )
	DRIVER( ryukyu )
	DRIVER( sdi )
	DRIVER( sdioj )
	DRIVER( shinobi )
	DRIVER( shinobia )
	DRIVER( shinobib )
	DRIVER( shinobl )
	DRIVER( snapper )
	DRIVER( sonicbom )
	DRIVER( suprleag )
	DRIVER( tetris )
	DRIVER( tetrisa )
	DRIVER( tetrisbl )
	DRIVER( timscanr )
	DRIVER( toryumon )
	DRIVER( tturf )
	DRIVER( tturfbl )
	DRIVER( tturfu )
	DRIVER( wb3 )
	DRIVER( wb3a )
	DRIVER( wb3bl )
	DRIVER( wrestwar )

	/* system24.c */
	DRIVER( bnzabros )
	DRIVER( crkdown )
	DRIVER( dcclub )
	DRIVER( gground )
	DRIVER( hotrod )
	DRIVER( mahmajn )
	DRIVER( mahmajn2 )
	DRIVER( qgh )
	DRIVER( qrouka )
	DRIVER( qsww )
	DRIVER( quizmeku )
	DRIVER( sgmast )
	DRIVER( sspirits )

	/* tail2nos.c */
	DRIVER( sformula )
	DRIVER( tail2nos )

	/* taito_f3.c */
	DRIVER( akkanvdr )
	DRIVER( arabiamj )
	DRIVER( arabiamu )
	DRIVER( arabianm )
	DRIVER( arkretrn )
	DRIVER( bubblemj )
	DRIVER( bublbob2 )
	DRIVER( bublbob2p )
	DRIVER( bubsympe )
	DRIVER( bubsymph )
	DRIVER( bubsympu )
	DRIVER( cleopatr )
	DRIVER( commandw )
	DRIVER( cupfinal )
	DRIVER( dankuga )
	DRIVER( dariusg )
	DRIVER( dariusgj )
	DRIVER( dariusgu )
	DRIVER( dariusgx )
	DRIVER( dungenmu )
	DRIVER( dungeonm )
	DRIVER( elvact2u )
	DRIVER( elvactr )
	DRIVER( elvactrj )
	DRIVER( gblchmp )
	DRIVER( gekirido )
	DRIVER( gseeker )
	DRIVER( gseekerj )
	DRIVER( gseekeru )
	DRIVER( gunlock )
	DRIVER( hthero93 )
	DRIVER( hthero95 )
	DRIVER( hthro95u )
	DRIVER( intcup94 )
	DRIVER( kaiserkj )
	DRIVER( kaiserkn )
	DRIVER( kirameki )
	DRIVER( ktiger2 )
	DRIVER( landmakr )
	DRIVER( landmkrp )
	DRIVER( lightbr )
	DRIVER( pbobbl2j )
	DRIVER( pbobbl2u )
	DRIVER( pbobbl2x )
	DRIVER( pbobbl3j )
	DRIVER( pbobbl3u )
	DRIVER( pbobbl4j )
	DRIVER( pbobbl4u )
	DRIVER( pbobble2 )
	DRIVER( pbobble3 )
	DRIVER( pbobble4 )
	DRIVER( popnpop )
	DRIVER( popnpopj )
	DRIVER( popnpopu )
	DRIVER( prmtmfgo )
	DRIVER( prmtmfgt )
	DRIVER( puchicar )
	DRIVER( pwrgoal )
	DRIVER( qtheater )
	DRIVER( quizhuhu )
	DRIVER( rayforce )
	DRIVER( rayforcj )
	DRIVER( recalh )
	DRIVER( ridefgtj )
	DRIVER( ridefgtu )
	DRIVER( ridingf )
	DRIVER( ringrage )
	DRIVER( ringragj )
	DRIVER( ringragu )
	DRIVER( scfinals )
	DRIVER( spcinv95 )
	DRIVER( spcinvdj )
	DRIVER( spcnv95u )
	DRIVER( tcobra2 )
	DRIVER( tcobra2u )
	DRIVER( trstar )
	DRIVER( trstarj )
	DRIVER( trstaro )
	DRIVER( trstaroj )
	DRIVER( twinqix )

	/* taito_x.c */
	DRIVER( ballbros )
	DRIVER( daisenpu )
	DRIVER( gigandes )
	DRIVER( kyustrkr )
	DRIVER( superman )
	DRIVER( twinhawk )
	DRIVER( twinhwku )

	/* taito_z.c */
	DRIVER( spacegun )

	/* taitosj.c */
	DRIVER( adcanoe )
	DRIVER( alpine )
	DRIVER( alpinea )
	DRIVER( bioatack )
	DRIVER( elevatob )
	DRIVER( elevator )
	DRIVER( frontlin )
	DRIVER( hwrace )
	DRIVER( jungleh )
	DRIVER( junglek )
	DRIVER( junglhbr )
	DRIVER( junglkj2 )
	DRIVER( kikstart )
	DRIVER( piratpet )
	DRIVER( sfposeid )
	DRIVER( spacecr )
	DRIVER( spaceskr )
	DRIVER( timetunl )
	DRIVER( tinstar )
	DRIVER( waterski )
	DRIVER( wwester1 )
	DRIVER( wwestern )

	/* thunderx.c */
	DRIVER( scontra )
	DRIVER( scontraj )
	DRIVER( thnderxj )
	DRIVER( thunderx )

	/* tigeroad.c */
	DRIVER( f1dream )
	DRIVER( f1dreamb )
	DRIVER( tigeroad )
	DRIVER( toramich )

	/* timelimt.c */
	DRIVER( progress )
	DRIVER( timelimt )

	/* tmnt.c */
	DRIVER( blswhstl )
	DRIVER( cuebrckj )
	DRIVER( detatwin )
	DRIVER( glfgreat )
	DRIVER( glfgretj )
	DRIVER( lgtnfght )
	DRIVER( mia )
	DRIVER( mia2 )
	DRIVER( prmrsocj )
	DRIVER( prmrsocr )
	DRIVER( punkshot )
	DRIVER( punksht2 )
	DRIVER( punkshtj )
	DRIVER( qgakumon )
	DRIVER( ssrdrabd )
	DRIVER( ssrdrebc )
	DRIVER( ssrdrebd )
	DRIVER( ssrdrjbd )
	DRIVER( ssrdruac )
	DRIVER( ssrdrubc )
	DRIVER( ssrdruda )
	DRIVER( ssriders )
	DRIVER( sunsetbl )
	DRIVER( thndrx2 )
	DRIVER( thndrx2a )
	DRIVER( tmht )
	DRIVER( tmht2p )
	DRIVER( tmnt )
	DRIVER( tmnt2 )
	DRIVER( tmnt22p )
	DRIVER( tmnt2a )
	DRIVER( tmnt2pj )
	DRIVER( tmnt2po )
	DRIVER( tmntj )
	DRIVER( tmntu )
	DRIVER( trigon )

	/* tnzs.c */
	DRIVER( arknid2j )
	DRIVER( arknid2u )
	DRIVER( arknoid2 )
	DRIVER( chukatai )
	DRIVER( chukataj )
	DRIVER( chukatau )
	DRIVER( drtoppel )
	DRIVER( drtopplj )
	DRIVER( drtopplu )
	DRIVER( extrmatn )
	DRIVER( insectx )
	DRIVER( kabukiz )
	DRIVER( kabukizj )
	DRIVER( kageki )
	DRIVER( kagekij )
	DRIVER( plumppop )
	DRIVER( tnzs )
	DRIVER( tnzs2 )
	DRIVER( tnzsb )

	/* toaplan1.c */
	DRIVER( demonwl1 )
	DRIVER( demonwld )
	DRIVER( fireshrk )
	DRIVER( hellfir1 )
	DRIVER( hellfire )
	DRIVER( outzone )
	DRIVER( outzonea )
	DRIVER( rallybik )
	DRIVER( samesam2 )
	DRIVER( samesame )
	DRIVER( truxton )
	DRIVER( vimana )
	DRIVER( vimana1 )
	DRIVER( vimanan )
	DRIVER( zerowing )

	/* toaplan2.c */
	DRIVER( batrider )
	DRIVER( batridra )
	DRIVER( batridrk )
	DRIVER( batsugun )
	DRIVER( battleg )
	DRIVER( battlega )
	DRIVER( battlegb )
	DRIVER( batugnsp )
	DRIVER( bbakradu )
	DRIVER( bbakraid )
	DRIVER( dogyuun )
	DRIVER( dogyuunto )
	DRIVER( fixeighb )
	DRIVER( fixeight )
	DRIVER( ghox )
	DRIVER( ghoxj )
	DRIVER( grindsta )
	DRIVER( grindstm )
	DRIVER( kbash )
	DRIVER( kbash2 )
	DRIVER( kingdmgp )
	DRIVER( mahoudai )
	DRIVER( pipibibi )
	DRIVER( pipibibs )
	DRIVER( shippumd )
	DRIVER( snowbro2 )
	DRIVER( sstriker )
	DRIVER( tekipaki )
	DRIVER( truxton2 )
	DRIVER( vfive )
	DRIVER( whoopee )

	/* toypop.c */
	DRIVER( liblrabl )
	DRIVER( toypop )

	/* triplhnt.c */
	DRIVER( triplhnt )

	/* tubep.c */
	DRIVER( rjammer )
	DRIVER( tubep )

	/* tumblep.c */
	DRIVER( bcstry )
	DRIVER( bcstrya )
	DRIVER( chokchok )
	DRIVER( cookbib )
	DRIVER( fncywld )
	DRIVER( htchctch )
	DRIVER( jumpkids )
	DRIVER( magipur )
	DRIVER( sdfight )
	DRIVER( suprtrio )
	DRIVER( tumblep )
	DRIVER( tumblep2 )
	DRIVER( tumblepb )
	DRIVER( tumblepj )

	/* tunhunt.c */
	DRIVER( tunhunt )
	DRIVER( tunhuntc )

	/* turbo.c */
	DRIVER( buckrog )
	DRIVER( buckrogn )
	DRIVER( subroc3d )

	/* twin16.c */
	DRIVER( cuebrick )
	DRIVER( darkadv )
	DRIVER( devilw )
	DRIVER( fround )
	DRIVER( froundl )
	DRIVER( gradius2 )
	DRIVER( grdius2a )
	DRIVER( grdius2b )
	DRIVER( hpuncher )
	DRIVER( majuu )
	DRIVER( miaj )
	DRIVER( vulcan )

	/* ultraman.c */
	DRIVER( ultraman )

	/* vendetta.c */
	DRIVER( esckids )
	DRIVER( esckidsj )
	DRIVER( vendet2p )
	DRIVER( vendetao )
	DRIVER( vendetas )
	DRIVER( vendetta )
	DRIVER( vendettj )
	DRIVER( vendtaso )

	/* videopin.c */
	DRIVER( videopin )

	/* vigilant.c */
	DRIVER( buccanra )
	DRIVER( buccanrs )
	DRIVER( kikcubic )
	DRIVER( vigilant )
	DRIVER( vigilntj )
	DRIVER( vigilntu )

	/* volfied.c */
	DRIVER( volfied )
	DRIVER( volfiedj )
	DRIVER( volfiedu )

	/* vsnes.c */
	DRIVER( balonfgt )
	DRIVER( bnglngby )
	DRIVER( btlecity )
	DRIVER( cluclu )
	DRIVER( cstlevna )
	DRIVER( drmario )
	DRIVER( duckhunt )
	DRIVER( excitbkj )
	DRIVER( excitebk )
	DRIVER( goonies )
	DRIVER( hogalley )
	DRIVER( iceclimb )
	DRIVER( iceclmbj )
	DRIVER( iceclmrj )
	DRIVER( jajamaru )
	DRIVER( ladygolf )
	DRIVER( machridj )
	DRIVER( machridr )
	DRIVER( mightybj )
	DRIVER( platoon )
	DRIVER( rbibb )
	DRIVER( rbibba )
	DRIVER( smgolf )
	DRIVER( smgolfb )
	DRIVER( smgolfj )
	DRIVER( starlstr )
	DRIVER( suprmrio )
	DRIVER( supxevs )
	DRIVER( tkoboxng )
	DRIVER( topgun )
	DRIVER( vsbbalja )
	DRIVER( vsbbaljb )
	DRIVER( vsbball )
	DRIVER( vsbballj )
	DRIVER( vsfdf )
	DRIVER( vsgradus )
	DRIVER( vsgshoe )
	DRIVER( vsmahjng )
	DRIVER( vspinbal )
	DRIVER( vspinblj )
	DRIVER( vsskykid )
	DRIVER( vsslalom )
	DRIVER( vssoccer )
	DRIVER( vstennis )
	DRIVER( vstetris )
	DRIVER( wrecking )

	/* wallc.c */
	DRIVER( wallc )

	/* wecleman.c */
	DRIVER( hotchase )
	DRIVER( wecleman )

	/* welltris.c */
	DRIVER( quiz18k )
	DRIVER( welltris )

	/* xevious.c */
	DRIVER( battles )
	DRIVER( sxevious )
	DRIVER( xevios )
	DRIVER( xevious )
	DRIVER( xeviousa )
	DRIVER( xeviousb )

	/* xexex.c */
	DRIVER( xexex )
	DRIVER( xexexj )

	/* xmen.c */
	DRIVER( xmen )
	DRIVER( xmen2p )
	DRIVER( xmen2pj )
	DRIVER( xmen6p )
	DRIVER( xmen6pu )

	/* yamato.c */
	DRIVER( toprollr )
	DRIVER( yamato )
	DRIVER( yamato2 )

	/* zaxxon.c */
	DRIVER( futspy )
	DRIVER( ixion )
	DRIVER( razmataz )
	DRIVER( szaxxon )
	DRIVER( zaxxon )
	DRIVER( zaxxon2 )
	DRIVER( zaxxonb )


	/* Second pass: GAMEC/GAMECX/GAMEB-form entries missed by the first sweep. */

	/* artmagic.c */
	DRIVER( stoneba2 )
	DRIVER( stonebal )

	/* atarig42.c */
	DRIVER( dangerex )

	/* cinemat.c */
	DRIVER( solarq )
	DRIVER( spacewar )
	DRIVER( warrior )

	/* cps1.c */
	DRIVER( sf2 )
	DRIVER( sf2accp2 )
	DRIVER( sf2ce )
	DRIVER( sf2cej )
	DRIVER( sf2ceua )
	DRIVER( sf2ceub )
	DRIVER( sf2ceuc )
	DRIVER( sf2eb )
	DRIVER( sf2j )
	DRIVER( sf2ja )
	DRIVER( sf2jc )
	DRIVER( sf2koryu )
	DRIVER( sf2m1 )
	DRIVER( sf2m2 )
	DRIVER( sf2m3 )
	DRIVER( sf2m4 )
	DRIVER( sf2m5 )
	DRIVER( sf2m6 )
	DRIVER( sf2m7 )
	DRIVER( sf2mix )
	DRIVER( sf2rb )
	DRIVER( sf2rb2 )
	DRIVER( sf2red )
	DRIVER( sf2ua )
	DRIVER( sf2ub )
	DRIVER( sf2ud )
	DRIVER( sf2ue )
	DRIVER( sf2uf )
	DRIVER( sf2ui )
	DRIVER( sf2uk )
	DRIVER( sf2v004 )
	DRIVER( sf2yyc )

	/* cps2.c */
	DRIVER( ddsom )
	DRIVER( ddsoma )
	DRIVER( ddsomar1 )
	DRIVER( ddsomj )
	DRIVER( ddsomjr1 )
	DRIVER( ddsomjr2 )
	DRIVER( ddsomr1 )
	DRIVER( ddsomr2 )
	DRIVER( ddsomr3 )
	DRIVER( ddsomu )
	DRIVER( ddsomur1 )
	DRIVER( hsf2 )
	DRIVER( hsf2a )
	DRIVER( hsf2j )
	DRIVER( hsf2j1 )

	/* cvs.c */
	DRIVER( 8ball )
	DRIVER( 8ball1 )
	DRIVER( cosmos )
	DRIVER( cvs )
	DRIVER( darkwar )
	DRIVER( dazzler )
	DRIVER( diggerc )
	DRIVER( goldbug )
	DRIVER( heartatk )
	DRIVER( hero )
	DRIVER( hunchbak )
	DRIVER( huncholy )
	DRIVER( logger )
	DRIVER( outline )
	DRIVER( radarzn1 )
	DRIVER( radarznt )
	DRIVER( radarzon )
	DRIVER( spacefrt )
	DRIVER( superbik )
	DRIVER( wallst )

	/* ddragon.c */
	DRIVER( ddragn2u )
	DRIVER( ddragon )
	DRIVER( ddragon2 )
	DRIVER( ddragonb )
	DRIVER( ddragonu )
	DRIVER( ddragonw )

	/* ddragon3.c */
	DRIVER( ddrago3b )
	DRIVER( ddragon3 )
	DRIVER( ddragon3j )

	/* deco32.c */
	DRIVER( nslashers )

	/* foodf.c */
	DRIVER( foodf )
	DRIVER( foodf2 )
	DRIVER( foodfc )

	/* galaga.c */
	DRIVER( digduga1 )
	DRIVER( galaga )
	DRIVER( galagamf )
	DRIVER( galagamk )
	DRIVER( galagamw )
	DRIVER( galagao )
	DRIVER( gallag )
	DRIVER( gatsbee )
	DRIVER( nebulbee )
	DRIVER( xeviousc )

	/* galaxian.c */
	DRIVER( azurian )
	DRIVER( blkhole )
	DRIVER( eagle )
	DRIVER( eagle2 )
	DRIVER( exodus )
	DRIVER( fantazia )
	DRIVER( galap1 )
	DRIVER( galap4 )
	DRIVER( galapx )
	DRIVER( galaxiaj )
	DRIVER( galaxian )
	DRIVER( galmidw )
	DRIVER( galmidwo )
	DRIVER( galturbo )
	DRIVER( gteikob2 )
	DRIVER( gteikokb )
	DRIVER( gteikoku )
	DRIVER( kingbalj )
	DRIVER( kingball )
	DRIVER( moonal2 )
	DRIVER( moonal2b )
	DRIVER( moonaln )
	DRIVER( mooncrgx )
	DRIVER( mooncrs2 )
	DRIVER( mooncrsa )
	DRIVER( mooncrsb )
	DRIVER( mooncrsg )
	DRIVER( mooncrst )
	DRIVER( mooncrsu )
	DRIVER( moonqsr )
	DRIVER( orbitron )
	DRIVER( pisces )
	DRIVER( piscesb )
	DRIVER( redufo )
	DRIVER( skybase )
	DRIVER( skyraidr )
	DRIVER( smooncrs )
	DRIVER( spacbatt )
	DRIVER( streakng )
	DRIVER( superg )
	DRIVER( swarm )
	DRIVER( tst_galx )
	DRIVER( uniwars )
	DRIVER( warofbug )
	DRIVER( zerotime )

	/* gottlieb.c */
	DRIVER( myqbert )
	DRIVER( qbert )
	DRIVER( qbertjp )
	DRIVER( qbertqub )
	DRIVER( qberttst )
	DRIVER( sqbert )

	/* lwings.c */
	DRIVER( trojan )
	DRIVER( trojanj )
	DRIVER( trojanr )

	/* m72.c */
	DRIVER( dbreed )

	/* midtunit.c */
	DRIVER( mk2 )
	DRIVER( mk2chal )
	DRIVER( mk2p )
	DRIVER( mk2r14 )
	DRIVER( mk2r21 )
	DRIVER( mk2r32 )
	DRIVER( mk2r42 )
	DRIVER( mk2r91 )
	DRIVER( nbajam )
	DRIVER( nbajamr2 )
	DRIVER( nbajamt1 )
	DRIVER( nbajamt2 )
	DRIVER( nbajamt3 )
	DRIVER( nbajamte )
	DRIVER( nbajamtr )

	/* midvunit.c */
	DRIVER( crusnu21 )
	DRIVER( crusnu40 )
	DRIVER( crusnusa )
	DRIVER( offroadc )

	/* midwunit.c */
	DRIVER( nbahangt )
	DRIVER( nbamht )
	DRIVER( nbamht1 )
	DRIVER( openice )
	DRIVER( rmpgwt )
	DRIVER( rmpgwt11 )
	DRIVER( umk3 )
	DRIVER( umk3p )
	DRIVER( umk3r11 )
	DRIVER( wwfmania )

	/* midxunit.c */
	DRIVER( revx )

	/* midyunit.c */
	DRIVER( hiimpact )
	DRIVER( narc )
	DRIVER( narc3 )
	DRIVER( shimpacp )
	DRIVER( shimpact )
	DRIVER( smashtv )
	DRIVER( smashtv4 )
	DRIVER( smashtv5 )
	DRIVER( smashtv6 )
	DRIVER( term2 )
	DRIVER( term2la1 )
	DRIVER( term2la2 )
	DRIVER( totcarn )
	DRIVER( totcarnp )
	DRIVER( trog )
	DRIVER( trog3 )
	DRIVER( trogp )
	DRIVER( trogpa6 )

	/* namcos2.c */
	DRIVER( gollygho )
	DRIVER( luckywld )
	DRIVER( sgunner )
	DRIVER( sgunner2 )
	DRIVER( sgunnr2j )

	/* neogeo.c */
	DRIVER( neogeo )

	/* qix.c */
	DRIVER( qix )
	DRIVER( qix2 )
	DRIVER( qixa )
	DRIVER( qixb )
	DRIVER( zookeep )
	DRIVER( zookeep2 )
	DRIVER( zookeep3 )

	/* rungun.c */
	DRIVER( rungun )
	DRIVER( rungunu )

	/* simpl156.c */
	DRIVER( charlien )

	/* skydiver.c */
	DRIVER( skydiver )

	/* taito_f3.c */
	DRIVER( bubblem )

	/* tempest.c */
	DRIVER( tempest )
	DRIVER( tempest1 )
	DRIVER( tempest2 )
	DRIVER( tempest3 )
	DRIVER( temptube )

	/* williams.c */
	DRIVER( defcmnd )
	DRIVER( defence )
	DRIVER( defender )
	DRIVER( defendg )
	DRIVER( defendw )
	DRIVER( inferno )
	DRIVER( joust )
	DRIVER( joust2 )
	DRIVER( joustr )
	DRIVER( joustwr )
	DRIVER( robotron )
	DRIVER( robotryo )
	DRIVER( sinista1 )
	DRIVER( sinista2 )
	DRIVER( sinistar )


	/* Phase 2: games from driver families newly added to the build. */

	/* 1945kiii.c */
	DRIVER( 1945kiii )
	DRIVER( flagrall )

	/* 8080bw_drivers.c */

	/* ashnojoe.c */
	DRIVER( ashnojoe )
	DRIVER( scessjoe )

	/* backfire.c */
	DRIVER( backfire )

	/* bishi.c */
	DRIVER( bishi )
	DRIVER( sbishi )
	DRIVER( sbishik )

	/* calorie.c */
	DRIVER( calorie )
	DRIVER( calorieb )

	/* compgolf.c */
	DRIVER( compglfo )
	DRIVER( compgolf )

	/* crospang.c */
	DRIVER( crospang )

	/* darkmist.c */
	DRIVER( darkmist )

	/* dblewing.c */
	DRIVER( dblewing )
	DRIVER( dblewingb )

	/* deco156.c */
	DRIVER( hvysmsh )
	DRIVER( hvysmsha )
	DRIVER( hvysmshj )
	DRIVER( wcvol95 )
	DRIVER( wcvol95x )

	/* deco_mlc.c */

	/* djmain.c */
	DRIVER( bm1stmix )
	DRIVER( bm2ndmix )
	DRIVER( bm2ndmxa )
	DRIVER( bm4thmix )
	DRIVER( bmcompmx )
	DRIVER( bmcorerm )
	DRIVER( bmdct )
	DRIVER( hmcompm2 )
	DRIVER( hmcompmx )

	/* dorachan.c */
	DRIVER( dorachan )

	/* embargo.c */
	DRIVER( embargo )

	/* funybubl.c */
	DRIVER( funybubl )

	/* goal92.c */
	DRIVER( cupsocbl )
	DRIVER( goal92 )

	/* homerun.c */
	DRIVER( homerun )

	/* inufuku.c */
	DRIVER( inufuku )

	/* jchan.c */
	DRIVER( jchan )

	/* jcross.c */
	DRIVER( jcross )

	/* jollyjgr.c */
	DRIVER( fspiderb )
	DRIVER( jollyjgr )

	/* konamigq.c */
	DRIVER( cryptklr )

	/* konamigx.c */
	DRIVER( daiskiss )
	DRIVER( dragoona )
	DRIVER( dragoonj )
	DRIVER( fantjour )
	DRIVER( ggreats2 )
	DRIVER( gokuparo )
	DRIVER( konamigx )
	DRIVER( le2 )
	DRIVER( le2u )
	DRIVER( opengolf )
	DRIVER( puzldama )
	DRIVER( racinfrc )
	DRIVER( rungun2 )
	DRIVER( rushhero )
	DRIVER( salmndr2 )
	DRIVER( sexyparo )
	DRIVER( sexyparoa )
	DRIVER( slamdnk2 )
	DRIVER( soccerss )
	DRIVER( tbyahhoo )
	DRIVER( tkmmpzdm )
	DRIVER( tokkae )
	DRIVER( vsnetscr )
	DRIVER( winspike )

	/* ladyfrog.c */
	DRIVER( ladyfrog )

	/* lethal.c */
	DRIVER( lethalen )

	/* madalien.c */
	DRIVER( madalien )

	/* malzak.c */
	DRIVER( malzak )

	/* marinedt.c */
	DRIVER( marinedt )

	/* mirax.c */
	DRIVER( mirax )
	DRIVER( miraxa )

	/* model1.c */
	DRIVER( vf )
	DRIVER( vr )

	/* mogura.c */
	DRIVER( mogura )

	/* mwarr.c */
	DRIVER( mwarr )

	/* mystwarr.c */
	DRIVER( dadandrn )
	DRIVER( gaiapols )
	DRIVER( metamrph )
	DRIVER( metamrpj )
	DRIVER( mmaulers )
	DRIVER( mtlchamp )
	DRIVER( mtlchmpj )
	DRIVER( mystwarr )
	DRIVER( mystwaru )
	DRIVER( viostorm )
	DRIVER( viostrma )
	DRIVER( viostrmj )
	DRIVER( viostrmu )

	/* n8080.c */

	/* namcos10.c */
	DRIVER( mrdrilr2 )

	/* namcos11.c */
	DRIVER( danceyes )
	DRIVER( dunkmnia )
	DRIVER( myangel3 )
	DRIVER( primglex )
	DRIVER( souledga )
	DRIVER( souledgb )
	DRIVER( souledge )
	DRIVER( starswep )
	DRIVER( tekken )
	DRIVER( tekken2 )
	DRIVER( tekken2a )
	DRIVER( tekken2b )
	DRIVER( tekkena )
	DRIVER( tekkenb )
	DRIVER( xevi3dg )

	/* namcos12.c */
	DRIVER( aquarush )
	DRIVER( ehrgeiz )
	DRIVER( fgtlayer )
	DRIVER( golgo13 )
	DRIVER( mdhorse )
	DRIVER( mrdrillr )
	DRIVER( pacapp )
	DRIVER( soulclbr )
	DRIVER( sws99 )
	DRIVER( tekken3 )
	DRIVER( tekkentt )

	/* nss.c */
	DRIVER( nss )
	DRIVER( nss_actr )
	DRIVER( nss_adam )
	DRIVER( nss_aten )
	DRIVER( nss_con3 )
	DRIVER( nss_fzer )
	DRIVER( nss_lwep )
	DRIVER( nss_ncaa )
	DRIVER( nss_rob3 )
	DRIVER( nss_skin )
	DRIVER( nss_smw )
	DRIVER( nss_ssoc )
	DRIVER( nss_sten )

	/* outrun.c */
	DRIVER( outrun )
	DRIVER( outruna )
	DRIVER( outrunb )
	DRIVER( shangon )
	DRIVER( shangonb )
	DRIVER( toutrun )
	DRIVER( toutrun3 )

	/* pturn.c */
	DRIVER( pturn )

	/* quizpani.c */
	DRIVER( quizpani )

	/* rabbit.c */
	DRIVER( rabbit )
	DRIVER( tmmjprd )

	/* runaway.c */
	DRIVER( qwak )
	DRIVER( runaway )

	/* seattle.c */
	DRIVER( biofreak )
	DRIVER( blitz )
	DRIVER( blitz2k )
	DRIVER( blitz99 )
	DRIVER( calspeed )
	DRIVER( carnevil )
	DRIVER( mace )
	DRIVER( sfrush )
	DRIVER( wg3dh )

	/* segaxbd.c */
	DRIVER( abcop )
	DRIVER( loffire )
	DRIVER( rachero )
	DRIVER( smgp )
	DRIVER( thndrbld )
	DRIVER( thndrbld1 )

	/* speedatk.c */
	DRIVER( speedatk )

	/* sprint8.c */
	DRIVER( sprint8 )
	DRIVER( sprint8a )

	/* sspeedr.c */
	DRIVER( sspeedr )

	/* strvmstr.c */
	DRIVER( strvmstr )

	/* stv.c */
	DRIVER( astrass )
	DRIVER( bakubaku )
	DRIVER( batmanfr )
	DRIVER( colmns97 )
	DRIVER( cotton2 )
	DRIVER( cottonbm )
	DRIVER( danchih )
	DRIVER( decathlt )
	DRIVER( diehard )
	DRIVER( dnmtdeka )
	DRIVER( ejihon )
	DRIVER( elandore )
	DRIVER( ffreveng )
	DRIVER( fhboxers )
	DRIVER( findlove )
	DRIVER( finlarch )
	DRIVER( gaxeduel )
	DRIVER( grdforce )
	DRIVER( groovef )
	DRIVER( hanagumi )
	DRIVER( introdon )
	DRIVER( kiwames )
	DRIVER( maruchan )
	DRIVER( mausuke )
	DRIVER( myfairld )
	DRIVER( othellos )
	DRIVER( pblbeach )
	DRIVER( prikura )
	DRIVER( puyosun )
	DRIVER( rsgun )
	DRIVER( sandor )
	DRIVER( sassisu )
	DRIVER( seabass )
	DRIVER( sfish2 )
	DRIVER( sfish2j )
	DRIVER( shanhigw )
	DRIVER( shienryu )
	DRIVER( sleague )
	DRIVER( sokyugrt )
	DRIVER( sss )
	DRIVER( stvbios )
	DRIVER( suikoenb )
	DRIVER( thunt )
	DRIVER( twcup98 )
	DRIVER( vfkids )
	DRIVER( vfremix )
	DRIVER( vmahjong )
	DRIVER( winterht )
	DRIVER( znpwfv )

	/* supertnk.c */
	DRIVER( supertnk )

	/* suprridr.c */
	DRIVER( suprridr )

	/* system18.c */
	DRIVER( aquario )
	DRIVER( astorm )
	DRIVER( astorm2p )
	DRIVER( astormbl )
	DRIVER( bloxeed )
	DRIVER( cltchitr )
	DRIVER( ddcrew )
	DRIVER( hamaway )
	DRIVER( lghost )
	DRIVER( moonwalk )
	DRIVER( moonwlka )
	DRIVER( moonwlkb )
	DRIVER( shdancbl )
	DRIVER( shdancer )
	DRIVER( shdancrb )
	DRIVER( shdancrj )

	/* tatsumi.c */
	DRIVER( bigfight )
	DRIVER( cyclwarr )
	DRIVER( roundup5 )

	/* tceptor.c */
	DRIVER( tceptor )
	DRIVER( tceptor2 )

	/* tryout.c */
	DRIVER( tryout )

	/* vamphalf.c */
	DRIVER( hidnctch )
	DRIVER( landbrk )
	DRIVER( racoon )
	DRIVER( vamphalf )
	DRIVER( xfiles )

	/* vmetal.c */
	DRIVER( vmetal )

	/* yumefuda.c */
	DRIVER( yumefuda )

	/* zn.c */
	DRIVER( acpsx )
	DRIVER( atpsx )
	DRIVER( beastrzb )
	DRIVER( beastrzr )
	DRIVER( brvblade )
	DRIVER( cpzn1 )
	DRIVER( cpzn2 )
	DRIVER( doapp )
	DRIVER( ftimpcta )
	DRIVER( gdarius )
	DRIVER( gdarius2 )
	DRIVER( glprac2j )
	DRIVER( glprac2l )
	DRIVER( glpracr )
	DRIVER( glpracr2 )
	DRIVER( jdredd )
	DRIVER( jdreddb )
	DRIVER( jgakuen )
	DRIVER( kikaioh )
	DRIVER( mgcldate )
	DRIVER( mgcldtea )
	DRIVER( nbajamex )
	DRIVER( primrag2 )
	DRIVER( psarc95 )
	DRIVER( psyforce )
	DRIVER( raystorm )
	DRIVER( rvschool )
	DRIVER( sfchamp )
	DRIVER( sfex )
	DRIVER( sfex2 )
	DRIVER( sfex2p )
	DRIVER( sfexj )
	DRIVER( sfexp )
	DRIVER( sfexpj )
	DRIVER( sg2j )
	DRIVER( shiryu2 )
	DRIVER( sncwgltd )
	DRIVER( starglad )
	DRIVER( strider2 )
	DRIVER( stridr2a )
	DRIVER( taitofx1 )
	DRIVER( techromn )
	DRIVER( tgmj )
	DRIVER( tondemo )
	DRIVER( tps )
	DRIVER( ts2 )
	DRIVER( ts2j )

#endif	/* DRIVER_RECURSIVE */
