function debug_out(s, br) {
    var x=document.getElementById("debug_area");
    x.innerHTML += s;
    if (br) {
	x.innerHTML += '<br />';
    }
};

function clear_debug() {
    var x=document.getElementById("debug_area");
    x.innerHTML = '';
};

// L-systems
var lsystems = [];
var current_lsystem;
var lsys_name;
var axiom;
var rules = {};
var pattern = '';
var level = 3;
var max_level = 9;

var angle;
var turn_angle;
var pipe_angle;
var linelen;
var lendiv;
var turtle_x;
var turtle_y;

var coords_arr = [];
var coords;

var poly_arr = [];
var poly;

var colours_arr = [];
var colours;
var current_colour = 15;
var multicolour = true;

var stack_x = [];
var stack_y = [];
var stack_len = [];
var stack_col = [];
var stack_angle = [];

function push() {
    stack_x.push(turtle_x);
    stack_y.push(turtle_y);
    stack_len.push(linelen);
    stack_col.push(current_colour);
    stack_angle.push(angle);
};

function pop() {
    turtle_x = stack_x.pop();
    turtle_y = stack_y.pop();
    linelen = stack_len.pop();
    current_colour = stack_col.pop();
    angle = stack_angle.pop();
};

var palette = [
    // Bright 0
    0.0, 0.0, 0.0, 1.0, // 0 Black
    0.0, 0.0, 0.5, 1.0, // 1 Blue
    0.5, 0.0, 0.0, 1.0, // 2 Red
    0.5, 0.0, 0.5, 1.0, // 3 Magenta
    0.0, 0.5, 0.0, 1.0, // 4 Green
    0.0, 0.5, 0.5, 1.0, // 5 Cyan
    0.5, 0.5, 0.0, 1.0, // 6 Yellow
    0.5, 0.5, 0.5, 1.0, // 7 White
    // Bright 1
    0.0, 0.0, 0.0, 1.0, // 8 Black
    0.0, 0.0, 1.0, 1.0, // 9 Blue
    1.0, 0.0, 0.0, 1.0, // 10 Red
    1.0, 0.0, 1.0, 1.0, // 11 Magenta
    0.0, 1.0, 0.0, 1.0, // 12 Green
    0.0, 1.0, 1.0, 1.0, // 13 Cyan
    1.0, 1.0, 0.0, 1.0, // 14 Yellow
    1.0, 1.0, 1.0, 1.0  // 15 White
];

function push_colour() {
    var i = current_colour * 4;
    colours.push(palette[i + 0]);
    colours.push(palette[i + 1]);
    colours.push(palette[i + 2]);
    colours.push(palette[i + 3]);
};

// clear() method for Array class
Array.prototype.clear = function() {
  while (this.length > 0) {
    this.pop();
  }
};

function clear_arr_arr(arr) {
    var i;
    for (i = 0; i < arr.length; i++) {
	arr[i].clear();
    }
    arr.clear();
};

