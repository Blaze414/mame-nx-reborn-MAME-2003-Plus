#include "osd_cpu.h"
#include "osdepend.h"
#include "mame2003.h"
#include "driver.h"
#include "usrintrf.h"
#include "nx_mame.h"
#include "nx_perf.h" 
#include "nx_joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <switch.h> 
 
#include <EGL/egl.h>    // EGL library
#include <EGL/eglext.h> // EGL extensions
#include <glad/glad.h>  // glad library (OpenGL loader)

#define GL_MATRIX_MODE				0x0BA0
#define GL_MODELVIEW				0x1700
#define GL_PROJECTION				0x1701
#define GL_TEXTURE					0x1702

#define GLM_FORCE_PURE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static int nTextureWidtssh = 0;
static int nTextureHeight = 0;

static INT32 frameCount = 0;
static float g_desiredFPS = 0.0f;
static struct osd_create_params		g_createParams = {0};

/* mirrors g_createParams - some drivers (segas32.c) read this directly,
   matching the RA video.c global of the same name that got deleted with
   the libretro glue dir. */
struct osd_create_params video_config = {0};

/* cpuexec.c's mame_frame() busy-waits on this (extern int gotFrame;) to
   know when one video frame's worth of CPU execution is done - set at the
   end of osd_update_video_and_audio() below. */
int gotFrame = 0;

/* vector-graphics drivers read this bare global (mame2003.h) instead of
   options.translucency directly - synced once on display creation. */
int translucency = 0;
int offsetx, offsety;
int newx, newy;

UINT32	g_pal32Lookup[65536] = {0};
 
// These will hold our original (pre filtered/scaled) width/height
static int                        g_OrigRenderWidth;
static int                        g_OrigRenderHeight;
 
/* Sized per driver in osd_create_display(): the palettized path writes one
   UINT32 per pixel, so a 288x224 game already needs 258KB. The fixed 100000
   this used to be overflowed on the first frame of every game. */
static unsigned char *pixels;
unsigned char menupixels[0x384000];
unsigned char *bg;

//-----------------------------------------------------------------------------
// EGL initialization
//-----------------------------------------------------------------------------

static EGLDisplay s_display;
static EGLContext s_context;
static EGLSurface s_surface;


static inline int VidGetTextureSize(int size)
{
	int textureSize = 128;
	while (textureSize < size) {
		textureSize <<= 1;
	}
	return textureSize;
}

/* The dirty rect is in BITMAP coordinates (mame.c:1011 hands us
   Machine->visible_area, whose min_x is 64 on CPS2), while pixels[] is sized
   from params.width, which is the visible area's WIDTH (mame.c:610). The
   render helpers already handle that by offsetting only the source and
   writing dest from column 0 - so the thing to bound is the copy length
   against the dest, never the coordinates themselves. Clamping max_x to
   g_OrigRenderWidth cut CPS2 from 384 columns to 320 and left the right of
   the screen black. set_visible_area() can still shrink or grow the area
   after creation, which is what makes a bound necessary at all. */
static void clamp_bounds( struct rectangle *b )
{
	if (b->min_x < 0) b->min_x = 0;
	if (b->min_y < 0) b->min_y = 0;
	if (b->max_x - b->min_x > g_OrigRenderWidth)  b->max_x = b->min_x + g_OrigRenderWidth;
	if (b->max_y - b->min_y > g_OrigRenderHeight) b->max_y = b->min_y + g_OrigRenderHeight;
}

static void Helper_RenderDirect16( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds )
{
	struct rectangle bounds = *bnds;
	++bounds.max_x;
	++bounds.max_y;
	clamp_bounds( &bounds );

	UINT16 *destBuffer;
	UINT16 *sourceBuffer = (UINT16*)bitmap->base;
  
	destBuffer = (UINT16*)dest;
 
	sourceBuffer += (bounds.min_y * bitmap->rowpixels) + bounds.min_x;
    destBuffer += (g_OrigRenderWidth);
	
	/* destBuffer/sourceBuffer are UINT16* here, so a row is width*2 bytes.
	   This was << 2, which copied twice the row and ran off both ends. */
	UINT32 scanLen = (bounds.max_x - bounds.min_x) * sizeof(*destBuffer);

		for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
		{
			memcpy( destBuffer, sourceBuffer, scanLen );
			destBuffer += g_OrigRenderWidth;
			sourceBuffer += bitmap->rowpixels;
		}

 
}


