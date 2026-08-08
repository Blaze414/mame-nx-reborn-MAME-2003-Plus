#include "Gfx.hpp"
#include <string.h>
#include <stdlib.h>

#include "stb_image.h"

extern "C" {
	/* Read and cleared by menu_flush() in nx_display.c. The UI texture is a
	   3.5 MB upload; doing it on a frame where nothing was drawn is ~220 MB/s
	   of memory traffic to re-send an identical image, which costs both frame
	   time and battery while the library just sits there. Set by every path
	   that writes Framebuffer, so a new primitive cannot silently skip it. */
	int menu_pixels_dirty = 1;
	int menu_create_display();
	void menu_render();
	void menu_flush();	 
	void menu_flush_overlay();
	void menu_cleanup();	 
	extern unsigned char menupixels[];
	extern unsigned char *bg;
}

typedef union
{
    u32 abgr;
    struct
    {
        u8 r,g,b,a;
    };
} color_t;

typedef struct
{
    u8 width, height;
    int8_t posX, posY, advance;
    const u8* data;
} glyph_t;

typedef struct
{
    u8 magic[4];
    int version;
    u16 npages;
    u8 height;
    u8 baseline;
} ffnt_header_t;

typedef struct
{
    u32 size, offset;
} ffnt_pageentry_t;

typedef struct
{
    u32 pos[0x100];
    u8 widths[0x100];
    u8 heights[0x100];
    int8_t advances[0x100];
    int8_t posX[0x100];
    int8_t posY[0x100];
} ffnt_pagehdr_t;

typedef struct
{
    ffnt_pagehdr_t hdr;
    u8 data[];
} ffnt_page_t;

extern const ffnt_header_t interuiregular20_nxfnt;
 

static u8 bcolor(u8 dst, u8 src, u8 alpha)
{
    return (u8)(((u32)src * alpha + (u32)dst * (255 - alpha) + 127) / 255);
}

/* Origin, global alpha and clip rect. The clip starts wider than any panel so
   an uninitialised state cannot silently swallow drawing. */
static Gfx::DrawState s_draw = { 0, 0, 255, -4096, -4096, 65536, 65536 };

Gfx::DrawState Gfx::pushState() { return s_draw; }
void Gfx::popState(const Gfx::DrawState &Saved) { s_draw = Saved; }

void Gfx::translate(int DX, int DY) { s_draw.ox += DX; s_draw.oy += DY; }

void Gfx::fadeBy(u8 Alpha)
{
    s_draw.alpha = (u8)(((u32)s_draw.alpha * Alpha + 127) / 255);
}

void Gfx::clipTo(int X, int Y, int Width, int Height)
{
    /* In the caller's coordinates, like everything else, so a clip set up
       inside a translated panel moves with the panel. */
    int l = std::max(s_draw.cx, X + s_draw.ox);
    int t = std::max(s_draw.cy, Y + s_draw.oy);
    int r = std::min(s_draw.cx + s_draw.cw, X + s_draw.ox + Width);
    int b = std::min(s_draw.cy + s_draw.ch, Y + s_draw.oy + Height);

    s_draw.cx = l;
    s_draw.cy = t;
    s_draw.cw = std::max(0, r - l);
    s_draw.ch = std::max(0, b - t);
}

static void pix(int x, int y, u8 r, u8 g, u8 b, u8 a)
{
    x += s_draw.ox;
    y += s_draw.oy;
    if(!Gfx::Framebuffer || x < 0 || y < 0 ||
       x >= (int)Gfx::FrameWidth || y >= (int)Gfx::FrameHeight) return;
    if(x < s_draw.cx || y < s_draw.cy ||
       x >= s_draw.cx + s_draw.cw || y >= s_draw.cy + s_draw.ch) return;
    if(s_draw.alpha != 255)
    {
        a = (u8)(((u32)a * s_draw.alpha + 127) / 255);
        if(!a) return;
    }
    menu_pixels_dirty = 1;
    size_t off = ((size_t)y * Gfx::FrameWidth + (u32)x) * 4;
    Gfx::Framebuffer[off] = bcolor(Gfx::Framebuffer[off], r, a);
    Gfx::Framebuffer[off + 1] = bcolor(Gfx::Framebuffer[off + 1], g, a);
    Gfx::Framebuffer[off + 2] = bcolor(Gfx::Framebuffer[off + 2], b, a);
    Gfx::Framebuffer[off + 3] = 0xff;
}

