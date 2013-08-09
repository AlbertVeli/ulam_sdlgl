/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * von Koch / Sierpinski curves using Lindenmayer systems
 *
 * See: http://en.wikipedia.org/wiki/L-system
 *
 *
 * Setting Orange, the 1st day of Bureaucracy in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "sdlgl.h"


/* Set to 1 to quit main loop */
static int quit = 0;

/* Camera */
static float zoom = -5;
static float move_x = 0;
static float move_y = 0;
static float dx = 0;
static float dy = 0;
static float dz = 0;

/* Initial expansion level */
static int level = 2;

#define NUM_COLORS 1
static GLubyte r[NUM_COLORS] = { 255 };
static GLubyte g[NUM_COLORS] = { 255 };
static GLubyte b[NUM_COLORS] = { 255 };

/* 4k * 512 = 2M
 * 2M for pattern_a and 2M for pattern_b = 4Mb in total.
 *
 * Expand/shrink PATTERN_LEN to fit the memory capabilities of
 * your computer. 4Mb is enough for about 12 iterations of
 * Sierpinski.
 */
#define PATTERN_LEN (4096 * 512)
char *pattern_a = NULL;
char *pattern_b = NULL;
double linelen;

enum lsys_action { A_NULL,    /* No action, symbol only */
                   A_FORWARD, /* Move turtle forward (draw) */
                   A_PLUS,    /* Rotate angle degrees */
                   A_MINUS    /* Rotate -angle degrees */
};

struct lsys_symbol {
   char symbol;
   enum lsys_action action;
};

struct lsys_rule {
   char left;   /* Left side of the rule */
   char *right; /* Right side */
   int rsz;     /* Size of right side (to simplify expansion) */
};

#define MAX_RULES 5
#define MAX_SYMBOLS 5
struct lsystem {
   int maxlevel;
   int reverse_angle;
   float angle;
   float angle_offset;
   float init_angle;
   float init_x;
   float init_y;
   float init_len;
   float len_divisor;
   char *axiom;
   int num_rules;
   struct lsys_rule rules[MAX_RULES];
   int num_symbols;
   struct lsys_symbol symbols[MAX_SYMBOLS];
};

