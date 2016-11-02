#pragma once

#if defined( __APPLE__ )
#define GLVIDEO_MAC
#elif defined( _WIN32 )
#define GLVIDEO_MSW
#endif


#ifdef GLVIDEO_MAC
  #include <OpenGL/gl.h>
  #include <OpenGL/OpenGL.h>
#else
  #ifdef GLVIDEO_MSW
    #include <windows.h>
  #endif
  #include <GL/gl.h>
#endif

#ifdef GLVIDEO_MSW
#include <cstdint>

typedef ptrdiff_t GLsizeiptr;
typedef struct __GLsync * GLsync;
typedef uint64_t GLuint64;
#endif