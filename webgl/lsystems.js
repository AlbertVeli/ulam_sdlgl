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
 * Boomtime, the 13th day of Bureaucracy in the YOLD 3179
 *
 * Albert Veli
 */

function lsys_levy() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/L%C3%A9vy_C_curve" target="_blank">Levy C curve</a>';
    if (multicolour) {
	axiom = '+C4F--C12F+';
	linelen = 2.0;
    } else {
	axiom = 'F';
	linelen = 3.0;
    }
    rules["F"] = '+F--F+';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 45;
    lendiv = Math.sqrt(2);
    max_level = 19;
    // Initial turtle position and angle
    turtle_x = -1.5;
    turtle_y = -1.0;
    angle = 0;
};

function lsys_terdragon() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Dragon_curve#Terdragon" target="_blank">Terdragon</a>';
    if (multicolour) {
	axiom = 'C2F+C14F-C4F';
	angle = -45 - (30 * level);
	linelen = 1.7;
    } else {
	axiom = 'F';
	angle = -20 - (30 * level);
	linelen = 3.0;
    }
    rules["F"] = 'F+F-F';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 120;
    lendiv = Math.sqrt(3);
    max_level = 12;
    turtle_x = -1.5;
    turtle_y = 0.5;
};

function lsys_twindragon() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Dragon_curve#Twindragon" target="_blank">Twindragon</a> (Davis-Knuth)';
    if (multicolour) {
	axiom = 'C4Fx+C14Fx+';
    } else {
	axiom = 'Fx+Fx-';
    }
    rules["F"] = '';
    rules["x"] = 'x+yF';
    rules["y"] = 'Fx-y';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 90;
    angle = -20 - (45 * level);
    linelen = 3.0;
    lendiv = Math.sqrt(2);
    max_level = 19;
    turtle_x = -0.961;
    turtle_y = -0.448;
};

function lsys_dragon() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Dragon_curve" target="_blank">Dragon</a> (Harter-Heighway)';
    if (multicolour) {
	axiom = 'C4Fx-C14Fy';
	rules["x"] = 'Fx+Fy';
	rules["y"] = 'Fx-Fy';
	angle = -110 - (45 * level);
	linelen = 1.5;
	turtle_x = 1.13;
	turtle_y = 0.77;
    } else {
	axiom = 'Fx';
	rules["x"] = 'x+yF+';
	rules["y"] = '-Fx-y';
	angle = -20 - (45 * level);
	linelen = 3.0;
	turtle_x = -0.8;
	turtle_y = -0.125;
    }
    rules["F"] = '';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 90;

    lendiv = Math.sqrt(2);
    max_level = 20;
};

function lsys_snowflake() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Koch_snowflake" target="_blank">Koch snowflake</a>';
    if (multicolour) {
	axiom = 'C2F--C14F--C4F';
    } else {
	axiom = 'F--F--F';
    }
    rules["F"] = 'F+F--F+F';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 60;
    linelen = 3.0;
    lendiv = 3;
    max_level = 9;
    turtle_x = -1.5;
    turtle_y = 0.85;
    angle = 0;
};

function lsys_flowsnake() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Gosper_curve" target="_blank">Flowsnake</a>';
    if (multicolour) {
	axiom = 'C2Fl-C6Fr--C3Fr+C4Fl++C10FlC14Fl+C12Fr-';
	linelen = 1.2;
	angle = 15;
    } else {
	axiom = 'Fl';
	linelen = 3.0;
	angle = 0;
    }
    rules["F"] = '';
    rules["l"] = 'Fl-Fr--Fr+Fl++FlFl+Fr-';
    rules["r"] = '+Fl-FrFr--Fr-Fl++Fl+Fr';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 60;
    lendiv = 2.65;
    max_level = 6;
    turtle_x = -1.5;
    turtle_y = 0.85;
    angle += 19.3 * level;
};

function lsys_sierpinski() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Sierpinski_triangle" target="_blank">Sierpinski triangle</a>';
    if (multicolour) {
	axiom = '+C2Fb-C14Fa-C4Fb+';
	linelen = 1.5;
    } else {
	axiom = 'Fa';
	linelen = 3;
    }
    rules["F"] = '';
    rules["a"] = '+Fb-Fa-Fb+';
    rules["b"] = '-Fa+Fb+Fa-';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 60;
    lendiv = 2.0;
    max_level = 12;
    turtle_x = -1.5;
    turtle_y = -1.299;
    angle = 0;
};