/* Sierpinski L-system */
struct lsystem sierpinski = {
   14, 1, 60.0, 0.0, 0.0, -1.5, -1.33, 3.0, 2.0,
   "A",     /* Axiom */
   4, {     /* Rules for expansion */
      { 'A', "B-A-B", sizeof("B-A-B") - 1 },
      { 'B', "A+B+A", sizeof("A+B+A") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   4, {     /* Symbol actions (for drawing) */
      { 'A', A_FORWARD },
      { 'B', A_FORWARD },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};

/* Dragon curve */
struct lsystem dragon = {
   19, 0, 90.0, -45.0, 0.0, -1.0, 0.0, 2.0, 1.414,
   "Fl",    /* Axiom */
   5, {     /* Rules for expansion */
      { 'F', "F", 1 },
      { 'l', "l+rF+", sizeof("l+rF+") - 1 },
      { 'r', "-Fl-r", sizeof("-Fl-r") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   5, {     /* Symbol actions (for drawing) */
      { 'F', A_FORWARD },
      { 'l', A_NULL },
      { 'r', A_NULL },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};

/* von Koch snowflake */
struct lsystem koch = {
   10, 0, 60.0, 0.0, 0.0, -1.5, 0.85, 3.0, 3.0,
   "F--F--F",     /* Axiom */
   3, {     /* Rules for expansion */
      { 'F', "F+F--F+F", sizeof("F+F--F+F") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   3, {     /* Symbol actions (for drawing) */
      { 'F', A_FORWARD },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};

/* Quadratic modification of von Koch snowflake */
struct lsystem quad_koch_snow = {
   8, 0, 90.0, 0.0, -90.0, -1.5, -0.66, 3.0, 3.0,
   "+F",     /* Axiom */
   3, {     /* Rules for expansion */
      { 'F', "F+F-F-F+F", sizeof("F+F-F-F+F") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   3, {     /* Symbol actions (for drawing) */
      { 'F', A_FORWARD },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};

/* Quadratic von Koch island */
struct lsystem quad_koch = {
   5, 0, 90.0, 0.0, 0.0, -1, 1, 2.0, 6.0,
   "F-F-F-F",  /* Axiom */
   3, {        /* Rules for expansion */
      { 'F', "F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F", sizeof("F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   3, {     /* Symbol actions (for drawing) */
      { 'F', A_FORWARD },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};

/* Hexagonal Gosper */
struct lsystem hexa_gosper = {
   11, 0, 60.0, -19.3, 0.0, -1.5, -1.0, 3.0, 2.65,
   "XF",    /* Axiom */
   5, {     /* Rules for expansion */
      { 'F', "F", 1 },
      { 'X', "X+YF++YF-FX--FXFX-YF+", sizeof("X+YF++YF-FX--FXFX-YF+") - 1 },
      { 'Y', "-FX+YFYF++YF+FX--FX-Y", sizeof("-FX+YFYF++YF+FX--FX-Y") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   5, {     /* Symbol actions (for drawing) */
      { 'F', A_FORWARD },
      { 'X', A_NULL },
      { 'Y', A_NULL },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};

/* Sierpinski square */
struct lsystem quad_sierpinski = {
   7, 0, 90.0, 0.0, 0.0, -1, 1, 2.0, 3.0,
   "F-F-F-F",  /* Axiom */
   3, {        /* Rules for expansion */
      { 'F', "FF-F-F-F-FF", sizeof("FF-F-F-F-FF") - 1 },
      { '+', "+", 1 },
      { '-', "-", 1 }
   },
   3, {     /* Symbol actions (for drawing) */
      { 'F', A_FORWARD },
      { '+', A_PLUS },
      { '-', A_MINUS }
   }
};


/* Current L-system */
static int lsys_num = 0;
static struct lsystem *current_lsys = NULL;


/* Expand pattern_a into pattern_b */
static void expand_lindenmayer(struct lsystem *lsys)
{
   char *ap = pattern_a;
   char *bp = pattern_b;
   int count = 0;
   int i;

   while (*ap) {
      /* Lookup rule */
      for(i = 0; i < lsys->num_rules; i++) {
         if (lsys->rules[i].left == *ap) {
            break;
         }
      }
      if (i == lsys->num_rules) {
         fprintf(stderr, "Error: expansion failed for token '%c', giving up\n", *ap);
         pattern_a[0] = 0;
         pattern_b[0] = 0;
         return;
      }
      /* Epansion in lsys->rules[i].right */
      if (count + lsys->rules[i].rsz > PATTERN_LEN - 1) {
         /* Reallocate memory here? */
         fprintf(stderr, "Warning: pattern memory full, skipping expansion\n");
         memcpy(pattern_b, pattern_a, PATTERN_LEN);
         level--;
         return;
      }
      memcpy(bp, lsys->rules[i].right, lsys->rules[i].rsz);
      bp += lsys->rules[i].rsz;
      count += lsys->rules[i].rsz;
      ap++;
   }
   *bp = 0;
   linelen /= lsys->len_divisor;
}


static void compile_lindenmayer_pattern(struct lsystem *lsys, int n)
{
   /* Dont recurse. Just expand axiom into pattern_b level times */
   snprintf(pattern_b, PATTERN_LEN, "%s", lsys->axiom);
   linelen = lsys->init_len;

   if (n > lsys->maxlevel) {
      n = lsys->maxlevel;
      level = n;
   }

   /* Expand */
   while (n > 0) {
      /* Copy pattern_b to pattern_a */
      strcpy(pattern_a, pattern_b);
      expand_lindenmayer(lsys);
      n--;
   }
}


static void draw_lindenmayer_system(struct lsystem *lsys)
{
   double angle;
   double rad;
   float new_x;
   float new_y;
   float turtle_x = lsys->init_x;
   float turtle_y = lsys->init_y;
   char *p;
   int i;

   angle = lsys->init_angle + lsys->angle_offset * level;
   p = pattern_b;
   while (*p) {
      /* Lookup action for *p */
      for (i = 0; i < lsys->num_symbols; i++) {
         if (lsys->symbols[i].symbol == *p) {
            break;
         }
      }
      if (i == lsys->num_symbols) {
         /* Lookup failed, ignore this symbol */
         fprintf(stderr, "Warning: lookup failed for symbol '%c'\n", *p);
      } else {
         /* Go */
         switch (lsys->symbols[i].action) {

         case A_FORWARD: /* Move turtle forward (draw) */
            rad = 0.0174532925 * angle;  /* radians (XXX: sin/cos could be precalculated) */
            new_x = turtle_x + linelen * cos(rad);
            new_y = turtle_y + linelen * sin(rad);
            glVertex2f(turtle_x, turtle_y);
            glVertex2f(new_x, new_y);
            turtle_x = new_x;
            turtle_y = new_y;
            break;

         case A_PLUS:    /* Rotate angle degrees */
            if (lsys->reverse_angle && ((level & 1) == 1)) {
               angle -= lsys->angle;
            } else {
               angle += lsys->angle;
            }
            break;

         case A_MINUS:   /* Rotate -angle degrees */
            if (lsys->reverse_angle && ((level & 1) == 1)) {
               angle += lsys->angle;
            } else {
               angle -= lsys->angle;
            }
            break;

         case A_NULL:    /* No action, symbol only */
            break;

         default:
            fprintf(stderr, "Action %d not implemented\n", lsys->symbols[i].action);
            break;
         }
      }

      p++;
   }
}


static void set_lsys(int num)
{
   switch (num) {
   case 1:
      current_lsys = &sierpinski;
      break;

   case 2:
      current_lsys = &dragon;
      break;

   case 3:
      current_lsys = &koch;
      break;

   case 4:
      current_lsys = &quad_koch_snow;
      break;

   case 5:
      current_lsys = &quad_koch;
      break;

   case 6:
      current_lsys = &hexa_gosper;
      break;

   case 7:
      current_lsys = &quad_sierpinski;
      break;

   default:
      fprintf(stderr, "Warning: no L-system selected\n");
      pattern_a[0] = 0;
      pattern_b[0] = 0;
      current_lsys = NULL;
      lsys_num = 0;
      break;
   }

   if ((lsys_num != num) && (current_lsys != NULL)) {
      lsys_num = num;
      compile_lindenmayer_pattern(current_lsys, level);
   }
}


static void init_linden(void)
{
   pattern_a = malloc(PATTERN_LEN);
   if (!pattern_a) {
      fprintf(stderr, "Out of memory\n");
      exit(EXIT_FAILURE);
   }
   pattern_b = malloc(PATTERN_LEN);
   if (!pattern_b) {
      fprintf(stderr, "Out of memory\n");
      exit(EXIT_FAILURE);
   }

   set_lsys(1); /* Start with Sierpinski */
}


static void cleanup_linden(void)
{
   if (pattern_a) {
      free(pattern_a);
      pattern_a = NULL;
   }
   if (pattern_b) {
      free(pattern_b);
      pattern_b = NULL;
   }
}


static void cleanup(void)
{
   cleanup_linden();
   cleanup_sdlgl();
}


static void init_everything(void)
{
   /* Run cleanup() at exit */
   atexit(cleanup);

   init_sdlgl(720, 576, 0, NULL, "Lindenmayer system");
   init_linden();
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
         case SDLK_7:
            set_lsys(event.key.keysym.sym - SDLK_1 + 1);
            break;

         case SDLK_a:
         case SDLK_f:
         case SDLK_u:
            /* Move down */
            ykey = 1;
            dy = 0.005;
            break;

         case SDLK_z:
         case SDLK_v:
         case SDLK_k:
            /* Move up */
            ykey = -1;
            dy = -0.005;
            break;

         case SDLK_LEFT:
         case SDLK_o:
         case SDLK_r: /* dvorak "o" */
            /* Move left */
            xkey = -1;
            dx = -0.005;
            break;

         case SDLK_RIGHT:
         case SDLK_p:
         case SDLK_l: /* dvorak "p" */
            /* Move right */
            xkey = 1;
            dx = 0.005;
            break;

         case SDLK_UP:
            /* Zoom in */
            zoomkey = 1;
            dz = 0.02;
            break;

         case SDLK_DOWN:
            /* Zoom out */
            zoomkey = -1;
            dz = -0.02;
            break;

         case SDLK_x:
         case SDLK_q: /* dvorak "x" */
         case SDLK_MINUS:
         case SDLK_KP_MINUS:
            /* Decrease recursion level */
            level--;
            if (level < 0) {
               level = 0;
            } else {
               compile_lindenmayer_pattern(current_lsys, level);
            }
            break;

         case SDLK_c:
         case SDLK_j: /* dvorak "c" key */
         case SDLK_PLUS:
         case SDLK_KP_PLUS:
            /* Increase recursion level */
            level++;
            compile_lindenmayer_pattern(current_lsys, level);
            break;

         case SDLK_SPACE:
            /* Reset zoom */
            zoom = -5;
            zoomkey = 0;
            dz = 0;
            move_x = 0;
            xkey = 0;
            dx = 0;
            move_y = 0;
            ykey = 0;
            dy = 0;
            break;

            /* None of the above, do nothing */
         default:
            break;
         }
      }

      if (event.type == SDL_KEYUP) {

         /* Key released, set pressed key to 0 */
         switch (event.key.keysym.sym) {
         case SDLK_UP:
            zoomkey = 0;
            break;

         case SDLK_DOWN:
            zoomkey = 0;
            break;

         case SDLK_LEFT:
         case SDLK_o:
         case SDLK_r:
            xkey = 0;
            break;

         case SDLK_RIGHT:
         case SDLK_p:
         case SDLK_l: /* dvorak "p" */
            xkey = 0;
            break;

         case SDLK_a:
         case SDLK_f:
         case SDLK_u:
            ykey = 0;
            break;

         case SDLK_z:
         case SDLK_v:
         case SDLK_k:
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

   /* Draw selected L-system */
   draw_lindenmayer_system(current_lsys);

   glEnd();

   /* Show frame */
   SDL_GL_SwapBuffers();

   framecount++;
}


int main(int argc __attribute__ ((unused)), char *argv[] __attribute__ ((unused)))
{
   init_everything();

   printf(
          "L-system\n\n"
          " 1        - Sierpinski triangle\n"
          " 2        - Dragon curve\n"
          " 3        - von Koch snowflake\n"
          " 4        - Quadratic von Koch snowflake\n"
          " 5        - Quadratic von Koch island\n"
          " 6        - Hexagonal Gosper curve\n"
          " 7        - Quadratic Sierpinski\n\n"
          "RECURSION\n\n"
          " minus, x - decrease recursion level\n"
          " plus, c  - increase recursion level\n\n"
          "CAMERA\n\n"
          " Up       - Zoom in\n"
          " Down     - Zoom out\n"
          " Left, o  - Move left\n"
          " Right, p - Move right\n"
          " a, f     - Move up\n"
          " z, v     - Move down\n"
          " Space    - Reset camera\n\n"
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
