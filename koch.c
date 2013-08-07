/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * Draw Koch Snowflake using OpenGL + SDL
 *
 *
 * Prickle-Prickle, the 73rd day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "sdlgl.h"


static int quit = 0;
static float zoom = -5;
static float move_x = 0;
static float move_y = 0;
static float dx = 0;
static float dy = 0;
static float dz = 0;
static float old_x = 0;
static float old_y = 0;
#define MAX_LEVEL 10
static int level = 1;

#define NUM_COLORS 1
static GLubyte r[NUM_COLORS] = { 128 };
static GLubyte g[NUM_COLORS] = { 0 };
static GLubyte b[NUM_COLORS] = { 255 };

static void cleanup(void)
{
   cleanup_sdlgl();
}


static void init_everything(void)
{
   /* Run cleanup() at exit */
   atexit(cleanup);

   init_sdlgl(720, 576, 0, NULL, "Koch Snowflake");
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

         case SDLK_a:
            /* Move down */
            dy = 0.01;
            break;

         case SDLK_z:
         case SDLK_q:
            /* Move up */
            dy = -0.01;
            break;

         case SDLK_LEFT:
            /* Move left */
            dx = -0.01;
            break;

         case SDLK_RIGHT:
            /* Move right */
            dx = 0.01;
            break;

         case SDLK_UP:
            /* Zoom in */
            dz = 0.05;
            break;

         case SDLK_DOWN:
            /* Zoom out */
            dz = -0.05;
            break;

         case SDLK_o:
         case SDLK_r: /* dvorak "o key" */
         case SDLK_MINUS:
         case SDLK_KP_MINUS:
            /* Decrease recursion level */
            level--;
            if (level < 0) {
               level = 0;
            }
            break;

         case SDLK_p:
         case SDLK_l: /* dvorak "p key" */
         case SDLK_PLUS:
         case SDLK_KP_PLUS:
         case SDLK_SPACE:
            /* Increase recursion level */
            level++;
            if (level > MAX_LEVEL) {
               level = MAX_LEVEL;
            }
            break;

            /* None of the above, do nothing */
         default:
            break;
         }
      }
   }
}


static void game_logic(void)
{
   /* Decrease dz and update zoom */
   dz = dz / 1.2;
   zoom += dz;
   dx = dx / 1.2;
   move_x += dx;
   dy = dy / 1.2;
   move_y += dy;
}


static void draw_koch(double angle, double len, int n)
{
   double rad = 0.0174532925 * angle;  /* convert to radians  */
   float new_x = old_x + len * cos(rad);
   float new_y = old_y + len * sin(rad);
   if (n == 0) {
      /* Recursion bottom, draw line */
      glVertex2f(old_x, old_y);
      glVertex2f(new_x, new_y);
      old_x = new_x;
      old_y = new_y;
   } else {
      n--;          /* reduce order  */
      len /= 3.0;   /* reduce length */
      draw_koch(angle, len, n);

      angle += 60.0;
      draw_koch(angle, len, n);

      angle -= 120.0;
      draw_koch(angle, len, n);

      angle += 60.0;
      draw_koch(angle, len, n);
   }
}


/* Draw one frame */
static void draw_scene(void)
{
   int colour = 0;
   static Uint32 framecount = 0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Move camera */
   glLoadIdentity();
   glTranslatef(move_x, move_y, zoom);

   /* Draw snowflake */
   glColor4ub(r[colour], g[colour], b[colour], 255);
   glLineWidth(1.5);
   glBegin(GL_LINES);

   /* Side 1 */
   old_x = -0.5;
   old_y = -0.33;
   draw_koch(60.0, 1.0, level);

   /* Side 2 */
   old_x = 0.0;
   old_y = sqrt(3) / 2.0 - 0.33;
   draw_koch(-60.0, 1.0, level);

   /* Side 3 */
   old_x = 0.5;
   old_y = -0.33;
   draw_koch(-180.0, 1.0, level);

   glEnd();

   /* Show frame */
   SDL_GL_SwapBuffers();

   framecount++;
}


int main(int argc __attribute__ ((unused)), char *argv[] __attribute__ ((unused)))
{
   init_everything();

   printf(
          "Keys:\n\n"
          " o, minus - decrease recursion level\n"
          " p, plus  - increase recursion level\n"
          " Up       - Zoom in\n"
          " Down     - Zoom out\n"
          " Left     - Move left\n"
          " Right    - Move right\n"
          " a        - Move up\n"
          " z        - Move down\n"
          " Esc      - Quit\n\n"
          );

   /* main loop */
   while (!quit) {

      /* Draw scene */
      draw_scene();

      /* Check events */
      check_events();

      /* Game logic */
      game_logic();

      /* Cut computer some slack until next frame */
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
