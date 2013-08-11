#ifndef __LSYSTEMS_H
#define __LSYSTEMS_H

/* (k) ALL RIGHTS REVERSED - Reprint what you like */

/* Turtle actions */
enum lsys_action { A_NULL,    /* No action, symbol only */
                   A_FORWARD, /* Move turtle forward (draw) */
                   A_MOVE,    /* Move forward without drawing */
                   A_PLUS,    /* Rotate angle degrees (turn left) */
                   A_MINUS,   /* Rotate -angle degrees (turn right) */
                   A_PIPE,    /* Turn around (left 180deg) */
                   A_PUSH,    /* Push position/angle to stack */
                   A_POP,     /* Pop position/angle from stack */
};

struct lsys_rule {
   char symbol;               /* Symbol (left side of rule) */
   enum lsys_action action;   /* Turtle action for symbol */
   char *right;               /* Right side */
   int rsz;                   /* Size of right side (speeds up expansion) */
};

#define MAX_RULES 15
struct lsystem {
   int maxlevel;              /* Max level for this L-system */
   int invert_angle;          /* Invert angle for odd iterations? */
   int angle;                 /* Turtle turn angle */
   float angle_offset;        /* Degrees to offset angle for each iteration */
   int init_angle;            /* Initial turtle angle */
   float init_x;              /* Initial turtle x/y position */
   float init_y;
   float init_len;            /* Initial turtle line length */
   float len_divisor;         /* Divide length by this divisor each iteration */
   char *axiom;               /* Axiom = starting rule of first iteration */
   int num_symbols;           /* Number of rules/symbols in this L-system */
   struct lsys_rule rules[MAX_RULES];
};

#define NUM_LSYSTEMS 28
extern struct lsystem *current_lsys;

extern int set_lsys(int num);
extern int munge_lsys(int change);


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */

#endif /* __LSYSTEMS_H */