static void fillRectangle(int x, int y, int width, int height, Gfx::RGBA color)
{
    if(!Gfx::Framebuffer || width <= 0 || height <= 0) return;

    x += s_draw.ox;
    y += s_draw.oy;

    if(s_draw.alpha != 255)
    {
        color.A = (u8)(((u32)color.A * s_draw.alpha + 127) / 255);
        if(!color.A) return;
    }

    int left = std::max(std::max(0, x), s_draw.cx);
    int top = std::max(std::max(0, y), s_draw.cy);
    int right = std::min(std::min((int)Gfx::FrameWidth, x + width), s_draw.cx + s_draw.cw);
    int bottom = std::min(std::min((int)Gfx::FrameHeight, y + height), s_draw.cy + s_draw.ch);
    if(left >= right || top >= bottom) return;

    menu_pixels_dirty = 1;

    for(int py = top; py < bottom; ++py)
    {
        u8 *row = Gfx::Framebuffer + ((size_t)py * Gfx::FrameWidth + left) * 4;
        for(int px = left; px < right; ++px, row += 4)
        {
            if(color.A == 255)
            {
                row[0] = color.R;
                row[1] = color.G;
                row[2] = color.B;
            }
            else if(color.A != 0)
            {
                row[0] = bcolor(row[0], color.R, color.A);
                row[1] = bcolor(row[1], color.G, color.A);
                row[2] = bcolor(row[2], color.B, color.A);
            }
            row[3] = 0xff;
        }
    }
}

const ffnt_page_t* FontGetPage(const ffnt_header_t* font, u32 page_id)
{
    if(page_id >= font->npages) return NULL;
    ffnt_pageentry_t* ent = &((ffnt_pageentry_t*)(font + 1))[page_id];
    if(ent->size == 0) return NULL;
    return (const ffnt_page_t*)((const u8*)font + ent->offset);
}

bool FontLoadGlyph(glyph_t* glyph, const ffnt_header_t* font, u32 codepoint)
{
    const ffnt_page_t* page = FontGetPage(font, codepoint >> 8);
    if(!page) return false;
    codepoint &= 0xFF;
    u32 off = page->hdr.pos[codepoint];
    if(off == ~(u32)0) return false;
    glyph->width = page->hdr.widths[codepoint];
    glyph->height = page->hdr.heights[codepoint];
    glyph->advance = page->hdr.advances[codepoint];
    glyph->posX = page->hdr.posX[codepoint];
    glyph->posY = page->hdr.posY[codepoint];
    glyph->data = &page->data[off];
    return true;
}

static int ScaleMetric(int value, u32 size, u32 baseSize)
{
    if(!baseSize) return value;
    int magnitude = (int)(((u32)(value < 0 ? -value : value) * size + baseSize / 2) / baseSize);
    return value < 0 ? -magnitude : magnitude;
}

static void DrawGlyph(int x, int y, color_t clr, const glyph_t* glyph, u32 size, u32 baseSize)
{
    if(!glyph->width || !glyph->height || !clr.a) return;
    int drawWidth = std::max(1, ScaleMetric(glyph->width, size, baseSize));
    int drawHeight = std::max(1, ScaleMetric(glyph->height, size, baseSize));
    const u8* data = glyph->data;
    const u8 requestedAlpha = clr.a;
    x += ScaleMetric(glyph->posX, size, baseSize);
    y += ScaleMetric(glyph->posY, size, baseSize);
    for(int j = 0; j < drawHeight; ++j)
    {
        int sourceY = j * glyph->height / drawHeight;
        for(int i = 0; i < drawWidth; ++i)
        {
            int sourceX = i * glyph->width / drawWidth;
            u32 coverage = data[sourceY * glyph->width + sourceX];
            /* 720p LCDs soften small anti-aliased strokes.  Drop near-invisible
             * fringe pixels and strengthen remaining glyph coverage. */
            if(coverage < 28) continue;
            coverage = std::min(255u, (coverage * 5 + 2) / 3);
            clr.a = (u8)((coverage * requestedAlpha + 127) / 255);
            if(!clr.a) continue;
            pix(x + i, y + j, clr.r, clr.g, clr.b, clr.a);
        }
    }
}