static void Helper_RenderPalettized16( void *dest, struct mame_bitmap *bitmap, const struct rectangle *bnds )
{
	struct rectangle bounds = *bnds;
	++bounds.max_x;
	++bounds.max_y;
	clamp_bounds( &bounds );

	UINT32 *destBuffer;
	UINT16 *sourceBuffer = (UINT16*)bitmap->base;
  
	destBuffer = (UINT32*)dest;
 
	sourceBuffer += (bounds.min_y * bitmap->rowpixels) + bounds.min_x;		 
	destBuffer += (g_OrigRenderWidth);
	 
	for( UINT32 y = bounds.min_y; y < bounds.max_y; ++y )
	{
		UINT32	*offset = destBuffer;
		UINT16  *sourceOffset = sourceBuffer;

		for( UINT32 x = bounds.min_x; x < bounds.max_x; ++x )
		{
			// Offset is in RGBX format	
			*(offset++) = g_pal32Lookup[ *(sourceOffset++) ];
		}
		
		destBuffer += g_OrigRenderWidth;
		sourceBuffer += bitmap->rowpixels;
		
		 
	}
 
}

bool initEgl(NWindow *win)
{
    // Connect to the EGL default display
	
    s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (!s_display)
    {
        //TRACE("Could not connect to display! error: %d", eglGetError());
        goto _fail0;
    }

    // Initialize the EGL display connection
    eglInitialize(s_display, NULL, NULL);

    // Select OpenGL (Core) as the desired graphics API
    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
    {
        //TRACE("Could not set API! error: %d", eglGetError());
        goto _fail1;
    }

     // Get an appropriate EGL framebuffer configuration
    EGLConfig config;
    EGLint numConfigs;
    static const EGLint framebufferAttributeList[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE,     8,
        EGL_GREEN_SIZE,   8,
        EGL_BLUE_SIZE,    8,
        EGL_ALPHA_SIZE,   8,
        EGL_DEPTH_SIZE,   24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };
	
    eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
    if (numConfigs == 0)
    {
        //TRACE("No config found! error: %d", eglGetError());
        goto _fail1;
    }

    /* Allocate the framebuffer at docked size up front and select the active
       region with nwindowSetCrop() later. Resizing the window after the EGL
       surface exists would reallocate buffers mid-flight on every dock and
       undock; cropping a 1080p buffer costs ~8MB and never reallocates. */
    nwindowSetDimensions(win, 1920, 1080);

    // Create an EGL window surface
    s_surface = eglCreateWindowSurface(s_display, config, win, NULL);
    if (!s_surface)
    {
        //TRACE("Surface creation failed! error: %d", eglGetError());
        goto _fail1;
    }

    // Create an EGL rendering context
    static const EGLint contextAttributeList[] =
    {
        EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
        EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
        EGL_CONTEXT_MINOR_VERSION_KHR, 3,
        EGL_NONE
    };
    s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
    if (!s_context)
    {
        //TRACE("Context creation failed! error: %d", eglGetError());
        goto _fail2;
    }

    // Connect the context to the surface
    eglMakeCurrent(s_display, s_surface, s_surface, s_context);

    /* Do not block on vsync. We already pace every frame to the DRIVER's rate
       in nx_pace_frame(), and MAME rates are not 60: MK2 is 53.205, CPS2
       59.633, Pac-Man 60.606. With the default interval of 1 both pacers run,
       and a 18.79ms driver frame then has to wait out a second 16.67ms vsync -
       MK2 was being pinned to 30fps with its audio starved to match. The
       compositor still presents at 60Hz, so this costs no tearing; it only
       stops the swap from adding up to a frame of input-to-photon delay. */
    eglSwapInterval(s_display, 0);

    return true;

_fail2:
    eglDestroySurface(s_display, s_surface);
    s_surface = NULL;
_fail1:
    eglTerminate(s_display);
    s_display = NULL;
_fail0:
    return false;
}

void deinitEgl()
{
    if (s_display)
    {
		eglMakeCurrent(s_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		  
        if (s_context)
        {
            eglDestroyContext(s_display, s_context);
            s_context = NULL;
        }
        if (s_surface)
        {
            eglDestroySurface(s_display, s_surface);
            s_surface = NULL;
        }
        
		eglTerminate(s_display);
        
		s_display = NULL;
    }
}

//-----------------------------------------------------------------------------
// Main program
//-----------------------------------------------------------------------------

static const char* const vertexShaderSource = R"text(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)text";

static const char* const fragmentShaderSource = R"text(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}
)text";


