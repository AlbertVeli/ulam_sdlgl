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
 * Rendered video: http://www.youtube.com/watch?v=z397rMWHV0g
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
 * List of primes are calculated at startup, before OpenGL
 * initialization.
 *
 *
 * Prickle-Prickle, the 73rd day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <SDL.h>
#include <string.h>
#include <stdlib.h>

#include "primes.h"
#include "sdlgl.h"


static int quit = 0;
static float zoom = -5;

/* Colors for composites/primes */

#define COLOR_COMPOSITES 1
#ifdef COLOR_COMPOSITES
#define NUM_COLORS 256 /* Primes, Composites filled in by init */
GLubyte r[NUM_COLORS] = { 64 };
GLubyte g[NUM_COLORS] = { 0 };
GLubyte b[NUM_COLORS] = { 255 };
#else
#define NUM_COLORS 2 /* Comp, Prime */
GLubyte r[NUM_COLORS] = { 32, 64 };
GLubyte g[NUM_COLORS] = { 32, 0 };
GLubyte b[NUM_COLORS] = { 32, 255 };
#endif


static void cleanup(void)
{
   cleanup_primes();
   cleanup_sdlgl();
}


static void init_everything(void)
{
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

   init_sdlgl(WIDTH, HEIGHT, 0, "Ulam Spiral");
   /*                        ^
    *                   fullscreen?
    */
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
      save_screenshot(framecount, WIDTH, HEIGHT);
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
