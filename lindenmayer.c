/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * Draw fractals using Lindenmayer systems
 *
 * http://en.wikipedia.org/wiki/L-system
 *
 * See lsystems.c for more info.
 *
 *
 * Setting Orange, the 1st day of Bureaucracy in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "lsystems.h"
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
static int level = 3;

/* TODO: Add keys for changing palettes */
#define NUM_COLORS 20
/*                                 0    1    2     3   | 4     5     6     7     8     9     10    11  | 12    13    14    15    16    17    18   19
 *                                     Plant colours   | Shaded                                        | Bright
 *                               White Green Brwn  DGr | Black Blue  Red   Mgnt  Grn   Cyan  Ylw   Wht | Black Blue  Red   Mgnt  Grn   Cyan  Ylw   Wht */
static GLubyte r[NUM_COLORS] = { 0xaa, 0x80, 0x7b, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0xaa, 0xaa, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff };
static GLubyte g[NUM_COLORS] = { 0xaa, 0xff, 0x3c, 0x80, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff };
static GLubyte b[NUM_COLORS] = { 0xaa, 0x80, 0x12, 0x00, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x80, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff };

/* Sin/cos lookup table */
#define SINTABSZ 360
double sintab[SINTABSZ + 1];
#define lookup_cos(x) (lookup_sin(x + 90))

/* Stack (for push/pop) */
#define STACKSZ 4096
static int stack_angle[STACKSZ];
static float stack_x[STACKSZ];
static float stack_y[STACKSZ];
static double stack_len[STACKSZ];
static int stack_col[STACKSZ];
/* static int stack_invert[STACKSZ]; */
static int sp = 0; /* Stack pointer */
static int invert = 1;

static int colour = 0;
static int show_turtle = 1;
static int turtle_lines = 0;

struct ccoords {
   float x;
   float y;
   int colour;
   unsigned char end_strip; /* 1 to end this line strip */
};
struct ccoords *coords = NULL;
static int num_coords = 0;
static int max_coords = 0;


/* 4k * 512 = 2M
 * 2M for pattern_a and 2M for pattern_b = 4Mb in total.
 *
 * Expand/shrink PATTERN_LEN to fit the memory capabilities of
 * your computer. 4Mb is enough for about 12 iterations of
 * Sierpinski.
 */
#define PATTERN_LEN (4096 * 512)
static char *pattern_a = NULL;
static char *pattern_b = NULL;
static double linelen;


static void push(int angle, float x, float y)
{
   sp++;
   if (sp >= STACKSZ) {
      fprintf(stderr, "Warning: stack overflow\n");
      sp--;
      return;
   }
   stack_angle[sp] = angle;
   stack_x[sp] = x;
   stack_y[sp] = y;
   stack_len[sp] = linelen;
   stack_col[sp] = colour;
   /* stack_invert[sp] = invert; */
}


static void pop(int *angle, float *x, float *y)
{
   if (sp == 0) {
      fprintf(stderr, "Warning: stack underrun\n");
      return;
   }
   *angle = stack_angle[sp];
   *x = stack_x[sp];
   *y = stack_y[sp];
   linelen = stack_len[sp];
   if (colour != stack_col[sp]) {
      colour = stack_col[sp];
      glColor4ub(r[colour], g[colour], b[colour], 255);
   }
   /* invert = stack_invert[sp]; */
   sp--;
}


/* See commit 549297ae50 for double version */
static inline double lookup_sin(int angle)
{
   while(angle < 0) {
     angle += SINTABSZ;
   }
   while(angle >= SINTABSZ) {
     angle -= SINTABSZ;
   }

   return sintab[angle];
}


