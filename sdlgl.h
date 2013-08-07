#ifndef __SDLGL_H
#define __SDLGL_H

/* (k) ALL RIGHTS REVERSED - Reprint what you like */

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define FPS 30

extern GLuint texture[];

extern void init_sdlgl(int w, int h, int fullscreen, const char *filename, const char *title);
extern void cleanup_sdlgl(void);
extern void save_screenshot(int num, int w, int h);
extern void wait_for_next_frame(void);

#endif /* __SDLGL_H */
