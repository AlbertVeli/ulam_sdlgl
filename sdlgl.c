/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * SDL/OpenGL initialization/cleanup stuff.
 *
 * The SDL OpenGL intialization and texture loading comes from NeHe
 * OpenGL lesson09 (by Jeff Molofee '99, ported to SDL by Sam Latinga
 * '2000).
 *
 *
 * Prickle-Prickle, the 73rd day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <SDL.h>
#include <string.h>
#include <stdlib.h>

#include "sdlgl.h"

/* storage for one texture  */
GLuint texture[1];

/* Screenshot pixel data */
static GLubyte *ssdata = NULL;


/* From lesson09 NeHe OpenGL tutorial */
static SDL_Surface *load_bitmap(const char *filename)
{
   Uint8 *rowhi, *rowlo;
   Uint8 *tmpbuf, tmpch;
   SDL_Surface *image;
   int i, j;

   image = SDL_LoadBMP(filename);
   if (!image) {
      fprintf(stderr, "Unable to load %s: %s\n", filename, SDL_GetError());
      return NULL;
   }

   /* GL surfaces are upsidedown and RGB, not BGR :-) */
   tmpbuf = (Uint8 *)malloc(image->pitch);
   if (!tmpbuf) {
      fprintf(stderr, "Out of memory\n");
      return NULL;
   }
   rowhi = (Uint8 *)image->pixels;
   rowlo = rowhi + (image->h * image->pitch) - image->pitch;
   for (i = 0; i < image->h/2; i++) {
      for (j = 0; j < image->w; j++) {
         tmpch = rowhi[j*3];
         rowhi[j*3] = rowhi[j*3+2];
         rowhi[j*3+2] = tmpch;
         tmpch = rowlo[j*3];
         rowlo[j*3] = rowlo[j*3+2];
         rowlo[j*3+2] = tmpch;
      }
      memcpy(tmpbuf, rowhi, image->pitch);
      memcpy(rowhi, rowlo, image->pitch);
      memcpy(rowlo, tmpbuf, image->pitch);
      rowhi += image->pitch;
      rowlo -= image->pitch;
   }
   free(tmpbuf);

   return image;
}


/* From lesson09 NeHe OpenGL tutorial */
static void load_textures(const char *filename)
{
   SDL_Surface *image1;

   image1 = load_bitmap(filename);
   if (!image1) {
      SDL_Quit();
      exit(EXIT_FAILURE);
   }

   /* Create Texture */
   glGenTextures(1, &texture[0]);
   glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

   /* 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, */
   /* border 0 (normal), rgb color data, unsigned byte data, and finally the data itself. */
   glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->w, image1->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->pixels);
   /* Valgrind wants us to free here, so lets do it.
    * (Does glTexImage2D copy the pixel data?)
    */
   SDL_FreeSurface(image1);
}


/* Partly from lesson09 NeHe OpenGL tutorial */
static void init_gl(int w, int h, const char *filename)
{
   glViewport(0, 0, w, h);
   if (filename) {
      load_textures(filename);
   }
   glEnable(GL_TEXTURE_2D);
   glClearColor(0, 0, 0, 0);     /* Clear bg to black */
   glClearDepth(1.0);
   glShadeModel(GL_SMOOTH);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity(); /* Reset The Projection Matrix */

   gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);

   glMatrixMode(GL_MODELVIEW);

   /* setup blending */
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   glEnable(GL_BLEND);
}


void init_sdlgl(int w, int h, int fullscreen, const char *filename, const char *title)
{
   Uint32 flags;

   /* Initialize SDL and OpenGL */
   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
   }
   flags = SDL_OPENGL;
   if (fullscreen) {
      flags |= SDL_FULLSCREEN;
   }
   if (SDL_SetVideoMode(w, h, 0, flags) == NULL ) {
      fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
      SDL_Quit();
      exit(EXIT_FAILURE);
   }

   /* title bar */
   SDL_WM_SetCaption(title, NULL);

   init_gl(w, h, filename);
}


void cleanup_sdlgl(void)
{
   if (ssdata) {
      free(ssdata);
      ssdata = NULL;
   }
   SDL_Quit();
}


void save_screenshot(int num, int w, int h)
{
   char buf[128];
   SDL_Surface *shot;

   if (ssdata == NULL) {
      ssdata = malloc(3 * w * h);
      if (!ssdata) {
         fprintf(stderr, "Unable to allocate memory for screenshot\n");
         exit(EXIT_FAILURE);
      }
   }
   glReadBuffer(GL_BACK);
   glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, ssdata);
   /* masks for little endian */
   shot = SDL_CreateRGBSurfaceFrom(ssdata, w, h, 24, w * 3, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
   snprintf(buf, 128, "video/s%05d.bmp", num);
   SDL_SaveBMP(shot, buf);
   SDL_FreeSurface(shot); /* pixeldata (ssdata) is not freed */
}


void wait_for_next_frame(void)
{
   static Uint32 next = 0;
   Uint32 curr;
   int delay;

   if (next != 0) {
      /* Not first, wait for next frame */
      curr = SDL_GetTicks();
      delay = next - curr;
      if (delay > 0) {
         /* printf("%f\n", zoom); */
         SDL_Delay(delay);
      }
   }
   next = SDL_GetTicks() + (1000 / (float)FPS);
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
