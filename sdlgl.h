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

#ifndef UNUSED
#define UNUSED __attribute__ ((unused))
#endif

/* #define WIDTH 1280 */
/* #define HEIGHT 720 */
#define WIDTH 720
#define HEIGHT 576
#define FPS 30

extern GLuint texture[];
extern int save_screenshots;

void init_sdlgl(int w, int h, int fullscreen, const char *title);
void cleanup_sdlgl(void);
void save_screenshot(int num, int w, int h);


#endif /* __SDLGL_H */