static void init_sintab(void)
{
   int i;
   double rad;

   sintab[0] = 0;

   for (i = 1; i < SINTABSZ / 4; i++) {
      /* Translate from 0 - (SINTABSZ - 1) to radians */
      rad = ((2.0 * 3.14159265358979323846) / (double)SINTABSZ) * (double)i;
      /* sin(rad) can be approximated with a taylor series */
      sintab[i] =
         rad -
	((rad * rad * rad) / (double)(1 * 2 * 3)) +
	((rad * rad * rad * rad * rad) / (double)(1 * 2 * 3 * 4 * 5)) -
	((rad * rad * rad * rad * rad * rad * rad) / (double)(1 * 2 * 3 * 4 * 5 * 6 * 7))
	+ ((rad * rad * rad * rad * rad * rad * rad * rad * rad) /
	 (double)(1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9))
	- ((rad * rad * rad * rad * rad * rad * rad * rad * rad * rad * rad) /
	 (double)(1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11));
   }
   sintab[SINTABSZ / 4] = 1.0;
   for (i = 1; i < SINTABSZ / 4; i++) {
     sintab[SINTABSZ / 4 + i] = sintab[SINTABSZ / 4 - i];
   }
   sintab[SINTABSZ / 2] = 0;
   for (i = 1; i < SINTABSZ / 2; i++) {
     sintab[SINTABSZ / 2 + i] = -sintab[SINTABSZ / 2 - i];
   }
   sintab[SINTABSZ] = 0;
}


