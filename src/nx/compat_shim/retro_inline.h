#ifndef __LIBRETRO_SDK_INLINE_H
#define __LIBRETRO_SDK_INLINE_H

#ifndef INLINE
#if defined(__STDC_VERSION__) && __STDC_VERSION__>=199901L
#define INLINE inline
#elif defined(__GNUC__)
#define INLINE __inline__
#else
#define INLINE
#endif
#endif

#endif