function lsys_sierpinski_polygon() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Sierpinski_triangle" target="_blank">Sierpinski Triangle</a>, Polygons';
    if (multicolour) {
	axiom = '{C4.F.+F.+F}';
	rules["F"] = '{.F.+F.}+f+fC4FC14';
    } else {
	axiom = '{.F.+F.+F}';
	rules["F"] = '{.F.+F.}+f+fF';
    }
    rules["f"] = 'ff';
    rules["+"] = '+';
    rules["-"] = '-';
    rules["{"] = '';
    rules["}"] = '';
    rules["."] = '';
    mode_poly = true;
    linelen = 3.0;
    pattern = axiom;
    turn_angle = 120;
    lendiv = 2.0;
    max_level = 9;
    turtle_x = -1.5;
    turtle_y = -1.299;
    angle = 0;
};

function lsys_church() {
    lsys_name = '<a href="http://www.nahee.com/spanky/www/fractint/lsys/spacefilling.html" target="_blank">Dekking\'s Church</a>';
    if (multicolour) {
	axiom = 'C1wC7xC15yC7z';
    } else {
	axiom = 'wxyz';
    }
    linelen = 2;
    rules["F"] = '';
    rules["w"] = 'Fw+F-zFw-F+x';
    rules["x"] = 'Fw+F-z';
    rules["y"] = '--F++y+F-z';
    rules["z"] = '--F++y-F+x--F++y-F+x';
    rules["+"] = '+';
    rules["-"] = '-';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 90;
    lendiv = 1.732;
    max_level = 11;
    turtle_x = -1.732;
    turtle_y = -1.0;
    angle = 0;
};

function lsys_plant() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/L-system#Example_7:_Fractal_plant" target="_blank">Plant</a>';
    if (multicolour) {
	axiom = 'C12x';
    } else {
	axiom = 'x';
    }
    rules["F"] = 'FF';
    rules["x"] = 'F-[[x]+x]+F[+Fx]-x';
    rules["+"] = '+';
    rules["-"] = '-';
    rules["["] = '[';
    rules["]"] = ']';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 23;
    linelen = 1.5;
    lendiv = 2;
    max_level = 9;
    turtle_x = 0.0;
    turtle_y = -2.0;
    angle = 90;
};

function lsys_sphinx() {
    lsys_name = '<a href="http://www.nahee.com/spanky/www/fractint/lsys/tilings.html" target="_blank">Sphinx</a> (Martin Gardner\'s "Penrose Tiles to Trapdoor Ciphers")';
    axiom = 'x';
    rules["F"] = 'ff';
    rules["f"] = 'ff';
    rules["x"] = '+FF-yFF+FF--FFF|x|F--yFFFyFFF|';
    rules["y"] = '-FF+xFF-FF++FFF|y|F++xFFFxFFF|';
    rules["+"] = '+';
    rules["-"] = '-';
    rules["|"] = '|';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 60;
    linelen = 2;
    lendiv = 2.0;
    max_level = 8;
    turtle_x = -3.0;
    turtle_y = -1.6;
    angle = 0;
};

function lsys_sphinx_polygon() {
    lsys_name = '<a href="http://www.nahee.com/spanky/www/fractint/lsys/tilings.html" target="_blank">Sphinx</a> Polygons';
    axiom = 'x';
    rules["F"] = 'ff';
    rules["f"] = 'ff';
    rules["x"] = '{C2.+FF.-yFFC10.}{C10.+FF.--FFFC2.|x|F.--yFFF.yFFF.|}';
    rules["y"] = '{C6.-FF.+xFFC14.}{C14.-FF.++FFFC6.|y|F.++xFFF.xFFF.|}';
    rules["+"] = '+';
    rules["-"] = '-';
    rules["|"] = '|';
    rules["{"] = '';
    rules["}"] = '';
    rules["."] = '';
    mode_poly = true;
    pattern = axiom;
    turn_angle = 60;
    linelen = 2;
    lendiv = 2.0;
    max_level = 8;
    turtle_x = -3.0;
    turtle_y = -1.6;
    angle = 0;
};

