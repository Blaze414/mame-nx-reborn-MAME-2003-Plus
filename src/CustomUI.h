
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
using namespace std;

#include <switch.h>
#include "nx_romlist.h"
#include "gfx.hpp"
#include "nx_artwork.h"
extern "C" {
#include "nx_dips.h"
#include "nx_mameui.h"
#include "nx_state.h"
/* Declared rather than #include "artwork.h": that header names mame_file,
   UINT8 and rgb_t, none of which exist yet this early in this file. */
void artwork_enable(int enable);
}

extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
#include "nx_perf.h"
#include "audit.h"
#include "nx_ui_sound.h"
#include "nx_pause_sound.h"
#include "nx_paths.h"
/* Queue depth for the audio OSD - see nx_sound.c. */
extern int nx_audio_buffers;
/* defined in nx_maintest.cpp, below this header's only include site */
void RenderMessage(char *name);

/* VerifyRomSet() reports per-file detail through a printf-alike rather than
   returning it, so collect the first few lines into a buffer we can put on
   screen. C linkage: the typedef it is assigned to is a C one. */
extern char g_auditDetail[256];
void CLIB_DECL nx_audit_printf(const char *fmt, ...);
}

#define	GAMESELECT 1
#define	CONFIGSCREEN 2

extern std::vector<std::string> m_vecAvailRomList;
extern std::map<std::string, int> mapRoms;
extern CRomList romList;

int MenuState;

namespace UI
{
    /*=================================================================
        Palette - deep indigo canvas, one cyan accent, amber for caution.
        Focus is never carried by colour alone: the focused thing also
        grows, gains a ring, gains corner brackets and gains a caret.
    =================================================================*/
    /* Two palettes, one material.

       The glass treatment - gradient body, hairline border, specular sheen,
       soft shadow - is carried entirely by Glass(), so a theme is nothing but
       a set of colours plus how hard to push the sheen and the shadow. Dark
       needs a far gentler sheen than pearl does: white at pearl's strength
       over a slate panel stops reading as a highlight and starts reading as
       fog.

       Surfaces come in pairs, a top and a bottom, because a single flat fill
       is exactly what makes plastic look like paint. Alpha below 255 is real
       translucency - the backdrop is drawn first and shows through. */
    struct Palette
    {
        Gfx::RGBA canvasTop, canvasBot, blob;
        Gfx::RGBA panel, panelBot, panelSoft;
        Gfx::RGBA card, cardBot, cardSel, cardSelBot;
        Gfx::RGBA edge, edgeSoft;
        Gfx::RGBA accent, accentInk, accentDim, accentWash;
        Gfx::RGBA amber, magenta, green, red;
        Gfx::RGBA text, dim, ink, scrim;
        Gfx::RGBA hintTop, hintBot;
        Gfx::RGBA plateBase;        /* floor the per-game placeholder tint sits on */
        u8  gloss;                  /* specular strength */
        u8  shadow;                 /* drop-shadow strength */
        /* Which way High Contrast pushes text. Away from the surface, so
           the direction is the theme's, not a fixed colour - a version that
           always said "white" made the most legible setting in the app the
           one that erased the text on a pearl background. */
        Gfx::RGBA hcText, hcDim;
        int plateSpread;            /* how far the placeholder tint varies */
    };

    /* Smoked plastic over slate. Deliberately not black: a true black canvas
       makes every translucent panel above it look like a grey rectangle,
       because there is nothing behind them to show through. */
    static const Palette kSlate =
    {
        {  30,  34,  46, 255 }, {  18,  21,  30, 255 }, { 255, 255, 255,  16 },
        {  50,  56,  73, 236 }, {  37,  42,  56, 236 }, {  74,  82, 102, 255 },
        {  58,  65,  83, 222 }, {  43,  49,  64, 222 }, {  74,  90, 118, 246 }, {  53,  66,  92, 246 },
        { 255, 255, 255,  70 }, { 255, 255, 255,  30 },
        { 100, 180, 240, 255 }, { 136, 200, 246, 255 }, {  48,  78, 110, 255 }, {  52,  86, 124, 236 },
        { 240, 182,  86, 255 }, { 238, 110, 158, 255 }, { 104, 200, 142, 255 }, { 228, 100, 100, 255 },
        { 232, 236, 245, 255 }, { 157, 167, 185, 255 }, {  16,  20,  30, 255 }, {   8,  10,  17, 196 },
        { 255, 255, 255,  26 }, {   0,   0,   0,  60 },
        {  44,  50,  66, 255 },
        46, 96,
        { 255, 255, 255, 255 }, { 214, 222, 236, 255 },
        0x1a
    };

    /* Pearl and polished plastic. */
    static const Palette kPearl =
    {
        { 244, 247, 251, 255 }, { 219, 226, 236, 255 }, { 255, 255, 255,  90 },
        { 252, 253, 255, 232 }, { 231, 237, 245, 232 }, { 208, 218, 231, 255 },
        { 253, 254, 255, 214 }, { 226, 233, 243, 214 }, { 236, 248, 255, 246 }, { 205, 231, 248, 246 },
        { 255, 255, 255, 200 }, { 148, 165, 186, 110 },
        {  74, 158, 214, 255 }, {  51, 110, 149, 255 }, { 176, 206, 230, 255 }, { 214, 236, 250, 235 },
        { 236, 168,  62, 255 }, { 226,  96, 146, 255 }, {  86, 180, 122, 255 }, { 214,  82,  82, 255 },
        {  44,  54,  70, 255 }, { 103, 117, 136, 255 }, { 255, 255, 255, 255 }, { 226, 233, 243, 188 },
        { 255, 255, 255, 150 }, { 222, 230, 240, 176 },
        { 224, 230, 238, 255 },
        116, 46,
        {   8,  12,  20, 255 }, {  56,  66,  84, 255 },
        0x17
    };

    /* Pearl is first on purpose. Existing installs that never deliberately
       chose a theme stored zero, so this also migrates the old dashboard-dark
       default to the calmer Wii-like surface without deleting a user's other
       settings. Midnight remains available for players who want it. */
    enum { THEME_PEARL = 0, THEME_SLATE, THEME_COUNT };
    static const char *kThemeName[THEME_COUNT] = { "Pearl", "Midnight" };

    static Palette g_pal = kSlate;

    /* The rest of the file names colours the way it always has. */
    #define kCanvasTop  g_pal.canvasTop
    #define kCanvasBot  g_pal.canvasBot
    #define kBlob       g_pal.blob
    #define kPanel      g_pal.panel
    #define kPanelBot   g_pal.panelBot
    #define kPanelSoft  g_pal.panelSoft
    #define kCard       g_pal.card
    #define kCardBot    g_pal.cardBot
    #define kCardSel    g_pal.cardSel
    #define kCardSelBot g_pal.cardSelBot
    #define kEdge       g_pal.edge
    #define kEdgeSoft   g_pal.edgeSoft
    #define kAccent     g_pal.accent
    #define kAccentInk  g_pal.accentInk
    #define kAccentDim  g_pal.accentDim
    #define kAccentWash g_pal.accentWash
    #define kAmber      g_pal.amber
    #define kMagenta    g_pal.magenta
    #define kGreen      g_pal.green
    #define kRed        g_pal.red
    #define kText       g_pal.text
    #define kDim        g_pal.dim
    #define kInk        g_pal.ink
    #define kScrim      g_pal.scrim

    /* Geometry of the shadow is shared; only its strength is per-theme. */
    enum { kShadowSpread = 4, kShadowDrop = 2 };
    #define kShadowAlpha g_pal.shadow

    enum Screen
    {
        SCREEN_LIBRARY = 0,
        SCREEN_GAMEINFO,
        SCREEN_SETTINGS,
        SCREEN_CONTROLLERS,
        SCREEN_OVERLAY          /* in-game, drawn over the paused frame */
    };

    enum Zone   { ZONE_GRID = 0, ZONE_LEFTRAIL, ZONE_RIGHTRAIL };
    enum Category { CAT_ALL = 0, CAT_FAV, CAT_RECENT, CAT_COUNT };
    enum ViewMode { VIEW_GRID = 0, VIEW_COMPACT, VIEW_LIST, VIEW_COUNT };
    enum SortMode { SORT_NAME = 0, SORT_YEAR, SORT_MAKER, SORT_COUNT };

    static const char *kCategoryName[CAT_COUNT] = { "All Games", "Favourites", "Recent" };
    static const char *kViewName[VIEW_COUNT]    = { "Artwork", "Compact", "List" };
    /* "A-Z" came out as "A 7" on device - the bundled font has no glyph for
       either character. Plain words render correctly and read better anyway. */
    static const char *kSortName[SORT_COUNT]    = { "Name", "Year", "Maker" };
    static const char *kRumbleName[3]           = { "Off", "Light", "Strong" };
    /* Buffers of audio queued ahead, as the delay they actually cost. */
    static const char *kAudioDelayName[3]       = { "Low  33ms", "Medium  50ms", "Safe  66ms" };

    /*=================================================================
        Persisted state
    =================================================================*/
    struct GameStat { int plays; long long lastPlayed; bool fav; };
    static std::map<std::string, GameStat> g_stats;

    struct Settings
    {
        int  textScale;       /* percent, 100..150 */
        bool highContrast;
        bool reducedMotion;
        bool reducedEffects;
        bool persistentHints;
        bool colourBlindFocus;
        bool navSounds;
        int  rumble;          /* 0 off, 1 light, 2 strong */
        bool holdToConfirm;
        bool hideNotWorking;
        bool allowDownloads;
        bool showArtwork;
        int  view;
        int  sort;
        int  brightness;      /* percent */
        int  gamma;           /* percent */
        bool useSamples;
        int  audioDelay;      /* 0 lowest .. 2 safest; buffers = 2 + this */
        int  theme;           /* THEME_SLATE / THEME_PEARL */
        bool useArtworkLayers;
        bool cropArtwork;
        bool skipGameInfo;
        bool skipDisclaimer;
        bool cheats;
    };

    static Settings g_cfg;

    static void SettingsDefaults()
    {
        g_cfg.textScale = 100;
        g_cfg.highContrast = false;
        g_cfg.reducedMotion = false;
        g_cfg.reducedEffects = false;
        g_cfg.persistentHints = true;
        g_cfg.colourBlindFocus = false;
        g_cfg.navSounds = true;
        g_cfg.rumble = 1;
        g_cfg.holdToConfirm = true;
        g_cfg.hideNotWorking = false;
        /* Off by default: this one reaches the network. */
        g_cfg.allowDownloads = false;
        g_cfg.showArtwork = true;
        g_cfg.view = VIEW_GRID;
        g_cfg.sort = SORT_NAME;
        g_cfg.brightness = 100;
        g_cfg.gamma = 100;
        g_cfg.useSamples = true;
        /* Safest by default: a late frame with no cushion crackles, and that
           is far more noticeable than 33ms of extra delay. */
        g_cfg.audioDelay = 2;
        g_cfg.theme = THEME_PEARL;
        g_cfg.useArtworkLayers = false;
        g_cfg.cropArtwork = false;
        g_cfg.skipGameInfo = true;
        g_cfg.skipDisclaimer = true;
        g_cfg.cheats = false;
    }

    static Screen   g_screen   = SCREEN_LIBRARY;
    static Screen   g_prevScreen = SCREEN_LIBRARY;
    static Zone     g_zone     = ZONE_GRID;
    static Category g_category = CAT_ALL;
    static std::string g_search;

    static std::vector<int> g_list;
    static int g_sel = 0, g_page = 0;
    static int g_leftSel = 0, g_rightSel = 0;

    std::string currentGame;
    bool gameRunning = false;

    char RomCountText[80];

    static PadState s_uiPad;
    static bool s_psmUp = false;
    static bool s_soundUp = false;
    static bool s_dirty = true;
    static int  s_lastMinute = -1;

    static u64  s_heldDir = 0;
    static bool s_wasMoving = false;
    static int s_heldFrames = 0;

    /* Hold-to-confirm: which action is being held and for how long. */
    static int s_holdAction = -1;
    static int s_holdFrames = 0;
    static const int kHoldFrames = 45;      /* 0.75s at 60Hz */

    /* Vibration, fired and cleared without blocking the loop. */
    static HidVibrationDeviceHandle s_vib[2];      /* handheld  */
    static HidVibrationDeviceHandle s_vibPad[2];   /* pad No1   */
    static bool s_vibUp = false;
    static bool s_vibPadUp = false;
    static int  s_vibFrames = 0;

    /* Filled by EnsureDirs() from the folder this .nro was launched out of, so
       a second copy of the app in a second folder keeps its own settings,
       library and artwork instead of fighting over one set. */
    static char kDataDir[256];
    static char kStatFile[288];
    static char kCfgFile[288];
    static char kArtDir[288];

    static void Draw();

    /*=================================================================
        Touch

        Hit targets are registered by the draw code as it lays elements out,
        never recomputed in the input handler. Layout maths written twice is
        layout maths that drifts, and an invisible hitbox that has slid off its
        control is worse than no touch at all.
    =================================================================*/
    enum {
        HIT_NONE = 0,
        HIT_LEFTRAIL   = 100,   /* +0..2 */
        HIT_RIGHTRAIL  = 110,   /* +0..2 */
        HIT_CHIP_CAT   = 120, HIT_CHIP_SORT, HIT_CHIP_VIEW, HIT_CHIP_SEARCH,
        HIT_CONFIRM    = 130,   /* +0 cancel, +1 go */
        HIT_OVROW      = 200,   /* +row */
        HIT_TILE       = 400    /* +index into the visible page */
    };

    struct HitRect { int x, y, w, h, id; };

    static std::vector<HitRect> s_hits;

    static void Hit(int x, int y, int w, int h, int id)
    {
        HitRect r = { x, y, w, h, id };
        s_hits.push_back(r);
    }

    static bool s_touchDown = false;
    static int  s_touchX = 0, s_touchY = 0;

    /* Fires on release, not on contact, and only when the finger has not
       wandered - a press that turns into a drag is a scroll gesture, not a
       tap, and launching a game on finger-down makes the screen feel
       trigger-happy. Panel coordinates are already 1280x720, so touch needs no
       scaling. */
    static bool ReadTap(int &tx, int &ty)
    {
        HidTouchScreenState st;
        bool down;

        if (!hidGetTouchScreenStates(&st, 1)) st.count = 0;
        down = st.count > 0;

        if (down && !s_touchDown)
        {
            s_touchDown = true;
            s_touchX = (int)st.touches[0].x;
            s_touchY = (int)st.touches[0].y;
            return false;
        }

        if (down)
        {
            int dx = (int)st.touches[0].x - s_touchX;
            int dy = (int)st.touches[0].y - s_touchY;

            if (dx * dx + dy * dy > 40 * 40) s_touchX = s_touchY = -1000;
            return false;
        }

        if (s_touchDown)
        {
            s_touchDown = false;
            if (s_touchX < 0) return false;
            tx = s_touchX;
            ty = s_touchY;
            return true;
        }
        return false;
    }

    /* Last registered wins, so an element drawn over another takes the tap. */
    static int HitTest(int x, int y)
    {
        for (int i = (int)s_hits.size() - 1; i >= 0; i--)
        {
            const HitRect &r = s_hits[i];
            if (x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h)
                return r.id;
        }
        return HIT_NONE;
    }

    /* Confirm dialog and screen transitions are used by code defined above
       their own bodies (Draw paints the modal, ConfirmInput fades out before
       quitting), so the state lives here and the bodies stay near each other
       further down. */
    enum { CONF_NONE = 0, CONF_EXIT_APP, CONF_LEAVE_GAME, CONF_RESET };

    static int  s_confirm    = CONF_NONE;
    static int  s_confirmSel = 0;            /* 0 Cancel, 1 the action */

    static void DrawConfirm();
    static void FadeOut();
    static void FadeIn();
    static void LaunchSelected();
    static void OverlayActivate();
    static void SetCategory(int cat);
    static void RebuildList();
    static void SyncPage();
    static int  PerPage();
    static void BuildRows();

    /*=================================================================
        Text scale
    =================================================================*/
    static u32 TS(int base)
    {
        int v = base * g_cfg.textScale / 100;
        return (u32)(v < 8 ? 8 : v);
    }

    static Gfx::RGBA Ink(Gfx::RGBA c)
    {
        /* High contrast drives text away from the surface rather than toward
           a fixed colour. On the old near-black canvas that meant pushing to
           white; on pearl it means pushing to near-black, and the version that
           still said "white" would have made the most legible setting in the
           app the one that erased the text. Hue is left alone so the accent
           keeps meaning what it means. */
        if (!g_cfg.highContrast) return c;
        if (c.R == kDim.R && c.G == kDim.G && c.B == kDim.B) return g_pal.hcDim;
        if (c.R == kText.R && c.G == kText.G && c.B == kText.B) return g_pal.hcText;
        return c;
    }

    /*=================================================================
        Motion

        Every animated quantity here is a value chasing a target, never a
        timeline from A to B. A timeline has to be restarted when the target
        changes, and restarting one mid-flight is exactly what makes a menu
        feel like it is arguing with someone flicking through it - the press
        lands, the old animation insists on finishing, and the cursor arrives
        late. A chase just re-aims: the frame after a new press already moves
        the right way, however many presses came before it.

        Rates are per-frame smoothing factors at 60Hz. A chase covering
        distance D to within epsilon takes ln(D/epsilon) / -ln(1-k) frames, so
        the rate that gives a wanted duration depends on how far the thing is
        going and how close counts as arrived - the times in the comments below
        were computed from that, not guessed, and each is inside the window the
        feel it is named for calls for.
    =================================================================*/
    static const float kFocusRate  = 0.48f;   /* 133ms a row, 150ms a tile */
    static const float kScrollRate = 0.42f;   /* 150ms one row of scroll   */
    static const float kPageRate   = 0.33f;   /* 233ms page change         */
    static const float kDialogRate = 0.40f;   /* 183ms dialog open         */
    static const float kNudgeRate  = 0.32f;   /* 150ms value recoil decay  */

    /* Growth of a focused thing over its resting size, as a fraction of the
       thing. 6.5% on a 148px tile is nine pixels of extra width - enough to
       read as "this one came forward" while still fitting in the gap the grid
       already leaves, so neighbours never move. */
    /* Old 6.5% growth made large cards swell into their gutters. Wii-style
       focus should feel buoyant, not zoomed: ring travel carries most of the
       state change and scale only confirms arrival. */
    static const float kFocusGrow = 0.045f;

    /* The focused tile's scale does not follow the travelling ring linearly.

       Everything about a tile - its size, its brightness, its sheen, its
       shadow - is driven by how near the ring is to it, and near the ring that
       relationship is steep while far from it the tail is very long. Linear
       falloff makes a tile arrive fast and *leave* fast: the outgoing tile has
       given up 95% of its lift within four frames, which is a snap, not a
       return. Taking the root of it stretches the tail and steepens the
       approach at once - the arriving tile is quicker off the mark and the
       leaving one lets go over about 130ms.

       sqrt rather than an arbitrary exponent because the search for a curve
       that satisfied all four of the timing constraints at once landed on 0.5
       exactly, and sqrtf costs a fraction of powf. */
    static float FocusCurve(float f)
    {
        if (f <= 0.0f) return 0.0f;
        if (f >= 1.0f) return 1.0f;
        return sqrtf(f);
    }

    /* Set by any chase that has not arrived yet. Draw() turns it back into a
       repaint request for the next frame, which is what keeps an idle screen
       costing nothing on a CPU rasteriser - the UI animates only while
       something is actually moving. */
    static bool s_animBusy   = false;
    static bool s_animMoved  = false;   /* was busy on the previous frame */

    static float Chase(float v, float target, float rate, float epsilon)
    {
        if (g_cfg.reducedMotion) return target;
        v += (target - v) * rate;
        if (fabsf(target - v) <= epsilon) return target;
        s_animBusy = true;
        return v;
    }

    /*-----------------------------------------------------------------
        Focus scale

        A spring rather than a chase, because this is the one place a small
        overshoot earns its keep: it is the difference between a thing
        arriving somewhere and a thing being snapped into place.

        A kick is an impulse, not a reset. Setting the position instead - which
        this used to do - meant that on the frame focus moved, every tile on
        screen was instantly at rest size: the tile being left behind dropped
        from full lift to almost none in one frame and then grew *back* as the
        spring rose, a visible bounce on the way out. An impulse leaves every
        tile exactly where it was and only changes where it is heading, so the
        outgoing tile can do nothing but shrink.

        Stiffness and damping were searched, not picked, against four
        constraints at once: the arriving tile peaks 1.6% beyond its focused
        size and settles in 217ms, the leaving one is back to rest in 134ms and
        never once grows on the way, and after the single overshoot the spring
        does not dip below its target at all - so a tile that keeps focus sits
        still instead of pulsing. Damping is 0.8 of critical, which is what
        buys exactly one overshoot and no ringing.

        The rest threshold is half a pixel of lift on a 148px tile. Tighter
        only buys repaints nobody can see.
    -----------------------------------------------------------------*/
    static const float kSpringStiff = 0.115f;
    static const float kSpringDamp  = 0.4574f;   /* 1 - 2*0.8*sqrt(stiff) */
    static const float kSpringRest  = 0.104f;

    static float s_grow = 1.0f, s_growVel = 0.0f;

    /* Positive springs the thing up past its focused size and back (focus
       arriving); negative presses it in below and back (a button press).
       Either way the position is untouched and only the velocity changes. */
    static void GrowKick(float impulse)
    {
        if (g_cfg.reducedMotion) return;
        s_growVel += impulse;
    }

    static void GrowStep()
    {
        if (g_cfg.reducedMotion) { s_grow = 1.0f; s_growVel = 0.0f; return; }

        s_growVel += (1.0f - s_grow) * kSpringStiff;
        s_growVel *= kSpringDamp;
        s_grow    += s_growVel;

        if (fabsf(1.0f - s_grow) < kSpringRest && fabsf(s_growVel) < kSpringRest)
        { s_grow = 1.0f; s_growVel = 0.0f; }
        else s_animBusy = true;
    }

    /*-----------------------------------------------------------------
        Travelling focus rect

        One rect for the whole UI: only ever one thing has focus, and giving
        each list its own would mean a stale rect waiting to slide in from
        wherever that list was last left.
    -----------------------------------------------------------------*/
    static float s_fx = 0, s_fy = 0, s_fw = 0, s_fh = 0;
    static int   s_fkey = -1;
    static bool  s_fLive = false;

    struct FRect { int x, y, w, h; };

    /* `key` identifies the list the rect belongs to. A different key snaps
       instead of travelling: sliding the ring across a screen change would
       draw a line between two things that have no spatial relationship to
       each other, which reads as a glitch rather than as movement. */
    static FRect FocusTrack(int key, int x, int y, int w, int h)
    {
        FRect r;

        if (key != s_fkey || !s_fLive || g_cfg.reducedMotion)
        {
            s_fkey = key; s_fLive = true;
            s_fx = (float)x; s_fy = (float)y;
            s_fw = (float)w; s_fh = (float)h;
        }
        else
        {
            s_fx = Chase(s_fx, (float)x, kFocusRate, 0.5f);
            s_fy = Chase(s_fy, (float)y, kFocusRate, 0.5f);
            s_fw = Chase(s_fw, (float)w, kFocusRate, 0.5f);
            s_fh = Chase(s_fh, (float)h, kFocusRate, 0.5f);
        }

        GrowStep();

        r.x = (int)(s_fx + 0.5f); r.y = (int)(s_fy + 0.5f);
        r.w = (int)(s_fw + 0.5f); r.h = (int)(s_fh + 0.5f);
        return r;
    }

    /* Lift for row-shaped things.

       A wide row cannot scale by a percentage: five percent of a 674px row is
       34px sideways against 2px down, which reads as the row getting longer
       rather than as it coming forward - and it leaves the plate a long way
       inside the ring that is supposed to be sitting on it. Rows take a flat
       few pixels on every edge instead, so the movement is the same the whole
       way round and plate and ring stay the same rect.

       Two pixels, not more, because that is what every row pitch in the UI has
       to spare: the overlay leaves 10px between rows, the settings card is
       pitch-4 and the library's list rows have a 4px gap. At two the lifted
       row exactly fills its pitch in the tightest of the three, so the row
       below never paints over the lift it is supposed to sit beside. */
    static const int kRowLift = 2;

