/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * von Koch / Sierpinski curves
 *
 * Hard-coded. TODO: translate to Lindenmayer systems
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
#define MAX_LEVEL 15
static int level = 2;
static int koch_type = 1;

#define NUM_COLORS 1
static GLubyte r[NUM_COLORS] = { 255 };
static GLubyte g[NUM_COLORS] = { 255 };
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
   static int zoomkey = 0;
   static int xkey = 0;
   static int ykey = 0;

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

         case SDLK_1:
         case SDLK_2:
         case SDLK_3:
         case SDLK_4:
         case SDLK_5:
         case SDLK_6:
            koch_type = event.key.keysym.sym - SDLK_1 + 1;
            break;

         case SDLK_a:
            /* Move down */
            ykey = 1;
            dy = 0.01;
            break;

         case SDLK_z:
         case SDLK_q:
            /* Move up */
            ykey = -1;
            dy = -0.01;
            break;

         case SDLK_LEFT:
            /* Move left */
            xkey = -1;
            dx = -0.01;
            break;

         case SDLK_RIGHT:
            /* Move right */
            xkey = 1;
            dx = 0.01;
            break;

         case SDLK_UP:
            /* Zoom in */
            zoomkey = 1;
            dz = 0.05;
            break;

         case SDLK_DOWN:
            /* Zoom out */
            zoomkey = -1;
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

      if (event.type == SDL_KEYUP) {

         /* Key released */
         switch (event.key.keysym.sym) {
         case SDLK_UP:
            /* Zoom in */
            zoomkey = 0;
            break;

         case SDLK_DOWN:
            /* Zoom out */
            zoomkey = 0;
            break;

         case SDLK_LEFT:
            /* Move left */
            xkey = 0;
            break;

         case SDLK_RIGHT:
            /* Move right */
            xkey = 0;
            break;

         case SDLK_a:
            /* Move down */
            ykey = 0;
            break;

         case SDLK_z:
         case SDLK_q:
            /* Move up */
            ykey = 0;
            break;

         default:
            break;
         }
      }
   }

   dz += 0.05 * zoomkey;
   dx += 0.025 * xkey;
   dy += 0.025 * ykey;
}


static void game_logic(void)
{
   /* Decrease dz and update zoom */
   dz = dz / 1.2;
   zoom += dz;
   dx = dx / 1.15;
   move_x += dx;
   dy = dy / 1.15;
   move_y += dy;
}


static void draw_koch_snowflake(double angle, double len, int n)
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
      draw_koch_snowflake(angle, len, n); /* F */
      angle += 60.0;                      /* + */
      draw_koch_snowflake(angle, len, n); /* F */
      angle -= 120.0;                     /* - - */
      draw_koch_snowflake(angle, len, n); /* F */
      angle += 60.0;                      /* + */
      draw_koch_snowflake(angle, len, n); /* F */
   }
}


static void draw_koch_island(double angle, double len, int n)
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
      len /= 4.0;   /* reduce length */
      draw_koch_island(angle, len, n); /* F */
      angle -= 90.0;                   /* - */
      draw_koch_island(angle, len, n); /* F */
      angle += 90.0;                   /* + */
      draw_koch_island(angle, len, n); /* F */
      angle += 90.0;                   /* + */
      draw_koch_island(angle, len, n); /* F */
      draw_koch_island(angle, len, n); /* F */
      angle -= 90.0;                   /* - */
      draw_koch_island(angle, len, n); /* F */
      angle -= 90.0;                   /* - */
      draw_koch_island(angle, len, n); /* F */
      angle += 90.0;                   /* + */
      draw_koch_island(angle, len, n); /* F */
   }
}