u8 DecodeByte(const char** ptr)
{
    u8 c = (u8)**ptr;
    *ptr += 1;
    return c;
}

int8_t DecodeUTF8Cont(const char** ptr)
{
    int c = DecodeByte(ptr);
    return ((c & 0xC0) == 0x80) ? (c & 0x3F) : -1;
}

u32 DecodeUTF8(const char** ptr)
{
    u32 r;
    u8 c;
    int8_t c1, c2, c3;

    c = DecodeByte(ptr);
    if((c & 0x80) == 0) return c;
    if((c & 0xE0) == 0xC0)
    {
        c1 = DecodeUTF8Cont(ptr);
        if(c1 >= 0)
        {
            r = ((c & 0x1F) << 6) | c1;
            if(r >= 0x80) return r;
        }
    }
    else if((c & 0xF0) == 0xE0)
    {
        c1 = DecodeUTF8Cont(ptr);
        if(c1 >= 0)
        {
            c2 = DecodeUTF8Cont(ptr);
            if(c2 >= 0)
            {
                r = ((c & 0x0F) << 12) | (c1 << 6) | c2;
                if(r >= 0x800 && (r < 0xD800 || r >= 0xE000)) return r;
            }
        }
    }
    else if((c & 0xF8) == 0xF0)
    {
        c1 = DecodeUTF8Cont(ptr);
        if(c1 >= 0)
        {
            c2 = DecodeUTF8Cont(ptr);
            if(c2 >= 0)
            {
                c3 = DecodeUTF8Cont(ptr);
                if(c3 >= 0)
                {
                    r = ((c & 0x07) << 18) | (c1 << 12) | (c2 << 6) | c3;
                    if (r >= 0x10000 && r < 0x110000) return r;
                }
            }
        }
    }
    return 0xFFFD;
}

static void DrawText_(const ffnt_header_t* font, int x, int y, color_t clr, const char* text,
                      u32 size, u32 max_width)
{
    u32 baseSize = font->height ? font->height : 20;
    if(!size) size = baseSize;
    y += ScaleMetric(font->baseline, size, baseSize);
    int origX = x;
    while(*text)
    {
        if(max_width && x - origX >= (int)max_width) break;

        glyph_t glyph;
        u32 codepoint = DecodeUTF8(&text);
        if(codepoint == '\n')
        {
            if(max_width) break;
            x = origX;
            y += ScaleMetric(font->height, size, baseSize);
            continue;
        }

        if(!FontLoadGlyph(&glyph, font, codepoint))
        {
            if(!FontLoadGlyph(&glyph, font, '?')) continue;
        }
        DrawGlyph(x, y, clr, &glyph, size, baseSize);
        x += ScaleMetric(glyph.advance, size, baseSize);
    } 
}

bool Gfx::init()
{
	if (menu_create_display() != 0)
		return false;
	Gfx::Framebuffer = menupixels;
	Gfx::FrameWidth = 1280;
	Gfx::FrameHeight = 720;
	return true;
}

void Gfx::drawPixel(u32 X, u32 Y, Gfx::RGBA Color)
{
    pix((int)X, (int)Y, Color.R, Color.G, Color.B, Color.A);
}

void Gfx::drawRectangle(u32 X, u32 Y, u32 Width, u32 Height, Gfx::RGBA Color)
{
    fillRectangle((int)X, (int)Y, (int)Width, (int)Height, Color);
}

/* How far in from the edge this row of a rounded rect starts. Shared so the
   filled and outlined versions round identically - an outline whose curve did
   not match the card underneath would read as a misalignment, not a style. */
static int roundedInset(int row, int height, int radius)
{
    int dy = 0;

    if(radius <= 0) return 0;
    if(row < radius) dy = radius - 1 - row;
    else if(row >= height - radius) dy = row - (height - radius);
    if(dy <= 0) return 0;

    const int radiusSquared = radius * radius;
    int dx = 0;
    while((dx + 1) * (dx + 1) + dy * dy <= radiusSquared) ++dx;
    return radius - dx;
}