    static FRect Lifted(FRect r, float f)
    {
        int d = (int)(kRowLift * f * s_grow + 0.5f);
        FRect o;

        o.x = r.x - d; o.y = r.y - d;
        o.w = r.w + d * 2; o.h = r.h + d * 2;
        return o;
    }

    static FRect LiftedRect(int x, int y, int w, int h, float f)
    {
        FRect r;

        r.x = x; r.y = y; r.w = w; r.h = h;
        return Lifted(r, f);
    }

    /* Scales a rect about its own centre by the current focus spring. Squares
       - tiles, rail buttons - can take a percentage because both axes are the
       same length. */
    static FRect GrownBy(FRect r, float amount)
    {
        int gx = (int)(r.w * amount * s_grow * 0.5f + 0.5f);
        int gy = (int)(r.h * amount * s_grow * 0.5f + 0.5f);
        FRect o;

        o.x = r.x - gx; o.y = r.y - gy;
        o.w = r.w + gx * 2; o.h = r.h + gy * 2;
        return o;
    }

    /* How lit a row or tile at (x,y) should be: 1 where the travelling ring
       has arrived, falling off over one step of travel. Highlights read their
       own state from this, so a filled card can never be lit somewhere the
       ring is not - the two are the same fact drawn twice. */
    static float FocusWeight(int x, int y, int spanX, int spanY)
    {
        float dx, dy;

        if (!s_fLive) return 0.0f;
        dx = fabsf(s_fx - (float)x) / (float)(spanX > 0 ? spanX : 1);
        dy = fabsf(s_fy - (float)y) / (float)(spanY > 0 ? spanY : 1);
        if (dx > 1.0f || dy > 1.0f) return 0.0f;
        dx = 1.0f - dx; dy = 1.0f - dy;
        return dx < dy ? dx : dy;
    }

    static Gfx::RGBA Mix(Gfx::RGBA a, Gfx::RGBA b, float t)
    {
        Gfx::RGBA o;

        if (t <= 0.0f) return a;
        if (t >= 1.0f) return b;
        o.R = (u8)((int)a.R + (int)((b.R - a.R) * t));
        o.G = (u8)((int)a.G + (int)((b.G - a.G) * t));
        o.B = (u8)((int)a.B + (int)((b.B - a.B) * t));
        o.A = (u8)((int)a.A + (int)((b.A - a.A) * t));
        return o;
    }

    /* Toward opaque white, whatever the theme. The placeholder plates used to
       reach for kInk to brighten their top edge, which is white on pearl and
       near-black on slate - so the one gradient that is meant to say "light
       from above" pointed the wrong way in the dark theme. */
    static Gfx::RGBA Lighter(Gfx::RGBA c, float t)
    {
        Gfx::RGBA w = { 255, 255, 255, c.A };
        return Mix(c, w, t);
    }

    static Gfx::RGBA Darker(Gfx::RGBA c, float t)
    {
        Gfx::RGBA k = { 0, 0, 0, c.A };
        return Mix(c, k, t);
    }

    static Gfx::RGBA Faded(Gfx::RGBA c, float t)
    {
        if (t >= 1.0f) return c;
        c.A = (u8)(c.A * (t <= 0.0f ? 0.0f : t));
        return c;
    }

    /*-----------------------------------------------------------------
        Smooth list scroll

        Windowed lists step their top row by whole rows, so the rows jump a
        pitch at a time at the ends of the window while the ring glides. The
        window keeps its integer top - every index calculation depends on it -
        and only the drawn offset is smoothed, with the list clipped to its
        column so a partly-scrolled row is cut off rather than spilling out of
        the panel.
    -----------------------------------------------------------------*/
    static float s_scroll = 0.0f;
    static int   s_scrollKey = -1;

    static int ScrollTrack(int key, int target)
    {
        if (key != s_scrollKey || g_cfg.reducedMotion)
        { s_scrollKey = key; s_scroll = (float)target; }
        else
            s_scroll = Chase(s_scroll, (float)target, kScrollRate, 0.5f);

        return (int)(s_scroll + 0.5f);
    }

    /*-----------------------------------------------------------------
        Page transitions
    -----------------------------------------------------------------*/
    static float s_pageT   = 1.0f;
    static int   s_pageDir = 1;
    static const int kPageSlide = 72;

    /* dir +1 for going deeper, -1 for coming back, so backing out of a page
       reverses the way it came in and the two directions stay meaningful. */
    static void PageEnter(int dir)
    {
        if (g_cfg.reducedMotion) { s_pageT = 1.0f; return; }
        s_pageT = 0.0f;
        s_pageDir = dir;
        s_dirty = true;
    }

    /* Wraps a panel's contents, never the backdrop behind it - a backdrop
       that slides with the panel is the panel not moving. */
    static Gfx::DrawState PageBegin()
    {
        Gfx::DrawState saved = Gfx::pushState();

        if (s_pageT < 1.0f)
        {
            s_pageT = Chase(s_pageT, 1.0f, kPageRate, 0.004f);
            Gfx::translate((int)((1.0f - s_pageT) * kPageSlide * s_pageDir), 0);
            /* Never all the way to nothing: a panel that starts from zero
               reads as appearing, and the point of the slide is that it was
               already on its way in. */
            Gfx::fadeBy((u8)(50 + 205 * s_pageT));
        }
        return saved;
    }

    /*-----------------------------------------------------------------
        Value recoil

        A setting that changes in place has nothing to animate but itself, so
        it takes a shove in the direction it was pushed and slides back. Pure
        decay - there is no target but zero.
    -----------------------------------------------------------------*/
    static float s_nudge = 0.0f;

    static void NudgeKick(int dir)
    {
        if (!g_cfg.reducedMotion) s_nudge = (float)(-dir * 12);
    }

    static int NudgeStep()
    {
        s_nudge = Chase(s_nudge, 0.0f, kNudgeRate, 0.4f);
        return (int)s_nudge;
    }

    /* Dialogs scale and fade in from the centre. */
    static float s_dlgT = 1.0f;

    /*=================================================================
        Feedback - sound and haptics
    =================================================================*/
    static void Beep(int kind)
    {
        if (!g_cfg.navSounds) return;

        /* Two synthesisers, one per owner of the audio stream. In the library
           nothing else is playing, so nx_ui_sound owns audout outright. In a
           game MAME owns it, and nx_pause_sound mixes the same tones into
           MAME's PCM instead - never a second session, so there is no
           buffer-ownership question between them at all. */
        if (gameRunning) nx_pause_sound_play(kind);
        else if (s_soundUp) nx_ui_sound_play(kind);
    }

    /* Amplitude is a fraction of the configured strength, and a pulse is a
       length in frames, so one intent ("this was a confirm") maps to one call
       instead of scattered magic numbers. Light is the default: a full-strength
       buzz on every cursor move is the fastest way to make someone turn
       vibration off entirely. */
    static void Rumble(float amp, int frames)
    {
        HidVibrationValue v[2];
        static const float scale[3] = { 0.0f, 0.45f, 1.0f };

        if (g_cfg.rumble <= 0 || (!s_vibUp && !s_vibPadUp)) return;

        amp *= scale[g_cfg.rumble > 2 ? 2 : g_cfg.rumble];
        if (amp <= 0.0f) return;

        memset(v, 0, sizeof(v));
        v[0].amp_low   = amp;  v[0].freq_low  = 160.0f;
        v[0].amp_high  = amp;  v[0].freq_high = 320.0f;
        v[1] = v[0];

        /* Both sets. Only one of them is the controller actually in the
           player's hands, and which one that is changes with the dock. The
           other is not connected, so its packet goes nowhere. */
        if (s_vibUp)    hidSendVibrationValues(s_vib, v, 2);
        if (s_vibPadUp) hidSendVibrationValues(s_vibPad, v, 2);
        s_vibFrames = frames;
    }

    /* The motors are level-triggered: they keep running until something sends
       amplitude zero. Nothing in the system does that for us on process exit,
       so a pulse still in flight when we quit runs until the next thing to
       touch the pad tells it otherwise. */
    /* Registered with atexit() in Init(). Exit() stops the motors itself, but
       it is not the only way out: exit(1) when the renderer fails to come up,
       a libnx abort, or any future path that calls exit() without going through
       here. Vibration is level-triggered - the motors run until something sends
       an amplitude-zero packet - so a pulse still in flight when the process
       dies keeps running on the pad after the app is gone, and the only thing
       that clears it is the next program to touch that pad. One registration
       covers every route. */
    static void RumbleAtExit(void);

    static void RumbleStop()
    {
        HidVibrationValue v[2];

        if (!s_vibUp && !s_vibPadUp) return;

        memset(v, 0, sizeof(v));
        v[0].freq_low = 160.0f; v[0].freq_high = 320.0f;
        v[1] = v[0];

        /* Unconditionally to both, whatever we think is connected: a stop
           packet to a device that is not listening costs nothing, and the whole
           point of this function is that it must not be able to miss. */
        if (s_vibUp)    hidSendVibrationValues(s_vib, v, 2);
        if (s_vibPadUp) hidSendVibrationValues(s_vibPad, v, 2);
        s_vibFrames = 0;
    }

    static void RumbleTick()
    {
        if (!s_vibUp || s_vibFrames <= 0) return;
        if (--s_vibFrames == 0) RumbleStop();
    }

    static void RumbleAtExit(void) { RumbleStop(); }

    /* Moving the cursor is the most frequent event by far, so it gets the
       faintest possible tick and only at Strong. Everything that changes state
       is felt; everything that merely moves is not. */
    /* Sound, haptics and motion are the same event told three ways, so they
       are kicked from the same place - a cue that fires without its animation
       is a cue the player notices as missing. s_dirty because a spring needs
       frames to run on, and most of these fire from paths that would
       otherwise leave the screen idle. */
    static void Navigated() { Beep(NX_UI_SOUND_NAVIGATE); if (g_cfg.rumble > 1) Rumble(0.16f, 2);
                              GrowKick( 0.38f); s_dirty = true; }
    static void Edged()     { Beep(NX_UI_SOUND_NAVIGATE); Rumble(0.30f, 3);
                              GrowKick( 0.30f); s_dirty = true; }
    /* Negative impulses press the thing in below its focused size before it
       springs back. -0.30 is a 1.3% dip over 184ms, and it returns to exactly
       its focused size without going past it - a press you feel rather than a
       rebound you watch. */
    static void Confirmed() { Beep(NX_UI_SOUND_CONFIRM);  Rumble(0.55f, 5);
                              GrowKick(-0.30f); s_dirty = true; }
    static void Cancelled() { Beep(NX_UI_SOUND_BACK);     Rumble(0.30f, 3);
                              GrowKick(-0.24f); s_dirty = true; }
    static void Refused()   { Beep(NX_UI_SOUND_ERROR);    Rumble(0.85f, 9);
                              GrowKick(-0.28f); s_dirty = true; }

    /*=================================================================
        Files
    =================================================================*/
    static void EnsureDirs()
    {
        nx_path(kDataDir,  sizeof(kDataDir),  0);
        nx_path(kStatFile, sizeof(kStatFile), "library.txt");
        nx_path(kCfgFile,  sizeof(kCfgFile),  "settings.txt");

        mkdir(kDataDir, 0777);
        nx_path_mkdir(kArtDir, sizeof(kArtDir), "artwork");
    }

    static void StatsLoad()
    {
        FILE *f = fopen(kStatFile, "r");
        char line[512];

        if (!f) return;

        while (fgets(line, sizeof(line), f))
        {
            char *p1 = strchr(line, '\t');
            char *p2 = p1 ? strchr(p1 + 1, '\t') : NULL;
            char *p3 = p2 ? strchr(p2 + 1, '\t') : NULL;
            GameStat st;
            size_t len;

            if (!p3) continue;
            *p1 = *p2 = *p3 = '\0';

            st.plays      = atoi(line);
            st.lastPlayed = atoll(p1 + 1);
            st.fav        = atoi(p2 + 1) != 0;

            len = strlen(p3 + 1);
            while (len && (p3[len] == '\n' || p3[len] == '\r')) p3[len--] = '\0';

            g_stats[std::string(p3 + 1)] = st;
        }
        fclose(f);
    }

    static void StatsSave()
    {
        FILE *f;

        EnsureDirs();
        f = fopen(kStatFile, "w");
        if (!f) return;

        for (std::map<std::string, GameStat>::iterator it = g_stats.begin(); it != g_stats.end(); ++it)
        {
            /* Only rows carrying information - one line per driver would be
               thousands of lines of zeroes. */
            if (!it->second.fav && it->second.plays == 0) continue;
            fprintf(f, "%d\t%lld\t%d\t%s\n", it->second.plays, it->second.lastPlayed,
                    it->second.fav ? 1 : 0, it->first.c_str());
        }
        fclose(f);
    }

    static GameStat &StatFor(const std::string &name)
    {
        std::map<std::string, GameStat>::iterator it = g_stats.find(name);

        if (it == g_stats.end())
        {
            GameStat st; st.plays = 0; st.lastPlayed = 0; st.fav = false;
            g_stats[name] = st;
            return g_stats[name];
        }
        return it->second;
    }

    static void ApplySettings()
    {
        options.brightness   = (float)g_cfg.brightness / 100.0f;
        options.gamma        = (float)g_cfg.gamma / 100.0f;
        options.use_samples  = g_cfg.useSamples;

        /* Read by osd_start_audio_stream(), so it lands on the next game
           launch rather than mid-stream - resizing a queue audout is
           already playing from is not worth the failure modes. */
        if (g_cfg.audioDelay < 0) g_cfg.audioDelay = 0;
        if (g_cfg.audioDelay > 2) g_cfg.audioDelay = 2;
        nx_audio_buffers = 2 + g_cfg.audioDelay;

        /* Swaps every colour at once. Nothing caches a colour, and the
           backdrop snapshot is dropped below, so the change is whole the
           moment the next frame is drawn. */
        if (g_cfg.theme < 0 || g_cfg.theme >= THEME_COUNT) g_cfg.theme = THEME_PEARL;
        g_pal = (g_cfg.theme == THEME_PEARL) ? kPearl : kSlate;
        /* The backdrop is cached as pixels, so it survives a palette change
           unless it is explicitly thrown away. */
        Gfx::dropSnapshot();
        options.skip_gameinfo   = g_cfg.skipGameInfo ? 1 : 0;
        options.skip_disclaimer = g_cfg.skipDisclaimer;
        options.skip_warnings   = g_cfg.skipDisclaimer;
        options.use_artwork  = g_cfg.useArtworkLayers
            ? (ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS)
            : ARTWORK_USE_NONE;
        options.artwork_crop = g_cfg.cropArtwork ? 1 : 0;
    }

    static void CfgSave()
    {
        FILE *f;

        EnsureDirs();
        f = fopen(kCfgFile, "w");
        if (!f) return;

        fprintf(f, "textScale %d\n", g_cfg.textScale);
        fprintf(f, "highContrast %d\n", g_cfg.highContrast);
        fprintf(f, "reducedMotion %d\n", g_cfg.reducedMotion);
        fprintf(f, "reducedEffects %d\n", g_cfg.reducedEffects);
        fprintf(f, "persistentHints %d\n", g_cfg.persistentHints);
        fprintf(f, "colourBlindFocus %d\n", g_cfg.colourBlindFocus);
        fprintf(f, "navSounds %d\n", g_cfg.navSounds);
        fprintf(f, "rumble %d\n", g_cfg.rumble);
        fprintf(f, "holdToConfirm %d\n", g_cfg.holdToConfirm);
        fprintf(f, "hideNotWorking %d\n", g_cfg.hideNotWorking);
        fprintf(f, "allowDownloads %d\n", g_cfg.allowDownloads);
        fprintf(f, "showArtwork %d\n", g_cfg.showArtwork);
        fprintf(f, "view %d\n", g_cfg.view);
        fprintf(f, "sort %d\n", g_cfg.sort);
        fprintf(f, "brightness %d\n", g_cfg.brightness);
        fprintf(f, "gamma %d\n", g_cfg.gamma);
        fprintf(f, "useSamples %d\n", g_cfg.useSamples);
        fprintf(f, "audioDelay %d\n", g_cfg.audioDelay);
        fprintf(f, "theme %d\n", g_cfg.theme);
        fprintf(f, "useArtworkLayers %d\n", g_cfg.useArtworkLayers);
        fprintf(f, "cropArtwork %d\n", g_cfg.cropArtwork);
        fprintf(f, "skipGameInfo %d\n", g_cfg.skipGameInfo);
        fprintf(f, "skipDisclaimer %d\n", g_cfg.skipDisclaimer);
        fprintf(f, "cheats %d\n", g_cfg.cheats);
        fclose(f);
    }

    static void CfgLoad()
    {
        FILE *f = fopen(kCfgFile, "r");
        char key[64];
        int val;

        SettingsDefaults();
        if (!f) { ApplySettings(); return; }

        while (fscanf(f, "%63s %d", key, &val) == 2)
        {
            if      (!strcmp(key, "textScale"))        g_cfg.textScale = val;
            else if (!strcmp(key, "highContrast"))     g_cfg.highContrast = val != 0;
            else if (!strcmp(key, "reducedMotion"))    g_cfg.reducedMotion = val != 0;
            else if (!strcmp(key, "reducedEffects"))   g_cfg.reducedEffects = val != 0;
            else if (!strcmp(key, "persistentHints"))  g_cfg.persistentHints = val != 0;
            else if (!strcmp(key, "colourBlindFocus")) g_cfg.colourBlindFocus = val != 0;
            else if (!strcmp(key, "navSounds"))        g_cfg.navSounds = val != 0;
            else if (!strcmp(key, "rumble"))           g_cfg.rumble = val;
            else if (!strcmp(key, "holdToConfirm"))    g_cfg.holdToConfirm = val != 0;
            else if (!strcmp(key, "hideNotWorking"))   g_cfg.hideNotWorking = val != 0;
            else if (!strcmp(key, "allowDownloads"))   g_cfg.allowDownloads = val != 0;
            else if (!strcmp(key, "showArtwork"))      g_cfg.showArtwork = val != 0;
            else if (!strcmp(key, "view"))             g_cfg.view = val;
            else if (!strcmp(key, "sort"))             g_cfg.sort = val;
            else if (!strcmp(key, "brightness"))       g_cfg.brightness = val;
            else if (!strcmp(key, "gamma"))            g_cfg.gamma = val;
            else if (!strcmp(key, "useSamples"))       g_cfg.useSamples = val != 0;
            else if (!strcmp(key, "audioDelay"))       g_cfg.audioDelay = val;
            else if (!strcmp(key, "theme"))           g_cfg.theme = val;
            else if (!strcmp(key, "useArtworkLayers")) g_cfg.useArtworkLayers = val != 0;
            else if (!strcmp(key, "cropArtwork"))      g_cfg.cropArtwork = val != 0;
            else if (!strcmp(key, "skipGameInfo"))     g_cfg.skipGameInfo = val != 0;
            else if (!strcmp(key, "skipDisclaimer"))   g_cfg.skipDisclaimer = val != 0;
            else if (!strcmp(key, "cheats"))           g_cfg.cheats = val != 0;
        }
        fclose(f);
        ApplySettings();
    }

    /*=================================================================
        Driver helpers
    =================================================================*/
    static const struct GameDriver *DriverFor(const std::string &name)
    {
        std::map<std::string, int>::iterator it = mapRoms.find(name);
        return (it == mapRoms.end()) ? NULL : drivers[it->second];
    }

    static int DriverIndexFor(const std::string &name)
    {
        std::map<std::string, int>::iterator it = mapRoms.find(name);
        return (it == mapRoms.end()) ? -1 : it->second;
    }

    /* Worst declared emulation-quality flag. Only the worst is shown - a tile
       with three chips reads as noise, and the worst is the one that decides
       whether the game is worth starting. */
    static const char *WarnChip(const struct GameDriver *drv)
    {
        UINT32 f;
        if (!drv) return NULL;
        f = drv->flags;
        if (f & GAME_NOT_WORKING)           return "BROKEN";
        if (f & GAME_UNEMULATED_PROTECTION) return "PROT";
        if (f & GAME_IMPERFECT_GRAPHICS)    return "GFX";
        if (f & GAME_NO_SOUND)              return "NO SND";
        if (f & GAME_IMPERFECT_SOUND)       return "SND";
        if (f & GAME_IMPERFECT_COLORS)      return "COL";
        return NULL;
    }

    static int PlayerCount(const struct GameDriver *drv)
    {
        const struct InputPortTiny *in;
        int players = 0;

        if (!drv || !drv->input_ports) return 0;
        for (in = drv->input_ports; in->type != IPT_END; in++)
        {
            int t = in->type & ~IPF_MASK;
            if (t == IPT_START1 && players < 1) players = 1;
            if (t == IPT_START2 && players < 2) players = 2;
            if (t == IPT_START3 && players < 3) players = 3;
            if (t == IPT_START4 && players < 4) players = 4;
        }
        return players;
    }

    /*=================================================================
        Artwork cache - <artwork dir>/<romset>.png|jpg, loaded a couple per
        repaint so a page turn never stalls the loop waiting on the SD card.
    =================================================================*/
    struct ArtSlot { int driver; Gfx::Image img; bool tried; };
    static const int kArtSlots = 24;
    static ArtSlot g_art[kArtSlots];
    static int g_artClock = 0;
    static int g_artBudget = 0;

    static ArtSlot *ArtFind(int driver)
    {
        for (int i = 0; i < kArtSlots; i++)
            if (g_art[i].driver == driver) return &g_art[i];
        return NULL;
    }

    static ArtSlot *ArtClaim(int driver)
    {
        int victim = 0;

        for (int i = 0; i < kArtSlots; i++)
            if (g_art[i].driver < 0) { victim = i; goto take; }

        victim = g_artClock;
        g_artClock = (g_artClock + 1) % kArtSlots;

    take:
        Gfx::freeImage(g_art[victim].img);
        g_art[victim].driver = driver;
        g_art[victim].tried = false;
        return &g_art[victim];
    }

    /* Returns the cached image, loading it if there is budget left this
       repaint. NULL means "nothing to draw yet" - the caller falls back to the
       placeholder plate and the next repaint tries again. */
    static Gfx::Image *ArtFor(const struct GameDriver *drv)
    {
        int index;
        ArtSlot *slot;

        if (!g_cfg.showArtwork || !drv || !drv->name) return NULL;

        index = DriverIndexFor(drv->description);
        if (index < 0) return NULL;

        slot = ArtFind(index);
        if (slot) return slot->img.Pixels ? &slot->img : NULL;

        if (g_artBudget <= 0) return NULL;
        g_artBudget--;

        slot = ArtClaim(index);
        slot->tried = true;

        {
            std::string base = std::string(kArtDir) + "/" + drv->name;

            if (!Gfx::loadImage(base + ".png", slot->img))
                if (!Gfx::loadImage(base + ".jpg", slot->img))
                    Gfx::loadImage(base + ".jpeg", slot->img);
        }

        return slot->img.Pixels ? &slot->img : NULL;
    }

