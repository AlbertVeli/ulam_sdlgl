/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * http://en.wikipedia.org/wiki/L-system
 *
 * Rules for most of these L-systems are from
 * The Algorithmic Beauty of Plants by Prusinkiewicz and Lindenmayer
 *
 * http://algorithmicbotany.org/papers/
 *
 * Some additional L-systems are from fractint.l, found in the source
 * tree of xfractint (fractint.org) which in turn mentions:
 *
 * - The Fractal Geometry of Nature, by Mandelbrot
 * - The Science of Fractal Images, by Peitgen, Saupe, Fisher, McGuire,
 *                                     Voss, Barnsley, Devaney, Mandelbrot
 * - the Algorithmic Beauty of Plants (see top)
 * - Penrose Tiles to Trapdoor Ciphers, by Martin Gardner
 *
 * And a some variations from
 * http://mathafou.free.fr/themes_en/fractls.html and
 * http://malsys.cz/
 *
 *
 * Boomtime, the 3rd day of Bureaucracy in the YOLD 3179
 *
 * Albert Veli
 */

#include "lsystems.h"

/* Sierpinski L-system */
#define A_SIERP "B-A-B"
#define B_SIERP "A+B+A"
struct lsystem sierpinski = {
   14, 1, 60, 0.0, 0, -1.5, -1.33, 3.0, 2.0,
   "A",  /* Axiom */
   4,    /* Number of rules/symbols */
   {  /* Sym Action     Expansion Size of expansion */
      { 'A', A_FORWARD, A_SIERP, sizeof(A_SIERP) - 1 },
      { 'B', A_FORWARD, B_SIERP, sizeof(B_SIERP) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Dragon curve */
#define L_DRAGON "l+rF+"
#define R_DRAGON "-Fl-r"
struct lsystem dragon = {
   19, 0, 90, -45.0, 0, -1.0, 0.0, 2.0, 1.414,
   "Fl",
   5,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'l', A_NULL, L_DRAGON, sizeof(L_DRAGON) - 1 },
      { 'r', A_NULL, R_DRAGON, sizeof(R_DRAGON) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Rounded 2-coloured dragon */
#define X_RDR "FX-@.5FZ@2-FY"
#define Y_RDR "FX+@.5FZ@2+FY"
#define Z_RDR "FZ"
struct lsystem rdragon = {
   19, 0, 45, 45.0, 40, -1.5, -0.5, 0.8, 1.414,
   /* "X", */
   "C0FX-C1FY",
   6,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'X', A_NULL, X_RDR, sizeof(X_RDR) - 1 },
      { 'Y', A_NULL, Y_RDR, sizeof(Y_RDR) - 1 },
      { 'Z', A_NULL, Z_RDR, sizeof(Z_RDR) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* 3-coloured dragon */
#define F_TDR "F+F-F"
struct lsystem terdragon = {
   19, 0, 120, -30.0, -45, -2.1, 0.5, 2.5, 1.73,
   "C3F+C10F-C6F", /* Use only "F" for monocolor */
   3,
   {
      { 'F', A_FORWARD, F_TDR, sizeof(F_TDR) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* von Koch snowflake */
#define F_KOCH "F+F--F+F"
struct lsystem koch = {
   10, 0, 60, 0.0, 0, -1.5, 0.85, 3.0, 3.0,
   "F--F--F",
   3,
   {
      { 'F', A_FORWARD, F_KOCH, sizeof(F_KOCH) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Quadratic modification of von Koch snowflake */
#define F_QKOCHS "F+F-F-F+F"
struct lsystem quad_koch_snow = {
   8, 0, 90, 0.0, -90, -1.5, -0.66, 3.0, 3.0,
   "+F",
   3,
   {
      { 'F', A_FORWARD, F_QKOCHS, sizeof(F_QKOCHS) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Quadratic von Koch island */
#define F_QKOCH "F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F"
struct lsystem quad_koch = {
   5, 0, 90, 0.0, 0, -1, 1, 2.0, 6.0,
   "F-F-F-F",
   3,
   {
      { 'F', A_FORWARD, F_QKOCH, sizeof(F_QKOCH) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* von Koch island variation */
#define F_KOCHVAR "F-FF|F-F"
struct lsystem koch_island_variation = {
   8, 0, 90, 26.6, 0, -1, 1, 2.0, 2.24,
   "F-F-F-F",
   4,
   {
      { 'F', A_FORWARD, F_KOCHVAR, sizeof(F_KOCHVAR) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '|', A_PIPE, "|", 1 }
   }
};

/* Quadratic von Koch islands/lakes */
#define F_ISLAKE "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF"
struct lsystem koch_islands_lakes = {
   5, 0, 90, 0.0, 0, -1, -1, 2.0, 6.0,
   "F+F+F+F",
   4,
   {
      { 'F', A_FORWARD, F_ISLAKE, sizeof(F_ISLAKE) - 1 },
      { 'f', A_MOVE, "ffffff", 6 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Hexagonal Gosper */
#define X_HEXAG "X+YF++YF-FX--FXFX-YF+"
#define Y_HEXAG "-FX+YFYF++YF+FX--FX-Y"
struct lsystem hexa_gosper = {
   11, 0, 60, -19.3, 0, -1.5, -1.0, 3.0, 2.65,
   "XF",
   5,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'X', A_NULL, X_HEXAG, sizeof(X_HEXAG) - 1 },
      { 'Y', A_NULL, Y_HEXAG, sizeof(Y_HEXAG) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Flowsnake */
#define L_FLOWS "FL-FR--FR+FL++FLFL+FR-"
#define R_FLOWS "+FL-FRFR--FR-FL++FL+FR"
struct lsystem flowsnake = {
   11, 0, 60, 19.3, 0, -1.0, 1.0, 1.0, 2.65,
   "C1FL-C5FR--C2FR+C7FL++C17FLC10FL+C3FR-", /* "FL" for monocolor */
   5,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'L', A_NULL, L_FLOWS, sizeof(L_FLOWS) - 1 },
      { 'R', A_NULL, R_FLOWS, sizeof(R_FLOWS) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Sierpinski square */
#define F_QSIERP "FF-F-F-F-FF"
struct lsystem quad_sierpinski = {
   5, 0, 90, 0.0, 0, -1, 1, 2.0, 3.0,
   "F-F-F-F",
   3,
   {
      { 'F', A_FORWARD, F_QSIERP, sizeof(F_QSIERP) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Peano */
#define L_PEANO "l+F+r-F-l+F+r-F-l-F-r+F+l-F-r-F-l+F+r-F-l-F-r-F-l+F+r+F+l+F+r-F-l+F+r+F+l-F-r+F+l+F+r-F-l+F+r-F-l"
#define R_PEANO "r-F-l+F+r-F-l+F+r+F+l-F-r+F+l+F+r-F-l+F+r+F+l+F+r-F-l-F-r-F-l+F+r-F-l-F-r+F+l-F-r-F-l+F+r-F-l+F+r"
struct lsystem peano = {
   5, 0, 45, 0.0, 45, -1.5, -1.5, 3.0, 5.0,
   "l",
   5,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'l', A_NULL, L_PEANO, sizeof(L_PEANO) - 1 },
      { 'r', A_NULL, R_PEANO, sizeof(R_PEANO) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Peano 2 */
#define L_PEANO2 "lFrFl-F-rFlFr+F+lFrFl"
#define R_PEANO2 "rFlFr+F+lFrFl-F-rFlFr"
struct lsystem peano2 = {
   7, 0, 90, 0.0, 90, -1.4, -1.5, 3.0, 3.0,
   "l",
   5,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'l', A_NULL, L_PEANO2, sizeof(L_PEANO2) - 1 },
      { 'r', A_NULL, R_PEANO2, sizeof(R_PEANO2) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Hilbert */
#define X_HILBERT "-YF+XFX+FY-"
#define Y_HILBERT "+XF-YFY-FX+"
struct lsystem hilbert = {
   9, 0, 90, 0.0, 0, -1.2, 1.0, 3.0, 2.05,
   "X",
   5,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'X', A_NULL, X_HILBERT, sizeof(X_HILBERT) - 1 },
      { 'Y', A_NULL, Y_HILBERT, sizeof(Y_HILBERT) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Mango kolam */
#define A_MKOLAM "f-F+Z+F-fA"
#define Z_MKOLAM "F-FF-F--[--Z]F-FF-F--F-FF-F--"
struct lsystem mango_kolam = {
   20, 0, 60, 0.0, 90, 0.0, -2.0, 1.0, 1.3,
   "A---A",
   8,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'f', A_MOVE, "f", 1 },
      { 'A', A_NULL, A_MKOLAM, sizeof(A_MKOLAM) - 1 },
      { 'Z', A_NULL, Z_MKOLAM, sizeof(Z_MKOLAM) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Hexagonal kolam */
#define X_HKOLAM "[-F+F[Y]+F][+F-F[X]-F]"
#define Y_HKOLAM "[-F+F[Y]+F][+F-F-F]"
struct lsystem hexa_kolam = {
   20, 0, 60, 0.0, 90, 0.0, -2.0, 1.0, 1.15,
   "X",
   7,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'X', A_NULL, X_HKOLAM, sizeof(X_HKOLAM) - 1 },
      { 'Y', A_NULL, Y_HKOLAM, sizeof(Y_HKOLAM) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Penrose L-systems. Taken from fractint, which in turn got them
 * from Martin Gardner's "Penrose Tiles to Trapdoor Ciphers".
 */
#define PENROSE_W "YF++ZF----XF[-YF----WF]++"
#define PENROSE_X "+YF--ZF[---WF--XF]+"
#define PENROSE_Y "-WF++XF[+++YF++ZF]-"
#define PENROSE_Z "--YF++++WF[+ZF++++XF]--XF"
struct lsystem penrose1 = {
   20, 0, 36, 0.0, 0, -1.5, 0.0, 2.0, 1.65,
   "+WF--XF---YF--ZF",
   9,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'W', A_NULL, PENROSE_W, sizeof(PENROSE_W) - 1 },
      { 'X', A_NULL, PENROSE_X, sizeof(PENROSE_X) - 1 },
      { 'Y', A_NULL, PENROSE_Y, sizeof(PENROSE_Y) - 1 },
      { 'Z', A_NULL, PENROSE_Z, sizeof(PENROSE_Z) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

#define CPENROSE_W "YC10FC3F++ZC1FC10F----XC3FC10F[-YC10FC3F----WC10FC1F]++"
#define CPENROSE_X "+YC10FC3F--ZC1FC10F[---WC10FC1F--XC3FC10F]+"
#define CPENROSE_Y "-WC10FC1F++XC3FC10F[+++YC10FC3F++ZC1FC10F]-"
#define CPENROSE_Z "--YC10FC3F++++WC10FC1F[+ZC1FC10F++++XC3FC10F]--XC3FC10F"
struct lsystem colorpenrose1 = {
   20, 0, 36, 0.0, 0, -1.5, 0.0, 1.0, 1.65,
   "+WC10FC1F--XC3FC10F---YC10FC3F--ZC1FC10F",
   9,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'W', A_NULL, CPENROSE_W, sizeof(CPENROSE_W) - 1 },
      { 'X', A_NULL, CPENROSE_X, sizeof(CPENROSE_X) - 1 },
      { 'Y', A_NULL, CPENROSE_Y, sizeof(CPENROSE_Y) - 1 },
      { 'Z', A_NULL, CPENROSE_Z, sizeof(CPENROSE_Z) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

struct lsystem penrose2 = {
   20, 0, 36, 0.0, 0, 0.0, 0.0, 2.0, 1.65,
   "++ZF----XF-YF----WF",
   9,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'W', A_NULL, PENROSE_W, sizeof(PENROSE_W) - 1 },
      { 'X', A_NULL, PENROSE_X, sizeof(PENROSE_X) - 1 },
      { 'Y', A_NULL, PENROSE_Y, sizeof(PENROSE_Y) - 1 },
      { 'Z', A_NULL, PENROSE_Z, sizeof(PENROSE_Z) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

struct lsystem penrose3 = {
   20, 0, 36, 0.0, 0, 0.0, 0.0, 2.0, 1.65,
   "[X]++[X]++[X]++[X]++[X]",
   9,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'W', A_NULL, PENROSE_W, sizeof(PENROSE_W) - 1 },
      { 'X', A_NULL, PENROSE_X, sizeof(PENROSE_X) - 1 },
      { 'Y', A_NULL, PENROSE_Y, sizeof(PENROSE_Y) - 1 },
      { 'Z', A_NULL, PENROSE_Z, sizeof(PENROSE_Z) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

struct lsystem penrose4 = {
   20, 0, 36, 0.0, 0, 0.0, 0.0, 2.0, 1.65,
   "[Y]++[Y]++[Y]++[Y]++[Y]",
   9,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'W', A_NULL, PENROSE_W, sizeof(PENROSE_W) - 1 },
      { 'X', A_NULL, PENROSE_X, sizeof(PENROSE_X) - 1 },
      { 'Y', A_NULL, PENROSE_Y, sizeof(PENROSE_Y) - 1 },
      { 'Z', A_NULL, PENROSE_Z, sizeof(PENROSE_Z) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

struct lsystem double_penrose = {
   20, 0, 36, 0.0, 0, 0.0, 0.0, 2.0, 1.65,
   "[X][Y]++[X][Y]++[X][Y]++[X][Y]++[X][Y]",
   9,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'W', A_NULL, PENROSE_W, sizeof(PENROSE_W) - 1 },
      { 'X', A_NULL, PENROSE_X, sizeof(PENROSE_X) - 1 },
      { 'Y', A_NULL, PENROSE_Y, sizeof(PENROSE_Y) - 1 },
      { 'Z', A_NULL, PENROSE_Z, sizeof(PENROSE_Z) - 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Also by Roger Penrose */
#define F_PENTAPLEX "F++F++F-----F-F++F"
struct lsystem pentaplexity = {
   20, 0, 36, 0.0, 0, -1.0, -1.5, 2.0, 2.62,
   "F++F++F++F++F",
   3,
   {
      { 'F', A_FORWARD, F_PENTAPLEX, sizeof(F_PENTAPLEX) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Plant */
#define X_PLANT "F-[[X]+X]+F[+FX]-X"
struct lsystem plant = {
   10, 0, 23, 0.0, 90, 0.0, -2.0, 1.5, 2.0,
   "C1X", /* Axiom = X, C1 = colour number 1 in palette */
   6,
   {
      { 'F', A_FORWARD, "FF", 2 },
      { 'X', A_NULL, X_PLANT, sizeof(X_PLANT) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 }
   }
};

/* Fern (ormbunke), ABoP, Fig 5.12b p.130 */
#define X_FERN "F[+A]FY"
#define Y_FERN "F[-B]FX"
struct lsystem fern = {
   34, 0, 45, 0.0, 90, 0.0, -2.0, 1, 1.23,
   "C1X",
   15,
   {
      { 'F', A_FORWARD, "@1.18F@0.84746", sizeof("@1.18F@0.84746") - 1 },
      { 'X', A_NULL, X_FERN, sizeof(X_FERN) - 1 },
      { 'Y', A_NULL, Y_FERN, sizeof(Y_FERN) - 1 },
      { 'A', A_NULL, "N", 1 },
      { 'N', A_NULL, "O", 1 },
      { 'O', A_NULL, "P", 1 },
      { 'P', A_NULL, "X", 1 },
      { 'B', A_NULL, "E", 1 },
      { 'E', A_NULL, "H", 1 },
      { 'H', A_NULL, "J", 1 },
      { 'J', A_NULL, "Y", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 }
   }
};

/* Leaf2, ABoP, Fig 5.12a p.130 */
#define A_LEAF2 "F[+X]FB"
#define B_LEAF2 "F[-Y]FA"
struct lsystem leaf2 = {
   40, 0, 45, 0.0, 90, 0.0, -2.0, 1, 1.36,
   "C1A",
   9,
   {
      { 'F', A_FORWARD, "@1.36F@0.735294", sizeof("@1.36F@0.735294") - 1 },
      { 'A', A_NULL, A_LEAF2, sizeof(A_LEAF2) - 1 },
      { 'B', A_NULL, B_LEAF2, sizeof(B_LEAF2) - 1 },
      { 'X', A_NULL, "A", 1 },
      { 'Y', A_NULL, "B", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 }
   }
};

#define X_CESARO "----F!X!++++++++F!X!----"
struct lsystem cesaro = {
   16, 1, 35, 0.0, 0, -1.0, -1.0, 1.5, 1.5,
   "FX",
   5,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'X', A_NULL, X_CESARO, sizeof(X_CESARO) - 1 },
      { '!', A_INVERT, "!", 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Sphinx, Martin Gardner's "Penrose Tiles to Trapdoor Ciphers" */
#define X_SPHINX "+FF-YFF+FF--FFF|X|F--YFFFYFFF|"
#define Y_SPHINX "-FF+XFF-FF++FFF|Y|F++XFFFXFFF|"
struct lsystem sphinx = {
   8, 0, 60, 0.0, 0, -2.0, -1.0, 1.4, 2.0,
   "X",
   7,
   {
      { 'F', A_FORWARD, "ff", 2 },
      { 'f', A_FORWARD, "ff", 2 },
      { 'X', A_NULL, X_SPHINX, sizeof(X_SPHINX) - 1 },
      { 'Y', A_NULL, Y_SPHINX, sizeof(Y_SPHINX) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '|', A_PIPE, "|", 1 }
   }
};

#define F_LEVY "+F--F+"
struct lsystem levy = {
   17, 0, 45, 0.0, 0, -1.0, -1.0, 1.5, 1.42,
   "+C0F--C1F+", /* Replace with "F" for monocolor */
   3,
   {
      { 'F', A_FORWARD, F_LEVY, sizeof(F_LEVY) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

#define W_LACE "+++x--F--zFx+"
#define X_LACE "---w++F++yFw-"
#define Y_LACE "+zFx--F--z+++"
#define Z_LACE "-yFw++F++y---"
struct lsystem lace3060 = {
   11, 0, 30, 0.0, 0, -1.8, -1.0, 0.33, 1.75,
   /* "w", for monocolor */
   "C3+++x--F--zFx+++F+++zFx--F--z+++F+++x--F--zFx--F"
   "C10--zFx--F--z+++F+++x--F--zFx+++F+++zFx--F--zF"
   "C6x--F--zFx+++F+++zFx--F--z+++F+++x--F--zFx+",
   7,
   {
      { 'F', A_FORWARD, "F", 1 },
      { 'w', A_NULL, W_LACE, sizeof(W_LACE) - 1 },
      { 'x', A_NULL, X_LACE, sizeof(X_LACE) - 1 },
      { 'y', A_NULL, Y_LACE, sizeof(Y_LACE) - 1 },
      { 'z', A_NULL, Z_LACE, sizeof(Z_LACE) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Rounded Peano */
#define X_PEANOR "FX-@0.5FZ@2.0-FX+@0.5FZ@2.0+FX+@0.5FZ@2.0+FX+@0.5FZ@2.0+FX-@0.5FZ@2.0-FX-@0.5FZ@2.0-FX-@0.5FZ@2.0-FX+@0.5FZ@2.0+FX"
struct lsystem peanornd = {
   5, 0, 45, 0.0, 0, -2.0, 0.0, 2.0, 3.0,
   "FX",
   5,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'X', A_NULL, X_PEANOR, sizeof(X_PEANOR) - 1 },
      { 'Z', A_NULL, "FZ", 2 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 }
   }
};

/* Dekking's Church */
#define W_DEKKING "Fw+F-zFw-F+x"
#define X_DEKKING "Fw+F-z"
#define Y_DEKKING "|F|y+F-z"
#define Z_DEKKING "|F|y-F+x|F|y-F+x"
struct lsystem dekkingschurch = {
   9, 0, 90, 0.0, 0, -0.5, -0.2, 2.0, 2.0,
   "wxyz",
   8,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'w', A_NULL, W_DEKKING, sizeof(W_DEKKING) - 1 },
      { 'x', A_NULL, X_DEKKING, sizeof(X_DEKKING) - 1 },
      { 'y', A_NULL, Y_DEKKING, sizeof(Y_DEKKING) - 1 },
      { 'z', A_NULL, Z_DEKKING, sizeof(Z_DEKKING) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '|', A_PIPE, "|", 1 }
   }
};

#define X_PENTAC "@.618034++f[C6@1.1755-------f][|y]----C2f[|x][|++@.618034g@1.618034x]------[x]f----[y]f"
#define Y_PENTAC "@.618034++++[x]C2f|++f[|y]--[y]f|++C2f[|x]"
struct lsystem pentacolor = {
   17, 0, 18, 0.0, 0, 0.0, 0.0, 2.0, 1.0,
   "C2[x]++++[x]++++[x]++++[x]++++[x]",
   9,
   {
      { 'f', A_FORWARD, "g", 1 },
      { 'g', A_MOVE, "g", 1 },
      { 'x', A_NULL, X_PENTAC, sizeof(X_PENTAC) - 1 },
      { 'y', A_NULL, Y_PENTAC, sizeof(Y_PENTAC) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '|', A_PIPE, "|", 1 }
   }
};

/* Looks a bit buggy, but nice pattern anyway */
#define W_KITESDARTSC "[C1F][++@1.618033989C3F][++G---@.618033989G|X-Y|G|W]"
#define X_KITESDARTSC "[C1F+++@1.618033989C3F][++@.618033989GZ|X|-G|W]"
#define Y_KITESDARTSC "[+C1F][@1.618033989C3F][+G@.618033989|Y+X]"
#define Z_KITESDARTSC "[-C1F][@1.618033989C3F][@.618033989G--WG|+Z]"
struct lsystem kites_darts_color = {
   17, 0, 36, 0.0, 0, -0.65, -2.0, 1.3, 1.0,
   "C1WG+XG+WG+XG+WG+XG+WG+XG+WG+X",
   11,
   {
      { 'F', A_FORWARD, "", 0 },
      { 'G', A_MOVE, "G", 1 },
      { 'W', A_NULL, W_KITESDARTSC, sizeof(W_KITESDARTSC) - 1 },
      { 'X', A_NULL, X_KITESDARTSC, sizeof(X_KITESDARTSC) - 1 },
      { 'Y', A_NULL, Y_KITESDARTSC, sizeof(Y_KITESDARTSC) - 1 },
      { 'Z', A_NULL, Z_KITESDARTSC, sizeof(Z_KITESDARTSC) - 1 },
      { '+', A_PLUS, "+", 1 },
      { '-', A_MINUS, "-", 1 },
      { '[', A_PUSH, "[", 1 },
      { ']', A_POP, "]", 1 },
      { '|', A_PIPE, "|", 1 }
   }
};


static struct lsystem *lsystems[NUM_LSYSTEMS] = {
   &sierpinski,            /* 0 */
   &terdragon,             /* 1 */
   &koch,                  /* 2 */
   &pentacolor,            /* 3 */
   &plant,                 /* 4 */
   &quad_sierpinski,       /* 5 */
   &quad_koch_snow,        /* 6 */
   &leaf2,                 /* 7 */
   &levy,                  /* 8 */
   &peano,                 /* 9 */
   &double_penrose,
   &hexa_kolam,
   &hexa_gosper,
   &flowsnake,
   &koch_island_variation,
   &koch_islands_lakes,
   &penrose1,
   &colorpenrose1,
   &penrose2,
   &penrose3,
   &penrose4,
   &pentaplexity,
   &sphinx,
   &cesaro,
   &kites_darts_color,
   &quad_koch,
   &hilbert,
   &fern,
   &lace3060,
   &dekkingschurch,
   &rdragon,
   &dragon,
   &mango_kolam,
   &peanornd,
   &peano2
};

/* Current L-system */
static int current_lsys_num = -1;
struct lsystem *current_lsys = (struct lsystem *)0;

/* Return 1 if lsystem was changed (new pattern needs to be compiled) */
int set_lsys(int num)
{
   if (num < 0 || num >= NUM_LSYSTEMS) {
      if (num < 0) {
         if (current_lsys_num != 0) {
            current_lsys = lsystems[0];
            current_lsys_num = 0;
            return 1;
         } else {
            return 0;
         }
      } else {
         if (current_lsys_num != NUM_LSYSTEMS - 1) {
            current_lsys = lsystems[NUM_LSYSTEMS - 1];
            current_lsys_num = NUM_LSYSTEMS - 1;
            return 1;
         } else {
            return 0;
         }
      }
   }

   /* num is in range */

   if (current_lsys_num != num) {
      current_lsys_num = num;
      current_lsys = lsystems[num];
      return 1;
   }

   return 0;
}


/* Increase/decrease current_lsys_num */
int munge_lsys(int change)
{
   return set_lsys(current_lsys_num + change);
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