void Gfx::drawRoundedRectangle(int X, int Y, int Width, int Height, int Radius, Gfx::RGBA Color)
{
    if(Width <= 0 || Height <= 0) return;
    Radius = std::max(0, std::min(Radius, std::min(Width, Height) / 2));
    if(Radius == 0)
    {
        fillRectangle(X, Y, Width, Height, Color);
        return;
    }

    for(int row = 0; row < Height; ++row)
    {
        int inset = roundedInset(row, Height, Radius);
        fillRectangle(X + inset, Y + row, Width - inset * 2, 1, Color);
    }
}

void Gfx::drawRoundedOutlineRectangle(int X, int Y, int Width, int Height,
                                      int Radius, int Thickness, Gfx::RGBA Color)
{
    if(Width <= 0 || Height <= 0 || Thickness <= 0) return;

    Radius    = std::max(0, std::min(Radius, std::min(Width, Height) / 2));
    Thickness = std::min(Thickness, std::min(Width, Height) / 2);

    if(Radius == 0 || Thickness <= 0)
    {
        drawOutlineRectangle(X, Y, Width, Height, Thickness, Color);
        return;
    }

    /* The hole is the same shape one thickness smaller, so its corner radius
       shrinks by the same amount - keeping Radius would make the border thin
       out around the curves. */
    const int innerHeight = Height - Thickness * 2;
    const int innerRadius = std::max(0, Radius - Thickness);

    for(int row = 0; row < Height; ++row)
    {
        int outer = roundedInset(row, Height, Radius);
        int span  = Width - outer * 2;
        int innerRow = row - Thickness;

        if(span <= 0) continue;

        /* Above and below the hole the border is solid across the row. */
        if(innerHeight <= 0 || innerRow < 0 || innerRow >= innerHeight)
        {
            fillRectangle(X + outer, Y + row, span, 1, Color);
            continue;
        }

        int inner = roundedInset(innerRow, innerHeight, innerRadius);
        int left  = Thickness + inner;
        int right = Width - Thickness - inner;

        if(left > outer)          fillRectangle(X + outer, Y + row, left - outer, 1, Color);
        if(Width - outer > right) fillRectangle(X + right, Y + row, (Width - outer) - right, 1, Color);
    }
}

void Gfx::drawRoundedGradient(int X, int Y, int Width, int Height, int Radius,
                              Gfx::RGBA Top, Gfx::RGBA Bottom)
{
    if(Width <= 0 || Height <= 0) return;
    Radius = std::max(0, std::min(Radius, std::min(Width, Height) / 2));

    const int denominator = std::max(1, Height - 1);

    for(int row = 0; row < Height; ++row)
    {
        int inset = roundedInset(row, Height, Radius);
        Gfx::RGBA color = {
            (u8)(((int)Top.R * (denominator - row) + (int)Bottom.R * row) / denominator),
            (u8)(((int)Top.G * (denominator - row) + (int)Bottom.G * row) / denominator),
            (u8)(((int)Top.B * (denominator - row) + (int)Bottom.B * row) / denominator),
            (u8)(((int)Top.A * (denominator - row) + (int)Bottom.A * row) / denominator)
        };

        fillRectangle(X + inset, Y + row, Width - inset * 2, 1, color);
    }
}

void Gfx::drawRoundedGloss(int X, int Y, int Width, int Height, int Radius,
                           int GlossHeight, u8 Alpha)
{
    if(Width <= 0 || Height <= 0 || GlossHeight <= 0 || !Alpha) return;
    Radius = std::max(0, std::min(Radius, std::min(Width, Height) / 2));

    if(GlossHeight > Height) GlossHeight = Height;

    for(int row = 0; row < GlossHeight; ++row)
    {
        /* Falls off with the square of the distance down, so the sheen is
           concentrated hard against the top edge instead of washing the whole
           upper half out - a flat ramp reads as fog, not as a highlight. */
        const int remain = GlossHeight - row;
        const u32 fade   = (u32)remain * (u32)remain;
        const u32 full   = (u32)GlossHeight * (u32)GlossHeight;
        int inset = roundedInset(row, Height, Radius);
        Gfx::RGBA color = { 255, 255, 255, (u8)(((u32)Alpha * fade) / full) };

        if(!color.A) continue;
        fillRectangle(X + inset, Y + row, Width - inset * 2, 1, color);
    }
}