static const char* const fragmentMenuShaderSource = R"text(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 texel0, texel1, resultColor;
	
	texel0 = texture2D(texture1, TexCoord);
    texel1 = texture2D(texture2, TexCoord);
	
	resultColor = mix(texel0, texel1, texel0.a);
	FragColor = resultColor;
}
)text";

static GLuint createAndCompileShader(GLenum type, const char* source)
{
    GLint success;
    GLchar msg[512];

    GLuint handle = glCreateShader(type);
    if (!handle)
    {
        //TRACE("%u: cannot create shader", type);
        return 0;
    }
    glShaderSource(handle, 1, &source, NULL);
    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(handle, sizeof(msg), NULL, msg);
        //TRACE("%u: %s\n", type, msg);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
}
 
float rotvertices[] = {
    // positions          // colors           // texture coords
	-0.4f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // top left 
     0.4f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.4f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // bottom right
    -0.4f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left    
};

float rotvertices90[] = {
    // positions          // colors           // texture coords
	 0.4f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // top left 
    -0.4f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    -0.4f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // bottom right
     0.4f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left    
};
 
unsigned int indices[] = {  
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};
	
static GLuint s_program;
static GLuint s_menuProgram;
static unsigned int VBO, VAO, EBO;
static unsigned int menuVBO, menuVAO, menuEBO;
static GLuint s_tex;
static int    s_texDirect;   /* which format s_tex's storage was allocated with */
extern int    menu_pixels_dirty;   /* gfx.cpp */
static GLuint texture1;
static GLuint texture2;
 
int menu_create_display()
{
 
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
	};
	// Load OpenGL routines using glad
	
	initEgl(nwindowGetDefault());
	
    gladLoadGL();

	GLint vsh = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLint fsh = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    s_menuProgram = glCreateProgram();
    glAttachShader(s_menuProgram, vsh);
    glAttachShader(s_menuProgram, fsh);
    glLinkProgram(s_menuProgram);

    GLint success;
    glGetProgramiv(s_menuProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char buf[512];
        glGetProgramInfoLog(s_menuProgram, sizeof(buf), NULL, buf);
        return EXIT_FAILURE;
    }
    glDeleteShader(vsh);
    glDeleteShader(fsh);
 
    glGenVertexArrays(1, &menuVAO);
    glGenBuffers(1, &menuVBO);
    glGenBuffers(1, &menuEBO);

    glBindVertexArray(menuVAO);

    glBindBuffer(GL_ARRAY_BUFFER, menuVBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, menuEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
 
	int width, height, nchan;
 
	bg = stbi_load("romfs:/Graphics/mamelogo-nx.jpg", &width, &height, &nchan, 0);
	 
	glGenTextures(1, &texture1);	 
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, menupixels);
	glUseProgram(s_menuProgram);

	/* Every other path out of this function returns a status, but the success
	   path fell off the end - the returned value was whatever happened to be in
	   w0. Gfx::init() now tests it, so it has to be real. */
	return 0;
}

void menu_render()
{ 
 
}

/*---------------------------------------------------------------------
	nx_display_update_mode - follow dock/undock

	Handheld is 1280x720, docked 1920x1080. Both are 16:9, so the quad
	geometry is unaffected and only the pixel count changes. Cheap enough
	to call every present; it only touches GL when the mode actually moves.
---------------------------------------------------------------------*/
static void nx_display_update_mode(void)
{
	static u32 s_w = 0, s_h = 0;
	u32 w, h;

	if (appletGetOperationMode() == AppletOperationMode_Console)
	{
		w = 1920; h = 1080;
	}
	else
	{
		w = 1280; h = 720;
	}

	if (w == s_w && h == s_h)
		return;

	/* The crop rect is top-left origin, glViewport is bottom-left, and the
	   buffer stays 1080 tall in both modes - so a handheld viewport at y=0
	   lands in the bottom 720 rows while the crop reads the top 720, and only
	   the overlap shows. Offset the viewport to sit under the crop. */
	nwindowSetCrop(nwindowGetDefault(), 0, 0, w, h);
	glViewport(0, (GLsizei)(1080 - h), (GLsizei)w, (GLsizei)h);

	s_w = w;
	s_h = h;
}