    /*=================================================================
        List building
    =================================================================*/
    static bool ContainsNoCase(const std::string &hay, const std::string &needle)
    {
        std::string a = hay, b = needle;
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);
        return a.find(b) != std::string::npos;
    }

    static bool SortLess(int a, int b)
    {
        const std::string &na = m_vecAvailRomList[a];
        const std::string &nb = m_vecAvailRomList[b];

        if (g_category == CAT_RECENT)
        {
            long long ta = StatFor(na).lastPlayed, tb = StatFor(nb).lastPlayed;
            if (ta != tb) return ta > tb;
            return na < nb;
        }
        if (g_cfg.sort == SORT_YEAR)
        {
            const struct GameDriver *da = DriverFor(na), *db = DriverFor(nb);
            int c = strcmp((da && da->year) ? da->year : "", (db && db->year) ? db->year : "");
            if (c) return c < 0;
        }
        else if (g_cfg.sort == SORT_MAKER)
        {
            const struct GameDriver *da = DriverFor(na), *db = DriverFor(nb);
            int c = strcmp((da && da->manufacturer) ? da->manufacturer : "",
                           (db && db->manufacturer) ? db->manufacturer : "");
            if (c) return c < 0;
        }
        return na < nb;
    }

    static void RebuildList()
    {
        std::string keep = (g_sel >= 0 && g_sel < (int)g_list.size())
                         ? m_vecAvailRomList[g_list[g_sel]] : std::string();

        g_list.clear();

        for (size_t i = 0; i < m_vecAvailRomList.size(); i++)
        {
            const std::string &name = m_vecAvailRomList[i];
            const struct GameDriver *drv = DriverFor(name);

            if (g_cfg.hideNotWorking && drv && (drv->flags & GAME_NOT_WORKING)) continue;
            if (g_category == CAT_FAV && !StatFor(name).fav) continue;
            if (g_category == CAT_RECENT && StatFor(name).lastPlayed == 0) continue;
            if (!g_search.empty() && !ContainsNoCase(name, g_search)) continue;

            g_list.push_back((int)i);
        }

        std::sort(g_list.begin(), g_list.end(), SortLess);

        /* Keep the cursor on the same game when it survives the filter -
           jumping to the top on every change is the most irritating thing a
           library can do. */
        g_sel = 0;
        if (!keep.empty())
            for (size_t i = 0; i < g_list.size(); i++)
                if (m_vecAvailRomList[g_list[i]] == keep) { g_sel = (int)i; break; }

        if (!g_list.empty()) currentGame = m_vecAvailRomList[g_list[g_sel]];
    }

    /*=================================================================
        Grid geometry - matches the 5x3 shelf in the reference shots
    =================================================================*/
    struct GridSpec { int cols, rows, tile, gapX, gapY, x0, y0; };

    static GridSpec Grid()
    {
        GridSpec g;

        if (g_cfg.view == VIEW_COMPACT)   { g.cols = 8; g.rows = 4; g.tile = 96;  g.gapX = 18; g.gapY = 16; }
        else if (g_cfg.view == VIEW_LIST) { g.cols = 1; g.rows = 11; g.tile = 42; g.gapX = 0;  g.gapY = 4;  }
        else                              { g.cols = 4; g.rows = 2; g.tile = 190; g.gapX = 28; g.gapY = 24; }

        if (g_cfg.view == VIEW_LIST) { g.x0 = 200; g.y0 = 116; }
        else
        {
            int w = g.cols * g.tile + (g.cols - 1) * g.gapX;
            int h = g.rows * g.tile + (g.rows - 1) * g.gapY;
            g.x0 = (1280 - w) / 2;
            g.y0 = 112 + (500 - h) / 2;
        }
        return g;
    }

    static int PerPage() { GridSpec g = Grid(); return g.cols * g.rows; }

    static int PageCount()
    {
        int per = PerPage(), n = (int)g_list.size();
        int p = (n + per - 1) / per;
        return p > 0 ? p : 1;
    }

    static void SyncPage()
    {
        int per = PerPage();

        if (g_sel >= (int)g_list.size()) g_sel = (int)g_list.size() - 1;
        if (g_sel < 0) g_sel = 0;
        g_page = g_list.empty() ? 0 : g_sel / per;
        if (!g_list.empty()) currentGame = m_vecAvailRomList[g_list[g_sel]];
    }

    /* Drops every cached image so freshly downloaded covers are picked up
       without restarting. Returns the bytes handed back. */
    static size_t ArtInvalidate()
    {
        size_t freed = 0;

        for (int i = 0; i < kArtSlots; i++)
        {
            if (g_art[i].img.Pixels)
                freed += (size_t)g_art[i].img.Width * g_art[i].img.Height * 4;
            Gfx::freeImage(g_art[i].img);
            g_art[i].driver = -1;
            g_art[i].tried = false;
        }
        return freed;
    }

    /* Everything the frontend holds that a running game has no use for. The
       heap this returns is the same heap MAME allocates ROM regions out of,
       and in applet mode that is a few hundred MB total - twenty-four decoded
       covers plus two full-screen scratch buffers is a real fraction of it,
       and nx_perf_driver_fits() refuses launches on exactly this budget.

       All of it is a cache, so nothing needs saving first: the covers reload
       from the SD card and the backdrop redraws on the next repaint. */
    static size_t ReleaseFrontendMemory()
    {
        size_t freed = ArtInvalidate();

        /* 1280x720x4 each: the backdrop cache and the transition buffer. */
        Gfx::dropSnapshot();
        Gfx::dropFade();
        freed += (size_t)1280 * 720 * 4 * 2;

        return freed;
    }

    /* Fetches covers for the games currently on screen, one at a time, with a
       progress line. Only ever reached from the explicit settings action, and
       only when the user has turned downloads on - nothing here runs on its
       own. Blocking is deliberate: each fetch has an 8s connect / 20s total
       timeout, and a background thread would need its own cancel path for what
       is a handful of files. */
    static void DownloadPageArtwork()
    {
        int per = PerPage(), first = g_page * per;
        int done = 0, failed = 0;
        std::string error;
        char msg[256];

        if (!g_cfg.allowDownloads)
        {
            Refused();
            RenderMessage((char *)"Turn on \"Allow artwork downloads\" first.");
            svcSleepThread(2500000000ULL);
            s_dirty = true;
            return;
        }

        if (!Artwork::Ready() && !Artwork::Init(error))
        {
            Refused();
            snprintf(msg, sizeof(msg), "Cannot start downloads: %s", error.c_str());
            RenderMessage(msg);
            svcSleepThread(3000000000ULL);
            s_dirty = true;
            return;
        }

        for (int i = 0; i < per; i++)
        {
            int idx = first + i;
            const struct GameDriver *drv;

            if (idx >= (int)g_list.size()) break;

            drv = DriverFor(m_vecAvailRomList[g_list[idx]]);
            if (!drv || !drv->name) continue;

            snprintf(msg, sizeof(msg), "Downloading artwork %d of %d - %s",
                     i + 1, per, drv->description ? drv->description : drv->name);
            RenderMessage(msg);

            {
                std::string parent = (drv->clone_of && drv->clone_of->name)
                                   ? drv->clone_of->name : "";
                Artwork::Result r = Artwork::Download(drv->name,
                                                      drv->description ? drv->description : "",
                                                      parent, error);
                if (r == Artwork::RESULT_READY) done++;
                else
                {
                    failed++;
                    /* A dead network fails identically for every remaining
                       game, so stop rather than grinding through 15 timeouts. */
                    if (r == Artwork::RESULT_NETWORK_ERROR) break;
                }
            }
        }

        ArtInvalidate();
        snprintf(msg, sizeof(msg), "Artwork: %d downloaded, %d unavailable.%s%s",
                 done, failed, failed ? "  " : "", failed ? error.c_str() : "");
        RenderMessage(msg);
        svcSleepThread(2500000000ULL);

        if (done) Confirmed(); else Refused();
        s_dirty = true;
    }

    /*=================================================================
        Glass

        One surface treatment, used by everything that is meant to look like a
        moulded piece rather than a painted rectangle. Four layers in a fixed
        order, because the order is what sells it: a shadow to lift the thing
        off the page, a gradient body, a sheen along the top edge, and a
        hairline border last so nothing paints over it.

        Reduced Effects drops the shadow and the sheen and keeps the body and
        the border - still legible, roughly half the fills.
    =================================================================*/
    static void Glass(int x, int y, int w, int h, int r,
                      Gfx::RGBA top, Gfx::RGBA bot, Gfx::RGBA edge,
                      bool shadow = true)
    {
        if (w <= 0 || h <= 0) return;

        if (shadow && !g_cfg.reducedEffects)
            Gfx::drawRoundedShadow(x, y, w, h, r, kShadowDrop, kShadowSpread, kShadowAlpha);

        Gfx::drawRoundedGradient(x, y, w, h, r, top, bot);

        if (!g_cfg.reducedEffects)
        {
            /* A third of the height, capped: on a tall panel the sheen belongs
               to the top edge, not to the top third of a 600px card. */
            int gloss = h / 3;
            if (gloss > 26) gloss = 26;
            Gfx::drawRoundedGloss(x, y, w, h, r, gloss, g_pal.gloss);
        }

        Gfx::drawRoundedOutlineRectangle(x, y, w, h, r, 1, edge);
    }

    /* Same surface, no shadow - for things sitting flat inside a panel that is
       already floating. Stacked shadows read as grime. */
    static void GlassFlat(int x, int y, int w, int h, int r,
                          Gfx::RGBA top, Gfx::RGBA bot, Gfx::RGBA edge)
    {
        Glass(x, y, w, h, r, top, bot, edge, false);
    }

    /* Focus ring.

       Three hairlines rather than one thick stroke: a white line outside the
       blue and another just inside it read as light catching a moulded edge,
       where a single fat band reads as a sticker laid on top. Same reason the
       blue is restrained - the ring says "this one", it is not decoration, and
       a neon border on a pearl interface is the one thing that would make the
       whole screen look cheap. */
    static void FocusRing(FRect r, int radius, int thickness)
    {
        if (r.w <= 0 || r.h <= 0) return;

        Gfx::drawRoundedOutlineRectangle(r.x - 1, r.y - 1, r.w + 2, r.h + 2,
                                         radius + 1, 1, kEdge);
        Gfx::drawRoundedOutlineRectangle(r.x, r.y, r.w, r.h, radius, thickness, kAccent);

        if (thickness > 1 && r.w > thickness * 2 && r.h > thickness * 2)
            Gfx::drawRoundedOutlineRectangle(r.x + thickness, r.y + thickness,
                                             r.w - thickness * 2, r.h - thickness * 2,
                                             radius - thickness, 1, kEdge);
    }

    /*=================================================================
        Widgets
    =================================================================*/
    static void Pill(int x, int y, int w, int h, Gfx::RGBA fill)
    {
        Glass(x, y, w, h, h / 2, fill, Darker(fill, 0.10f), kEdge);
    }

    static int Chip(int x, int y, const string &text, Gfx::RGBA fill, Gfx::RGBA ink)
    {
        int w = (int)Gfx::textWidth(text, TS(15)) + 18;
        int h = (int)TS(15) + 10;

        /* Colour chips (FAV, the warning badges) keep their colour and take a
           gloss on top; neutral ones become pearl glass like everything else. */
        GlassFlat(x, y, w, h, 7, Lighter(fill, 0.16f), Darker(fill, 0.14f), kEdge);
        Gfx::drawText(x + 9, y + 4, text, Ink(ink), TS(15));
        return w;
    }

    /* The top row's mode buttons: a translucent lozenge, pale blue when it is
       the one in force and pearl otherwise. Returns its width so the row can
       pack itself. */
    static int ChipPill(int x, int y, const string &text, bool on)
    {
        int w = (int)Gfx::textWidth(text, TS(15)) + 30;
        int h = (int)TS(15) + 12;

        Glass(x, y, w, h, h / 2,
              on ? kAccentWash : kPanel,
              on ? kAccentDim  : kPanelBot,
              on ? kEdge : kEdgeSoft);
        Gfx::drawTextCentered(x + w / 2, y + 6, text, Ink(kText), TS(15), w - 18);
        return w;
    }

    /* Button disc + label. The font has no controller glyphs, so the letter is
       drawn inside a disc. */
    static int Hint(int x, int y, const char *btn, const char *label)
    {
        int r = (int)TS(11);
        int lw = (int)Gfx::textWidth(label, TS(15));

        GlassFlat(x, y, r * 2, r * 2, r, kAccentWash, kAccentDim, kEdge);
        Gfx::drawTextCentered(x + r, y + r - (int)TS(15) / 2 - 1, btn, Ink(kText), TS(14), r * 2);
        Gfx::drawText(x + r * 2 + 7, y + r - (int)TS(15) / 2, label, Ink(kDim), TS(15));
        return r * 2 + 7 + lw + 20;
    }

    static void HintBar(const char *hints[][2], int count)
    {
        int total = 0, x, r = (int)TS(11);

        /* A translucent strip rather than a bar: this is the least important
           thing on screen and should not be the heaviest. */
        Gfx::drawVerticalGradient(0, 682, 1280, 38, g_pal.hintTop, g_pal.hintBot);
        Gfx::drawRectangle(0, 682, 1280, 1, kEdge);

        for (int i = 0; i < count; i++)
            total += r * 2 + 7 + (int)Gfx::textWidth(hints[i][1], TS(15)) + 20;

        x = (1280 - total) / 2;
        if (x < 12) x = 12;
        for (int i = 0; i < count; i++)
            x += Hint(x, 690, hints[i][0], hints[i][1]);
    }

    /* Toggle switch, drawn so the state reads from the knob position as well
       as the fill - not colour alone. */
    static void Toggle(int x, int y, bool on)
    {
        int w = 56, h = 28;

        GlassFlat(x, y, w, h, h / 2,
                  on ? kGreen : kPanelSoft,
                  on ? Darker(kGreen, 0.20f) : Mix(kPanelSoft, kCanvasBot, 0.5f),
                  kEdgeSoft);
        Glass(on ? x + w - h + 3 : x + 3, y + 3, h - 6, h - 6, (h - 6) / 2,
              kInk, kPanelBot, kEdge);
        if (on) Gfx::drawText(x + 9, y + 5, "I", Ink(kText), TS(14));
        else    Gfx::drawText(x + w - 18, y + 5, "O", Ink(kDim), TS(14));
    }

    static void Slider(int x, int y, int w, int pct)
    {
        int fill = w * pct / 100;

        /* Track recessed, knob raised - the one place an inner shadow earns
           its keep, because a groove that sticks out reads as a bug. */
        Gfx::drawRoundedRectangle(x, y + 10, w, 8, 4, kPanelSoft);
        Gfx::drawRoundedRectangle(x, y + 10, w, 3, 2, Mix(kPanelSoft, kText, 0.20f));
        Gfx::drawRoundedRectangle(x, y + 10, fill < 8 ? 8 : fill, 8, 4, kAccent);
        Glass(x + fill - 8, y + 2, 16, 24, 8, kInk, kPanelBot, kEdge);
    }

    /* Hold-to-confirm ring: a filling bar under the prompt. */
    static void HoldBar(int x, int y, int w)
    {
        int fill = w * s_holdFrames / kHoldFrames;

        Gfx::drawRoundedRectangle(x, y, w, 8, 4, kPanelSoft);
        if (fill > 0) Gfx::drawRoundedRectangle(x, y, fill, 8, 4, kRed);
    }

    /* Frosted plate behind a game with no artwork. Pale and low-contrast: it
       is a placeholder, and a saturated one competes with the real covers
       either side of it. */
    static Gfx::RGBA PlateColour(const std::string &name)
    {
        u32 h = 2166136261u;
        Gfx::RGBA c;

        for (size_t i = 0; i < name.size(); i++) { h ^= (u8)name[i]; h *= 16777619u; }
        c.R = (u8)(g_pal.plateBase.R + (h & g_pal.plateSpread));
        c.G = (u8)(g_pal.plateBase.G + ((h >> 8) & g_pal.plateSpread));
        c.B = (u8)(g_pal.plateBase.B + ((h >> 16) & g_pal.plateSpread));
        c.A = 255;
        return c;
    }

    /*=================================================================
        Chrome
    =================================================================*/
    static void DrawBackdrop()
    {
        if (Gfx::restore()) return;

        Gfx::drawVerticalGradient(0, 0, 1280, 720, kCanvasTop, kCanvasBot);

        if (!g_cfg.reducedEffects)
        {
            /* Kept, but barely: on the old near-black canvas these were the
               only thing stopping a flat fill reading as a crash. On pearl
               they are almost white on white, and that is the point - a pale
               background wants texture you notice only when you look for it,
               not decoration. */
            static const int blob[][3] = {
                { 120, 380, 26 }, { 250, 120, 14 }, { 1010, 250, 20 }, { 1180, 560, 30 },
                { 700, 60, 12 }, { 420, 640, 18 }, { 880, 660, 14 }, { 60, 120, 18 }
            };
            for (unsigned i = 0; i < sizeof(blob) / sizeof(blob[0]); i++)
                Gfx::drawRoundedRectangle(blob[i][0], blob[i][1], blob[i][2] * 2, blob[i][2] * 2,
                                          blob[i][2], kBlob);
        }
        Gfx::snapshot();
    }

    static void DrawClock()
    {
        time_t now = time(NULL);
        struct tm tmv;
        char buf[64];

        localtime_r(&now, &tmv);
        s_lastMinute = tmv.tm_min;

        Glass(24, 20, 152, 50, 14, kPanel, kPanelBot, kEdge);
        strftime(buf, sizeof(buf), "%H:%M", &tmv);
        Gfx::drawText(42, 25, buf, Ink(kText), TS(24));
        strftime(buf, sizeof(buf), "%d/%m/%Y", &tmv);
        Gfx::drawText(42, 50, buf, Ink(kDim), TS(13));
    }

    static void DrawBattery()
    {
        u32 pct = 0;
        PsmChargerType ct = PsmChargerType_Unconnected;
        Gfx::RGBA lvl = kGreen;
        char buf[32];
        int bx = 1104;

        if (s_psmUp)
        {
            psmGetBatteryChargePercentage(&pct);
            psmGetChargerType(&ct);
        }
        if (pct <= 15) lvl = kRed;
        else if (pct <= 35) lvl = kAmber;

        Glass(bx, 20, 152, 50, 14, kPanel, kPanelBot, kEdge);

        /* The cell is a recess in the plate, so it takes the border and no
           gloss - the sheen belongs to the panel it is cut into. */
        Gfx::drawRoundedRectangle(bx + 44, 27, 48, 20, 4, Mix(kPanelSoft, kCanvasBot, 0.4f));
        Gfx::drawRoundedOutlineRectangle(bx + 44, 27, 48, 20, 4, 1, kEdgeSoft);
        Gfx::drawRectangle(bx + 92, 33, 5, 8, kEdgeSoft);
        if (pct > 0) Gfx::drawRoundedRectangle(bx + 47, 30, (int)(42 * pct / 100), 14, 3, lvl);
        if (ct != PsmChargerType_Unconnected) Gfx::drawText(bx + 64, 27, "+", Ink(kText), TS(16));

        snprintf(buf, sizeof(buf), "%lu%%", (unsigned long)pct);
        Gfx::drawTextCentered(bx + 76, 48, buf, Ink(kText), TS(15), 140);
    }

    /* Rail icons.
       Drawn from primitives rather than glyphs: the one bundled font has no
       symbol pages, so anything pictorial has to be constructed. Each shape is
       built from strokes so it stays legible at 50px on a handheld screen -
       filled blobs turn to mush at this size. */
    /* Rail buttons and their art. The icon was authored against a 50px cell;
       everything scales off kRailCell so making the buttons bigger is one
       number rather than six re-drawn icons. */
    enum { kRailCell = 68, kRailStep = 96, kRailArt = 50 };

    static int RS(int v) { return v * kRailCell / kRailArt; }

    /* Three buttons, vertically centred, so the rail stays balanced whatever
       the cell size is. */
    static int RailX(bool onLeft) { return onLeft ? 24 : 1280 - 24 - kRailCell; }
    static int RailY(int i)
    {
        const int span = kRailCell * 3 + (kRailStep - kRailCell) * 2;
        return (720 - span) / 2 + i * kRailStep;
    }

    static void RailIcon(int kind, int x, int y, Gfx::RGBA c)
    {
        switch (kind)
        {
            case 0: /* All games: two stacked arcade cartridges */
                Gfx::drawOutlineRectangle(x + RS(-14), y + RS(-8), RS(20), RS(22), RS(2), c);
                Gfx::drawOutlineRectangle(x + RS(-6), y + RS(-14), RS(20), RS(22), RS(2), c);
                Gfx::drawRectangle(x + RS(-2), y + RS(-10), RS(12), RS(2), c);
                Gfx::drawRectangle(x + RS(-2), y + RS(-6), RS(12), RS(2), c);
                break;

            case 1: /* Favourites: heart, two lobes over a taper */
                Gfx::drawRoundedRectangle(x + RS(-11), y + RS(-10), RS(11), RS(11), RS(6), c);
                Gfx::drawRoundedRectangle(x, y + RS(-10), RS(11), RS(11), RS(6), c);
                Gfx::drawRectangle(x + RS(-10), y + RS(-4), RS(20), RS(4), c);
                for (int i = 0; i < RS(10); i++)
                    Gfx::drawRectangle(x + RS(-9) + i, y + i, (RS(9) - i) * 2, 1, c);
                break;

            case 2: /* Recent: clock face with hands */
                Gfx::drawRoundedRectangle(x + RS(-13), y + RS(-13), RS(26), RS(26), RS(13), c);
                Gfx::drawRoundedRectangle(x + RS(-10), y + RS(-10), RS(20), RS(20), RS(10), kPanel);
                Gfx::drawRectangle(x + RS(-1), y + RS(-8), RS(2), RS(9), c);
                Gfx::drawRectangle(x + RS(-1), y + RS(-1), RS(8), RS(2), c);
                break;

            case 3: /* Controllers: gamepad - body, grips, stick, buttons */
                Gfx::drawRoundedRectangle(x + RS(-15), y + RS(-8), RS(30), RS(16), RS(6), c);
                Gfx::drawRoundedRectangle(x + RS(-15), y + RS(-2), RS(8), RS(12), RS(4), c);
                Gfx::drawRoundedRectangle(x + RS(7), y + RS(-2), RS(8), RS(12), RS(4), c);
                Gfx::drawRoundedRectangle(x + RS(-11), y + RS(-4), RS(7), RS(7), RS(3), kPanel);
                Gfx::drawRoundedRectangle(x + RS(5), y + RS(-5), RS(4), RS(4), RS(2), kPanel);
                Gfx::drawRoundedRectangle(x + RS(5), y + RS(1), RS(4), RS(4), RS(2), kPanel);
                break;

            case 4: /* Settings: three tuning sliders with offset handles */
                Gfx::drawRectangle(x + RS(-13), y + RS(-9), RS(26), RS(2), c);
                Gfx::drawRectangle(x + RS(-13), y + RS(-1), RS(26), RS(2), c);
                Gfx::drawRectangle(x + RS(-13), y + RS(7), RS(26), RS(2), c);
                Gfx::drawRoundedRectangle(x + RS(-8), y + RS(-12), RS(5), RS(8), RS(2), c);
                Gfx::drawRoundedRectangle(x + RS(4), y + RS(-4), RS(5), RS(8), RS(2), c);
                Gfx::drawRoundedRectangle(x + RS(-3), y + RS(4), RS(5), RS(8), RS(2), c);
                break;

            case 5: /* Quit: arrow leaving a doorway */
                Gfx::drawOutlineRectangle(x + RS(-14), y + RS(-13), RS(15), RS(26), RS(2), c);
                Gfx::drawRectangle(x + RS(-2), y + RS(-2), RS(13), RS(3), c);
                Gfx::drawRectangle(x + RS(7), y + RS(-6), RS(3), RS(11), c);
                Gfx::drawRectangle(x + RS(10), y + RS(-4), RS(3), RS(7), c);
                break;
        }
    }

    static const char *kLeftRailLabel[3]  = { "Games", "Faves", "Recent" };
    static const char *kRightRailLabel[3] = { "Pads", "Settings", "Quit" };

    /* Label sits beside the focused icon. Rails are icon-only at rest, so
       without this the only way to learn what a button does is to press it. */
    static void RailLabel(int x, int y, const char *text, bool onLeft)
    {
        int w  = (int)Gfx::textWidth(text, TS(16)) + 28;
        int lx = onLeft ? x + kRailCell + 12 : x - w - 12;
        int ly = y + (kRailCell - 32) / 2;

        Glass(lx, ly, w, 32, 10, kAccentWash, kAccentDim, kEdge);
        Gfx::drawTextCentered(lx + w / 2, ly + 8, text, Ink(kText), TS(16), w - 16);

        /* little notch pointing back at the icon */
        Gfx::drawRectangle(onLeft ? lx - 5 : lx + w, ly + 12, 6, 8, kAccentDim);
    }

    static void DrawRailButton(int x, int y, int kind, float f, bool active)
    {
        const int r = RS(16);
        bool focused = (f > 0.5f);
        int grow = (int)(kRailCell * kFocusGrow * 0.5f * f * s_grow + 0.5f);
        int size = kRailCell + grow * 2;

        /* Pearl at rest, washing to pale blue as focus arrives, so the button
           lights from within rather than gaining a ring around it. */
        Gfx::RGBA top = Mix(kPanel, kAccentWash, f);
        Gfx::RGBA bot = Mix(kPanelBot, kAccentDim, f);

        x -= grow; y -= grow;

        /* The active category is tinted even without focus - which filter is
           applied has to be readable while the cursor is out in the grid. */
        if (active && f < 0.5f)
        {
            top = Mix(top, kAccentWash, 0.55f);
            bot = Mix(bot, kAccentDim, 0.55f);
        }

        Glass(x, y, size, size, r, top, bot, focused ? kEdge : kEdgeSoft);

        RailIcon(kind, x + size / 2, y + size / 2, Mix(Ink(kDim), Ink(kText), f));

        /* The focus ring travels and is drawn once for the whole screen; what
           stays here is the active-category marker, which is a different fact
           and has to be readable while the cursor is out in the grid. */
        if (!focused && active)
        {
            /* Active-but-unfocused category keeps a marker, so which filter is
               applied is readable while the cursor is out in the grid. */
            Gfx::drawRoundedOutlineRectangle(x, y, kRailCell, kRailCell, r, 2, kAccent);
            Gfx::drawRoundedRectangle(x + kRailCell / 2 - 7, y + kRailCell + 4, 14, 4, 2, kAccent);
        }
    }

    static void DrawRails()
    {
        static const int leftKind[3]  = { 0, 1, 2 };   /* all, favourites, recent */
        static const int rightKind[3] = { 3, 4, 5 };   /* controllers, settings, quit */

        for (int i = 0; i < 3; i++)
        {
            float lf = FocusWeight(RailX(true),  RailY(i), kRailCell, kRailStep);
            float rf = FocusWeight(RailX(false), RailY(i), kRailCell, kRailStep);

            DrawRailButton(RailX(true),  RailY(i), leftKind[i],  lf, (int)g_category == i);
            DrawRailButton(RailX(false), RailY(i), rightKind[i], rf, false);

            /* Icon-only rails failed first-use: labels appeared only after
               focus reached a mystery button. Keep short names visible in the
               spare 28px between cells; focused flyouts remain as the larger
               confirmation without being the sole explanation. */
            Gfx::drawTextCentered(RailX(true) + kRailCell / 2,
                                  RailY(i) + kRailCell + 7,
                                  kLeftRailLabel[i], Ink(kDim), TS(11), kRailCell + 8);
            Gfx::drawTextCentered(RailX(false) + kRailCell / 2,
                                  RailY(i) + kRailCell + 7,
                                  kRightRailLabel[i], Ink(kDim), TS(11), kRailCell + 8);

            /* Touch targets are the drawn rects, registered here rather than
               recomputed in the input handler - a hitbox worked out twice is a
               hitbox that drifts. */
            Hit(RailX(true),  RailY(i), kRailCell, kRailCell, HIT_LEFTRAIL + i);
            Hit(RailX(false), RailY(i), kRailCell, kRailCell, HIT_RIGHTRAIL + i);
        }

        /* Drawn after the buttons so a label can overlap its neighbours. */
        for (int i = 0; i < 3; i++)
        {
            if (g_zone == ZONE_LEFTRAIL && g_leftSel == i)
                RailLabel(RailX(true), RailY(i), kLeftRailLabel[i], true);
            if (g_zone == ZONE_RIGHTRAIL && g_rightSel == i)
                RailLabel(RailX(false), RailY(i), kRightRailLabel[i], false);
        }
    }

    /*=================================================================
        Tiles
    =================================================================*/
    /* `f` is how much of the travelling focus rect is over this tile, 0..1.
       Everything focus does to a tile - the lift, the plate, the halo - is
       scaled by it, so the tile the ring is leaving settles back at exactly
       the rate the ring departs and the two never disagree. */
    static void DrawTile(int x, int y, int size, const std::string &name, float f)
    {
        const struct GameDriver *drv = DriverFor(name);
        const char *warn = WarnChip(drv);
        GameStat &st = StatFor(name);
        Gfx::Image *art = ArtFor(drv);
        bool focused = (f > 0.5f);

        /* One curve for the whole tile. Scale, brightness, sheen and shadow
           all ride it, so "the tile comes forward" is a single movement rather
           than four effects that happen to fire together. */
        f = FocusCurve(f);

        int grow = (int)(size * kFocusGrow * 0.5f * f * s_grow + 0.5f);

        x -= grow; y -= grow; size += grow * 2;

        /* Frame first: the artwork sits inside a thin polished bezel, so the
           tile is a moulded part holding a picture rather than a picture with
           a border drawn on it. The shadow deepens a little with focus, which
           is most of what "raised" reads as. */
        if (!g_cfg.reducedEffects)
            Gfx::drawRoundedShadow(x, y, size, size, 16, kShadowDrop + (int)(2 * f),
                                   kShadowSpread + (int)(2 * f),
                                   (u8)(kShadowAlpha + 26 * f));

        Gfx::drawRoundedGradient(x, y, size, size, 16,
                                 Mix(kCard, kCardSel, f), Mix(kCardBot, kCardSelBot, f));

        if (art)
            Gfx::drawImage(*art, x + 5, y + 5, size - 10, size - 10, 255, true);
        else
        {
            /* Placeholder: a frosted plate carrying the game's initials, tinted
               a shade per game so a set without artwork still looks like itself
               every visit. Abbreviation large and dark, title beneath it in the
               secondary grey. */
            std::string ini;

            GlassFlat(x + 5, y + 5, size - 10, size - 10, 12,
                      Lighter(PlateColour(name), 0.22f), PlateColour(name), kEdgeSoft);

            for (size_t i = 0; i < name.size() && ini.size() < 3; i++)
                if (isalnum((unsigned char)name[i]) && (i == 0 || name[i - 1] == ' '))
                    ini += (char)toupper((unsigned char)name[i]);
            if (ini.empty() && !name.empty()) ini += (char)toupper((unsigned char)name[0]);

            Gfx::drawTextCentered(x + size / 2, y + size / 2 - (int)TS(30) / 2, ini,
                                  Ink(kText), TS(30), size - 20);
            Gfx::drawTextCentered(x + size / 2, y + size - 34, name, Ink(kDim), TS(12), size - 14);
        }

        /* Sheen and bezel over the artwork, not under it - glass in front of a
           picture is the whole idea, and drawn first it would be covered. */
        if (!g_cfg.reducedEffects)
            Gfx::drawRoundedGloss(x, y, size, size, 16, size / 4,
                                  (u8)(g_pal.gloss * 0.64f + g_pal.gloss * 0.34f * f));

        Gfx::drawRoundedOutlineRectangle(x, y, size, size, 16, 1,
                                         focused ? kEdge : kEdgeSoft);

        if (st.fav)  Chip(x + size - 48, y + 8, "FAV", kMagenta, kInk);
        if (warn)    Chip(x + 8, y + size - 32, warn,
                          (drv && (drv->flags & GAME_NOT_WORKING)) ? kRed : kAmber, kInk);

        /* The ring itself is drawn once by the caller at the travelling rect,
           not per tile - a ring per tile would have to blink from one to the
           next, which is the jump the travel exists to remove. */
        if (focused && g_cfg.colourBlindFocus)
            Chip(x + 8, y + 8, ">", kText, kInk);
    }

    static void DrawListRow(int x, int y, int w, int h, const std::string &name, float f)
    {
        const struct GameDriver *drv = DriverFor(name);
        const char *warn = WarnChip(drv);
        GameStat &st = StatFor(name);
        /* Indent follows the ring rather than snapping: the caret it makes
           room for slides in at the same rate. */
        int tx = x + 18 + (int)(12 * f);
        FRect p = LiftedRect(x, y, w, h, f);

        GlassFlat(p.x, p.y, p.w, p.h, 8, Mix(kCard, kCardSel, f),
                  Mix(kCardBot, kCardSelBot, f), f > 0.5f ? kEdge : kEdgeSoft);
        Gfx::drawTextClipped(tx, y + h / 2 - (int)TS(17) / 2, name,
                             Mix(Ink(kDim), Ink(kText), f), TS(17), w - 400);

        if (drv && drv->year)
            Gfx::drawText(x + w - 360, y + h / 2 - (int)TS(15) / 2, drv->year, Ink(kDim), TS(15));
        if (drv && drv->manufacturer)
            Gfx::drawTextClipped(x + w - 300, y + h / 2 - (int)TS(15) / 2, drv->manufacturer,
                                 Ink(kDim), TS(15), 160);
        if (st.fav) Chip(x + w - 120, y + h / 2 - 13, "FAV", kMagenta, kInk);
        if (warn)   Chip(x + w - 60, y + h / 2 - 13, warn,
                         (drv && (drv->flags & GAME_NOT_WORKING)) ? kRed : kAmber, kInk);
    }

    /*=================================================================
        Library
    =================================================================*/
    static void DrawEmptyState()
    {
        const char *head, *body;

        if (!g_search.empty())            { head = "No games match that search";
                                            body = "Press B to clear it, or X to search again."; }
        else if (g_category == CAT_FAV)   { head = "No favourites yet";
                                            body = "Highlight a game and press Y to add it here."; }
        else if (g_category == CAT_RECENT){ head = "Nothing played yet";
                                            body = "Games you launch appear here, newest first."; }
        else                              { head = "No ROMs found";
                                            body = "Put your .zip ROM sets in the roms/ folder next to the app."; }

        Glass(340, 250, 600, 190, 20, kPanel, kPanelBot, kEdge);
        Gfx::drawTextCentered(640, 300, head, Ink(kText), TS(22), 560);
        Gfx::drawTextCentered(640, 344, body, Ink(kDim), TS(16), 560);
    }

    static void DrawLibrary()
    {
        GridSpec g = Grid();
        int per = g.cols * g.rows, first = g_page * per, pages = PageCount();
        FRect ring;
        Gfx::DrawState saved;

        DrawBackdrop();
        DrawClock();
        DrawBattery();

        /* Focus is resolved before anything reads it: the rails and the grid
           both take their highlight off the same travelling rect, so it has to
           be stepped once, up front, rather than by whichever of them happens
           to draw first. Rails and tiles share one key so the ring travels
           between them instead of teleporting at the edge of the grid. */
        {
            int fx, fy, fw, fh;

            if (g_zone == ZONE_LEFTRAIL)
            { fx = RailX(true);  fy = RailY(g_leftSel);  fw = fh = kRailCell; }
            else if (g_zone == ZONE_RIGHTRAIL)
            { fx = RailX(false); fy = RailY(g_rightSel); fw = fh = kRailCell; }
            else if (g_list.empty())
            {
                /* Nothing to focus. Parked off screen, and s_fLive cleared so
                   it snaps there - left to chase, the ring would spend a dozen
                   repaints flying somewhere nobody can see it go. */
                s_fLive = false;
                fx = fy = -8000; fw = fh = 0;
            }
            else if (g_cfg.view == VIEW_LIST)
            { fx = g.x0; fy = g.y0 + (g_sel - first) * (g.tile + g.gapY);
              fw = 880; fh = g.tile; }
            else
            { fx = g.x0 + ((g_sel - first) % g.cols) * (g.tile + g.gapX);
              fy = g.y0 + ((g_sel - first) / g.cols) * (g.tile + g.gapY);
              fw = fh = g.tile; }

            /* The view mode is part of the key: changing it relays the whole
               screen, and a ring gliding between two unrelated layouts is a
               line drawn across a page that no longer exists. */
            ring = FocusTrack(10 + g_cfg.view, fx, fy, fw, fh);
        }

        saved = PageBegin();

        DrawRails();

        /* category / sort / view chips, centred between the two pills.

           Pill-shaped translucent buttons: the active one takes a pale blue
           glass and the rest stay pearl. Deliberately not a saturated fill -
           the text has to stay dark and readable, which a strong blue would
           forbid, and the difference between pearl and pale blue is plenty at
           this size. */
        {
            int x = 200;
            char buf[64];
            const int ch = (int)TS(15) + 12;

            {
                int cw = ChipPill(x, 31, kCategoryName[g_category], true);
                Hit(x, 31, cw, ch, HIT_CHIP_CAT); x += cw + 10;
            }
            snprintf(buf, sizeof(buf), "Sort  %s", kSortName[g_cfg.sort]);
            {
                int cw = ChipPill(x, 31, buf, false);
                Hit(x, 31, cw, ch, HIT_CHIP_SORT); x += cw + 10;
            }
            snprintf(buf, sizeof(buf), "View  %s", kViewName[g_cfg.view]);
            {
                int cw = ChipPill(x, 31, buf, false);
                Hit(x, 31, cw, ch, HIT_CHIP_VIEW); x += cw + 10;
            }
            if (!g_search.empty())
            {
                snprintf(buf, sizeof(buf), "\"%s\"", g_search.c_str());
                Chip(x, 33, buf, kMagenta, kInk);
            }
        }

        if (g_list.empty()) DrawEmptyState();
        else
        {
            for (int i = 0; i < per; i++)
            {
                int idx = first + i;
                if (idx >= (int)g_list.size()) break;

                if (g_cfg.view == VIEW_LIST)
                {
                    int ry = g.y0 + i * (g.tile + g.gapY);
                    DrawListRow(g.x0, ry, 880, g.tile, m_vecAvailRomList[g_list[idx]],
                                FocusWeight(g.x0, ry, 880, g.tile + g.gapY));
                    Hit(g.x0, ry, 880, g.tile, HIT_TILE + i);
                }
                else
                {
                    int tx = g.x0 + (i % g.cols) * (g.tile + g.gapX);
                    int ty = g.y0 + (i / g.cols) * (g.tile + g.gapY);
                    DrawTile(tx, ty, g.tile, m_vecAvailRomList[g_list[idx]],
                             FocusWeight(tx, ty, g.tile + g.gapX, g.tile + g.gapY));
                    Hit(tx, ty, g.tile, g.tile, HIT_TILE + i);
                }
            }

            /* One ring for the screen, drawn over the tiles at wherever the
               chase has got to. Lifted by the same spring the focused tile
               grows on, so ring and tile stay the same size as each other
               through the settle. */
            if (g_zone == ZONE_GRID)
            {
                /* Tiles are square and take the percentage; a list row is
                   wide and takes the flat lift, same as every other row in
                   the UI. Either way this is the rect the card underneath was
                   just drawn on. */
                FRect r = (g_cfg.view == VIEW_LIST) ? Lifted(ring, 1.0f)
                                                    : GrownBy(ring, kFocusGrow);
                FocusRing(r, g_cfg.view == VIEW_LIST ? 8 : 16,
                             g_cfg.view == VIEW_LIST ? 2 : 3);
                if (g_cfg.view == VIEW_LIST)
                    Gfx::drawText(r.x + 6, r.y + r.h / 2 - (int)TS(18) / 2, ">", kAccentInk, TS(18));
            }

            if (g_cfg.view != VIEW_LIST)
            {
                const std::string &sel = m_vecAvailRomList[g_list[g_sel]];
            int w = (int)Gfx::textWidth(sel, TS(21)) + 32;

                if (w > 880) w = 880;
            /* Selected title is information, not another control. Old pill
               looked clickable and competed with every channel card. */
            Gfx::drawTextCentered(640, 636, sel, Ink(kText), TS(21), w);
            Gfx::drawRoundedRectangle(604, 665, 72, 4, 2, kAccent);
            }

            if (pages > 1)
            {
                int shown = pages > 14 ? 14 : pages;
                int dx = 640 - shown * 9;

                for (int p = 0; p < shown; p++)
                {
                    bool on = (pages > 14) ? (p == g_page * shown / pages) : (p == g_page);
                    Gfx::drawRoundedRectangle(dx + p * 18, on ? 683 : 685, on ? 20 : 10,
                                              on ? 8 : 6, 4, on ? kAccent : kEdgeSoft);
                }
            }
        }

        /* Same ring, out on a rail. Drawn here rather than inside DrawRails so
           there is exactly one of it on the screen at any moment - two rings
           that hand focus over between them is the jump this replaces. */
        if (g_zone != ZONE_GRID)
        {
            FRect r = GrownBy(ring, kFocusGrow);
            FocusRing(r, RS(16) + 2, 3);
        }

        {
            char buf[96];
            snprintf(buf, sizeof(buf), "%d shown  -  page %d of %d",
                     (int)g_list.size(), g_page + 1, pages);
            Gfx::drawText(24, 84, RomCountText, Ink(kDim), TS(14));
            Gfx::drawText(24, 656, buf, Ink(kDim), TS(14));
        }

        if (g_cfg.persistentHints)
        {
            static const char *hints[][2] = {
                { "A", "Play" }, { "+", "Info" },
                { "Y", "Favourite" }, { "X", "Search" }
            };
            HintBar(hints, 4);
        }

        Gfx::popState(saved);
    }

    /*=================================================================
        Game info
    =================================================================*/
    static void InfoRow(int x, int y, const char *label, const string &value)
    {
        Gfx::drawText(x, y, label, Ink(kDim), TS(16));
        Gfx::drawTextClipped(x + 210, y, value, Ink(kText), TS(16), 400);
    }

    static void DrawGameInfo()
    {
        const std::string name = currentGame;
        const struct GameDriver *drv = DriverFor(name);
        GameStat &st = StatFor(name);
        const char *warn = WarnChip(drv);
        Gfx::Image *art = ArtFor(drv);
        char buf[128];
        int y;

        DrawBackdrop();
        DrawClock();
        DrawBattery();

        Glass(60, 96, 1160, 568, 24, kPanel, kPanelBot, kEdge);

        if (art) Gfx::drawImage(*art, 100, 136, 300, 300, 255, true);
        else     GlassFlat(100, 136, 300, 300, 18,
                           Lighter(PlateColour(name), 0.22f),
                           PlateColour(name), kEdgeSoft);
        Gfx::drawRoundedOutlineRectangle(100, 136, 300, 300, 18, 1, kEdge);

        Gfx::drawTextClipped(440, 140, name, Ink(kText), TS(28), 740);

        {
            int cx = 440;
            if (warn) cx += Chip(cx, 184, warn,
                                 (drv && (drv->flags & GAME_NOT_WORKING)) ? kRed : kAmber, kInk) + 8;
            if (st.fav) Chip(cx, 184, "FAVOURITE", kMagenta, kInk);
        }

        y = 232;
        InfoRow(440, y, "Year",         (drv && drv->year) ? drv->year : "Unknown");                y += 36;
        InfoRow(440, y, "Manufacturer", (drv && drv->manufacturer) ? drv->manufacturer : "Unknown");y += 36;
        snprintf(buf, sizeof(buf), "%d", PlayerCount(drv));
        InfoRow(440, y, "Players",      PlayerCount(drv) ? buf : "Unknown");                        y += 36;
        InfoRow(440, y, "Performance",  drv ? nx_perf_tier_name(nx_perf_driver_tier(drv)) : "-");   y += 36;
        snprintf(buf, sizeof(buf), "%d", st.plays);
        InfoRow(440, y, "Times played", buf);                                                       y += 36;
        if (st.lastPlayed)
        {
            time_t t = (time_t)st.lastPlayed; struct tm tmv;
            localtime_r(&t, &tmv);
            strftime(buf, sizeof(buf), "%d %b %Y at %H:%M", &tmv);
            InfoRow(440, y, "Last played", buf);
        }
        else InfoRow(440, y, "Last played", "Never");
        y += 36;
        InfoRow(440, y, "ROM set", (drv && drv->name) ? drv->name : "-");

        /* Play is the loudest thing here, by size and by colour. */
        Glass(100, 470, 300, 72, 18, kAccentWash, kAccentDim, kEdge);
        Gfx::drawTextCentered(250, 470 + 36 - (int)TS(24) / 2, "PLAY", Ink(kText), TS(24), 280);
        Gfx::drawRoundedOutlineRectangle(100, 470, 300, 72, 18, 2, kAccent);

        Gfx::drawTextCentered(250, 560, st.fav ? "Y   Remove favourite" : "Y   Add favourite",
                              Ink(kDim), TS(15), 300);
        Gfx::drawTextClipped(440, 500, "Display, audio and controller options open from the",
                             Ink(kDim), TS(15), 740);
        Gfx::drawTextClipped(440, 526, "in-game menu once the game is running.",
                             Ink(kDim), TS(15), 740);

        {
            static const char *hints[][2] = { { "A", "Play" }, { "Y", "Favourite" }, { "B", "Back" } };
            HintBar(hints, 3);
        }
    }

    /*=================================================================
        Settings - sidebar left, rows right
    =================================================================*/
    enum { CAT_GENERAL = 0, CAT_DISPLAY, CAT_AUDIO, CAT_CONTROLS, CAT_LIBRARY,
           CAT_ARTWORK, CAT_PERF, CAT_INTERFACE, CAT_ACCESS, CAT_ABOUT, SETCAT_COUNT };

    static const char *kSetCat[SETCAT_COUNT] = {
        "General", "Display", "Audio", "Controllers", "Game Library",
        "Artwork", "Performance", "Interface", "Accessibility", "About"
    };
    static const char *kSetCatHelp[SETCAT_COUNT] = {
        "Startup behaviour and safe defaults",
        "Picture calibration for this console",
        "Latency, samples and interface sound",
        "Arcade mapping and controller assignment",
        "What appears on the channel grid",
        "Cover downloads and in-game layers",
        "Speed, memory and reduced effects",
        "Theme, hints and menu movement",
        "Readability, feedback and safer actions",
        "Build, core and runtime information"
    };

    static int g_setCat = 0, g_setRow = 0;
    static bool g_setInSidebar = true;

    enum RowKind { ROW_TOGGLE, ROW_SLIDER, ROW_CHOICE, ROW_TEXT, ROW_ACTION };

    struct SetRow
    {
        RowKind kind;
        const char *label;
        const char *help;
        void *value;            /* bool* or int* */
        int   lo, hi, step;
        const char **choices;
        int   choiceCount;
    };

    static SetRow g_rows[16];
    static int g_rowCount = 0;

    static void AddToggle(const char *l, const char *h, bool *v)
    { SetRow r; memset(&r,0,sizeof(r)); r.kind=ROW_TOGGLE; r.label=l; r.help=h; r.value=v; g_rows[g_rowCount++]=r; }

    static void AddSlider(const char *l, const char *h, int *v, int lo, int hi, int st)
    { SetRow r; memset(&r,0,sizeof(r)); r.kind=ROW_SLIDER; r.label=l; r.help=h; r.value=v; r.lo=lo; r.hi=hi; r.step=st; g_rows[g_rowCount++]=r; }

    static void AddChoice(const char *l, const char *h, int *v, const char **c, int n)
    { SetRow r; memset(&r,0,sizeof(r)); r.kind=ROW_CHOICE; r.label=l; r.help=h; r.value=v; r.choices=c; r.choiceCount=n; g_rows[g_rowCount++]=r; }

    static void AddText(const char *l, const char *h)
    { SetRow r; memset(&r,0,sizeof(r)); r.kind=ROW_TEXT; r.label=l; r.help=h; g_rows[g_rowCount++]=r; }

    static void AddAction(const char *l, const char *h)
    { SetRow r; memset(&r,0,sizeof(r)); r.kind=ROW_ACTION; r.label=l; r.help=h; g_rows[g_rowCount++]=r; }

    static void BuildRows()
    {
        g_rowCount = 0;
        switch (g_setCat)
        {
            case CAT_GENERAL:
                AddToggle("Skip MAME legal screen", "Hide the emulator disclaimer; the arcade boot still runs.", &g_cfg.skipDisclaimer);
                AddToggle("Skip hardware summary", "Hide the driver info page shown before a game starts.", &g_cfg.skipGameInfo);
                /* This core has no options.cheat flag - the cheat engine is
                   driven entirely from MAME's own in-game menu. */
                AddText("Cheats", "Open the in-game menu (right stick click) and choose Cheat.");
                AddAction("Reset all settings", "Put every option on this screen back to its default.");
                break;
            case CAT_DISPLAY:
                AddSlider("Brightness", "Luminance of the emulated screen.", &g_cfg.brightness, 50, 150, 5);
                AddSlider("Gamma", "Contrast curve of the emulated screen.", &g_cfg.gamma, 50, 150, 5);
                AddText("Output resolution", "1080p docked, 720p handheld. Follows the dock automatically.");
                AddText("Refresh", "The panel is fixed at 60Hz; games are paced to their own rate.");
                break;
            case CAT_AUDIO:
                AddText("Sample rate", "48000 Hz - fixed by the console's audio output.");
                AddToggle("External samples", "Use original arcade .wav samples when a set provides them.", &g_cfg.useSamples);
                AddChoice("Audio delay",
                          "Lower is more responsive; raise it if sound crackles. Applies next launch.",
                          &g_cfg.audioDelay, kAudioDelayName, 3);
                AddToggle("Navigation sounds", "Play a short tone when moving around this interface.", &g_cfg.navSounds);
                break;
            case CAT_CONTROLS:
                AddText("Open in-game menu", "Click the right stick during a game for MAME's own menu.");
                AddText("Pause overlay", "Click the left stick during a game.");
                AddText("Exit to library", "Click both sticks together, or use the overlay row.");
                AddText("Arcade buttons", "A B X Y L R are buttons 1-6; ZL and ZR are 9 and 10.");
                AddText("Start and Coin", "Plus is Start, Minus is Coin - never eaten by the UI.");
                AddChoice("Vibration", "How strongly the pad responds to the interface.", &g_cfg.rumble, kRumbleName, 3);
                AddToggle("Hold to confirm", "Reset and Exit need the button held, to prevent mistakes.", &g_cfg.holdToConfirm);
                AddAction("Controller assignment", "Open the player and controller screen.");
                break;
            case CAT_LIBRARY:
                AddToggle("Hide non-working games", "Leave drivers marked BROKEN out of the library.", &g_cfg.hideNotWorking);
                AddChoice("Default sort", "Order games are listed in.", &g_cfg.sort, kSortName, SORT_COUNT);
                AddChoice("Default view", "Layout the library opens with.", &g_cfg.view, kViewName, VIEW_COUNT);
                AddAction("Rescan ROM folder", "Look through roms/ again for newly added sets.");
                break;
            case CAT_ARTWORK:
                AddToggle("Show cover artwork", "Draw box art on tiles when an image is available.", &g_cfg.showArtwork);
                AddToggle("In-game artwork layers", "Backdrops, overlays and bezels. Costs speed on V1.", &g_cfg.useArtworkLayers);
                AddToggle("Crop artwork", "Trim artwork to the game display area.", &g_cfg.cropArtwork);
                AddText("Artwork folder", kArtDir);
                AddToggle("Allow artwork downloads", "Let this app fetch missing covers over the internet.", &g_cfg.allowDownloads);
                AddAction("Download covers on this page", "Fetch artwork for the games currently shown.");
                break;
            case CAT_PERF:
                AddText("CPU boost", "Raised automatically for heavier drivers while a game runs.");
                AddText("Frameskip", "Adaptive - video frames are dropped only to catch up.");
                AddToggle("Reduced interface effects", "Drop glows and background shapes to save time.", &g_cfg.reducedEffects);
                AddText("Memory mode", nx_perf_is_applet()
                        ? "Applet - limited heap. Launch by title takeover for the full heap."
                        : "Full heap - launched by title takeover.");
                break;
            case CAT_INTERFACE:
                AddChoice("Theme", "Pearl by default; Midnight remains optional.",
                          &g_cfg.theme, kThemeName, THEME_COUNT);
                AddToggle("Always show button hints", "Keep the control bar visible on every screen.", &g_cfg.persistentHints);
                AddToggle("Reduced motion", "Stop the focused tile growing and skip screen fades.", &g_cfg.reducedMotion);
                AddChoice("Library view", "Layout used right now.", &g_cfg.view, kViewName, VIEW_COUNT);
                break;
            case CAT_ACCESS:
                AddSlider("Text size", "Scale all interface text.", &g_cfg.textScale, 100, 150, 10);
                AddToggle("High contrast", "Push body text to full white against the panels.", &g_cfg.highContrast);
                AddToggle("Colour-blind focus mark", "Add a caret badge to the focused tile.", &g_cfg.colourBlindFocus);
                AddToggle("Reduced motion", "Stop the focused tile growing and skip screen fades.", &g_cfg.reducedMotion);
                AddToggle("Navigation sounds", "Play a short tone when moving around.", &g_cfg.navSounds);
                AddChoice("Vibration", "How strongly the pad responds to the interface.", &g_cfg.rumble, kRumbleName, 3);
                AddToggle("Hold to confirm", "Reset and Exit need the button held down.", &g_cfg.holdToConfirm);
                break;
            case CAT_ABOUT:
                AddText("MAME NX Reborn 2003 Plus",
                        "MAME2003-Plus core on Nintendo Switch, standalone.");
                AddText("Built on", "mame-nx by MVG, and the MAME2003-Plus core.");
                AddText("Core", "MAME 0.78 derived driver set, no RetroArch involved.");
                AddText("Data folder", kDataDir);
                {
                    /* Sampled about once a second while a game runs, so on this
                       screen it is the reading from the last session. Shown
                       because a governor you cannot observe is a governor you
                       cannot trust. */
                    static char temp[64];
                    int c = nx_perf_soc_temp();

                    if (c < 0) snprintf(temp, sizeof(temp), "Not available");
                    else snprintf(temp, sizeof(temp), "%d C%s", c,
                                  nx_perf_thermal_limited() ? "  -  boost held back" : "");
                    AddText("SoC temperature", temp);
                }
                break;
        }
        if (g_setRow >= g_rowCount) g_setRow = g_rowCount ? g_rowCount - 1 : 0;
    }

    /* Row height follows the text rather than being a constant. The Text size
       slider goes to 150%, and with a fixed 56px pitch the label and its help
       line ran into the row below well before that - an accessibility control
       that breaks the layout when you use it is not one.

       Nothing here varies with dock state on purpose. The framebuffer is a
       fixed 1280x720 scaled to whichever output, so a docked pixel and a
       handheld pixel are the same fraction of the screen; what differs is
       viewing distance, and a TV at 2-3m subtends about what the panel does at
       arm's length. Sizing per mode would make the UI jump when you dock and
       fix nothing. The fix is a bigger floor in both. */
    static int SetRowPitch() { return (int)TS(20) + (int)TS(15) + 29; }
    static int SetRowCard()  { return SetRowPitch() - 4; }

    static int g_setRowTop = 0;

    /* Panel geometry, named once. The input handler needs the same visible
       count the draw uses; passing the numbers in by hand is how the two end
       up disagreeing after someone resizes the panel. */
    enum {
        SET_PX = 40, SET_PY = 60, SET_PW = 1200, SET_PH = 612,
        SET_SBW = 248, SET_HEADER = 88
    };

    static int SetRowsVisible()
    {
        int room = (SET_PY + SET_PH - 24) - (SET_PY + SET_HEADER);
        int n = room / SetRowPitch();
        return n < 1 ? 1 : n;
    }

    static void DrawSettings()
    {
        int px = SET_PX, py = SET_PY, pw = SET_PW, ph = SET_PH;
        int sbw = SET_SBW;
        int ry = py + SET_HEADER;
        int pitch = SetRowPitch();
        int vis = SetRowsVisible();
        int rx = px + sbw + 26, rw = pw - sbw - 56;
        int scroll, nudge;
        FRect ring;
        Gfx::DrawState saved, clipped;

        DrawBackdrop();
        DrawClock();
        DrawBattery();

        Gfx::drawRectangle(0, 0, 1280, 720, kScrim);

        saved = PageBegin();
        Glass(px, py, pw, ph, 26, kPanel, kPanelBot, kEdge);

        /* Sidebar and rows are one list as far as focus is concerned - the
           cursor crosses between them with Left and Right - so they share a
           key and the ring travels across the divider instead of jumping it. */
        scroll = ScrollTrack(50, g_setRowTop * pitch);
        nudge  = NudgeStep();
        ring   = g_setInSidebar
                     ? FocusTrack(50, px + 14, py + 22 + g_setCat * 54, sbw - 28, 46)
                     : FocusTrack(50, rx - 10, ry - 6 + g_setRow * pitch - scroll,
                                  rw, SetRowCard());

        /* sidebar */
        for (int i = 0; i < SETCAT_COUNT; i++)
        {
            int iy = py + 22 + i * 54;
            bool sel = (i == g_setCat);
            float f = FocusWeight(px + 14, iy, sbw - 28, 54);

            if (sel)
            {
                GlassFlat(px + 14, iy, sbw - 28, 46, 14,
                          Mix(kCard, kAccentWash, f), Mix(kCardBot, kAccentDim, f),
                          f > 0.5f ? kEdge : kEdgeSoft);
                /* left marker so the active category is not colour-only */
                Gfx::drawRoundedRectangle(px + 14, iy + 10, 5, 26, 2, kAccent);
            }
            Gfx::drawText(px + 32, iy + 23 - (int)TS(20) / 2, kSetCat[i],
                          sel ? Ink(kText) : Ink(kDim), TS(20));
        }

        Gfx::drawRectangle(px + sbw, py + 20, 1, ph - 40, kEdgeSoft);

        /* Right pane needs identity before controls. Previously category name
           lived only in sidebar, leaving sparse categories looking like an
           unfinished blank panel. */
        Gfx::drawText(rx, py + 24, kSetCat[g_setCat], Ink(kText), TS(25));
        Gfx::drawText(rx, py + 57, kSetCatHelp[g_setCat], Ink(kDim), TS(15));
        Gfx::drawRectangle(rx, py + SET_HEADER - 10, rw, 1, kEdgeSoft);

        /* rows */
        clipped = Gfx::pushState();
        Gfx::clipTo(px + sbw + 4, py + 12, pw - sbw - 8, ph - 30);

        for (int i = g_setRowTop; i < g_rowCount && i < g_setRowTop + vis + 1; i++)
        {
            SetRow &r = g_rows[i];
            int y = ry + i * pitch - scroll;
            float f = g_setInSidebar ? 0.0f
                                     : FocusWeight(rx - 10, y - 6, rw, pitch);
            bool focused = (f > 0.5f);
            int mid = y - 6 + SetRowCard() / 2;

            /* Faded in rather than mixed towards kCard: an unfocused settings
               row has never had a card behind it, and giving it one at f=0
               would be a layout change wearing an animation's clothes. Lifted
               onto the same rect as the ring below, so the two share an edge. */
            if (f > 0.02f)
            {
                FRect p = LiftedRect(rx - 10, y - 6, rw, SetRowCard(), f);
                Gfx::drawRoundedRectangle(p.x, p.y, p.w, p.h, 12, Faded(kCardSel, f));
            }

            Gfx::drawText(rx, y, r.label, Ink(kText), TS(20));
            if (r.help)
                Gfx::drawTextClipped(rx, y + (int)TS(20) + 6, r.help, Ink(kDim), TS(15), rw - 280);

            switch (r.kind)
            {
                case ROW_TOGGLE:
                    Toggle(rx + rw - 90, mid - 16, *(bool *)r.value);
                    break;
                /* The recoil rides on the value only, never on the row: what
                   changed is the number, and shoving the whole row would say
                   the row moved. `n` is zero on every row but the focused one,
                   which is the only one that can have just changed. */
                case ROW_SLIDER:
                {
                    int v = *(int *)r.value, n = focused ? nudge : 0;
                    char buf[24];
                    snprintf(buf, sizeof(buf), "%d%%", v);
                    Slider(rx + rw - 230, mid - 14, 130, (v - r.lo) * 100 / (r.hi - r.lo));
                    Gfx::drawText(rx + rw - 84 + n, mid - (int)TS(17) / 2, buf, Ink(kText), TS(17));
                    break;
                }
                case ROW_CHOICE:
                {
                    int v = *(int *)r.value, n = focused ? nudge : 0;
                    const char *txt = (v >= 0 && v < r.choiceCount) ? r.choices[v] : "-";
                    int w = 180, ch = (int)TS(17) + 20;
                    int cy = mid - ch / 2;
                    GlassFlat(rx + rw - w - 10, cy, w, ch, 10, kCard, kCardBot, kEdgeSoft);
                    Gfx::drawText(rx + rw - w, mid - (int)TS(16) / 2, "<", Ink(kDim), TS(16));
                    Gfx::drawTextCentered(rx + rw - w / 2 - 10 + n, mid - (int)TS(17) / 2, txt,
                                          Ink(kText), TS(17), w - 48);
                    Gfx::drawText(rx + rw - 28, mid - (int)TS(16) / 2, ">", Ink(kDim), TS(16));
                    break;
                }
                case ROW_ACTION:
                {
                    int ah = (int)TS(17) + 20;
                    bool reset = !strcmp(r.label, "Reset all settings");
                    GlassFlat(rx + rw - 140, mid - ah / 2, 130, ah, 10,
                              focused ? (reset ? Lighter(kRed, 0.55f) : kAccentWash) : kCard,
                              focused ? (reset ? Lighter(kRed, 0.28f) : kAccentDim) : kCardBot,
                              reset ? kRed : (focused ? kEdge : kEdgeSoft));
                    Gfx::drawTextCentered(rx + rw - 75, mid - (int)TS(17) / 2,
                                          reset ? "Reset" : "Open",
                                          focused ? Ink(kText) : (reset ? kRed : Ink(kDim)),
                                          TS(17), 120);
                    break;
                }
                case ROW_TEXT:
                    break;
            }
        }

        Gfx::popState(clipped);

        /* Outside the row clip: the same ring also has to be drawable over in
           the sidebar, which is not in that column. */
        {
            FRect r = Lifted(ring, 1.0f);
            FocusRing(r, g_setInSidebar ? 14 : 12, 2);
        }

        if (g_rowCount > vis)
        {
            char buf[48];
            snprintf(buf, sizeof(buf), "%d of %d", g_setRow + 1, g_rowCount);
            Gfx::drawText(px + sbw + 26, py + ph - 26, buf, Ink(kDim), TS(14));
        }

        if (s_holdAction >= 0)
        {
            Gfx::drawTextCentered(640, 640, "Keep holding A to reset every setting",
                                  Ink(kText), TS(17), 700);
            HoldBar(440, 664, 400);
        }
        else
        {
            if (g_setInSidebar)
            {
                static const char *hints[][2] = {
                    { "A", "Open" }, { "B", "Library" }
                };
                HintBar(hints, 2);
            }
            else
            {
                static const char *hints[][2] = {
                    { "A", "Change" }, { "<", "Adjust" }, { "B", "Categories" }
                };
                HintBar(hints, 3);
            }
        }

        Gfx::popState(saved);
    }

    /*=================================================================
        Controllers and players
    =================================================================*/
    static PadState s_playerPad[2];
    static bool s_playerPadUp = false;

    static void DrawPlayerPanel(int x, int y, int w, int h, int player, bool focused)
    {
        PadState *pad = &s_playerPad[player];
        bool connected = s_playerPadUp && padIsConnected(pad);
        u64 held = connected ? padGetButtons(pad) : 0;
        const char *type = "Not connected";
        char buf[96];

        if (connected) type = padIsHandheld(pad) ? "Handheld" : "Pro Controller / Joy-Con";

        Glass(x, y, w, h, 20, focused ? kCardSel : kCard,
              focused ? kCardSelBot : kCardBot, focused ? kEdge : kEdgeSoft);
        if (focused)
        {
            { FRect r; r.x = x; r.y = y; r.w = w; r.h = h; FocusRing(r, 20, 3); }
        }

        snprintf(buf, sizeof(buf), "Player %d", player + 1);
        Gfx::drawText(x + 26, y + 22, buf, Ink(kText), TS(24));

        /* Ready state carries an icon and a word, not just a colour. */
        Chip(x + w - 130, y + 26, connected ? "READY" : "NOT READY",
             connected ? kGreen : kRed, kInk);

        Gfx::drawText(x + 26, y + 74, "Controller", Ink(kDim), TS(15));
        Gfx::drawTextClipped(x + 160, y + 74, type, Ink(kText), TS(15), w - 190);

        Gfx::drawText(x + 26, y + 106, "Assigned", Ink(kDim), TS(15));
        snprintf(buf, sizeof(buf), "Pad %d", player + 1);
        Gfx::drawText(x + 160, y + 106, connected ? buf : "-", Ink(kText), TS(15));

        Gfx::drawText(x + 26, y + 138, "Battery", Ink(kDim), TS(15));
        Gfx::drawText(x + 160, y + 138, "Reported by the system", Ink(kDim), TS(15));

        /* Live input test - press anything and the matching pip lights. */
        Gfx::drawText(x + 26, y + 178, "Input test", Ink(kDim), TS(15));
        {
            static const struct { const char *n; u64 m; } bits[] = {
                { "A", HidNpadButton_A }, { "B", HidNpadButton_B },
                { "X", HidNpadButton_X }, { "Y", HidNpadButton_Y },
                { "L", HidNpadButton_L }, { "R", HidNpadButton_R },
                { "ZL", HidNpadButton_ZL }, { "ZR", HidNpadButton_ZR },
                { "-", HidNpadButton_Minus }, { "+", HidNpadButton_Plus }
            };
            for (unsigned i = 0; i < sizeof(bits) / sizeof(bits[0]); i++)
            {
                int bx = x + 26 + (int)i * 44;
                bool on = (held & bits[i].m) != 0;
                GlassFlat(bx, y + 204, 38, 38, 10,
                          on ? kAccentWash : kCard, on ? kAccentDim : kCardBot,
                          on ? kEdge : kEdgeSoft);
                Gfx::drawTextCentered(bx + 19, y + 214, bits[i].n,
                                      on ? Ink(kText) : Ink(kDim), TS(15), 36);
            }
        }
    }

    static void DrawControllers()
    {
        DrawBackdrop();
        DrawClock();
        DrawBattery();

        Gfx::drawRectangle(0, 0, 1280, 720, kScrim);
        Glass(40, 60, 1200, 612, 26, kPanel, kPanelBot, kEdge);

        Gfx::drawText(72, 84, "Controllers and players", Ink(kText), TS(26));
        Gfx::drawTextClipped(72, 118, "Player 1 uses the handheld or the first paired controller. "
                                      "Connect a second to enable two-player games.",
                             Ink(kDim), TS(15), 1140);

        DrawPlayerPanel(72, 160, 552, 280, 0, g_setRow == 0);
        DrawPlayerPanel(656, 160, 552, 280, 1, g_setRow == 1);

        /* two-player availability, spelled out */
        {
            bool p2 = s_playerPadUp && padIsConnected(&s_playerPad[1]);
            const struct GameDriver *drv = DriverFor(currentGame);
            int players = PlayerCount(drv);
            char buf[160];

            GlassFlat(72, 464, 1136, 90, 18, kCard, kCardBot, kEdgeSoft);

            if (!p2)
                snprintf(buf, sizeof(buf), "Two-player mode is unavailable - no second controller is connected.");
            else if (players >= 2)
                snprintf(buf, sizeof(buf), "Two-player mode is ready. %s supports %d players.",
                         currentGame.c_str(), players);
            else
                snprintf(buf, sizeof(buf), "Second controller connected, but %s is a one-player game.",
                         currentGame.empty() ? "the selected game" : currentGame.c_str());

            Chip(96, 486, p2 ? "2P READY" : "1P ONLY", p2 ? kGreen : kAmber, kInk);
            Gfx::drawTextClipped(96, 520, buf, Ink(kText), TS(16), 1090);
        }

        {
            static const char *hints[][2] = { { "D", "Switch player" }, { "B", "Back" } };
            HintBar(hints, 2);
        }
    }

    /*=================================================================
        In-game overlay
    =================================================================*/
    /* One overlay, several pages, one renderer. The rows used to hand the
       player straight over to MAME's own menu (options.display_setup) for
       anything deeper than Resume, which is why the pause flow looked like two
       different programs the moment you opened a submenu: MAME's pages are
       drawn by usrintrf.c's bitmap-font renderer, with its own focus style and
       its own idea of which button goes back. Every page below is drawn by
       OverlayFrame()/OverlayRow() instead, so depth changes the contents and
       nothing else. */
    /* Save and Load are two pages rather than one page with a mode, so each
       keeps its own cursor - the slot you last saved to and the slot you last
       loaded from are rarely the same one. */
    enum { OVP_MAIN = 0, OVP_DIPS, OVP_CHEAT, OVP_INPUT_GAME, OVP_INPUT_GEN,
           OVP_ANALOG, OVP_MEMCARD, OVP_HISTORY, OVP_STATES, OVP_SAVE, OVP_LOAD,
           OVP_PAGES };

    /* Row metrics. Pitch is deliberately larger than the row so the cards read
       as separate objects rather than one striped block, and the gap before
       the destructive group is larger still. */
    enum { kRowsVisible = 8, kOvRowH = 50, kOvRowPitch = 60, kOvCarefulGap = 34 };

    enum { OVA_RESUME = 0, OVA_STATES,
           OVA_DIPS, OVA_CHEAT, OVA_INPUT_GAME, OVA_INPUT_GEN,
           OVA_ANALOG, OVA_MEMCARD, OVA_HISTORY, OVA_ARTWORK,
           OVA_RESET, OVA_LIBRARY, OVA_EXIT };

    struct OvRow
    {
        int         action;
        const char *label;
        const char *help;
    };

    static OvRow g_ovRows[20];
    static int   g_ovRowCount = 0;
    static int   g_ovPage     = OVP_MAIN;

    /* What Draw() last painted, so a page change can be noticed in one place
       rather than at each of the dozen sites that assign g_ovPage. */
    static int   s_prevOvPage = OVP_MAIN;

    /* Per-page cursor and window top. Kept separately so backing out of a
       submenu and going in again lands where you left, which matters most on
       the long lists (a driver can declare a hundred cheats). */
    static int   g_ovSelOf[OVP_PAGES] = { 0 };
    static int   g_ovTopOf[OVP_PAGES] = { 0 };

    /* Index currently being rebound, or -1. While this is set the overlay
       ignores its own buttons - the press being recorded would otherwise drive
       the menu at the same time as landing in the sequence. */
    static int   g_ovCapture = -1;

    static int   g_ovMemSlot = 0;

    /* Wrapped history text, built once when the page opens rather than per
       frame - it is a few hundred lines of measuring. */
    static std::vector<std::string> g_ovHistory;

    /* Alias so the main page keeps reading naturally. */
    #define g_ovSel g_ovSelOf[OVP_MAIN]

    /* Artwork carries its state in its own label - the row list has no value
       column, and a second column for one boolean is not worth the width. */
    static char g_ovArtworkLabel[32];

    static bool OverlayDestructive(int a) { return a == OVA_RESET || a == OVA_EXIT; }

    /* history.dat is parsed off the SD card, so it is read once per launched
       game and held. Empty string means the file or the entry is missing,
       which is the normal case - it is an optional download. */
    static const char *HistoryText()
    {
        static char  text[16384];
        static char  loadedFor[64] = "";

        if (strncmp(loadedFor, currentGame.c_str(), sizeof(loadedFor) - 1) != 0)
        {
            snprintf(loadedFor, sizeof(loadedFor), "%s", currentGame.c_str());
            if (nx_history_load(text, sizeof(text)) != 0)
                text[0] = 0;
        }
        return text;
    }

    /* Rebuilt on open rather than held static: whether the Dip Switches row
       exists at all depends on the driver that is running. Mirrors what
       setup_menu_init() does upstream, for the same reason. */
    static void BuildOverlayRows()
    {
        int n = 0;

        g_ovRows[n].action = OVA_RESUME;
        g_ovRows[n].label  = "Resume Game";
        g_ovRows[n++].help = "Go back to the game";

        /* Directly under Resume because state management is one of the most
           common reasons to open the pause menu.

           Always present, even where the driver cannot serialize. Hiding them
           was the first version and it was wrong: a row that is absent because
           the game does not support it looks exactly like a row that was never
           written, and the player has no way to tell those apart. The page
           behind the row says which. */
        g_ovRows[n].action = OVA_STATES;
        g_ovRows[n].label  = "States";
        g_ovRows[n++].help = "Save or return to a moment";

        if (nx_dip_count() > 0)
        {
            g_ovRows[n].action = OVA_DIPS;
            g_ovRows[n].label  = "Cabinet Settings";
            g_ovRows[n++].help = "Dip switches the operator would set";
        }

        /* Each of these appears only when the running driver actually has one,
           the same conditions setup_menu_init() applies. An empty page is
           worse than a missing row: it looks broken rather than absent. */
        g_ovRows[n].action = OVA_INPUT_GAME;
        g_ovRows[n].label  = "Controls for This Game";
        g_ovRows[n++].help = "Rebind buttons for this romset only";

        g_ovRows[n].action = OVA_INPUT_GEN;
        g_ovRows[n].label  = "Controls for All Games";
        g_ovRows[n++].help = "The defaults every driver inherits";

        if (nx_analog_count() > 0)
        {
            g_ovRows[n].action = OVA_ANALOG;
            g_ovRows[n].label  = "Analog Controls";
            g_ovRows[n++].help = "Speed, sensitivity and direction";
        }

        if (nx_cheat_count() > 0)
        {
            g_ovRows[n].action = OVA_CHEAT;
            g_ovRows[n].label  = "Cheats";
            g_ovRows[n++].help = "From cheat.dat, for this romset";
        }

        if (HistoryText()[0])
        {
            g_ovRows[n].action = OVA_HISTORY;
            g_ovRows[n].label  = "Game History";
            g_ovRows[n++].help = "What this cabinet was, from history.dat";
        }

        if (nx_memcard_available())
        {
            g_ovRows[n].action = OVA_MEMCARD;
            g_ovRows[n].label  = "Memory Card";
            g_ovRows[n++].help = "Neo Geo save cartridge";
        }

        snprintf(g_ovArtworkLabel, sizeof(g_ovArtworkLabel), "Artwork: %s",
                 options.use_artwork == ARTWORK_USE_NONE ? "Off" : "On");
        g_ovRows[n].action = OVA_ARTWORK;
        g_ovRows[n].label  = g_ovArtworkLabel;
        g_ovRows[n++].help = "Bezels and backdrops - costs frame time";

        g_ovRows[n].action = OVA_RESET;
        g_ovRows[n].label  = "Reset Game";
        g_ovRows[n++].help = "Restart from the arcade boot";

        g_ovRows[n].action = OVA_LIBRARY;
        g_ovRows[n].label  = "Return to Game Library";
        g_ovRows[n++].help = "Close the game and go back";

        g_ovRows[n].action = OVA_EXIT;
        g_ovRows[n].label  = "Exit Application";
        g_ovRows[n++].help = "Close MAME NX Reborn completely";

        g_ovRowCount = n;
        if (g_ovSel >= n) g_ovSel = n - 1;
    }

    /* Shared geometry. Every overlay page is the same panel in the same place,
       so moving between them never moves the furniture. */
    enum { OVX = 110, OVY = 66, OVW = 1060, OVH = 588, OVSIDE = 330 };

    static int OvListX() { return OVX + OVSIDE + 28; }
    static int OvListW() { return OVW - OVSIDE - 56; }

    /* Scrim, panel and the "what you paused" pane, plus the page's name. Drawn
       over the last presented game frame - menu_flush_overlay() draws the game
       quad first and blends this on top, so the scrim's alpha is what dims the
       game rather than hiding it. */
    static void OverlayFrame(const char *section)
    {
        const struct GameDriver *drv = DriverFor(currentGame);
        Gfx::Image *art = ArtFor(drv);

        /* The scrim is pale now, so the paused game shows through as a wash
           rather than a shadow - the panel has to carry its own weight with a
           shadow of its own instead of relying on a dark field behind it. */
        Gfx::clear(kScrim);
        Glass(OVX, OVY, OVW, OVH, 26, kPanel, kPanelBot, kEdge);

        if (art) Gfx::drawImage(*art, OVX + 30, OVY + 30, 270, 270, 255, true);
        else     GlassFlat(OVX + 30, OVY + 30, 270, 270, 16,
                           Lighter(PlateColour(currentGame), 0.22f),
                           PlateColour(currentGame), kEdgeSoft);
        Gfx::drawRoundedOutlineRectangle(OVX + 30, OVY + 30, 270, 270, 16, 1, kEdge);

        Chip(OVX + 30, OVY + 314, section, kAmber, kInk);
        Gfx::drawTextClipped(OVX + 30, OVY + 350, currentGame, Ink(kText), TS(21), OVSIDE - 40);

        {
            int iy = OVY + 392;
            char buf[96];

            if (drv && drv->year && drv->manufacturer)
            {
                snprintf(buf, sizeof(buf), "%s  -  %s", drv->year, drv->manufacturer);
                Gfx::drawTextClipped(OVX + 30, iy, buf, Ink(kDim), TS(15), OVSIDE - 40);
                iy += 28;
            }
            if (drv)
            {
                snprintf(buf, sizeof(buf), "%d player%s  -  %s tier",
                         PlayerCount(drv), PlayerCount(drv) == 1 ? "" : "s",
                         nx_perf_tier_name(nx_perf_driver_tier(drv)));
                Gfx::drawTextClipped(OVX + 30, iy, buf, Ink(kDim), TS(15), OVSIDE - 40);
                iy += 28;
            }
            snprintf(buf, sizeof(buf), "Played %d time%s",
                     StatFor(currentGame).plays, StatFor(currentGame).plays == 1 ? "" : "s");
            Gfx::drawText(OVX + 30, iy, buf, Ink(kDim), TS(15));
        }

        Gfx::drawRectangle(OVX + OVSIDE, OVY + 26, 1, OVH - 52, kEdgeSoft);
    }

    /* The single row treatment for the whole overlay. `value`, when present,
       is right-aligned in the same row - that is how a dip reads its setting
       without needing a page layout of its own. */
    /* `f` is how much of the travelling ring is over this row, 0..1. The ring
       and caret themselves are drawn once per page at the chased position, so
       what is left here is the part that genuinely belongs to the row. */
    static void OverlayRow(int y, const char *label, const char *help,
                           const char *value, float f, bool bad, int nudge)
    {
        const int x = OvListX(), w = OvListW();
        bool sel = (f > 0.5f);
        /* Exactly the rect OverlayRing will draw on, so the plate reaches the
           ring instead of stopping short of it. */
        FRect p = LiftedRect(x, y, w, kOvRowH, f);

        GlassFlat(p.x, p.y, p.w, p.h, 12,
                  Mix(kCard, bad ? Lighter(kRed, 0.55f) : kCardSel, f),
                  Mix(kCardBot, bad ? Lighter(kRed, 0.30f) : kCardSelBot, f),
                  f > 0.5f ? kEdge : kEdgeSoft);

        if (bad && !sel)
            Gfx::drawRoundedOutlineRectangle(p.x, p.y, p.w, p.h, 12, 2, kRed);

        /* A value needs room, so the label gives up half the row when one is
           present rather than the two overlapping at long driver names. */
        int labelw = value ? (w - 60) / 2 : (w - 60);
        Gfx::RGBA ink = Mix(Ink(kDim), Ink(kText), f);

        Gfx::drawTextClipped(x + 40, y + (help ? 8 : kOvRowH / 2 - (int)TS(18) / 2), label,
                             ink, TS(18), labelw);

        if (help)
            Gfx::drawTextClipped(x + 40, y + 29, help, ink, TS(14), w - 60);

        if (value)
        {
            /* Arrows only on the focused row: they say "this one is editable
               right now", which is not true of the rest of the list. The
               recoil is on the value alone - the row it sits in did not
               change, only the number did. */
            int vw = (int)Gfx::textWidth(value, TS(17));
            int vx = x + w - 20 - vw + (sel ? nudge : 0);
            int vy = y + kOvRowH / 2 - (int)TS(17) / 2;

            if (sel)
            {
                Gfx::drawText(vx - 22, vy, "<", kAccentInk, TS(17));
                Gfx::drawText(x + w - 16, vy, ">", kAccentInk, TS(17));
            }
            Gfx::drawText(vx, vy, value, Mix(Ink(kDim), kAccentInk, f), TS(17));
        }
    }

    /* The one focus ring for an overlay page. Same curve as the row card, so
       it sits on the card instead of cutting its corners, and lifted by the
       same spring the rest of the UI grows on. */
    static void OverlayRing(FRect r, bool bad)
    {
        FRect g = Lifted(r, 1.0f);
        Gfx::RGBA c = bad ? kRed : kAccent;
        Gfx::RGBA caret = bad ? kRed : kAccentInk;

        if (bad)
        {
            /* Destructive rows keep red for the ring, but the same three-line
               treatment - the material should not change just because the
               meaning did. */
            Gfx::drawRoundedOutlineRectangle(g.x - 1, g.y - 1, g.w + 2, g.h + 2, 13, 1, kEdge);
            Gfx::drawRoundedOutlineRectangle(g.x, g.y, g.w, g.h, 12, 2, c);
        }
        else FocusRing(g, 12, 2);

        Gfx::drawText(g.x + 14, g.y + g.h / 2 - (int)TS(18) / 2, ">", caret, TS(18));
    }

    /* Position readout lives in the left pane, under the game facts. On the
       main page eight rows plus the Careful gap reach y+596, which is past
       where a readout at the foot of the list column would sit - and the left
       pane has the room going spare. */
    static void OverlayPosition(int sel, int total)
    {
        char buf[48];
        snprintf(buf, sizeof(buf), "%d of %d", sel + 1, total);
        Gfx::drawText(OVX + 30, OVY + OVH - 60, buf, Ink(kDim), TS(15));
    }

    /* Where a row of the main page sits before the window is subtracted. Safe
       actions run down the pane; the destructive ones are parked below a rule,
       so neither Reset nor Exit sits where muscle memory expects Resume. The
       gap belongs to every row from the boundary down, not just the first -
       shifting only the boundary row pushed Reset into Return to Game Library
       and drew the two on top of each other. */
    static int OvMainRowY(int index, int firstBad)
    {
        return index * kOvRowPitch +
               (firstBad >= 0 && index >= firstBad ? kOvCarefulGap : 0);
    }

    /* The list column, clipped so a row caught mid-scroll is cut off at the
       panel rather than drawn over the furniture around it. */
    static void OvClipList()
    {
        Gfx::clipTo(OvListX() - 12, OVY + 22, OvListW() + 24, OVH - 48);
    }

    static void DrawOverlayMain()
    {
        int firstBad = -1;
        int top = g_ovTopOf[OVP_MAIN];
        int base = OVY + 26, scroll;
        FRect ring;
        Gfx::DrawState saved, clipped;

        OverlayFrame("PAUSED");
        saved = PageBegin();

        for (int i = 0; i < g_ovRowCount; i++)
            if (OverlayDestructive(g_ovRows[i].action)) { firstBad = i; break; }

        /* The window still steps by whole rows - every index calculation
           depends on that - and only the drawn offset is smoothed. */
        scroll = ScrollTrack(OVP_MAIN, OvMainRowY(top, firstBad));
        ring   = FocusTrack(100 + OVP_MAIN, OvListX(),
                            base + OvMainRowY(g_ovSel, firstBad) - scroll,
                            OvListW(), kOvRowH);

        clipped = Gfx::pushState();
        OvClipList();

        /* One row past the window: while the list is mid-scroll the row
           arriving is partly on screen, and the clip is what stops it. */
        for (int index = top; index < g_ovRowCount && index < top + kRowsVisible + 1; index++)
        {
            int y = base + OvMainRowY(index, firstBad) - scroll;

            if (index == firstBad)
            {
                Gfx::drawRectangle(OvListX(), y - 22, OvListW(), 2, kPanelSoft);
                Gfx::drawText(OvListX(), y - 18, "Careful", Ink(kDim), TS(12));
            }

            OverlayRow(y, g_ovRows[index].label, g_ovRows[index].help, 0,
                       FocusWeight(OvListX(), y, OvListW(), kOvRowPitch),
                       OverlayDestructive(g_ovRows[index].action), 0);
            Hit(OvListX(), y, OvListW(), kOvRowH, HIT_OVROW + index);
        }

        OverlayRing(ring, OverlayDestructive(g_ovRows[g_ovSel].action));
        Gfx::popState(clipped);

        if (g_ovRowCount > kRowsVisible) OverlayPosition(g_ovSel, g_ovRowCount);

        static const char *hints[][2] = {
            { "A", "Select" }, { "B", "Resume" }, { "D", "Move" }
        };
        HintBar(hints, 3);
        Gfx::popState(saved);
    }

    /* Every list page below is the same shape: a windowed list of rows, an
       optional position readout, a footnote and a hint bar. Only what fills the
       label and value columns differs, so that is all each page supplies. */
    static void ListPage(const char *section, int total,
                         const char *(*label)(int),
                         const char *(*value)(int),
                         const char *empty, const char *footnote,
                         const char *hints[][2], int hintCount)
    {
        int &sel = g_ovSelOf[g_ovPage];
        int &top = g_ovTopOf[g_ovPage];
        int base = OVY + 26, scroll, nudge;
        FRect ring;
        Gfx::DrawState saved, clipped;

        OverlayFrame(section);
        saved = PageBegin();

        /* Wrapped, not clipped: an empty page's only job is to explain itself,
           and the explanations that are worth reading are longer than one
           line - ellipsising them cuts off exactly the part that answers the
           question. */
        if (total <= 0)
            Gfx::drawTextWrapped(OvListX(), OVY + 40, empty, Ink(kDim), TS(17),
                                 OvListW(), 4);

        scroll = ScrollTrack(g_ovPage, top * kOvRowPitch);
        ring   = FocusTrack(100 + g_ovPage, OvListX(),
                            base + sel * kOvRowPitch - scroll, OvListW(), kOvRowH);
        nudge  = NudgeStep();

        clipped = Gfx::pushState();
        OvClipList();

        for (int index = top; index < total && index < top + kRowsVisible + 1; index++)
        {
            int y = base + index * kOvRowPitch - scroll;

            OverlayRow(y, label(index), 0, value ? value(index) : 0,
                       FocusWeight(OvListX(), y, OvListW(), kOvRowPitch), false, nudge);
            Hit(OvListX(), y, OvListW(), kOvRowH, HIT_OVROW + index);
        }

        if (total > 0) OverlayRing(ring, false);
        Gfx::popState(clipped);

        /* Position readout rather than a scrollbar: these lists are short
           enough that "3 of 11" answers the question a bar would. */
        if (total > kRowsVisible) OverlayPosition(sel, total);

        if (footnote)
            Gfx::drawTextClipped(OvListX(), OVY + OVH - 54, footnote,
                                 Ink(kDim), TS(15), OvListW());

        HintBar(hints, hintCount);
        Gfx::popState(saved);
    }

    /* Thunks: ListPage takes plain function pointers so each page stays a few
       lines, and the general/this-game input lists differ only by a flag. */
    static const char *DipLabel(int i)   { return nx_dip_name(i); }
    static const char *DipValue(int i)   { return nx_dip_value(i); }
    static const char *CheatLabel(int i) { return nx_cheat_name(i); }
    static const char *CheatValue(int i) { return nx_cheat_value(i); }
    static const char *InGameLabel(int i){ return nx_input_name(i, 0); }
    static const char *InGameValue(int i){ return nx_input_binding(i, 0); }
    static const char *InGenLabel(int i) { return nx_input_name(i, 1); }
    static const char *InGenValue(int i) { return nx_input_binding(i, 1); }

    /* Analog ports expose three fields each, so the flat row index splits into
       a port and a field rather than mapping one-to-one. */
    static const char *AnalogLabel(int i)
    {
        static char buf[128];
        snprintf(buf, sizeof(buf), "%s  %s",
                 nx_analog_name(i / NX_ANALOG_FIELDS),
                 nx_analog_field_name(i % NX_ANALOG_FIELDS));
        return buf;
    }
    static const char *AnalogValue(int i)
    {
        return nx_analog_field_value(i / NX_ANALOG_FIELDS, i % NX_ANALOG_FIELDS);
    }

    /* Both slot pages list the same ten slots the same way; only what A does
       differs, and that is decided by the page rather than by the rows. */
    static const char *SlotLabel(int i)
    {
        static char buf[16];
        snprintf(buf, sizeof(buf), "Slot %d", i + 1);
        return buf;
    }
    static const char *SlotValue(int i) { return nx_state_slot_info(i); }

    static const char *StatesLabel(int i)
    {
        static const char *labels[] = { "Save State", "Load State" };
        return labels[i];
    }
    static const char *StatesValue(int) { return NULL; }

    /* Footnote of the slot pages. Holds the standing explanation until a save
       or load has something to report, then holds that - the pause overlay has
       no other place for a result, and MAME's usrintf_showmessage() goes to a
       renderer this frontend never runs. */
    static char g_ovStateNote[192];

    /* Reason the slot list is empty, when it is. Held in a buffer because
       ListPage takes a plain pointer and nx_state_error() is overwritten by
       the next thing that fails. */
    static char g_ovStateWhy[192];

    static void StateNoteDefault(bool saving)
    {
        if (!nx_state_available())
        {
            snprintf(g_ovStateWhy, sizeof(g_ovStateWhy), "%s", nx_state_error());
            g_ovStateNote[0] = 0;
            return;
        }

        g_ovStateWhy[0] = 0;
        snprintf(g_ovStateNote, sizeof(g_ovStateNote),
                 "%s   One state is %lu KB.",
                 saving ? "A slot already in use is written over."
                        : "A state only loads back into the game it came from.",
                 (unsigned long)(nx_state_size() + 1023) / 1024);
    }

    /* Zero rows when the driver cannot serialize, which is what puts the
       explanation on screen in place of ten slots that would all fail. */
    static int StateSlotCount() { return nx_state_available() ? NX_STATE_SLOTS : 0; }

    static const char *kMemcardRow[] = {
        "Slot", "Insert Card", "Save Card", "Eject Card", "Create Card"
    };
    enum { MEM_SLOT = 0, MEM_LOAD, MEM_SAVE, MEM_EJECT, MEM_CREATE, MEM_COUNT };

    static const char *MemLabel(int i) { return kMemcardRow[i]; }
    static const char *MemValue(int i)
    {
        static char buf[32];
        if (i == MEM_SLOT) { snprintf(buf, sizeof(buf), "%d", g_ovMemSlot); return buf; }
        if (i == MEM_LOAD) return nx_memcard_inserted() ? "Inserted" : "Empty";
        return 0;
    }

    static void DrawOverlayCapture()
    {
        OverlayFrame("REBIND");

        Gfx::drawTextClipped(OvListX(), OVY + 60,
                             nx_input_name(g_ovCapture,
                                           g_ovPage == OVP_INPUT_GEN),
                             Ink(kText), TS(22), OvListW());

        Gfx::drawText(OvListX(), OVY + 110, "Press the buttons you want.",
                      kAccentInk, TS(17));
        Gfx::drawTextClipped(OvListX(), OVY + 146,
                             "Hold several together for a combination. "
                             "Stop pressing and it saves on its own.",
                             Ink(kDim), TS(14), OvListW());

        Gfx::drawTextClipped(OvListX(), OVY + OVH - 54,
                             "Waiting for input...", kAmber, TS(15), OvListW());

        static const char *hints[][2] = { { "-", "Clear this binding" } };
        HintBar(hints, 1);
    }

    /* Greedy word wrap against the real measured font, done once when the page
       opens - history.dat entries run to a few hundred lines, and measuring
       them every frame would cost more than drawing them. */
    static void WrapHistory()
    {
        const char *text  = HistoryText();
        const int   width = OvListW();
        std::string line;

        g_ovHistory.clear();
        g_ovTopOf[OVP_HISTORY] = 0;

        while (*text)
        {
            const char *end = text;
            while (*end && *end != '\n' && *end != '\r') end++;

            std::string paragraph(text, end - text);
            text = end;
            while (*text == '\n' || *text == '\r') text++;

            if (paragraph.empty()) { g_ovHistory.push_back(""); continue; }

            line.clear();
            size_t pos = 0;
            while (pos < paragraph.size())
            {
                size_t space = paragraph.find(' ', pos);
                if (space == std::string::npos) space = paragraph.size();

                std::string word = paragraph.substr(pos, space - pos);
                std::string test = line.empty() ? word : line + " " + word;

                if (!line.empty() && Gfx::textWidth(test, TS(15)) > width)
                {
                    g_ovHistory.push_back(line);
                    line = word;
                }
                else line = test;

                pos = space + 1;
            }
            if (!line.empty()) g_ovHistory.push_back(line);
        }
    }

    static void DrawOverlayHistory()
    {
        int &top = g_ovTopOf[OVP_HISTORY];
        int visible = 15;
        int lines = (int)g_ovHistory.size();
        int scroll;
        Gfx::DrawState saved, clipped;

        OverlayFrame("HISTORY");
        saved = PageBegin();

        /* Prose scrolls a line at a time, which is the one place a hard jump
           is genuinely disorienting - the eye is mid-sentence. */
        scroll = ScrollTrack(OVP_HISTORY, top * 34);

        clipped = Gfx::pushState();
        OvClipList();
        for (int i = top; i < lines && i < top + visible + 1; i++)
            Gfx::drawText(OvListX(), OVY + 26 + i * 34 - scroll,
                          g_ovHistory[i].c_str(), Ink(kText), TS(15));
        Gfx::popState(clipped);

        if (lines > visible)
        {
            char buf[48];
            snprintf(buf, sizeof(buf), "line %d of %d", top + 1, lines);
            Gfx::drawText(OvListX(), OVY + OVH - 54, buf, Ink(kDim), TS(13));
        }

        static const char *hints[][2] = { { "D", "Scroll" }, { "B", "Back" } };
        HintBar(hints, 2);
        Gfx::popState(saved);
    }

    static void DrawOverlay()
    {
        static const char *change[][2] = {
            { "L/R", "Change" }, { "B", "Back" }, { "D", "Move" }
        };
        static const char *rebind[][2] = {
            { "A", "Rebind" }, { "-", "Clear" }, { "B", "Back" }
        };
        static const char *act[][2] = {
            { "A", "Do it" }, { "L/R", "Slot" }, { "B", "Back" }
        };
        static const char *slot[][2] = {
            { "A", "Use this slot" }, { "B", "Back" }
        };
        static const char *choose[][2] = {
            { "A", "Choose" }, { "B", "Back" }
        };

        if (g_ovCapture >= 0) { DrawOverlayCapture(); return; }

        switch (g_ovPage)
        {
        case OVP_DIPS:
            ListPage("CABINET", nx_dip_count(), DipLabel, DipValue,
                     "This cabinet has no dip switches.",
                     "Changes apply immediately. Some need a reset to take hold.",
                     change, 3);
            break;

        case OVP_CHEAT:
            ListPage("CHEATS", nx_cheat_count(), CheatLabel, CheatValue,
                     "No cheats for this romset.",
                     "Cheats change the game's memory. Some break a running game.",
                     change, 3);
            break;

        case OVP_INPUT_GAME:
            ListPage("CONTROLS", nx_input_count(0), InGameLabel, InGameValue,
                     "This game declares no remappable controls.",
                     "Applies to this romset only.",
                     rebind, 3);
            break;

        case OVP_INPUT_GEN:
            ListPage("DEFAULTS", nx_input_count(1), InGenLabel, InGenValue,
                     "No default controls to remap.",
                     "Applies to every game that has no override of its own.",
                     rebind, 3);
            break;

        case OVP_ANALOG:
            ListPage("ANALOG", nx_analog_count() * NX_ANALOG_FIELDS,
                     AnalogLabel, AnalogValue,
                     "This game has no analog controls.",
                     "Digital speed is how fast a d-pad sweeps an analog axis.",
                     change, 3);
            break;

        case OVP_MEMCARD:
            ListPage("MEMORY CARD", MEM_COUNT, MemLabel, MemValue,
                     "", "Neo Geo cards are files under the mame-nx folder.",
                     act, 3);
            break;

        case OVP_STATES:
            ListPage("STATES", 2, StatesLabel, StatesValue,
                     "", "Save progress or return to a saved moment.",
                     choose, 2);
            break;

        case OVP_SAVE:
            ListPage("SAVE STATE", StateSlotCount(), SlotLabel, SlotValue,
                     g_ovStateWhy, g_ovStateNote, slot, 2);
            break;

        case OVP_LOAD:
            ListPage("LOAD STATE", StateSlotCount(), SlotLabel, SlotValue,
                     g_ovStateWhy, g_ovStateNote, slot, 2);
            break;

        case OVP_HISTORY:
            DrawOverlayHistory();
            break;

        default:
            DrawOverlayMain();
            break;
        }
    }

    /*=================================================================
    =================================================================*/
    static void Draw()
    {
        /* Rebuilt every repaint: a stale rect points at a control that has
           moved or gone. */
        s_hits.clear();

        /* Zero while the cursor is moving - see the note in Loop() - and while
           anything is animating, which is the same argument: a PNG decode is
           far longer than a frame, and spending one in the middle of a slide
           is the hitch the slide exists to avoid. The settling repaint is what
           actually fills the tiles in. */
        g_artBudget = (s_wasMoving || s_animMoved) ? 0 : 2;

        /* A screen change is a page arriving. Deeper screens come in from the
           right and back out to the left, so going back reverses the way in
           and the direction keeps meaning something. */
        if (g_screen != g_prevScreen)
        {
            PageEnter(g_screen > g_prevScreen ? 1 : -1);
            g_prevScreen = g_screen;
        }
        else if (g_screen == SCREEN_OVERLAY && g_ovPage != s_prevOvPage)
        {
            PageEnter(g_ovPage > s_prevOvPage ? 1 : -1);
            s_prevOvPage = g_ovPage;
        }

        s_animBusy = false;

        switch (g_screen)
        {
            case SCREEN_GAMEINFO:    DrawGameInfo();    break;
            case SCREEN_SETTINGS:    DrawSettings();    break;
            case SCREEN_CONTROLLERS: DrawControllers(); break;
            case SCREEN_OVERLAY:     DrawOverlay();     break;
            default:                 DrawLibrary();     break;
        }

        /* Last, and over everything - a modal that something could paint on
           top of is not modal. */
        if (s_confirm != CONF_NONE) DrawConfirm();

        /* The only thing that keeps an animation running. Nothing here is on a
           timer: a chase that has arrived stops asking for frames, so an idle
           screen costs exactly what it did before any of this existed.

           One repaint past the last moving frame, because the frame that
           settles still drew with the art budget at zero - without it the
           tiles that were skipped during the movement would stay placeholders
           until the next button press. */
        {
            bool wasBusy = s_animMoved;

            s_animMoved = s_animBusy;
            s_dirty = s_animBusy || wasBusy;
        }
    }

    void Exit()
    {
        /* First, before anything that can block. Quitting goes through the
           confirm dialog, whose Confirmed() fires a five-frame pulse and is
           then followed straight by FadeOut() and this - so without an
           explicit stop the motor was still running on every single exit. */
        RumbleStop();

        /* A game still running at this point has never been torn down: quitting
           from the pause overlay raises nx_exit_to_menu and the main loop calls
           mame_done(), but quitting the *app* - and the applet being closed from
           the home menu - goes straight here with the machine still live. The
           audio stream in particular is still queued and playing, and the CPU
           cores still hold their contexts. Ending the session properly is what
           makes a long game session exit the same way a short one does. */
        if (gameRunning)
        {
            gameRunning = false;
            mame_done();
        }

        StatsSave();
        CfgSave();
        if (s_soundUp) nx_ui_sound_exit();
        Artwork::Exit();
        if (s_psmUp) psmExit();
        Gfx::dropSnapshot();
        Gfx::dropFade();
        Gfx::exit();
        romfsExit();
        exit(0);
    }

    /*=================================================================
        Confirm dialog

        Replaces hold-to-confirm on the three actions that throw work away.
        Holding a button is a fine gate but a poor question: it never says what
        is about to happen, and there is nothing to read while you hold. A
        modal names the action, and starting on Cancel means the careless press
        is the safe one.
    =================================================================*/
    static void AskConfirm(int what)
    {
        s_confirm    = what;
        s_confirmSel = 0;
        s_dirty      = true;
        s_dlgT       = g_cfg.reducedMotion ? 1.0f : 0.0f;
    }

    static void DrawConfirm()
    {
        const int w = 620, hgt = 260;
        const char *title, *body, *go;

        switch (s_confirm)
        {
            case CONF_EXIT_APP:
                title = "Close MAME NX Reborn?";
                body  = "The app will quit back to the Switch home menu.";
                go    = "Close";
                break;
            case CONF_LEAVE_GAME:
                title = "Leave the game?";
                body  = "The game stops and you go back to the library. "
                        "Anything not saved to the cabinet's memory is lost.";
                go    = "Leave";
                break;
            default:
                title = "Reset the game?";
                body  = "The machine restarts from its boot screen, "
                        "the same as power-cycling the cabinet.";
                go    = "Reset";
                break;
        }

        Gfx::clear(kScrim);

        /* Scales and fades in from the centre. The scale is deliberately tiny
           - four percent - because the panel's text is laid out for the final
           size and drawn at the final size: growing the box without growing
           the words would reflow the body copy every frame otherwise, and a
           paragraph rewrapping itself while it appears is the sort of motion
           that draws the eye to the animation instead of the question. The
           clip below is what keeps the copy inside the smaller box on the way
           in; by the time it could overflow noticeably the box has arrived. */
        {
            Gfx::DrawState saved = Gfx::pushState();
            float t = Chase(s_dlgT, 1.0f, kDialogRate, 0.004f);
            float s = 0.96f + 0.04f * t;
            int sw = (int)(w * s), sh = (int)(hgt * s);
            int sx = 640 - sw / 2, sy = 360 - sh / 2;

            s_dlgT = t;
            Gfx::fadeBy((u8)(30 + 225 * t));
            Gfx::clipTo(sx, sy, sw, sh);

            Glass(sx, sy, sw, sh, 22, kPanel, kPanelBot, kEdge);

            Gfx::drawTextClipped(sx + 36, sy + 30, title, Ink(kText), TS(24), w - 72);
            Gfx::drawTextWrapped(sx + 36, sy + 76, body, Ink(kDim), TS(15), w - 72, 3);

            /* Cancel sits left and is the landing spot, so the reflex press of
               A does nothing destructive. */
            for (int i = 0; i < 2; i++)
            {
                const int bw = 250, bh = 56;
                const int bx = sx + 36 + i * (bw + 48);
                const int by = sy + sh - bh - 30;
                bool sel  = (i == s_confirmSel);
                bool bad  = (i == 1);
                /* The chosen button takes the press dip, so activating it
                   looks like pressing it rather than like the screen
                   changing on its own. */
                FRect b; b.x = bx; b.y = by; b.w = bw; b.h = bh;
                if (sel) b = GrownBy(b, kFocusGrow * 0.7f);

                GlassFlat(b.x, b.y, b.w, b.h, 14,
                          sel ? (bad ? Lighter(kRed, 0.62f) : kCardSel) : kCard,
                          sel ? (bad ? Lighter(kRed, 0.34f) : kCardSelBot) : kCardBot,
                          sel ? kEdge : kEdgeSoft);
                if (sel)
                {
                    FRect ring = b;
                    if (bad)
                        Gfx::drawRoundedOutlineRectangle(ring.x, ring.y, ring.w, ring.h,
                                                         14, 2, kRed);
                    else FocusRing(ring, 14, 2);
                }
                else if (bad)
                    Gfx::drawRoundedOutlineRectangle(b.x, b.y, b.w, b.h, 14, 1, kRed);

                Gfx::drawTextCentered(bx + bw / 2, by + bh / 2 - (int)TS(19) / 2,
                                      i == 0 ? "Cancel" : go,
                                      sel ? Ink(kText) : Ink(kDim), TS(19), bw - 24);
                Hit(bx, by, bw, bh, HIT_CONFIRM + i);
            }

            Gfx::popState(saved);
        }

        static const char *hints[][2] = {
            { "A", "Choose" }, { "B", "Cancel" }, { "L/R", "Move" }
        };
        HintBar(hints, 3);
    }

    /* Returns true when it consumed the frame's input. */
    static bool ConfirmInput(u64 k, bool left, bool right)
    {
        if (s_confirm == CONF_NONE) return false;

        if (left  && s_confirmSel != 0) { s_confirmSel = 0; Navigated(); s_dirty = true; }
        if (right && s_confirmSel != 1) { s_confirmSel = 1; Navigated(); s_dirty = true; }

        if (k & HidNpadButton_B)
        {
            Cancelled();
            s_confirm = CONF_NONE;
            s_dirty = true;
            return true;
        }

        if (k & HidNpadButton_A)
        {
            int what = s_confirm;

            s_confirm = CONF_NONE;
            s_dirty = true;

            if (s_confirmSel == 0) { Cancelled(); return true; }

            Confirmed();
            switch (what)
            {
                case CONF_EXIT_APP:
                    FadeOut();
                    Exit();
                    break;
                case CONF_LEAVE_GAME:
                    g_screen = SCREEN_LIBRARY;
                    nx_exit_to_menu = 1;
                    break;
                case CONF_RESET:
                    machine_reset();
                    g_screen = SCREEN_LIBRARY;
                    break;
            }
        }
        return true;
    }

    /* Pad and touch both land here, so a row can never mean one thing to a
       button and another to a finger. */
    static void OverlayActivate()
    {
        if (OverlayDestructive(g_ovRows[g_ovSel].action))
        {
            AskConfirm(g_ovRows[g_ovSel].action == OVA_RESET
                           ? CONF_RESET : CONF_EXIT_APP);
            return;
        }

        Confirmed();
        switch (g_ovRows[g_ovSel].action)
            {
                case OVA_RESUME:  g_screen = SCREEN_LIBRARY; break;
                case OVA_LIBRARY: AskConfirm(CONF_LEAVE_GAME); break;
                case OVA_STATES:     g_ovPage = OVP_STATES;     break;
                case OVA_DIPS:       g_ovPage = OVP_DIPS;       break;
                case OVA_CHEAT:      g_ovPage = OVP_CHEAT;      break;
                case OVA_INPUT_GAME: g_ovPage = OVP_INPUT_GAME; break;
                case OVA_INPUT_GEN:  g_ovPage = OVP_INPUT_GEN;  break;
                case OVA_ANALOG:     g_ovPage = OVP_ANALOG;     break;
                case OVA_MEMCARD:    g_ovPage = OVP_MEMCARD;    break;
                case OVA_HISTORY:
                    WrapHistory();
                    g_ovPage = OVP_HISTORY;
                    break;
                case OVA_ARTWORK:
                    /* Same pair usrintrf.c's UI_ARTWORK toggles between,
                       and the same artwork_enable() call - this is the
                       core's switch, moved to a row that matches the
                       rest of the overlay. */
                    options.use_artwork =
                        (options.use_artwork == ARTWORK_USE_NONE)
                            ? (ARTWORK_USE_BACKDROPS | ARTWORK_USE_OVERLAYS | ARTWORK_USE_BEZELS)
                            : ARTWORK_USE_NONE;
                    artwork_enable(options.use_artwork != ARTWORK_USE_NONE);
                    BuildOverlayRows();
                    break;
            }
        s_dirty = true;
    }

    /* One place where a tap becomes the same action a button press would
       cause, so touch and pad can never disagree about what a control does. */
    static void HandleTap(int id)
    {
        if (id == HIT_NONE) return;

        if (s_confirm != CONF_NONE)
        {
            if (id >= HIT_CONFIRM && id < HIT_CONFIRM + 2)
            {
                s_confirmSel = id - HIT_CONFIRM;
                ConfirmInput(HidNpadButton_A, false, false);
            }
            return;
        }

        if (g_screen == SCREEN_OVERLAY && id >= HIT_OVROW && id < HIT_OVROW + 64)
        {
            int row = id - HIT_OVROW;

            if (g_ovPage == OVP_MAIN)
            {
                /* Tapping a row that is not focused focuses it; tapping the
                   focused one activates. Stops a stray tap firing Exit. */
                if (row != g_ovSel) { g_ovSel = row; Navigated(); }
                else                { OverlayActivate(); }
            }
            else
            {
                int &sel = g_ovSelOf[g_ovPage];
                if (row != sel)
                {
                    sel = row;
                    Navigated();
                }
                else if (g_ovPage == OVP_STATES)
                {
                    bool saving = sel == 0;
                    nx_state_refresh();
                    StateNoteDefault(saving);
                    g_ovPage = saving ? OVP_SAVE : OVP_LOAD;
                    Confirmed();
                }
            }
            s_dirty = true;
            return;
        }

        if (g_screen != SCREEN_LIBRARY || gameRunning) return;

        if (id >= HIT_TILE)
        {
            int idx = g_page * PerPage() + (id - HIT_TILE);

            if (idx < 0 || idx >= (int)g_list.size()) return;
            g_zone = ZONE_GRID;
            if (idx != g_sel) { g_sel = idx; Navigated(); }
            else              { Confirmed(); LaunchSelected(); }
            s_dirty = true;
            return;
        }

        if (id >= HIT_LEFTRAIL && id < HIT_LEFTRAIL + 3)
        {
            g_zone = ZONE_LEFTRAIL;
            g_leftSel = id - HIT_LEFTRAIL;
            Confirmed();
            SetCategory(g_leftSel);
            g_zone = ZONE_GRID;
            s_dirty = true;
            return;
        }

        if (id >= HIT_RIGHTRAIL && id < HIT_RIGHTRAIL + 3)
        {
            g_zone = ZONE_RIGHTRAIL;
            g_rightSel = id - HIT_RIGHTRAIL;
            Confirmed();
            if (g_rightSel == 0)      { g_screen = SCREEN_CONTROLLERS; g_setRow = 0; }
            else if (g_rightSel == 1) { g_screen = SCREEN_SETTINGS; g_setInSidebar = true; BuildRows(); }
            else                        AskConfirm(CONF_EXIT_APP);
            s_dirty = true;
            return;
        }

        switch (id)
        {
            case HIT_CHIP_CAT:
                SetCategory((g_category + 1) % CAT_COUNT);
                g_leftSel = g_category;
                Navigated();
                break;
            case HIT_CHIP_SORT:
                g_cfg.sort = (g_cfg.sort + 1) % SORT_COUNT;
                RebuildList(); SyncPage(); CfgSave(); Navigated();
                break;
            case HIT_CHIP_VIEW:
                g_cfg.view = (g_cfg.view + 1) % VIEW_COUNT;
                SyncPage(); CfgSave(); Navigated();
                break;
            default:
                return;
        }
        s_dirty = true;
    }

    /*=================================================================
        Transitions

        Only used where the screen is about to be replaced wholesale - into a
        game and back out. Everything else stays static: this is a CPU
        rasteriser, and animating a screen that is otherwise idle would burn a
        full repaint every frame for decoration.
    =================================================================*/
    /* Leaving is quicker than arriving. A screen on its way out has nothing
       left to look at, so lingering on it only delays what was asked for;
       a screen on its way in is the thing being waited for, and giving it the
       longer curve is what makes it read as settling rather than snapping. */
    enum { kFadeOutFrames = 7,      /* 117ms - going away  */
           kFadeInFrames  = 12 };   /* 200ms - coming back */

    /* flush() presents the menu layer alone; flushOverlay() composites it over
       the last game frame. Picking the wrong one drops the game out from under
       a fade that starts on the pause overlay. */
    static void FadePresent()
    {
        if (gameRunning) Gfx::flushOverlay();
        else             Gfx::flush();
    }

    static void FadeOut()
    {
        if (g_cfg.reducedMotion) return;

        Gfx::captureFade();
        for (int i = 1; i <= kFadeOutFrames; i++)
        {
            /* Squared, so the picture leaves slowly for a frame or two and
               then goes - an ease-in, matching the way the incoming screen
               eases out on arrival. */
            int t = 255 * i / kFadeOutFrames;
            Gfx::fadeFromCapture((u8)(t * t / 255));
            RumbleTick();
            FadePresent();
        }
        Gfx::dropFade();
    }

    static void FadeIn()
    {
        if (g_cfg.reducedMotion) { FadePresent(); return; }

        /* Snapshot the finished screen, then walk the darkness off it. The
           caller has already drawn what should appear. */
        Gfx::captureFade();
        for (int i = kFadeInFrames; i >= 0; i--)
        {
            /* Squared the other way round: most of the brightness arrives in
               the first few frames and the last of the darkness comes off
               slowly, which is the ease-out an arriving screen wants. */
            int t = 255 * i / kFadeInFrames;
            Gfx::fadeFromCapture((u8)(t * t / 255));
            RumbleTick();
            FadePresent();
        }
        Gfx::dropFade();
    }

    /* Neo Geo and a few other platforms split the hardware's own ROMs into a
       separate set that every game on that hardware needs alongside its own.
       Those sets are declared NOT_A_DRIVER and sit in the clone_of chain, so
       the audit reports their files as plain missing ROMs - which reads as a
       broken dump rather than "you are missing the BIOS". */
    static const struct GameDriver *BiosSetFor(const struct GameDriver *drv)
    {
        const struct GameDriver *p = drv ? drv->clone_of : NULL;

        while (p)
        {
            if (p->flags & NOT_A_DRIVER) return p;
            p = p->clone_of;
        }
        return NULL;
    }

    /*=================================================================
        Launch
    =================================================================*/
    static void LaunchSelected()
    {
        int gameIndex = DriverIndexFor(currentGame);

        if (gameIndex < 0) { Refused(); return; }

        ApplySettings();

        /* Before the clear, or the snapshot catches a blank screen instead of
           the library the player is leaving. */
        FadeOut();
        Gfx::clear(kCanvasBot);

        /* After the fade - it needs its buffer - and before init_game(), which
           is where the ROM regions are allocated and where the heap actually
           has to be there. */
        ReleaseFrontendMemory();

        /* GAME_NOT_WORKING means the driver's own author marked it as not
           emulated well enough to run - it will hang, show a black screen, or
           die inside the core. Blocked outright: a crash back to hbmenu reads
           as a broken port. */
        if (drivers[gameIndex]->flags & GAME_NOT_WORKING)
        {
            char msg[192];
            Refused();
            sprintf(msg, "%s is marked not working and cannot be started.",
                    drivers[gameIndex]->description);
            RenderMessage(msg);
            svcSleepThread(3000000000ULL);
            s_dirty = true; Draw();
            return;
        }

        /* Check the driver's ROM regions against the heap we actually have
           before loading anything. In applet mode that heap is a fraction of a
           full launch, and the heavier drivers will not fit. */
        {
            unsigned long need = 0, avail = 0;
            nx_fit_t fit = nx_perf_driver_fits(drivers[gameIndex], &need, &avail);

            if (fit != NX_FIT_OK)
            {
                char msg[224];
                Refused();
                sprintf(msg, "%s needs %lu MB, only %lu MB free.%s",
                        drivers[gameIndex]->description,
                        need / (1024ul * 1024ul), avail / (1024ul * 1024ul),
                        fit == NX_FIT_NEEDS_FULL_RAM
                            ? "  Relaunch mame-nx by holding R over a game to get the full heap."
                            : "");
                RenderMessage(msg);
                svcSleepThread(3000000000ULL);
                s_dirty = true; Draw();
                return;
            }
        }

        options.ui_orientation = drivers[gameIndex]->flags & ORIENTATION_MASK;
        if (options.ui_orientation & ORIENTATION_SWAP_XY)
        {
            if ((options.ui_orientation & ROT180) == ORIENTATION_FLIP_X ||
                (options.ui_orientation & ROT180) == ORIENTATION_FLIP_Y)
                options.ui_orientation ^= ROT180;
        }

        /* core's fileio.c uses this to bootstrap per-game NVRAM. strdup is
           POSIX, not ISO C++, and CXXFLAGS builds with strict -std=c++11 which
           hides it from <string.h> - malloc()+strcpy() to match the free(). */
        free(options.romset_filename_noext);
        {
            const char *drvname = drivers[gameIndex]->name;
            options.romset_filename_noext = (char *)malloc(strlen(drvname) + 1);
            strcpy(options.romset_filename_noext, drvname);
        }

        /* MAME owns audout outright while a game runs; the pause menu's tones
           go through nx_pause_sound into MAME's own PCM, so the launcher's
           session is not needed and holding it only invites the two to fight
           over released buffers. */
        if (s_soundUp) { nx_ui_sound_exit(); s_soundUp = false; }

        /* init_game() is the half of the old retro_load_game() sequence that is
           easy to miss: run_game() ignores its argument and works off the
           Machine globals, so without this Machine->gamedrv and Machine->drv are
           still NULL when init_machine() dereferences them. Note the inverted
           conventions - init_game() returns true on success, run_game() zero. */
        if (!init_game(gameIndex))
        {
            RenderMessage((char *)"Failed to initialise the machine driver.");
            svcSleepThread(3000000000ULL);
            if (!s_soundUp) s_soundUp = nx_ui_sound_init() != 0;
            s_dirty = true; Draw();
            return;
        }

        if (run_game(gameIndex) == 0)
        {
            GameStat &st = StatFor(currentGame);

            st.plays++;
            st.lastPlayed = (long long)time(NULL);
            StatsSave();

            nx_perf_game_start(drivers[gameIndex]);
            gameRunning = true;
            g_screen = SCREEN_LIBRARY;
        }
        else
        {
            /* run_game() reports failure as a bare non-zero, so ask audit.c what
               is actually wrong with the set. Failure path only - VerifyRomSet()
               hashes every file, far too slow to run before every launch. */
            char msg[512];
            const char *why;

            RenderMessage((char *)"Checking ROM set...");
            g_auditDetail[0] = '\0';
            switch (VerifyRomSet(gameIndex, nx_audit_printf))
            {
                case NOTFOUND:       why = "ROMs not found";              break;
                case INCORRECT:      why = "ROM set is wrong or corrupt"; break;
                case CLONE_NOTFOUND: why = "parent ROM set is missing";   break;
                case BEST_AVAILABLE: why = "ROM set is incomplete";       break;
                default:             why = "the driver failed to start";  break;
            }
            {
                const struct GameDriver *bios = BiosSetFor(drivers[gameIndex]);

                /* Lead with the thing the player can act on. The CRC dump
                   still follows, but it is evidence, not the headline. */
                if (bios)
                    snprintf(msg, sizeof(msg),
                             "Cannot run %s: %s.\n"
                             "This is %s hardware, so it also needs the BIOS set: "
                             "put %s.zip in your roms folder next to %s.zip.\n%s",
                             drivers[gameIndex]->description, why,
                             bios->description, bios->name,
                             drivers[gameIndex]->name, g_auditDetail);
                else
                    snprintf(msg, sizeof(msg), "Cannot run %s: %s.\n%s",
                             drivers[gameIndex]->description, why, g_auditDetail);
            }
            RenderMessage(msg);
            svcSleepThread(4000000000ULL);
            if (!s_soundUp) s_soundUp = nx_ui_sound_init() != 0;
            s_dirty = true; Draw();
        }
    }

    /* Called by the main loop after a game has been torn down. */
    void Refresh()
    {
        sprintf(RomCountText, "%d of %lu drivers have ROMs",
                romList.AvRoms(), (unsigned long)romList.totalMAMEGames);
        /* Every route back from a game passes through here, and MAME has just
           released audout, so this is where the launcher takes it back. */
        if (!s_soundUp) s_soundUp = nx_ui_sound_init() != 0;
        RebuildList();
        SyncPage();
        g_screen = SCREEN_LIBRARY;
        s_dirty = true;
        Draw();
    }

    /*=================================================================
        Search
    =================================================================*/
    static void OpenSearch()
    {
        SwkbdConfig kbd;
        char out[64] = { 0 };

        if (R_FAILED(swkbdCreate(&kbd, 0))) { Refused(); return; }

        swkbdConfigMakePresetDefault(&kbd);
        swkbdConfigSetGuideText(&kbd, "Search the library");
        swkbdConfigSetInitialText(&kbd, g_search.c_str());
        swkbdConfigSetStringLenMax(&kbd, sizeof(out) - 1);

        if (R_SUCCEEDED(swkbdShow(&kbd, out, sizeof(out))))
        {
            g_search = out;
            RebuildList();
            SyncPage();
            Confirmed();
        }
        swkbdClose(&kbd);
        s_dirty = true;
    }

    /*=================================================================
        Navigation
    =================================================================*/
    static void MoveGrid(int dx, int dy)
    {
        GridSpec g = Grid();
        int per = g.cols * g.rows, n = (int)g_list.size();
        int page, col, row;
        int wasPage = g_page;

        if (n == 0) return;

        page = g_sel / per;
        col  = (g_sel % per) % g.cols;
        row  = (g_sel % per) / g.cols;

        if (dx < 0)
        {
            /* Leaving the grid at the left edge lands on the left rail rather
               than doing nothing - no dead ends. */
            if (col == 0 && page == 0) { g_zone = ZONE_LEFTRAIL; Edged(); s_dirty = true; return; }
            if (col == 0) { page--; col = g.cols - 1; } else col--;
        }
        else if (dx > 0)
        {
            if (col == g.cols - 1 && (page + 1) * per >= n)
            { g_zone = ZONE_RIGHTRAIL; Edged(); s_dirty = true; return; }
            if (col == g.cols - 1) { page++; col = 0; } else col++;
        }

        if (dy < 0) { if (row == 0) { if (page > 0) { page--; row = g.rows - 1; } } else row--; }
        else if (dy > 0)
        {
            if (row == g.rows - 1) { if ((page + 1) * per < n) { page++; row = 0; } }
            else row++;
        }

        g_sel = page * per + row * g.cols + col;
        if (g_sel >= n) g_sel = n - 1;
        if (g_sel < 0) g_sel = 0;

        SyncPage();
        Navigated();

        /* Walking off the edge of a page is a page change, so the shelf slides
           the way the cursor went rather than swapping its contents in place. */
        if (g_page != wasPage) PageEnter(g_page > wasPage ? 1 : -1);

        s_dirty = true;
    }

    static void SetCategory(int c)
    {
        int was = (int)g_category;

        g_category = (Category)c;
        RebuildList();
        SyncPage();
        /* Categories are a horizontal row of rails, so they slide horizontally
           - and wrapping from the last back to the first still reads as
           forward, which is the direction the press meant. */
        if (c != was) PageEnter(c > was || (was == CAT_COUNT - 1 && c == 0) ? 1 : -1);
        s_dirty = true;
    }

    static void ToggleFavourite()
    {
        if (g_list.empty()) { Refused(); return; }
        {
            GameStat &st = StatFor(currentGame);
            st.fav = !st.fav;
            StatsSave();
            Confirmed();
        }
        if (g_category == CAT_FAV) { RebuildList(); SyncPage(); }
        s_dirty = true;
    }

    /* Returns true on the frame the hold completes. */
    static bool HoldProgress(int action, bool buttonHeld)
    {
        if (!g_cfg.holdToConfirm) return buttonHeld;

        if (!buttonHeld) { if (s_holdAction == action) { s_holdAction = -1; s_holdFrames = 0; s_dirty = true; } return false; }

        if (s_holdAction != action) { s_holdAction = action; s_holdFrames = 0; }
        s_holdFrames++;
        s_dirty = true;

        /* Ramp the pad as the bar fills - the hold is a safety gate, so it
           should be obvious it is charging without watching the screen. */
        if ((s_holdFrames % 6) == 0)
            Rumble(0.20f + 0.55f * (float)s_holdFrames / (float)kHoldFrames, 2);

        if (s_holdFrames >= kHoldFrames)
        { s_holdAction = -1; s_holdFrames = 0; return true; }

        return false;
    }

    /*=================================================================
    =================================================================*/
    void Loop()
    {
        u64 k, h, dir, move;
        bool repeat;

        padUpdate(&s_uiPad);
        if (s_playerPadUp) { padUpdate(&s_playerPad[0]); padUpdate(&s_playerPad[1]); }
        RumbleTick();

        k = padGetButtonsDown(&s_uiPad);
        h = padGetButtons(&s_uiPad);

        dir = h & (HidNpadButton_Up | HidNpadButton_Down | HidNpadButton_Left | HidNpadButton_Right |
                   HidNpadButton_StickLUp | HidNpadButton_StickLDown |
                   HidNpadButton_StickLLeft | HidNpadButton_StickLRight);

        if (dir != s_heldDir) { s_heldDir = dir; s_heldFrames = 0; }
        else if (dir) s_heldFrames++;

        /* Accelerating repeat. The old flat 24-frame delay then one step every
           5 frames caps at 12 moves a second, which is a long hold to cross a
           library of any size. Same deliberate first step, then it winds up. */
        {
            int interval = 6;
            if (s_heldFrames > 90)      interval = 2;
            else if (s_heldFrames > 50) interval = 3;
            else if (s_heldFrames > 32) interval = 4;

            repeat = (s_heldFrames > 20 && (s_heldFrames % interval) == 0);
        }
        move = k | (repeat ? dir : 0);

        /* Decoding a PNG off the SD card takes far longer than a frame, and
           ArtFor() does up to two per repaint. Held on a direction that is a
           decode every few frames - the hitch people read as "the UI is not
           smooth". Load nothing while the cursor is moving; take one extra
           repaint once it settles and fill the tiles in then. */
        {
            bool moving = (dir != 0);
            if (!moving && s_wasMoving) s_dirty = true;
            s_wasMoving = moving;
        }

        /* A tap acts on what is drawn, so it needs a frame already drawn to
           test against - s_hits is filled by Draw(). */
        {
            int tx, ty;
            if (ReadTap(tx, ty)) HandleTap(HitTest(tx, ty));
        }

        {
            bool up    = (move & (HidNpadButton_Up    | HidNpadButton_StickLUp))    != 0;
            bool down  = (move & (HidNpadButton_Down  | HidNpadButton_StickLDown))  != 0;
            bool left  = (move & (HidNpadButton_Left  | HidNpadButton_StickLLeft))  != 0;
            bool right = (move & (HidNpadButton_Right | HidNpadButton_StickLRight)) != 0;

            /* The dialog is modal, so it eats the frame before any screen sees
               it - otherwise the same A press both answers the question and
               activates whatever sits under the panel. */
            if (!ConfirmInput(k, left, right))
            switch (g_screen)
            {
            /*-------------------------------------------------- overlay */
            case SCREEN_OVERLAY:
                /* A capture owns every button on the pad, including the ones
                   that normally drive this menu - that is the whole point of
                   it. Minus is the one escape, and it is not a bindable
                   arcade button. */
                if (g_ovCapture >= 0)
                {
                    if (k & HidNpadButton_Minus)
                    {
                        nx_input_capture_clear(g_ovCapture, g_ovPage == OVP_INPUT_GEN);
                        g_ovCapture = -1;
                        Cancelled();
                    }
                    else if (nx_input_capture_poll(g_ovCapture, g_ovPage == OVP_INPUT_GEN))
                    {
                        g_ovCapture = -1;
                        Confirmed();
                    }
                    s_dirty = true;
                    break;
                }

                if (g_ovPage == OVP_HISTORY)
                {
                    int &top   = g_ovTopOf[OVP_HISTORY];
                    int  lines = (int)g_ovHistory.size();
                    int  last  = lines - 15;

                    if (last < 0) last = 0;

                    /* Scrolling stops at both ends rather than wrapping -
                       prose read top to bottom, and looping back to the start
                       mid-sentence reads as a glitch. */
                    if (up   && top > 0)    { top--; Navigated(); s_dirty = true; }
                    if (down && top < last) { top++; Navigated(); s_dirty = true; }

                    if (k & HidNpadButton_B) { Cancelled(); g_ovPage = OVP_MAIN; s_dirty = true; }
                    break;
                }

                if (g_ovPage != OVP_MAIN)
                {
                    int &sel = g_ovSelOf[g_ovPage];
                    int &top = g_ovTopOf[g_ovPage];
                    int  total = 0;

                    switch (g_ovPage)
                    {
                        case OVP_DIPS:       total = nx_dip_count(); break;
                        case OVP_CHEAT:      total = nx_cheat_count(); break;
                        case OVP_INPUT_GAME: total = nx_input_count(0); break;
                        case OVP_INPUT_GEN:  total = nx_input_count(1); break;
                        case OVP_ANALOG:     total = nx_analog_count() * NX_ANALOG_FIELDS; break;
                        case OVP_MEMCARD:    total = MEM_COUNT; break;
                        case OVP_STATES:     total = 2; break;
                        /* Zero when the driver cannot serialize, so the whole
                           input block below is skipped and A cannot reach a
                           slot that was never listed. */
                        case OVP_SAVE:
                        case OVP_LOAD:       total = StateSlotCount(); break;
                    }

                    if (total > 0)
                    {
                        if (up)   { sel = (sel + total - 1) % total; Navigated(); s_dirty = true; }
                        if (down) { sel = (sel + 1) % total;         Navigated(); s_dirty = true; }

                        /* Keep the cursor inside the visible window, following
                           it rather than paging - the lists are short. */
                        if (sel < top) top = sel;
                        if (sel >= top + kRowsVisible) top = sel - kRowsVisible + 1;

                        if (left || right)
                        {
                            int dir = right ? 1 : -1;

                            /* The value takes a shove in the direction it was
                               pushed and slides back. Kicked before the change
                               so it is running on the same frame the new value
                               first appears. */
                            NudgeKick(dir);

                            switch (g_ovPage)
                            {
                            case OVP_DIPS:
                            {
                                /* nx_dip_change() clamps at both ends, so
                                   compare the label to tell "moved" from
                                   "already at the end" and cue them apart. */
                                char seen[64];
                                snprintf(seen, sizeof(seen), "%s", nx_dip_value(sel));
                                nx_dip_change(sel, dir);
                                if (strcmp(seen, nx_dip_value(sel)) != 0) Confirmed();
                                else                                     Edged();
                                break;
                            }
                            case OVP_CHEAT:
                                if (nx_cheat_selectable(sel)) { nx_cheat_change(sel, dir); Confirmed(); }
                                else                            Refused();
                                break;
                            case OVP_ANALOG:
                                nx_analog_change(sel / NX_ANALOG_FIELDS,
                                                 sel % NX_ANALOG_FIELDS, dir);
                                Confirmed();
                                break;
                            case OVP_MEMCARD:
                                if (sel == MEM_SLOT)
                                {
                                    g_ovMemSlot += dir;
                                    if (g_ovMemSlot < 0)  g_ovMemSlot = 0;
                                    if (g_ovMemSlot > 15) g_ovMemSlot = 15;
                                    Navigated();
                                }
                                break;
                            }
                            s_dirty = true;
                        }

                        if (k & HidNpadButton_A)
                        {
                            switch (g_ovPage)
                            {
                            case OVP_STATES:
                            {
                                bool saving = sel == 0;
                                nx_state_refresh();
                                StateNoteDefault(saving);
                                g_ovPage = saving ? OVP_SAVE : OVP_LOAD;
                                Confirmed();
                                break;
                            }
                            case OVP_INPUT_GAME:
                            case OVP_INPUT_GEN:
                                /* Start the capture, then let the poll above
                                   own the pad until the sequence settles. */
                                nx_input_capture_start();
                                g_ovCapture = sel;
                                Confirmed();
                                break;
                            case OVP_CHEAT:
                                if (nx_cheat_selectable(sel)) { nx_cheat_activate(sel); Confirmed(); }
                                else                            Refused();
                                break;
                            case OVP_SAVE:
                                if (nx_state_save(sel) == 0)
                                {
                                    snprintf(g_ovStateNote, sizeof(g_ovStateNote),
                                             "Saved to slot %d.", sel + 1);
                                    Confirmed();
                                }
                                else
                                {
                                    snprintf(g_ovStateNote, sizeof(g_ovStateNote),
                                             "%s", nx_state_error());
                                    Refused();
                                }
                                break;

                            case OVP_LOAD:
                                if (nx_state_load(sel) == 0)
                                {
                                    /* Straight back into the game. Loading is
                                       the whole errand - stopping to admire
                                       the menu afterwards is not. */
                                    Confirmed();
                                    g_ovPage = OVP_MAIN;
                                    g_screen = SCREEN_LIBRARY;
                                }
                                else
                                {
                                    snprintf(g_ovStateNote, sizeof(g_ovStateNote),
                                             "%s", nx_state_error());
                                    Refused();
                                }
                                break;

                            case OVP_MEMCARD:
                            {
                                int action = -1;
                                if (sel == MEM_LOAD)   action = NX_MEMCARD_LOAD;
                                if (sel == MEM_SAVE)   action = NX_MEMCARD_SAVE;
                                if (sel == MEM_EJECT)  action = NX_MEMCARD_EJECT;
                                if (sel == MEM_CREATE) action = NX_MEMCARD_CREATE;

                                if (action < 0) Navigated();
                                else if (nx_memcard_action(action, g_ovMemSlot) == 0) Confirmed();
                                else Refused();
                                break;
                            }
                            default:
                                /* Dips and analog have no separate confirm;
                                   A stepping forward matches Right. */
                                if (g_ovPage == OVP_DIPS)  { nx_dip_change(sel, 1); Confirmed(); }
                                if (g_ovPage == OVP_ANALOG){ nx_analog_change(sel / NX_ANALOG_FIELDS,
                                                                              sel % NX_ANALOG_FIELDS, 1);
                                                             Confirmed(); }
                                break;
                            }
                            s_dirty = true;
                        }
                    }

                    if (k & HidNpadButton_Minus &&
                        (g_ovPage == OVP_INPUT_GAME || g_ovPage == OVP_INPUT_GEN))
                    {
                        nx_input_capture_clear(sel, g_ovPage == OVP_INPUT_GEN);
                        Cancelled();
                        s_dirty = true;
                    }

                    if (k & HidNpadButton_B)
                    {
                        Cancelled();
                        g_ovPage = (g_ovPage == OVP_SAVE || g_ovPage == OVP_LOAD)
                                 ? OVP_STATES : OVP_MAIN;
                        s_dirty = true;
                    }
                    break;
                }

                if (up)   { g_ovSel = (g_ovSel + g_ovRowCount - 1) % g_ovRowCount; Navigated(); s_dirty = true; }
                if (down) { g_ovSel = (g_ovSel + 1) % g_ovRowCount;                Navigated(); s_dirty = true; }

                if (g_ovSel < g_ovTopOf[OVP_MAIN]) g_ovTopOf[OVP_MAIN] = g_ovSel;
                if (g_ovSel >= g_ovTopOf[OVP_MAIN] + kRowsVisible)
                    g_ovTopOf[OVP_MAIN] = g_ovSel - kRowsVisible + 1;

                if (k & HidNpadButton_A) OverlayActivate();

                if (k & HidNpadButton_B) { Cancelled(); g_screen = SCREEN_LIBRARY; s_dirty = true; }
                break;

            /*-------------------------------------------------- settings */
            case SCREEN_SETTINGS:
                if (g_setInSidebar)
                {
                    if (up)   { g_setCat = (g_setCat + SETCAT_COUNT - 1) % SETCAT_COUNT; BuildRows(); Navigated(); s_dirty = true; }
                    if (down) { g_setCat = (g_setCat + 1) % SETCAT_COUNT;                BuildRows(); Navigated(); s_dirty = true; }
                    if (right && g_rowCount) { g_setInSidebar = false; g_setRow = 0; g_setRowTop = 0; Navigated(); s_dirty = true; }
                    if (k & HidNpadButton_A) { if (g_rowCount) { g_setInSidebar = false; g_setRow = 0; g_setRowTop = 0; Confirmed(); s_dirty = true; } }
                    if (k & HidNpadButton_B) { Cancelled(); CfgSave(); ApplySettings(); g_screen = SCREEN_LIBRARY; g_zone = ZONE_RIGHTRAIL; s_dirty = true; }
                }
                else
                {
                    SetRow &r = g_rows[g_setRow];

                    if (up)   { g_setRow = (g_setRow + g_rowCount - 1) % g_rowCount; Navigated(); s_dirty = true; }
                    if (down) { g_setRow = (g_setRow + 1) % g_rowCount;              Navigated(); s_dirty = true; }

                    /* Window follows the cursor. Recomputed against the same
                       helpers the draw uses, so changing the text size cannot
                       leave the two disagreeing about how many rows fit. */
                    {
                        int vis = SetRowsVisible();
                        if (g_setRow < g_setRowTop) g_setRowTop = g_setRow;
                        if (g_setRow >= g_setRowTop + vis) g_setRowTop = g_setRow - vis + 1;
                    }

                    if (r.kind == ROW_SLIDER && (left || right))
                    {
                        int *v = (int *)r.value;
                        *v += right ? r.step : -r.step;
                        if (*v < r.lo) *v = r.lo;
                        if (*v > r.hi) *v = r.hi;
                        NudgeKick(right ? 1 : -1);
                        ApplySettings(); Navigated(); s_dirty = true;
                    }
                    else if (r.kind == ROW_CHOICE && (left || right))
                    {
                        int *v = (int *)r.value;
                        *v = (*v + (right ? 1 : r.choiceCount - 1)) % r.choiceCount;
                        /* Choice rows used to skip this. Harmless while every
                           one of them was a library filter; wrong the moment
                           one of them repaints the whole interface. */
                        ApplySettings();
                        NudgeKick(right ? 1 : -1);
                        RebuildList(); SyncPage(); Navigated(); s_dirty = true;
                    }
                    else if (left) { g_setInSidebar = true; Navigated(); s_dirty = true; }

                    if (r.kind == ROW_ACTION && !strcmp(r.label, "Reset all settings"))
                    {
                        if (HoldProgress(999, (h & HidNpadButton_A) != 0))
                        { SettingsDefaults(); ApplySettings(); CfgSave(); BuildRows(); Confirmed(); s_dirty = true; }
                    }
                    else if (k & HidNpadButton_A)
                    {
                        Confirmed();
                        if (r.kind == ROW_TOGGLE) { bool *b = (bool *)r.value; *b = !*b; ApplySettings(); RebuildList(); SyncPage(); }
                        else if (r.kind == ROW_ACTION)
                        {
                            if (!strcmp(r.label, "Rescan ROM folder"))
                            { romList.RefreshRomList(); Refresh(); g_screen = SCREEN_SETTINGS; }
                            else if (!strcmp(r.label, "Download covers on this page"))
                                DownloadPageArtwork();
                            else if (!strcmp(r.label, "Controller assignment"))
                            { g_screen = SCREEN_CONTROLLERS; g_setRow = 0; }
                        }
                        s_dirty = true;
                    }

                    if (k & HidNpadButton_B) { Cancelled(); g_setInSidebar = true; s_dirty = true; }
                }
                break;

            /*-------------------------------------------------- controllers */
            case SCREEN_CONTROLLERS:
                if (left  && g_setRow > 0) { g_setRow = 0; Navigated(); s_dirty = true; }
                if (right && g_setRow < 1) { g_setRow = 1; Navigated(); s_dirty = true; }
                if (k & HidNpadButton_B) { Cancelled(); g_screen = SCREEN_LIBRARY; g_zone = ZONE_RIGHTRAIL; s_dirty = true; }
                s_dirty = true;     /* live input test needs a repaint per frame */
                break;

            /*-------------------------------------------------- game info */
            case SCREEN_GAMEINFO:
                if (k & HidNpadButton_B) { Cancelled(); g_screen = SCREEN_LIBRARY; s_dirty = true; }
                if (k & HidNpadButton_Y) ToggleFavourite();
                if (k & HidNpadButton_A) { Confirmed(); LaunchSelected(); s_dirty = true; }
                break;

            /*-------------------------------------------------- library */
            default:
                if (g_zone == ZONE_GRID)
                {
                    if (up)    MoveGrid(0, -1);
                    if (down)  MoveGrid(0, +1);
                    if (left)  MoveGrid(-1, 0);
                    if (right) MoveGrid(+1, 0);
                }
                else if (g_zone == ZONE_LEFTRAIL)
                {
                    if (up)    { g_leftSel = (g_leftSel + 2) % 3; Navigated(); s_dirty = true; }
                    if (down)  { g_leftSel = (g_leftSel + 1) % 3; Navigated(); s_dirty = true; }
                    if (right) { g_zone = ZONE_GRID; Navigated(); s_dirty = true; }
                    if (k & HidNpadButton_A) { Confirmed(); SetCategory(g_leftSel); g_zone = ZONE_GRID; }
                }
                else /* right rail */
                {
                    if (up)   { g_rightSel = (g_rightSel + 2) % 3; Navigated(); s_dirty = true; }
                    if (down) { g_rightSel = (g_rightSel + 1) % 3; Navigated(); s_dirty = true; }
                    if (left) { g_zone = ZONE_GRID; Navigated(); s_dirty = true; }

                    if (g_rightSel == 2)   /* quit */
                    {
                        if (k & HidNpadButton_A) AskConfirm(CONF_EXIT_APP);
                    }
                    else if (k & HidNpadButton_A)
                    {
                        Confirmed();
                        if (g_rightSel == 0) { g_screen = SCREEN_CONTROLLERS; g_setRow = 0; }
                        else { g_screen = SCREEN_SETTINGS; g_setInSidebar = true; BuildRows(); }
                        s_dirty = true;
                    }
                }

                if (k & HidNpadButton_ZL) { g_sel -= PerPage(); SyncPage(); Navigated(); PageEnter(-1); s_dirty = true; }
                if (k & HidNpadButton_ZR) { g_sel += PerPage(); SyncPage(); Navigated(); PageEnter(+1); s_dirty = true; }
                if (k & HidNpadButton_L)  { SetCategory((g_category + CAT_COUNT - 1) % CAT_COUNT); Navigated(); }
                if (k & HidNpadButton_R)  { SetCategory((g_category + 1) % CAT_COUNT); Navigated(); }

                if (k & HidNpadButton_Minus)
                { g_cfg.view = (g_cfg.view + 1) % VIEW_COUNT; SyncPage(); CfgSave(); Navigated(); s_dirty = true; }

                if (k & HidNpadButton_StickL)
                { g_cfg.sort = (g_cfg.sort + 1) % SORT_COUNT; RebuildList(); SyncPage(); CfgSave(); Navigated(); s_dirty = true; }

                if (k & HidNpadButton_X) OpenSearch();
                if (k & HidNpadButton_Y) ToggleFavourite();

                if ((k & HidNpadButton_Plus) && !g_list.empty())
                { Confirmed(); g_screen = SCREEN_GAMEINFO; s_dirty = true; }

                if ((k & HidNpadButton_A) && g_zone == ZONE_GRID && !g_list.empty())
                { Confirmed(); LaunchSelected(); s_dirty = true; }

                if (k & HidNpadButton_B)
                {
                    /* B unwinds one step at a time; at the bottom of the ladder
                       there is nothing left to back out of but the app. */
                    if (!g_search.empty()) { Cancelled(); g_search.clear(); RebuildList(); SyncPage(); }
                    else if (g_category != CAT_ALL) { Cancelled(); SetCategory(CAT_ALL); }
                    else if (g_zone != ZONE_GRID)   { Cancelled(); g_zone = ZONE_GRID; }
                    else AskConfirm(CONF_EXIT_APP);
                    s_dirty = true;
                }
                break;
            }
        }

        /* Clock has to stay honest even when nothing else changes. */
        {
            time_t now = time(NULL);
            struct tm tmv;
            localtime_r(&now, &tmv);
            if (tmv.tm_min != s_lastMinute) s_dirty = true;
        }

        if (s_dirty && (!gameRunning || g_screen == SCREEN_OVERLAY)) Draw();
    }

    /* Opened from the main loop while a game is running. */
    void OpenOverlay()
    {
        g_ovSel = 0;
        g_ovPage = OVP_MAIN;
        s_prevOvPage = OVP_MAIN;    /* or the first Draw() reads a page change
                                       left over from the last time it was open */
        BuildOverlayRows();
        g_screen = SCREEN_OVERLAY;
        s_holdAction = -1;
        s_holdFrames = 0;
        s_dirty = true;
        Draw();
    }

    bool OverlayOpen() { return g_screen == SCREEN_OVERLAY; }

    /* Called every frame by the game loop as well as by Loop(), so a pulse
       started on the last overlay frame still gets its stop packet. */
    void TickHaptics() { RumbleTick(); }


    /* Called by the main loop after a game tears down and the library has been
       redrawn, so the return reads as a transition rather than a hard cut. */
    void TransitionIn() { FadeIn(); }

    void Init()
    {
        MenuState = GAMESELECT;

        /* Deliberately not another padConfigureInput(). main() has already run
           it once for every player this app will ever use; calling it again
           here with a different player count - and a third time in
           nxInitializeJoystick() with a fourth - rewrites the console's
           supported-controller set three times during startup, each call
           disagreeing with the last about how many pads exist. It is a
           configure-once call. */
        padInitializeDefault(&s_uiPad);
        hidInitializeTouchScreen();
        padInitialize(&s_playerPad[0], HidNpadIdType_No1, HidNpadIdType_Handheld);
        padInitialize(&s_playerPad[1], HidNpadIdType_No2);
        s_playerPadUp = true;

        for (int i = 0; i < kArtSlots; i++) { g_art[i].driver = -1; g_art[i].tried = false; }

        romfsInit();
        if (!Gfx::init())
        {
            /* Nothing can be drawn, so there is nothing useful left to do. */
            romfsExit();
            exit(1);
        }
        setsysExit();

        EnsureDirs();
        CfgLoad();
        StatsLoad();

        s_psmUp = R_SUCCEEDED(psmInitialize());
        s_soundUp = nx_ui_sound_init() != 0;

        atexit(RumbleAtExit);

        /* Both targets, not one or the other.

           This used to take Handheld and fall back to No1 only if that failed,
           binding the handles once at startup and keeping them for the life of
           the process. Docking or undocking moves the player between those two
           npad IDs, so after the first dock change the handles addressed a
           device that was no longer the one being held - every stop packet went
           somewhere nobody was listening, and a pulse started before the change
           ran until something else on the console cleared it. Holding both sets
           costs four handles and removes the question. */
        s_vibUp    = R_SUCCEEDED(hidInitializeVibrationDevices(s_vib, 2, HidNpadIdType_Handheld,
                                                              HidNpadStyleSet_NpadStandard));
        s_vibPadUp = R_SUCCEEDED(hidInitializeVibrationDevices(s_vibPad, 2, HidNpadIdType_No1,
                                                               HidNpadStyleSet_NpadStandard));

        sprintf(RomCountText, "%d of %lu drivers have ROMs",
                romList.AvRoms(), (unsigned long)romList.totalMAMEGames);

        BuildRows();
        RebuildList();
        SyncPage();

        s_dirty = true;
        Draw();
        Gfx::flush();
    }
}