void Gfx::drawRoundedShadow(int X, int Y, int Width, int Height, int Radius,
                            int OffsetY, int Spread, u8 Alpha)
{
    if(Width <= 0 || Height <= 0 || Spread <= 0 || !Alpha) return;

    /* Outermost ring first so the darker inner ones land on top. Each ring is
       one pixel wider all round than the last and proportionally fainter,
       which at a spread of a few pixels is a soft edge without a single blur
       kernel being run.

       Rings, not filled rects. The caller's panel covers the middle, so filling
       it is work thrown away - and thrown away fifteen times over on a grid of
       tiles, where it was the single most expensive thing on the screen. Each
       ring reaches inward far enough to cover the drop offset and meet the one
       inside it, which on a 148px tile is roughly a sixth of the pixels a fill
       would have touched. */
    for(int ring = Spread; ring >= 1; --ring)
    {
        Gfx::RGBA color = { 0, 0, 0, (u8)(((u32)Alpha * (u32)(Spread + 1 - ring)) / (u32)(Spread * 2)) };

        if(!color.A) continue;
        Gfx::drawRoundedOutlineRectangle(X - ring, Y + OffsetY - ring,
                                         Width + ring * 2, Height + ring * 2,
                                         Radius + ring,
                                         ring + OffsetY + 1, color);
    }
}

void Gfx::drawOutlineRectangle(int X, int Y, int Width, int Height, int Thickness, Gfx::RGBA Color)
{
    if(Width <= 0 || Height <= 0 || Thickness <= 0) return;
    Thickness = std::min(Thickness, std::min(Width, Height) / 2);
    if(Thickness <= 0)
    {
        fillRectangle(X, Y, Width, Height, Color);
        return;
    }
    fillRectangle(X, Y, Width, Thickness, Color);
    fillRectangle(X, Y + Height - Thickness, Width, Thickness, Color);
    fillRectangle(X, Y + Thickness, Thickness, Height - Thickness * 2, Color);
    fillRectangle(X + Width - Thickness, Y + Thickness, Thickness, Height - Thickness * 2, Color);
}

void Gfx::drawVerticalGradient(int X, int Y, int Width, int Height, Gfx::RGBA Top, Gfx::RGBA Bottom)
{
    if(Width <= 0 || Height <= 0) return;
    int denominator = std::max(1, Height - 1);
    for(int row = 0; row < Height; ++row)
    {
        Gfx::RGBA color = {
            (u8)(((int)Top.R * (denominator - row) + (int)Bottom.R * row) / denominator),
            (u8)(((int)Top.G * (denominator - row) + (int)Bottom.G * row) / denominator),
            (u8)(((int)Top.B * (denominator - row) + (int)Bottom.B * row) / denominator),
            (u8)(((int)Top.A * (denominator - row) + (int)Bottom.A * row) / denominator)
        };
        fillRectangle(X, Y + row, Width, 1, color);
    }
}

bool Gfx::loadImage(const string &Path, Gfx::Image &Result)
{
    Gfx::freeImage(Result);
    int channels = 0;
    Result.Pixels = stbi_load(Path.c_str(), &Result.Width, &Result.Height, &channels, 4);
    if(!Result.Pixels || Result.Width <= 0 || Result.Height <= 0)
    {
        Gfx::freeImage(Result);
        return false;
    }
    return true;
}

void Gfx::freeImage(Gfx::Image &Target)
{
    if(Target.Pixels)
        stbi_image_free(Target.Pixels);
    Target.Pixels = NULL;
    Target.Width = 0;
    Target.Height = 0;
}