function lsys_penrose1() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Penrose_tiling" target="_blank">Penrose</a> #1';
    if (multicolour) {
	axiom = '+wC4FC6F--xC14FC4F---yC4FC14F--zC6FC4F';
	rules["w"] = 'yC4FC14F++zC6FC4F----xC14FC4F[-yC4FC14F----wC4FC6F]++';
	rules["x"] = '+yC4FC14F--zC6FC4F[---wC4FC6F--xC14FC4F]+';
	rules["y"] = '-wC4FC6F++xC14FC4F[+++yC4FC14F++zC6FC4F]-';
	rules["z"] = '--yC4FC14F++++wC4FC6F[+zC6FC4F++++xC14FC4F]--xC14FC4F';
	linelen = 1.0;
    } else {
	axiom = '+wF--xF---yF--zF';
	rules["w"] = 'yF++zF----xF[-yF----wF]++';
	rules["x"] = '+yF--zF[---wF--xF]+';
	rules["y"] = '-wF++xF[+++yF++zF]-';
	rules["z"] = '--yF++++wF[+zF++++xF]--xF';
	linelen = 2.0;
    }
    rules["F"] = '';
    rules["+"] = '+';
    rules["-"] = '-';
    rules["["] = '[';
    rules["]"] = ']';
    mode_poly = false;
    pattern = axiom;
    turn_angle = 36;
    lendiv = 1.65;
    max_level = 7;
    turtle_x = -1.5;
    turtle_y = 0.0;
    angle = 0;
};

function lsys_penrose_polygon() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Penrose_tiling" target="_blank">Penrose</a> Polygons';
    axiom = '[x]++[x]++[x]++[x]++[x]';
    rules["F"] = '';
    if (multicolour) {
	rules["w"] = 'yF++{C13.zF.----xF.[-yF.----wF.]}++';
	rules["x"] = '+{C1.yF.--zF.[---wF.--xF.]}+';
	rules["y"] = '-{C9.wF.++xF.[+++yF.++zF.]}-';
	rules["z"] = '--{C5.yF.++++wF.[+zF.++++xF.]}--xF';
    } else {
	rules["w"] = 'yF++{C7.zF.----xF.[-yF.----wF.]}++';
	rules["x"] = '+{C15.yF.--zF.[---wF.--xF.]}+';
	rules["y"] = '-{C15.wF.++xF.[+++yF.++zF.]}-';
	rules["z"] = '--{C7.yF.++++wF.[+zF.++++xF.]}--xF';
    }
    rules["+"] = '+';
    rules["-"] = '-';
    rules["["] = '[';
    rules["]"] = ']';
    rules["{"] = '{';
    rules["}"] = '}';
    rules["."] = '.';
    mode_poly = true;
    linelen = 2.0;
    pattern = axiom;
    turn_angle = 36;
    lendiv = 1.65;
    max_level = 7;
    turtle_x = 0.0;
    turtle_y = 0.0;
    angle = 0;
};

function lsys_penrose_polygon_2() {
    lsys_name = '<a href="http://en.wikipedia.org/wiki/Penrose_tiling" target="_blank">Penrose</a> Polygons 2';
    axiom = '[y]++[y]++[y]++[y]++[y]';
    rules["F"] = '';
    if (multicolour) {
	rules["w"] = 'yF++{C14.zF.----xF.[-yF.----wF.]}++';
	rules["x"] = '+{C1.yF.--zF.[---wF.--xF.]}+';
	rules["y"] = '-{C9.wF.++xF.[+++yF.++zF.]}-';
	rules["z"] = '--{C6.yF.++++wF.[+zF.++++xF.]}--xF';
    } else {
	rules["w"] = 'yF++{C7.zF.----xF.[-yF.----wF.]}++';
	rules["x"] = '+{C15.yF.--zF.[---wF.--xF.]}+';
	rules["y"] = '-{C15.wF.++xF.[+++yF.++zF.]}-';
	rules["z"] = '--{C7.yF.++++wF.[+zF.++++xF.]}--xF';
    }
    rules["+"] = '+';
    rules["-"] = '-';
    rules["["] = '[';
    rules["]"] = ']';
    rules["{"] = '{';
    rules["}"] = '}';
    rules["."] = '.';
    mode_poly = true;
    linelen = 2.0;
    pattern = axiom;
    turn_angle = 36;
    lendiv = 1.65;
    max_level = 7;
    turtle_x = 0.0;
    turtle_y = 0.0;
    angle = 0;
};