/*---------------------------------------------------------------------
	nx_pace_frame - hold the frame to the driver's rate

	Sole pacer now that eglSwapBuffers no longer blocks. Must run on EVERY
	frame, including frameskipped ones: those carry no bitmap update, and
	while this lived inside the GAME_BITMAP_CHANGED branch they ran free -
	irregular frame times, and one audio submit per frame arriving at an
	irregular rate is exactly what a starved audout queue sounds like.
---------------------------------------------------------------------*/
#define SPIN_GUARD_NS 250000ULL

static void nx_pace_frame(double fps)
{
	static cycles_t lastFrameEndTime = 0;
	cycles_t persec = osd_cycles_per_second();
	cycles_t target;
	cycles_t now = osd_cycles();

	if (fps < 1.0)
		fps = 60.0;

	target = (cycles_t)((double)persec / (fps * 1.001));

	/* Catch wraparound (which won't happen for a long time :)) */
	if (lastFrameEndTime && now >= lastFrameEndTime)
	{
		cycles_t elapsed = now - lastFrameEndTime;

		if (elapsed < target)
		{
			/* Sleep off the bulk instead of spinning it away - a core held at
			   100% on V1 throttles, and a throttled core produces the late
			   frames this is here to prevent.

			   The spin window used to be a full millisecond, which at 60Hz is
			   6% of every core-second burnt for timing precision the audio
			   clock cannot even use. svcSleepThread overshoots by tens of
			   microseconds, not hundreds, so 250us of guard is ample and the
			   other 750us goes back to the sleep. */
			u64 ns = (u64)((double)(target - elapsed) * 1000000000.0 / (double)persec);

			if (ns > SPIN_GUARD_NS)
				svcSleepThread(ns - SPIN_GUARD_NS);

			for (;;)
			{
				now = osd_cycles();
				if (now < lastFrameEndTime || (now - lastFrameEndTime) >= target)
					break;
			}
		}
	}

	lastFrameEndTime = osd_cycles();
}

void menu_flush()
{
    nx_display_update_mode();
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture1);
	/* Sub-image, not TexImage2D: the storage was allocated once in
	   menu_create_display() and re-specifying it every frame made the driver
	   reallocate 3.5MB of texture per present. And only when something drew -
	   the browser presents every frame for pacing, but repaints only on
	   input, so most presents were re-uploading an identical 3.5MB image. */
	if (menu_pixels_dirty)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1280, 720, GL_RGBA, GL_UNSIGNED_BYTE, menupixels);
		menu_pixels_dirty = 0;
	}
	glUseProgram(s_menuProgram);
    glBindVertexArray(menuVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/* The browser needs pacing of its own now that the swap does not block -
	   otherwise this loop free-runs, burning the battery and making every
	   held-direction repeat fire at whatever rate the CPU happens to manage. */
	nx_pace_frame(60.0);

	eglSwapBuffers(s_display, s_surface);
}

/*---------------------------------------------------------------------
	menu_flush_overlay - present the paused game with the frontend's
	overlay composited on top.

	The game and the menu live in different textures with different
	programs, so the overlay cannot just be drawn into the game's buffer.
	Draw the game quad first, then the menu quad with blending on: the
	menu buffer carries alpha (Gfx::clear writes it verbatim), so the
	scrim dims the frame and the panels sit opaque over it.
---------------------------------------------------------------------*/
void menu_flush_overlay(void)
{
	nx_display_update_mode();
	glClear(GL_COLOR_BUFFER_BIT);

	/* game, as it was on the last presented frame */
	if (s_tex)
	{
		glUseProgram(s_program);
		glBindTexture(GL_TEXTURE_2D, s_tex);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	/* overlay */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture1);
	if (menu_pixels_dirty)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1280, 720, GL_RGBA, GL_UNSIGNED_BYTE, menupixels);
		menu_pixels_dirty = 0;
	}
	glUseProgram(s_menuProgram);
	glBindVertexArray(menuVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);

	nx_pace_frame(60.0);
	eglSwapBuffers(s_display, s_surface);
}

void menu_cleanup()
{
	
	if (bg)
	{
		stbi_image_free(bg);
		bg = NULL;
	}
 
	glDeleteBuffers(1, &menuVBO);
    glDeleteBuffers(1, &menuEBO); 	
    glDeleteVertexArrays(1, &menuVAO);   
	glDeleteProgram(s_menuProgram);
	
	deinitEgl();
}

static void nx_UpdatePalette( struct mame_display *display );