static void draw_koch_island_2(double angle, double len, int n)
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
      len /= 6.0;   /* reduce length */
      draw_koch_island_2(angle, len, n); /* F */
      angle += 90.0;                     /* + */
      draw_koch_island_2(angle, len, n); /* F */
      draw_koch_island_2(angle, len, n); /* F */
      angle -= 90.0;                     /* - */
      draw_koch_island_2(angle, len, n); /* F */
      draw_koch_island_2(angle, len, n); /* F */
      angle -= 90.0;                     /* - */
      draw_koch_island_2(angle, len, n); /* F */
      angle -= 90.0;                     /* - */
      draw_koch_island_2(angle, len, n); /* F */
      angle += 90.0;                     /* + */
      draw_koch_island_2(angle, len, n); /* F */
      angle += 90.0;                     /* + */
      draw_koch_island_2(angle, len, n); /* F */
      draw_koch_island_2(angle, len, n); /* F */
      angle -= 90.0;                     /* - */
      draw_koch_island_2(angle, len, n); /* F */
      angle -= 90.0;                     /* - */
      draw_koch_island_2(angle, len, n); /* F */
      angle += 90.0;                     /* + */
      draw_koch_island_2(angle, len, n); /* F */
      angle += 90.0;                     /* + */
      draw_koch_island_2(angle, len, n); /* F */
      draw_koch_island_2(angle, len, n); /* F */
      angle += 90.0;                     /* + */
      draw_koch_island_2(angle, len, n); /* F */
      draw_koch_island_2(angle, len, n); /* F */
      angle -= 90.0;                     /* - */
      draw_koch_island_2(angle, len, n); /* F */
   }
}

static void draw_sierpinski_square(double angle, double len, int n)
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
      n--;        /* reduce order  */
      len /= 3;   /* reduce length */
      draw_sierpinski_square(angle, len, n); /* F */
      draw_sierpinski_square(angle, len, n); /* F */
      angle -= 90.0;                         /* - */
      draw_sierpinski_square(angle, len, n); /* F */
      angle -= 90.0;                         /* - */
      draw_sierpinski_square(angle, len, n); /* F */
      angle -= 90.0;                         /* - */
      draw_sierpinski_square(angle, len, n); /* F */
      angle -= 90.0;                         /* - */
      draw_sierpinski_square(angle, len, n); /* F */
      draw_sierpinski_square(angle, len, n); /* F */
   }
}

static void draw_dragon_right(double angle, double len, int n);

static void draw_dragon_left(double angle, double len, int n)
{
   double rad = 0.0174532925 * angle;  /* convert to radians  */
   float new_x = old_x + len * cos(rad);
   float new_y = old_y + len * sin(rad);

   angle += 90.0;

   if (n == 0) {
      /* Recursion bottom, draw line */
      glVertex2f(old_x, old_y);
      glVertex2f(new_x, new_y);
      old_x = new_x;
      old_y = new_y;
   } else {
      n--;         /* reduce order  */
      len /= 1.33; /* reduce length */
      draw_dragon_left(angle, len, n);   /* Fl */
      angle += 90.0;                     /* +  */
      draw_dragon_right(angle, len, n);  /* Fr */
      angle += 90.0;                     /* +  */
   }
}

static void draw_dragon_right(double angle, double len, int n)
{
   double rad = 0.0174532925 * angle;  /* convert to radians  */
   float new_x = old_x + len * cos(rad);
   float new_y = old_y + len * sin(rad);

   angle -= 90.0;

   if (n == 0) {
      /* Recursion bottom, draw line */
      glVertex2f(old_x, old_y);
      glVertex2f(new_x, new_y);
      old_x = new_x;
      old_y = new_y;
   } else {
      n--;         /* reduce order  */
      len /= 1.33; /* reduce length */
      angle -= 90.0;                     /* - */
      draw_dragon_left(angle, len, n);   /* Fl */
      angle -= 90.0;                     /* - */
      draw_dragon_right(angle, len, n);  /* Fr */
   }
}


static void draw_sierpinski_right(double angle, double len, int n);