void Gfx::drawImage(const Gfx::Image &Source, int X, int Y, int Width, int Height,
    u8 Alpha, bool Cover)
{
    if(!Source.Pixels || Source.Width <= 0 || Source.Height <= 0 ||
       Width <= 0 || Height <= 0 || !Alpha) return;

    X += s_draw.ox;
    Y += s_draw.oy;
    if(s_draw.alpha != 255)
    {
        Alpha = (u8)(((u32)Alpha * s_draw.alpha + 127) / 255);
        if(!Alpha) return;
    }

    int drawWidth = Width;
    int drawHeight = Height;
    int sourceX = 0;
    int sourceY = 0;
    int sourceWidth = Source.Width;
    int sourceHeight = Source.Height;

    if(Cover)
    {
        if((long long)Source.Width * Height > (long long)Source.Height * Width)
        {
            sourceWidth = Source.Height * Width / Height;
            sourceX = (Source.Width - sourceWidth) / 2;
        }
        else
        {
            sourceHeight = Source.Width * Height / Width;
            sourceY = (Source.Height - sourceHeight) / 2;
        }
    }
    else
    {
        if((long long)Source.Width * Height > (long long)Source.Height * Width)
            drawHeight = std::max(1, Source.Height * Width / Source.Width);
        else
            drawWidth = std::max(1, Source.Width * Height / Source.Height);
        X += (Width - drawWidth) / 2;
        Y += (Height - drawHeight) / 2;
    }

    /* Clip once against the framebuffer instead of bounds-checking every
       pixel inside pix(), and write opaque pixels straight through. Box art is
       almost entirely opaque, and the old path ran three blend calls plus a
       bounds test per pixel - fifteen tiles at 148px is ~330k pixels every
       repaint, which is the single largest cost of drawing the library. */
    if(!Gfx::Framebuffer) return;
    menu_pixels_dirty = 1;

    int startX = std::max(std::max(0, -X), s_draw.cx - X);
    int startY = std::max(std::max(0, -Y), s_draw.cy - Y);
    int endX   = std::min(std::min(drawWidth,  (int)Gfx::FrameWidth  - X),
                          s_draw.cx + s_draw.cw - X);
    int endY   = std::min(std::min(drawHeight, (int)Gfx::FrameHeight - Y),
                          s_draw.cy + s_draw.ch - Y);

    for(int dy = startY; dy < endY; ++dy)
    {
        int sy = sourceY + dy * sourceHeight / drawHeight;
        const unsigned char *srcRow = Source.Pixels + (size_t)sy * Source.Width * 4;
        u8 *dstRow = Gfx::Framebuffer +
                     ((size_t)(Y + dy) * Gfx::FrameWidth + (X + startX)) * 4;

        for(int dx = startX; dx < endX; ++dx, dstRow += 4)
        {
            int sx = sourceX + dx * sourceWidth / drawWidth;
            const unsigned char *pixel = srcRow + (size_t)sx * 4;
            u8 alpha = (Alpha == 255) ? pixel[3]
                                      : (u8)(((u32)pixel[3] * Alpha + 127) / 255);

            if(alpha == 255)
            {
                dstRow[0] = pixel[0];
                dstRow[1] = pixel[1];
                dstRow[2] = pixel[2];
            }
            else if(alpha)
            {
                dstRow[0] = bcolor(dstRow[0], pixel[0], alpha);
                dstRow[1] = bcolor(dstRow[1], pixel[1], alpha);
                dstRow[2] = bcolor(dstRow[2], pixel[2], alpha);
            }
            else continue;

            dstRow[3] = 0xff;
        }
    }
}

void Gfx::drawBgImage()
{
	if(!bg)
    {
        Gfx::drawVerticalGradient(0, 0, (int)FrameWidth, (int)FrameHeight,
                                  { 12, 15, 34, 255 }, { 35, 14, 57, 255 });
        return;
    }

    const u8 *raw = bg;
    for(u32 y = 0; y < FrameHeight; ++y)
    {
        u8 *dest = Framebuffer + (size_t)y * FrameWidth * 4;
        const u8 *source = raw + (size_t)y * FrameWidth * 3;
        for(u32 x = 0; x < FrameWidth; ++x)
        {
            *dest++ = *source++;
            *dest++ = *source++;
            *dest++ = *source++;
            *dest++ = 0xff;
        }
    }
}

