
#pragma once
 
#include <switch.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
 
 
using namespace std;
 
namespace Gfx
{
    struct RGBA
    {
        u8 R;
        u8 G;
        u8 B;
        u8 A;
    };

    struct Image
    {
        unsigned char *Pixels;
        int Width;
        int Height;
        Image() : Pixels(NULL), Width(0), Height(0) {}
    };
    
    bool init();
    void drawPixel(u32 X, u32 Y, RGBA Color);
    void drawRectangle(u32 X, u32 Y, u32 Width, u32 Height, RGBA Color);
    void drawRoundedRectangle(int X, int Y, int Width, int Height, int Radius, RGBA Color);
    void drawOutlineRectangle(int X, int Y, int Width, int Height, int Thickness, RGBA Color);
    /* Focus ring for anything drawn with drawRoundedRectangle - same corner
       curve, so the ring sits on the card instead of cutting its corners. */
    void drawRoundedOutlineRectangle(int X, int Y, int Width, int Height, int Radius, int Thickness, RGBA Color);
    void drawVerticalGradient(int X, int Y, int Width, int Height, RGBA Top, RGBA Bottom);

    /* Glass material.

       The look is built from gradients, thin borders and specular highlights
       rather than from blur - there is no realtime blur here to spend, and a
       CPU rasteriser at 720p cannot afford one. Each of these is a per-row
       fill through the same rounded-corner inset the solid version uses, so a
       panel, its gloss and its shadow all share one silhouette. */

    /* Rounded rect filled with a vertical gradient. The base of every glass
       surface: a pale panel a little brighter at the top than the bottom. */
    void drawRoundedGradient(int X, int Y, int Width, int Height, int Radius,
                             RGBA Top, RGBA Bottom);

    /* Specular sheen down the top GlossHeight rows, fading white to nothing.
       This is what reads as polished plastic rather than paint. */
    void drawRoundedGloss(int X, int Y, int Width, int Height, int Radius,
                          int GlossHeight, u8 Alpha);

    /* Soft drop shadow: a few concentric rounded rects offset down, each
       fainter than the last. Cheap, and at these sizes indistinguishable from
       a blurred one. */
    void drawRoundedShadow(int X, int Y, int Width, int Height, int Radius,
                           int OffsetY, int Spread, u8 Alpha);
    bool loadImage(const string &Path, Image &Result);
    void freeImage(Image &Target);
    void drawImage(const Image &Source, int X, int Y, int Width, int Height,
        u8 Alpha, bool Cover);
    void drawText(u32 X, u32 Y, string Text, RGBA Color, u32 Size);   
    u32 textWidth(string Text, u32 Size);
    void flush();
    void flushOverlay();
    void clear(RGBA Color);
    void exit();
	void drawBgImage();

    /* Corner brackets - a focus cue that still reads for a colour-blind player
       and on a washed-out handheld screen, unlike a tint. */
    void drawCorners(int X, int Y, int Width, int Height, int Len, int Thickness, RGBA Color);

    /* Text helpers built on drawText/textWidth. */
    void drawTextClipped(int X, int Y, const string &Text, RGBA Color, u32 Size, int MaxW);
    void drawTextCentered(int CX, int Y, const string &Text, RGBA Color, u32 Size, int MaxW);
    /* Greedy word wrap to MaxW, at most MaxLines rows. Returns rows drawn. */
    int  drawTextWrapped(int X, int Y, const string &Text, RGBA Color, u32 Size,
                         int MaxW, int MaxLines);

    /* Keep a copy of the framebuffer and paint it back later. The backdrop is
       fixed but expensive - a gradient is a per-row fill over the whole screen -
       so draw it once and memcpy after. */
    void snapshot();
    bool restore();
    void dropSnapshot();

    /* Transitions keep their own buffer. snapshot()/restore() above is the
       backdrop cache and is live across every repaint - a fade borrowing it
       would evict the backdrop and force a full gradient redraw afterwards.
       captureFade() grabs the current screen; fadeFromCapture() paints it back
       darkened by Level (0 unchanged, 255 black) in one sequential pass.

       Done instead as restore() plus a full-screen translucent fill, a fade
       frame costs a 3.5 MB memcpy and ~920k blends through the per-pixel path,
       which does not hold 60fps on this hardware. */
    void captureFade();
    bool fadeFromCapture(u8 Level);
    void dropFade();

    /* Draw state, applied by every primitive that touches the framebuffer.
       Transitions slide and fade whole panels and a smooth-scrolling list has
       to be cut off at its panel edge; threading an offset, an alpha and a clip
       rect through every call site is how half of them end up not honouring it.

       clear() and drawBgImage() deliberately ignore all three - they are the
       full-screen backdrop under whatever is being moved. */
    struct DrawState { int ox, oy; u8 alpha; int cx, cy, cw, ch; };

    DrawState pushState();                      /* returns the state to restore */
    void popState(const DrawState &Saved);
    void translate(int DX, int DY);             /* relative to the current origin */
    void fadeBy(u8 Alpha);                      /* multiplies the current alpha */
    void clipTo(int X, int Y, int Width, int Height);   /* intersects current clip */

    static u8 *Framebuffer;
    static u32 FrameWidth;
    static u32 FrameHeight;
}