/* Expand pattern_a into pattern_b */
static void expand_lindenmayer(struct lsystem *lsys, int remove_null)
{
   char *ap = pattern_a;
   char *bp = pattern_b;
   int count = 0;
   int i;

   while (*ap) {

      /* Reserved symbols */

      if (*ap == '@' || *ap == 'C') {
         /* @/C are special, must be followed by a float/int */
         char *endptr;
         /* Check where float/int ends. Compiler might warn about
          * unused result of strtof/strtoul, that's ok, we only want
          * endptr.
          */
         if (*ap == '@') {
            strtof((ap + 1), &endptr);
         } else {
            strtoul((ap + 1), &endptr, 10);
         }
         if (endptr == ap + 1) {
            fprintf(stderr, "Error: failed to read %s, giving up\n",
                    *ap == '@' ? "float after @" : "int after C");
            pattern_a[0] = 0;
            pattern_b[0] = 0;
            return;
         }
         if (count + (endptr - (ap + 1)) > PATTERN_LEN - 1) {
            fprintf(stderr, "Warning: pattern memory full, skipping expansion (%c)\n", *ap);
            memcpy(pattern_b, pattern_a, PATTERN_LEN);
            level--;
            return;
         }

         memcpy(bp, ap, (endptr - ap));
         bp += (endptr - ap);
         count += (endptr - ap);
         ap = endptr;
         continue; /* skip rest of while-loop */
      }

      /* Normal rules */

      /* Lookup rule */
      for(i = 0; i < lsys->num_symbols; i++) {
         if (lsys->rules[i].symbol == *ap) {
            break;
         }
      }
      if (i == lsys->num_symbols) {
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
      if (remove_null && (lsys->rules[i].right[0] != '@') && (lsys->rules[i].right[0] != 'C')) {
         /* Skip null symbols from expansion if remove_null != 0 */
         char *sym = lsys->rules[i].right;
         while (*sym) {
            /* look up rule number for expansion symbol */
            for(i = 0; i < lsys->num_symbols; i++) {
               if (lsys->rules[i].symbol == *sym) {
                  break;
               }
            }
            if (lsys->rules[i].action != A_NULL) {
               *bp++ = *sym;
               count++;
            }
            sym++;
         }
      } else {
         memcpy(bp, lsys->rules[i].right, lsys->rules[i].rsz);
         bp += lsys->rules[i].rsz;
         count += lsys->rules[i].rsz;
      }
      ap++;
   }
   *bp = 0;
   linelen /= lsys->len_divisor;
}


/* Return first byte after reserved word */
static char *skip_reserved(char *p)
{
   char *endptr;

   if (*p == '@') {
      float l = strtof((p + 1), &endptr);
   } else if (*p == 'C') {
      int c = strtoul((p + 1), &endptr, 10);
   } else {
      fprintf(stderr, "Error: '%c' not reserved\n", *p);
      return NULL;
   }
   if (endptr == p + 1) {
      fprintf(stderr, "Error: failed to read val after %c\n", *p);
      return NULL;
   }

   return endptr;
}


/* Compile final output in pattern_b into x/y coordinates */
static void compile_xy_coordinates(struct lsystem *lsys)
{
   char *p;
   int angle, turn_angle;
   float turtle_x = lsys->init_x;
   float turtle_y = lsys->init_y;
   float tmp_x, tmp_y;
   int i;

   /* First pass, count number of lines */
   p = pattern_b;
   num_coords = 0;

   while (*p) {

      /* Reserved symbols */
      if (*p == '@' || *p == 'C') {
         p = skip_reserved(p);
         if (!p) {
            /* epic fail */
            num_coords = 0;
            if (coords) {
               free(coords);
               coords = NULL;
            }
            return;
         }
         continue;
      }

      /* Lookup action for *p */
      for (i = 0; i < lsys->num_symbols; i++) {
         if (lsys->rules[i].symbol == *p) {
            break;
         }
      }
      if (i == lsys->num_symbols) {
         /* Lookup failed */
         fprintf(stderr, "Error: lookup failed for symbol '%c'\n", *p);
         num_coords = 0;
         if (coords) {
            free(coords);
            coords = NULL;
         }
         return;
      }

      /* All ok. */
      switch (lsys->rules[i].action) {

      case A_FORWARD: /* Count number of lines */
      case A_POP:     /* pop might add vertex (we don't know yet) */
      case A_MOVE:    /* Move causes at least one extra vertex */
         num_coords++;
         break;
      case A_PLUS: /* Ignore the rest in this pass */
      case A_MINUS:
      case A_PIPE:
      case A_INVERT:
      case A_PUSH:
      case A_NULL:
         break;

      default:
         fprintf(stderr, "Error: action %d not implemented\n", lsys->rules[i].action);
         num_coords = 0;
         if (coords) {
            free(coords);
            coords = NULL;
         }
         return;
         break;
      }
      p++;
   }

   /* Allocate memory for coords */
   if (coords) {
      free(coords);
   }
   coords = malloc(sizeof(struct ccoords) * (num_coords + 1));
   if (!coords) {
      fprintf(stderr, "Error: could not allocate memory for coords\n");
      num_coords = 0;
      return;
   }

   max_coords = num_coords + 1;

   /* Second pass. Fill in data. */

   invert = 1;
   sp = 0;
   colour = 0;
   angle = lsys->init_angle + (int)(lsys->angle_offset * (float)level + 0.5);
   turn_angle = lsys->angle;
   if (lsys->invert_angle && ((level & 1) == 1)) {
      turn_angle = -turn_angle;
   }

   coords[0].x = turtle_x;
   coords[0].y = turtle_y;
   coords[0].colour = colour;
   coords[0].end_strip = 0;
   num_coords = 1;

   p = pattern_b;
   while (*p) {

      /* Reserved symbols */
      if (*p == '@' || *p == 'C') {
         char *endptr;
         float lenfactor;
         if (*p == '@') {
            lenfactor = strtof((p + 1), &endptr);
            linelen *= lenfactor;
         } else {
            colour = strtoul((p + 1), &endptr, 10);
            if (colour < 0) {
               fprintf(stderr, "Warning: read negative colour\n");
               colour = 0;
            }
            if (colour >= NUM_COLORS) {
               fprintf(stderr, "Warning: colour overflow\n");
               colour = NUM_COLORS - 1;
            }
         }
         if (endptr == p + 1) {
            fprintf(stderr, "Error: failed to read val after '%c'\n", *p);
            num_coords = 0;
            if (coords) {
               free(coords);
               coords = NULL;
            }
            return;
         }
         p = endptr;
         continue; /* skip rest of while-loop */
      }

      /* Normal symbols */

      /* Lookup action for *p */
      for (i = 0; i < lsys->num_symbols; i++) {
         if (lsys->rules[i].symbol == *p) {
            break;
         }
      }
      if (i == lsys->num_symbols) {
         /* Really should not happen, we just checked the same array. */
         fprintf(stderr, "Warning: lookup failed for symbol '%c'\n", *p);
      } else {
         /* Go */
         switch (lsys->rules[i].action) {

         case A_FORWARD: /* Move turtle forward (draw) */
            if (num_coords >= max_coords) {
               fprintf(stderr, "Error: beyond allocated memory %d %d\n", num_coords, max_coords);
               quit = 1;
               return;
            }
            if (coords[num_coords - 1].end_strip == 1) {
               /* Last vertex was an end_strip.
                * Start new strip
                */
               coords[num_coords].x = turtle_x;
               coords[num_coords].y = turtle_y;
               coords[num_coords].colour = colour;
               coords[num_coords].end_strip = 0;
               num_coords++;
            }
            turtle_x += linelen * lookup_cos(angle);
            turtle_y += linelen * lookup_sin(angle);
            coords[num_coords].x = turtle_x;
            coords[num_coords].y = turtle_y;
            coords[num_coords].colour = colour;
            coords[num_coords].end_strip = 0;
            num_coords++;
            break;

         case A_MOVE:    /* Move forward without drawing */
            turtle_x += linelen * lookup_cos(angle);
            turtle_y += linelen * lookup_sin(angle);
            coords[num_coords - 1].end_strip = 1; /* End prev. strip. */
            break;

         case A_PLUS:    /* Rotate angle degrees */
            angle += turn_angle * invert;
            break;

         case A_MINUS:   /* Rotate -angle degrees */
            angle -= turn_angle * invert;
            break;

         case A_PIPE:    /* Turn around 180 degrees */
            angle += 180;
            while (angle >=360) {
               angle -= 360;
            }
            break;

         case A_INVERT:  /* Reverse +/- */
            invert *= -1;
            break;

         case A_PUSH:    /* Push pos/angle to stack */
            push(angle, turtle_x, turtle_y);
            break;

         case A_POP:     /* Pop pos/angle from stack */
            tmp_x = turtle_x;
            tmp_y = turtle_y;
            pop(&angle, &turtle_x, &turtle_y);
            if (tmp_x - turtle_x > 0.001 || tmp_x - turtle_x < -0.001 ||
                tmp_y - turtle_y > 0.001 || tmp_y - turtle_y< -0.001) {
               /* pop changed position of turtle */
               coords[num_coords - 1].end_strip = 1; /* End prev. strip. */
            }
            break;

         case A_NULL:    /* No action, symbol only */
            break;

         default:
            fprintf(stderr, "Action %d not implemented\n", lsys->rules[i].action);
            break;
         }
      }
      p++;
   }
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
      /* skip A_NULL symbols if n is 1 (last iteration) */
      expand_lindenmayer(lsys, n == 1);
      n--;
   }

   compile_xy_coordinates(lsys);

   turtle_lines = 0;
}