void Gfx::drawText(u32 X, u32 Y, string Text, Gfx::RGBA Color, u32 Size)
{
    color_t clr;
    clr.r = Color.R;
    clr.g = Color.G;
    clr.b = Color.B;
    clr.a = Color.A;
    const ffnt_header_t* font = &interuiregular20_nxfnt;
   
	 
    DrawText_(font, (int)X, (int)Y, clr, Text.c_str(), Size, 0);
}

u32 Gfx::textWidth(string Text, u32 Size)
{
    const ffnt_header_t* font = &interuiregular20_nxfnt;
    u32 baseSize = font->height ? font->height : 20;
    if(!Size) Size = baseSize;
    const char *text = Text.c_str();
    int width = 0;
    int maxWidth = 0;
    while(*text)
    {
        u32 codepoint = DecodeUTF8(&text);
        if(codepoint == '\n')
        {
            maxWidth = std::max(maxWidth, width);
            width = 0;
            continue;
        }
        glyph_t glyph;
        if(!FontLoadGlyph(&glyph, font, codepoint) && !FontLoadGlyph(&glyph, font, '?'))
            continue;
        width += ScaleMetric(glyph.advance, Size, baseSize);
    }
    return (u32)std::max(maxWidth, width);
}


void Gfx::flush()
{
	menu_flush();
}

/* Present the paused game with the overlay blended on top. */
void Gfx::flushOverlay()
{
	menu_flush_overlay();
}

void Gfx::clear(Gfx::RGBA Color)
{
    if(!Framebuffer) return;
    menu_pixels_dirty = 1;
    for(u32 y = 0; y < FrameHeight; ++y)
    {
        u8 *row = Framebuffer + (size_t)y * FrameWidth * 4;
        for(u32 x = 0; x < FrameWidth; ++x, row += 4)
        {
            row[0] = Color.R;
            row[1] = Color.G;
            row[2] = Color.B;
            row[3] = Color.A;
        }
    }
}

void Gfx::exit()
{
	menu_cleanup();
    //gfxExit();
}

/*---------------------------------------------------------------------
    Port additions - focus brackets, text fitting, backdrop caching
---------------------------------------------------------------------*/

void Gfx::drawCorners(int X, int Y, int Width, int Height, int Len, int Thickness, Gfx::RGBA Color)
{
    if(Width <= 0 || Height <= 0 || Len <= 0 || Thickness <= 0) return;
    if(Len > Width / 2) Len = Width / 2;
    if(Len > Height / 2) Len = Height / 2;

    fillRectangle(X, Y, Len, Thickness, Color);
    fillRectangle(X, Y, Thickness, Len, Color);

    fillRectangle(X + Width - Len, Y, Len, Thickness, Color);
    fillRectangle(X + Width - Thickness, Y, Thickness, Len, Color);

    fillRectangle(X, Y + Height - Thickness, Len, Thickness, Color);
    fillRectangle(X, Y + Height - Len, Thickness, Len, Color);

    fillRectangle(X + Width - Len, Y + Height - Thickness, Len, Thickness, Color);
    fillRectangle(X + Width - Thickness, Y + Height - Len, Thickness, Len, Color);
}

void Gfx::drawTextClipped(int X, int Y, const string &Text, Gfx::RGBA Color, u32 Size, int MaxW)
{
    if(MaxW <= 0) return;

    if((int)Gfx::textWidth(Text, Size) <= MaxW)
    {
        Gfx::drawText((u32)X, (u32)Y, Text, Color, Size);
        return;
    }

    /* Trim from the end until the string plus its ellipsis fits. Titles are
       short and this runs once per tile, so a linear walk is fine. */
    string cut = Text;
    int dots = (int)Gfx::textWidth("...", Size);

    while(!cut.empty() && (int)Gfx::textWidth(cut, Size) + dots > MaxW)
        cut.erase(cut.size() - 1);

    Gfx::drawText((u32)X, (u32)Y, cut + "...", Color, Size);
}

void Gfx::drawTextCentered(int CX, int Y, const string &Text, Gfx::RGBA Color, u32 Size, int MaxW)
{
    int w = (int)Gfx::textWidth(Text, Size);

    if(w > MaxW)
    {
        Gfx::drawTextClipped(CX - MaxW / 2, Y, Text, Color, Size, MaxW);
        return;
    }

    Gfx::drawText((u32)(CX - w / 2), (u32)Y, Text, Color, Size);
}