//---------------------------------------------------------------------
//	osd_create_display
//---------------------------------------------------------------------
int osd_create_display( const struct osd_create_params *params, UINT32 *rgb_components )
{	 
 
	float vertices[] = {
    // positions          // colors           // texture coords
     0.76f*((float)(3.0f/params->aspect_y)),  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
     0.76f*((float)(3.0f/params->aspect_y)), -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
    -0.76f*((float)(3.0f/params->aspect_y)), -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
    -0.76f*((float)(3.0f/params->aspect_y)),  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
	};
 
	// Store the creation params
	memcpy( &g_createParams, params, sizeof(g_createParams) );

	/* Validate and allocate BEFORE creating any GL object, so a failure here
	   returns without leaking a program, VAO, VBO or texture - the caller
	   (artwork.c) just propagates the error and never calls
	   osd_close_display() to clean up after us. */
	if (g_createParams.width <= 0 || g_createParams.height <= 0)
		return 1;

	/* One UINT32 per pixel - the palettized path is the wider of the two, and
	   sizing for it keeps a single buffer valid for both. The two extra rows
	   are because both Helper_Render* start one row in (destBuffer += width)
	   and the dirty rect is clamped, not trusted. */
	free(pixels);
	pixels = (unsigned char *)calloc((size_t)g_createParams.width *
									 ((size_t)g_createParams.height + 2),
									 sizeof(UINT32));
	if (!pixels)
		return 1;

    // Fill out the orientation from the game driver
	g_createParams.orientation = (Machine->gamedrv->flags & ORIENTATION_MASK);
	g_desiredFPS = params->fps;
	video_config = g_createParams;
	translucency = options.translucency;
 
    // Load OpenGL routines using glad
    gladLoadGL();
	
	GLint vsh = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLint fsh = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    s_program = glCreateProgram();
    glAttachShader(s_program, vsh);
    glAttachShader(s_program, fsh);
    glLinkProgram(s_program);

    GLint success;
    glGetProgramiv(s_program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char buf[512];
        glGetProgramInfoLog(s_program, sizeof(buf), NULL, buf);
        return EXIT_FAILURE;
    }
    glDeleteShader(vsh);
    glDeleteShader(fsh);
 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	if (g_createParams.orientation  & ORIENTATION_FLIP_Y)	
		glBufferData(GL_ARRAY_BUFFER, sizeof(rotvertices), rotvertices, GL_STATIC_DRAW);
	else if (g_createParams.orientation  & ORIENTATION_FLIP_X)
		glBufferData(GL_ARRAY_BUFFER, sizeof(rotvertices), rotvertices90, GL_STATIC_DRAW);	
	else
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
 

    // Textures
    glGenTextures(1, &s_tex);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, s_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Allocate the game texture's storage once, here, so the per-frame path
	   can sub-upload into it. It used to call glTexImage2D every frame, which
	   re-specifies the image and makes the driver orphan and recreate the
	   texture object sixty times a second. The format/type pair has to match
	   what the update actually sends, so it is chosen the same way. */
	s_texDirect = (g_createParams.video_attributes & VIDEO_RGB_DIRECT) != 0;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	             g_createParams.width, g_createParams.height, 0, GL_BGRA,
	             s_texDirect ? GL_UNSIGNED_SHORT_1_5_5_5_REV
	                         : GL_UNSIGNED_INT_8_8_8_8_REV, NULL);

	glUseProgram(s_program);
 
	set_ui_visarea( 0,0,0,0 );
 		
	if(Machine->color_depth == 32)
	{
		rgb_components[0] = 0xFF0000;
		rgb_components[1] = 0x00FF00;
		rgb_components[2] = 0x0000FF;
	}
	else  
	{       
		rgb_components[0] = 0x7C00;
		rgb_components[1] = 0x03E0;
		rgb_components[2] = 0x001F;  
	}
 
	  // Store our original width and height
	g_OrigRenderWidth  = g_createParams.width;
	g_OrigRenderHeight = g_createParams.height;

	return 0;
}

//---------------------------------------------------------------------
//	osd_close_display
//---------------------------------------------------------------------
void osd_close_display(void)
{
	
	// clean up opengl

	free(pixels);
	pixels = NULL;

	/* osd_create_display() glGenTextures() a fresh s_tex on every launch, so
	   without this each game played leaks one full-size GPU texture for the
	   life of the process. */
	glDeleteTextures(1, &s_tex);
	s_tex = 0;

	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO); 	
    glDeleteVertexArrays(1, &VAO);    	
	glDeleteProgram(s_program);
 	 
}