function render_arrays() {
    var first = true;
    var c;

    clear_arr_arr(coords_arr);
    clear_arr_arr(poly_arr);
    clear_arr_arr(colours_arr);
    coords = new Array();
    poly = new Array();
    colours = new Array();
    current_colour = 15;

    for(i = 0; i < pattern.length; i++) {

	switch (pattern[i]) {

	case 'F': // Draw line Forward
	    if (first) {
		// First F in this line strip
		coords.push(turtle_x);
		coords.push(turtle_y);
		push_colour();
		first = false;
	    }
	    turtle_x += linelen * Math.cos(angle);
	    turtle_y += linelen * Math.sin(angle);
	    coords.push(turtle_x);
	    coords.push(turtle_y);
	    push_colour();
	    break;

	case 'f': // Move forward without drawing
	    if (!first) {
		// End line strip
		coords_arr.push(coords);
		colours_arr.push(colours);
		coords = new Array();
		colours = new Array();
		first = true;
	    }
	    turtle_x += linelen * Math.cos(angle);
	    turtle_y += linelen * Math.sin(angle);
	    break;

	case '+':
	    angle += turn_angle;
	    break;

	case '-':
	    angle -= turn_angle;
	    break;

	case '|':
	    angle += pipe_angle;
	    break;

	case '[':
	    push();
	    break;

	case ']':
	    if (!first) {
		// End strip
		if (coords.length > 0) {
		    coords_arr.push(coords);
		    coords = new Array();
		}
		if (poly.length > 0) {
		    poly_arr.push(poly);
		    poly = new Array();
		}
		if (colours.length > 0) {
		    colours_arr.push(colours);
		    colours = new Array();
		}
		first = true;
	    }
	    pop();
	    break;

	case 'G': // Move forward without recording vertex
	    if (first) {
		poly.push(turtle_x);
		poly.push(turtle_y);
		push_colour();
		first = false;
	    }
	    turtle_x += linelen * Math.cos(angle);
	    turtle_y += linelen * Math.sin(angle);
	    break;

	case '{':
	    if (!first) {
//		poly.push(turtle_x);
//		poly.push(turtle_y);
//		push_colour();
		if (poly.length > 0) {
		    poly_arr.push(poly);
		    poly = new Array();
		}
		if (colours.length > 0) {
		    colours_arr.push(colours);
		    colours = new Array();
		}
		first = true;
	    }
	    break;

	case '}':
	    if (!first) {
		// End polygon
//		poly.push(turtle_x);
//		poly.push(turtle_y);
//		push_colour();
		if (poly.length > 0) {
		    poly_arr.push(poly);
		    poly = new Array();
		}
		if (colours.length > 0) {
		    colours_arr.push(colours);
		    colours = new Array();
		}
		first = true;
	    }
	    break;

	case '.': // Record polygon vertex
	    first = false;
	    poly.push(turtle_x);
	    poly.push(turtle_y);
	    push_colour();
	    break;

	case 'C':
	    i++;
	    c = pattern[i];
	    var num = '';
	    while (c >= '0' && c <= '9' && i < pattern.length) {
		num += c;
		i++;
		c = pattern[i];
	    }
	    current_colour = parseInt(num);
	    i--; // increased by for loop
	    break;
	}
    }
    // End last strip
    if (coords.length > 0) {
	coords_arr.push(coords);
    }
    if (poly.length > 0) {
	poly_arr.push(poly);
    }
    if (colours.length > 0) {
	colours_arr.push(colours);
    }
};

function angles_to_radians() {
    angle *= 0.01745329251994;
    turn_angle *= 0.01745329251994;
    pipe_angle = -180 * 0.01745329251994;
}

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
    pattern = axiom;
    turn_angle = 60;
    lendiv = 2.0;
    max_level = 12;
    turtle_x = -1.5;
    turtle_y = -1.33;
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
//    axiom = '[y]++[y]++[y]++[y]++[y]';
//    axiom = '+wG--xG--yG--zG';
    rules["G"] = '';
    rules["w"] = 'yG++{C1.zG.----xG.[-yG.----wG.}]++';
    rules["x"] = '+{C6.yG.--zG.[---wG.--xG.}]+';
    rules["y"] = '-{C14.wG.++xG.[+++yG.++zG.}]-';
    rules["z"] = '--{C3.yG.++++wG.[+zG.++++xG.}]--xG';
    rules["+"] = '+';
    rules["-"] = '-';
    rules["["] = '[';
    rules["]"] = ']';
    rules["{"] = '';
    rules["}"] = '';
    rules["."] = '';
    linelen = 2.0;
    pattern = axiom;
    turn_angle = 36;
    lendiv = 1.65;
    max_level = 7;
    turtle_x = 0.0;
    turtle_y = 0.0;
    angle = 0;
};

function clear_lsys() {
    pattern = '';
    axiom = '';
    rules = {};
};

function expand_lsys() {
    var i;
    var new_pattern = '';

    for(i = 0; i < pattern.length; i++) {
	var c = pattern[i];

	// Pass through any C followed by integer
	if (c == 'C') {
	    new_pattern += c;
	    i++;
	    c = pattern[i];
	    while (c >= '0' && c <= '9' && i < pattern.length) {
		new_pattern += c;
		i++;
		c = pattern[i];
	    }
	    i--;
	    if (i >= pattern.length) {
		break; // finished
	    }
	} else {
	    // Normal rule
	    new_pattern += rules[c];
	}
    }
    pattern = new_pattern;
    linelen /= lendiv;
};

function out_keys() {
    debug_out('<br />' + 'KEYS');
    debug_out('<pre>' + 
	      ' o,p        ; Cycle through L-systems' + '\n' +
	      ' m          ; Toggle multi/mono colour' + '\n' +
	      ' +,c        ; Increase level' + '\n' +
	      ' -,x        ; Decrease level</pre>');
    debug_out('CAMERA');
    debug_out('<pre>' + 
	      ' Up,Down    ; Zoom in/out (z-axis)' + '\n' +
	      ' Left,Right ; Move left/right (x-axis)' + '\n' +
	      ' f,v        ; Move up/down (y-axis)' + '\n' +
	      ' Space      ; Reset camera</pre>');
};

