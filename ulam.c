/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * Draw Ulam Spiral using OpenGL + SDL
 *
 * The Ulam spiral is a method of visualizing the prime numbers, that
 * reveals the apparent tendency of certain quadratic polynomials to
 * generate unusually large numbers of primes. It was discovered by the
 * mathematician Stanislaw Ulam in 1963, while he was doodling during the
 * presentation of a "long and very boring paper" at a scientific
 * meeting.
 *
 * Read more: http://en.wikipedia.org/wiki/Ulam_spiral
 *
 * Rendered video: https://www.youtube.com/watch?v=7ZGqOF_6zso
 *
 * The video was rendered by first setting save_screenshots = 1 in the
 * source code and then using ffmpeg to assemble the screenshots into a
 * video file:
 *
 * ffmpeg -f image2 -i "s%05d.bmp" -c:v prores -an -r 30 ulam.mov
 *
 * My video editor prefers the prores codec, but you can use any video
 * codec ffmpeg supports, run ffmpeg -encoders to list available codecs
 * for video encoding.
 *
 * The SDL OpenGL intialization and texture loading comes from NeHe
 * OpenGL lesson09 (by Jeff Molofee '99, ported to SDL by Sam Latinga
 * '2000).
 *
 * List of primes are calculated at startup, before OpenGL
 * initialization.
 *
 *
 * Prickle-Prickle, the 73rd day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <string.h>
#include <stdlib.h>

#include "primes.h"

#ifndef UNUSED
#define UNUSED __attribute__ ((unused))
#endif

/* #define WIDTH 1280 */
/* #define HEIGHT 720 */
#define WIDTH 720
#define HEIGHT 576
#define FPS 30

/* storage for one texture  */
GLuint texture[1];

int quit = 0;
float zoom = -5;

/* Colors for composites/primes */

#define COLOR_COMPOSITES 1
#ifdef COLOR_COMPOSITES
#define NUM_COLORS 256 /* Comp, Prime */
GLubyte r[NUM_COLORS] = { 64 };
GLubyte g[NUM_COLORS] = { 0 };
GLubyte b[NUM_COLORS] = { 255 };
#else
#define NUM_COLORS 2 /* Comp, Prime */
GLubyte r[NUM_COLORS] = { 32, 64 };
GLubyte g[NUM_COLORS] = { 32, 0 };
GLubyte b[NUM_COLORS] = { 32, 255 };
#endif

/* Set to 1 to save bitmap for each frame */
int save_screenshots = 0;
/* Screenshot pixel data */
GLubyte *ssdata = NULL;



/* From lesson09 NeHe OpenGL tutorial */
static SDL_Surface *load_bitmap(char *filename)
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


/* Load Bitmaps And Convert To Textures */
static void load_textures(void)
{
   SDL_Surface *image1;

   image1 = load_bitmap("Star.bmp");
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
};


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
static void init_gl(int w, int h)
{
   glViewport(0, 0, w, h);
   load_textures();
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


static void cleanup(void)
{
   cleanup_primes();

   if (ssdata) {
      free(ssdata);
      ssdata = NULL;
   }
   SDL_Quit();
}


static void init_everything(void)
{
   Uint32 flags;

   printf("Initializing primes, this may take a while...\n");

#ifdef COLOR_COMPOSITES
   {
      int i, c;
      for (i = 1; i < 256; i++) {
         c = 16 + i * 8;
         if (c > 255) {
            c = 255;
         }
         r[i] = c;
         g[i] = c;
         b[i] = c;
      }
   }
   init_prime_composites();
#else
   init_primes();
#endif

   printf("Done\n");

   /* Run cleanup() at exit */
   atexit(cleanup);

   /* Initialize SDL and OpenGL */
   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
   }
   flags = SDL_OPENGL;
#if 0
   flags |= SDL_FULLSCREEN;
#endif
   if (SDL_SetVideoMode(WIDTH, HEIGHT, 0, flags) == NULL ) {
      fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
      SDL_Quit();
      exit(EXIT_FAILURE);
   }

   /* title bar */
   SDL_WM_SetCaption("Ulam Spiral", NULL);

   init_gl(WIDTH, HEIGHT);
}


static void wait_for_next_frame(void)
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


static void check_events(void)
{
   SDL_Event event;

   while (SDL_PollEvent(&event)) {

      if (event.type == SDL_QUIT) {
         quit = 1;
      }

      if (event.type == SDL_KEYDOWN) {

         /* Key pressed */
         switch (event.key.keysym.sym) {

         case SDLK_ESCAPE:
            quit = 1;
            break;

         case SDLK_UP:
            zoom += 1;
            printf("%f\n", zoom);
            break;

         case SDLK_DOWN:
            zoom -= 1;
            printf("%f\n", zoom);
            break;

            /* None of the above, do nothing */
         default:
            break;
         }
      }
   }
}


static void save_screenshot(Uint32 num)
{
   char buf[128];
   SDL_Surface *shot;

   if (ssdata == NULL) {
      ssdata = malloc(3 * WIDTH * HEIGHT);
      if (!ssdata) {
         fprintf(stderr, "Unable to allocate memory for screenshot\n");
         exit(EXIT_FAILURE);
      }
   }
   glReadBuffer(GL_BACK);
   glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, ssdata);
   /* masks for little endian */
   shot = SDL_CreateRGBSurfaceFrom(ssdata, WIDTH, HEIGHT, 24, WIDTH * 3, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
   snprintf(buf, 128, "video/s%05d.bmp", num);
   SDL_SaveBMP(shot, buf);
   SDL_FreeSurface(shot); /* pixeldata (ssdata) is not freed */
}


/* Draw one frame */
static void draw_scene(void)
{
   int composite;
   int state = 0;
   int curr_count = 0;
   int counts = 1;
   Uint32 i;
   Uint8 *p;
   static double drawnum = 1;
   static Uint32 framecount = 0;


   /* Calculate zoom and number of stars to draw for each frame.
    * constants for polynomials by trial and error.
    */
   if (zoom < 0) {
      zoom -= 0.2 + 0.000059 * framecount - 0.00000055 * (framecount) * (framecount);
   }
   drawnum += framecount * 0.0005 + framecount * framecount * 0.0002;
   if (drawnum > NUM_PRIMES) {
      drawnum = NUM_PRIMES;
   }

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Move camera */
   glLoadIdentity();
   glTranslatef(0, 0, zoom);

   /* Slowly rotate whole scene at constant speed */
   glRotatef(framecount * -0.2, 0.0f, 0.0f, 1.0f);

   glBindTexture(GL_TEXTURE_2D, texture[0]);   /* texture */

   /* Loop through drawnum stars */
   p = &primes[1];
   for (i = 1; i < drawnum; i++) {

      composite = *p++;

      /* Draw stars in spiral */
      switch (state) {
      case 0:
         /* Move right */
         glTranslatef(1, 0, 0);
         curr_count++;
         if (curr_count == counts) {
            curr_count = 0;
            state = 1;
         }
         break;
      case 1:
         /* Move Up */
         glTranslatef(0, 1, 0);
         curr_count++;
         if (curr_count == counts) {
            curr_count = 0;
            state = 2;
            counts++;
         }
         break;
      case 2:
         /* Move left */
         glTranslatef(-1, 0, 0);
         curr_count++;
         if (curr_count == counts) {
            curr_count = 0;
            state = 3;
         }
         break;
      case 3:
         /* Move down */
         glTranslatef(0, -1, 0);
         curr_count++;
         if (curr_count == counts) {
            curr_count = 0;
            state = 0;
            counts++;
         }
         break;

      default:
         break;
      }

      /* Draw one star */
      glColor4ub(r[composite], g[composite], b[composite], 255);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f(-1, -1,  0);
      glTexCoord2f(1, 0); glVertex3f( 1, -1,  0);
      glTexCoord2f(1, 1); glVertex3f( 1,  1,  0);
      glTexCoord2f(0, 1); glVertex3f(-1,  1,  0);
      glEnd();
   }

   if (save_screenshots && (zoom < 0)) {
      save_screenshot(framecount);
   }

   /* Show frame */
   SDL_GL_SwapBuffers();

   framecount++;
}


int main(int argc UNUSED, char *argv[] UNUSED)
{
   init_everything();

   /* main loop */
   while (!quit) {

      /* Draw scene */
      draw_scene();

      /* Check events */
      check_events();

      /* Game logic */

      /* Give computer some slack until next frame */
      wait_for_next_frame();
   }

   cleanup();

   return 0;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