static void draw_lindenmayer_system(void)
{
   int i;
   float turtlesize = 0.5;
   float turtle_x = 0;
   float turtle_y = 0;
   int lines = 0;
   struct ccoords *cp;

   if (num_coords == 0) {
      /* Nothing to draw */
      return;
   }

   glLineWidth(1.5);
   glBegin(GL_LINE_STRIP);
   colour = coords[0].colour;
   glColor4ub(r[colour], g[colour], b[colour], 255);
   i = 0;
   cp = coords;

   turtle_lines++;

   /* If show_turtle, then draw one more line than last time, else draw all */
   while (i < num_coords && ((show_turtle == 0) || (lines < turtle_lines))) {
      if (colour != cp->colour) {
         colour = cp->colour;
         glColor4ub(r[colour], g[colour], b[colour], 255);
      }
      turtle_x = cp->x;
      turtle_y = cp->y;
      glVertex2f(turtle_x, turtle_y);
      lines++;
      if (cp->end_strip) {
         /* End this segment, begin new */
         glEnd();
         glBegin(GL_LINE_STRIP);
      }

      cp++;
      i++;
   }

   /* End last segment */
   glEnd();

   if (show_turtle) {
      float angle;
      glEnable(GL_TEXTURE_2D);
      glColor4ub(255, 255, 255, 255);
      glTranslatef(turtle_x, turtle_y, 0);
      if (i < 2) {
         angle = 90;
      } else {
         float dx, dy;
         i -= 2;
         dx = coords[i+1].x - coords[i].x;
         dy = coords[i+1].y - coords[i].y;
         angle = atan2f(dy, dx) * (180.0 / M_PI);
      }
      glRotatef(angle, 0.0f, 0.0f, 1.0f);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f(-turtlesize, -turtlesize,  0.1);
      glTexCoord2f(1, 0); glVertex3f(turtlesize, -turtlesize,  0.1);
      glTexCoord2f(1, 1); glVertex3f(turtlesize, turtlesize,  0.1);
      glTexCoord2f(0, 1); glVertex3f(-turtlesize, turtlesize,  0.1);
      glEnd();
      glRotatef(-angle, 0.0f, 0.0f, 1.0f);
      glTranslatef(-turtle_x, -turtle_y, 0);
      glDisable(GL_TEXTURE_2D);
   }
}