var rendered;


// Called every time lsystem or level is changed
function init_lsystem() {
    clear_debug();
    lsystems[current_lsystem]();
    angles_to_radians();
    // max_level might have changed
    if (level > max_level) {
	level = max_level;
    }
    debug_out('Calculating coordinates for ' + lsys_name + ' level: ' + level, true);
    // Wait 10ms so message will be output before crunching begins
    rendered = false;
    setTimeout(init_lsystem2, 10);
};

// Continue here after small delay
function init_lsystem2() {
    var i, vertices, cols;

    clear_debug();
    for (i = 0; i < level; i++) {
	expand_lsys();
    }
    render_arrays();
    debug_out('L-system: ' + lsys_name, true);
    debug_out('Level: ' + level, true);
    vertices = 0;
    cols = 0;
    for (i = 0; i < coords_arr.length; i++) {
	vertices += coords_arr[i].length / 2;
	cols += colours_arr[i].length / 4;
    }
    for (i = 0; i < poly_arr.length; i++) {
	vertices += poly_arr[i].length / 2;
	cols += colours_arr[i].length / 4;
    }
    debug_out('Number of vertices: ' + vertices + ' (' + cols + ')', true);
    out_keys();
    init_buffers();
    rendered = true;
};

// Called only once, at startup
function init_lindenmayer() {
    // L-systems will appear in this order
    lsystems.push(lsys_penrose_polygon);
    lsystems.push(lsys_dragon);
    lsystems.push(lsys_twindragon);
    lsystems.push(lsys_sierpinski);
    lsystems.push(lsys_penrose1);
    lsystems.push(lsys_plant);
    lsystems.push(lsys_terdragon);
    lsystems.push(lsys_church);
    lsystems.push(lsys_levy);
    lsystems.push(lsys_snowflake);
    lsystems.push(lsys_flowsnake);
    lsystems.push(lsys_sphinx);
    current_lsystem = 0;
    init_lsystem();
};

// Mostly OpenGL stuff below

var linebuf = [];
var polybuf = [];
var colbuf = [];
var linebuf_sz = 2;
var polybuf_sz = 2;
var colbuf_sz = 4;

function init_buffers() {
    var i;

    linebuf.clear();
    polybuf.clear();
    colbuf.clear();

    for (i = 0; i < coords_arr.length; i++) {
	linebuf[i] = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, linebuf[i]);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(coords_arr[i]), gl.STATIC_DRAW);
	linebuf[i].num_items = coords_arr[i].length / 2;
    }
    for (i = 0; i < poly_arr.length; i++) {
	polybuf[i] = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, polybuf[i]);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(poly_arr[i]), gl.STATIC_DRAW);
	polybuf[i].num_items = poly_arr[i].length / 2;
    }
    for (i = 0; i < colours_arr.length; i++) {
	colbuf[i] = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, colbuf[i]);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colours_arr[i]), gl.STATIC_DRAW);
    }
};

function draw_scene() {
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    mat4.identity(mv_matrix);

    // Move camera
    mat4.translate(mv_matrix, [xpos, ypos, zoom]);

    // Draw line segments (if render is finised)
    if (rendered) {
	var i;

	set_mv_uniform();

	for (i = 0; i < coords_arr.length; i++) {
	    gl.bindBuffer(gl.ARRAY_BUFFER, linebuf[i]);
	    gl.vertexAttribPointer(shader_program.vertex_pos, linebuf_sz, gl.FLOAT, false, 0, 0);
	    gl.bindBuffer(gl.ARRAY_BUFFER, colbuf[i]);
	    gl.vertexAttribPointer(shader_program.vertex_col, colbuf_sz, gl.FLOAT, false, 0, 0);
	    gl.drawArrays(gl.LINE_STRIP, 0, linebuf[i].num_items);
	}
	for (i = 0; i < poly_arr.length; i++) {
	    gl.bindBuffer(gl.ARRAY_BUFFER, polybuf[i]);
	    gl.vertexAttribPointer(shader_program.vertex_pos, polybuf_sz, gl.FLOAT, false, 0, 0);
	    gl.bindBuffer(gl.ARRAY_BUFFER, colbuf[i]);
	    gl.vertexAttribPointer(shader_program.vertex_col, colbuf_sz, gl.FLOAT, false, 0, 0);
	    gl.drawArrays(gl.TRIANGLE_STRIP, 0, polybuf[i].num_items);
	}
    }
};