static void draw_sierpinski_left(double angle, double len, int n)
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
      n--;         /* reduce order  */
      len /= 1.8; /* reduce length */
      angle += 60.0;
      angle += 60.0;
      draw_sierpinski_right(angle, len, n);
      angle += 60.0;
      draw_sierpinski_left(angle, len, n);
      angle += 60.0;
      draw_sierpinski_right(angle, len, n);
   }
}

static void draw_sierpinski_right(double angle, double len, int n)
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
      n--;         /* reduce order  */
      len /= 1.8; /* reduce length */
      angle -= 60.0;
      angle -= 60.0;
      draw_sierpinski_left(angle, len, n);
      angle -= 60.0;
      draw_sierpinski_right(angle, len, n);
      angle -= 60.0;
      draw_sierpinski_left(angle, len, n);
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

   glColor4ub(r[colour], g[colour], b[colour], 255);
   glLineWidth(1.5);
   glBegin(GL_LINES);

   /* Draw koch fractal */

   switch (koch_type) {

   case 1: /* Koch snowflake */
      if (level > 10) {
         level = 10;
      }
      /* Side 1 */
      old_x = -1;
      old_y = -0.66;
      draw_koch_snowflake(60.0, 2.0, level);

      /* Side 2 */
      old_x = 0.0;
      old_y = sqrt(3) - 0.66;
      draw_koch_snowflake(-60.0, 2.0, level);

      /* Side 3 */
      old_x = 1;
      old_y = -0.66;
      draw_koch_snowflake(-180.0, 2.0, level);
      break;

   case 2: /* Koch island */
      if (level > 7) {
         level = 7;
      }
      /* Side 1 */
      old_x = -1;
      old_y = 1;
      draw_koch_island(0.0, 2.0, level);
      /* Side 2 */
      old_x = 1;
      old_y = 1;
      draw_koch_island(-90.0, 2.0, level);
      /* Side 3 */
      old_x =  1;
      old_y =  -1;
      draw_koch_island(-180.0, 2.0, level);
      /* Side 4 */
      old_x = -1;
      old_y = -1;
      draw_koch_island(-270.0, 2.0, level);
      break;

   case 3: /* Koch island, second variant */
      if (level > 5) {
         level = 5;
      }
      /* Side 1 */
      old_x = -1;
      old_y = 1;
      draw_koch_island_2(0.0, 2.0, level);
      /* Side 2 */
      old_x = 1;
      old_y = 1;
      draw_koch_island_2(-90.0, 2.0, level);
      /* Side 3 */
      old_x =  1;
      old_y =  -1;
      draw_koch_island_2(-180.0, 2.0, level);
      /* Side 4 */
      old_x = -1;
      old_y = -1;
      draw_koch_island_2(-270.0, 2.0, level);
      break;

   case 4: /* Sierpinski square */
      if (level > 7) {
         level = 7;
      }
      /* Side 1 */
      old_x = -1;
      old_y = 1;
      draw_sierpinski_square(0.0, 2.0, level);
      /* Side 2 */
      old_x = 1;
      old_y = 1;
      draw_sierpinski_square(-90.0, 2.0, level);
      /* Side 3 */
      old_x =  1;
      old_y =  -1;
      draw_sierpinski_square(-180.0, 2.0, level);
      /* Side 4 */
      old_x = -1;
      old_y = -1;
      draw_sierpinski_square(-270.0, 2.0, level);
      break;

   case 5: /* Dragon */
      old_x = -0.5;
      old_y = 0.5;
      draw_dragon_left(0.0 - 135*level, 1.0, level);
      break;

   case 6: /* Sierpinski */
      if (level > 14) {
         level = 14;
      }
      old_x = -0.5;
      old_y = 0.5;
      draw_sierpinski_right(0.0, 1.0, level);
      break;

   default:
      break;
   }

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
          " 1        - Koch snowflake\n"
          " 2        - Koch island\n"
          " 3        - Koch island, variant 1\n"
          " 4        - Sierpinski square\n"
          " 5        - Dragon curve\n"
          " 6        - Sierpinski triangle\n"
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