static void set_and_compile_lsys(int num)
{
   if (set_lsys(num)) {
      /* L-system changed, compile pattern */
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

   set_and_compile_lsys(0); /* Start with first L-system */
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
   if (coords) {
      free(coords);
      coords = NULL;
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

   init_sdlgl(720, 576, 0, "turtle.bmp", "Lindenmayer system");
   init_sintab();
   init_linden();
}


static void check_events(void)
{
   SDL_Event event;
   static int zoomkey = 0;
   static int xkey = 0;
   static int ykey = 0;
   int i;

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

         case SDLK_0:
         case SDLK_1:
         case SDLK_2:
         case SDLK_3:
         case SDLK_4:
         case SDLK_5:
         case SDLK_6:
         case SDLK_7:
         case SDLK_8:
         case SDLK_9:
            set_and_compile_lsys(event.key.keysym.sym - SDLK_0);
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

         case SDLK_o:
         case SDLK_r: /* dvorak "o" */
            /* Scroll L-systems down */
            if (munge_lsys(-1)) {
               compile_lindenmayer_pattern(current_lsys, level);
            } else {
               fprintf(stderr, "At first L-system\n");
            }
            break;

         case SDLK_p:
         case SDLK_l: /* dvorak "p" */
            /* Scroll L-systems up */
            if (munge_lsys(1)) {
               compile_lindenmayer_pattern(current_lsys, level);
            } else {
               fprintf(stderr, "At last L-system\n");
            }
            break;

         case SDLK_LEFT:
            /* Move left */
            xkey = -1;
            dx = -0.005;
            break;

         case SDLK_RIGHT:
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

#if 1
         case SDLK_d:
            /* Debug pattern_b/coords */
            printf("%s\n", pattern_b);
            for (i = 0; i < num_coords; i++) {
               printf("(%f,%f,%d) ", coords[i].x, coords[i].y, coords[i].colour);
               if (coords[i].end_strip) {
                  printf("e\n");
               }
            }
            printf("\nnum_coords %d\n",num_coords);
            break;
#endif

         case SDLK_t:
            /* Toggle show_turtle */
            show_turtle = 1 - show_turtle;
            turtle_lines = 0;
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
            xkey = 0;
            break;

         case SDLK_RIGHT:
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

   dz += 0.05 * (0-zoom/5) * zoomkey;
   dx += 0.025 * (0-zoom/5) * xkey;
   dy += 0.025 * (0-zoom/5) * ykey;
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
   static Uint32 framecount = 0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Move camera */
   glLoadIdentity();
   glTranslatef(move_x, move_y, zoom);

   /* Draw selected L-system */
   draw_lindenmayer_system();

   /* Show frame */
   SDL_GL_SwapBuffers();

   framecount++;
}


int main(int argc __attribute__ ((unused)), char *argv[] __attribute__ ((unused)))
{
   init_everything();

   printf(
          "KEYS\n\n"
          " t        - Toggle Turtle\n\n"
          "Browse L-systems with o/p\n\n"
          " 0        - Sierpinski triangle\n"
          " 1        - Terdragon curve\n"
          " 2        - von Koch snowflake\n"
          " 3        - Penrose (pentagons, colored)\n"
          " 4        - Plant\n"
          " 5        - Quadratic Sierpinski\n"
          " 6        - Quadratic von Koch snowflake\n"
          " 7        - Leaf\n"
          " 8        - Levy curve\n"
          " 9        - Peano curve\n\n"
          "Browse with o/p for more curves\n\n"
          "RECURSION\n\n"
          " minus, x - decrease recursion level\n"
          " plus, c  - increase recursion level\n\n"
          "CAMERA\n\n"
          " Up       - Zoom in\n"
          " Down     - Zoom out\n"
          " Left     - Move left\n"
          " Right    - Move right\n"
          " a, f     - Move up\n"
          " z, v     - Move down\n"
          " Space    - Reset camera\n\n"
          "QUIT      - Esc\n\n"
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