//---------------------------------------------------------------------
//	osd_skip_this_frame
//---------------------------------------------------------------------
int osd_skip_this_frame(void)
{
	/* mame_frame() consults this several times while running one frame, so
	   it has to be the value nx_perf latched at frame start, not a fresh
	   decision each call. */
	return nx_perf_skip_this_frame();
}

//---------------------------------------------------------------------
//	osd_update_video_and_audio
//---------------------------------------------------------------------
void osd_update_video_and_audio(struct mame_display *display)
{
	const struct performance_info *performance = mame_get_performance_info();

	/* Drop the input latch so the next frame reads the pad fresh. Here rather
	   than anywhere in the input code because this is the one function MAME
	   calls exactly once per frame whether or not the frame was skipped. */
	nx_joystick_frame();

	/* Outside the GAME_BITMAP_CHANGED branch below: the quad is drawn every
	   call, so the viewport has to be right even on a frame with no new
	   bitmap - otherwise docking during a skipped frame draws once at the
	   old size. */
	nx_display_update_mode();
	
	
   
	if( display->changed_flags & GAME_VISIBLE_AREA_CHANGED )
	{
				
			// Pass the new coords on to the UI
		set_ui_visarea( display->game_visible_area.min_x,
										display->game_visible_area.min_y,
										display->game_visible_area.max_x,
										display->game_visible_area.max_y );
	}

	if( display->changed_flags & GAME_PALETTE_CHANGED )
	{	
		nx_UpdatePalette( display );
	}
 
	
	if( display->changed_flags & GAME_BITMAP_CHANGED )
	{
		/* menu_flush() shares this GL context and leaves the menu program and
		   menu texture bound. Set our own every frame rather than relying on
		   what osd_create_display() left behind - otherwise a game frame gets
		   uploaded into the menu's texture and drawn with the menu's shader. */
		glUseProgram(s_program);
		glBindTexture(GL_TEXTURE_2D, s_tex);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if( g_createParams.video_attributes & VIDEO_RGB_DIRECT )
		{			
			Helper_RenderDirect16(pixels, display->game_bitmap, &display->game_bitmap_update );

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			                g_createParams.width, g_createParams.height,
			                GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, pixels);
		}
		else
		{
			// Have to translate the colors through the palette lookup table			 
			Helper_RenderPalettized16(pixels, display->game_bitmap, &display->game_bitmap_update );

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			                g_createParams.width, g_createParams.height,
			                GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
		}
	}

    // draw our textured cube
    glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	eglSwapBuffers(s_display, s_surface);

	/* Throttle AFTER presenting, not before. Held ahead of the swap, a frame
	   that finished emulating early sat completed-but-invisible for the whole
	   remainder of its slot, and the wait landed between the input poll and
	   the pixels it produced - up to a full frame of dead latency on every
	   frame. Presenting first ships the image the moment it is ready and moves
	   the wait to just before the next mame_frame(), so the poll that opens
	   that frame reads the pad as late as it possibly can. */
	nx_pace_frame(g_desiredFPS);

	/* mame_frame() (cpuexec.c) busy-waits on this to know one video
	   frame's worth of CPU execution has completed - matches the RA
	   video.c this replaced, which set it here too. */
	gotFrame = 1;
}


//============================================================
//	osd_override_snapshot
//============================================================

struct mame_bitmap *osd_override_snapshot(struct mame_bitmap *bitmap, struct rectangle *bounds)
{
	 
	return NULL;
}

 
const char *osd_get_fps_text( const struct performance_info *performance )
{
 
	return NULL;
}
 
  

//------------------------------------------------------------
//	nx_UpdatePalette
//------------------------------------------------------------

static void nx_UpdatePalette( struct mame_display *display )
{
	UINT32 i, j;
 
		// The game_palette_dirty entry is a bitflag specifying which
		// palette entries need to be updated

	for( i = 0, j = 0; i < display->game_palette_entries; i += 32, ++j )
	{
		UINT32 palDirty = display->game_palette_dirty[j];
		if( palDirty )
		{
			UINT32 idx = 0;
			for( ; idx < 32 && i + idx < display->game_palette_entries; ++idx )
			{
				if( palDirty & (1<<idx) )
					g_pal32Lookup[i+idx] = display->game_palette[i+idx];
			}

			display->game_palette_dirty[ j ] = 0;
		}
	}
}