static u8 *s_snapshot = NULL;

void Gfx::snapshot()
{
    size_t bytes = (size_t)Gfx::FrameWidth * Gfx::FrameHeight * 4;

    if(!Gfx::Framebuffer) return;
    if(!s_snapshot) s_snapshot = (u8 *)malloc(bytes);
    if(s_snapshot) memcpy(s_snapshot, Gfx::Framebuffer, bytes);
}

bool Gfx::restore()
{
    size_t bytes = (size_t)Gfx::FrameWidth * Gfx::FrameHeight * 4;

    if(!s_snapshot || !Gfx::Framebuffer) return false;
    memcpy(Gfx::Framebuffer, s_snapshot, bytes);
    menu_pixels_dirty = 1;
    return true;
}

static u8 *s_fadeCopy = NULL;

void Gfx::captureFade()
{
    size_t bytes = (size_t)Gfx::FrameWidth * Gfx::FrameHeight * 4;

    if(!Gfx::Framebuffer) return;
    if(!s_fadeCopy) s_fadeCopy = (u8 *)malloc(bytes);
    if(s_fadeCopy) memcpy(s_fadeCopy, Gfx::Framebuffer, bytes);
}

void Gfx::dropFade()
{
    free(s_fadeCopy);
    s_fadeCopy = NULL;
}

bool Gfx::fadeFromCapture(u8 Level)
{
    const size_t pixels = (size_t)Gfx::FrameWidth * Gfx::FrameHeight;
    const u32 keep = 255u - Level;

    if(!s_fadeCopy || !Gfx::Framebuffer) return false;
    menu_pixels_dirty = 1;

    if(Level == 0)   { memcpy(Gfx::Framebuffer, s_fadeCopy, pixels * 4); return true; }
    if(Level == 255) { memset(Gfx::Framebuffer, 0, pixels * 4); return true; }

    /* Always reads the capture, never the framebuffer - darkening in place
       would compound and the fade would fall off a cliff early. Alpha stays
       opaque: this buffer is uploaded as the whole screen. */
    for(size_t i = 0; i < pixels; ++i)
    {
        const size_t off = i * 4;
        Gfx::Framebuffer[off]     = (u8)((s_fadeCopy[off]     * keep) / 255u);
        Gfx::Framebuffer[off + 1] = (u8)((s_fadeCopy[off + 1] * keep) / 255u);
        Gfx::Framebuffer[off + 2] = (u8)((s_fadeCopy[off + 2] * keep) / 255u);
        Gfx::Framebuffer[off + 3] = 0xff;
    }
    return true;
}

void Gfx::dropSnapshot()
{
    free(s_snapshot);
    s_snapshot = NULL;
}

int Gfx::drawTextWrapped(int X, int Y, const std::string &Text, Gfx::RGBA Color,
                         u32 Size, int MaxW, int MaxLines)
{
    /* Line height tracks the glyph size rather than a constant, so wrapped
       body text at any size keeps the same rhythm as single-line text. */
    const int lineHeight = (int)(Size * 3 / 2);
    std::string line;
    size_t pos = 0;
    int rows = 0;

    if(MaxW <= 0 || MaxLines <= 0) return 0;

    while(pos <= Text.size() && rows < MaxLines)
    {
        size_t space = Text.find(' ', pos);
        bool   last  = (space == std::string::npos);
        std::string word = Text.substr(pos, (last ? Text.size() : space) - pos);
        std::string test = line.empty() ? word : line + " " + word;

        if(!line.empty() && (int)Gfx::textWidth(test, Size) > MaxW)
        {
            Gfx::drawText(X, Y + rows * lineHeight, line, Color, Size);
            rows++;
            line = word;
        }
        else line = test;

        if(last) break;
        pos = space + 1;
    }

    if(!line.empty() && rows < MaxLines)
    {
        Gfx::drawText(X, Y + rows * lineHeight, line, Color, Size);
        rows++;
    }
    return rows;
}
