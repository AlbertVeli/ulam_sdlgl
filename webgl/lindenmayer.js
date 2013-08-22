/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * http://en.wikipedia.org/wiki/L-system
 *
 * Boomtime, the 13th day of Bureaucracy in the YOLD 3179
 *
 * Albert Veli
 */

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
var mode_poly = false;
var debug = false;

var angle;
var turn_angle;
var pipe_angle;
var linelen;
var lendiv;
var turtle_x;
var turtle_y;

var coords_arr = [];
var coords;

var poly_level = 0;
var poly_arr = [];
var poly_col_arr = [];

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

    if (mode_poly) {
	poly_col_arr[poly_level].push(palette[i + 0]);
	poly_col_arr[poly_level].push(palette[i + 1]);
	poly_col_arr[poly_level].push(palette[i + 2]);
	poly_col_arr[poly_level].push(palette[i + 3]);
    } else {
	colours.push(palette[i + 0]);
	colours.push(palette[i + 1]);
	colours.push(palette[i + 2]);
	colours.push(palette[i + 3]);
    }
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
    var poly = [];
    var saved_colour = [];

    if (mode_poly) {
	poly_level = 0;
	if (poly_arr.length > 0) {
	    clear_arr_arr(poly_arr);
	}
	poly_col_arr = [];
	poly_col_arr[poly_level] = new Array();
	poly[poly_level] = new Array();
    } else {
	clear_arr_arr(coords_arr);
	coords = new Array();
    }
    if (colours_arr.length > 0) {
	clear_arr_arr(colours_arr);
    }
    colours = new Array();
    current_colour = 15;

    for(i = 0; i < pattern.length; i++) {

	switch (pattern[i]) {

	case 'F': // Draw line Forward
	    if (first && !mode_poly) {
		// First F in this line strip
		coords.push(turtle_x);
		coords.push(turtle_y);
		push_colour();
		first = false;
	    }
	    turtle_x += linelen * Math.cos(angle);
	    turtle_y += linelen * Math.sin(angle);
	    if (debug) {
		debug_out('F', true);
	    }
	    if (!mode_poly) {
		coords.push(turtle_x);
		coords.push(turtle_y);
		push_colour();
	    }
	    break;

	case 'f': // Move forward without drawing
	    if (!first && !mode_poly) {
		// End line strip
		if (coords.length > 0) {
		    coords_arr.push(coords);
		    coords = new Array();
		}
		if (colours.length > 0) {
		    colours_arr.push(colours);
		    colours = new Array();
		}
		first = true;
	    }
	    turtle_x += linelen * Math.cos(angle);
	    turtle_y += linelen * Math.sin(angle);
	    if (debug) {
		debug_out('f', true);
	    }
	    break;

	case '+':
	    if (debug) {
		debug_out('+, turn left', true);
	    }
	    angle += turn_angle;
	    break;

	case '-':
	    if (debug) {
		debug_out('-, turn right', true);
	    }
	    angle -= turn_angle;
	    break;

	case '|':
	    if (debug) {
		debug_out('turn around', true);
	    }
	    angle += pipe_angle;
	    break;

	case '[':
	    if (debug) {
		debug_out('push', true);
	    }
	    push();
	    break;

	case ']':
	    if (debug) {
		debug_out('pop', true);
	    }
	    if (!first && !mode_poly) {
		if (coords.length > 0) {
		    coords_arr.push(coords);
		    coords = new Array();
		}
		if (colours.length > 0) {
		    colours_arr.push(colours);
		    colours = new Array();
		}
		first = true;
	    }
	    pop();
	    break;

	case '{':
	    saved_colour.push(current_colour);
	    poly_level++;
	    poly[poly_level] = new Array();
	    poly_col_arr[poly_level] = new Array();
	    first = true;
	    if (debug) {
		debug_out('Save C' + current_colour + ', { level: ' + poly_level, true);
	    }
	    break;

	case '}':
	    if (debug) {
		debug_out('} level: ' + poly_level + ', ' + poly[poly_level].length / 2 + ' vertices, ');
	    }
	    if (poly[poly_level].length > 0) {
		poly_arr.push(poly[poly_level]);
		colours_arr.push(poly_col_arr[poly_level]);
		if (poly_level > 0) {
		    poly_level--;
		} else {
		    debug_out('Warning: unmatched polygon end', true);
		    poly[poly_level] = new Array();
		    poly_col_arr[poly_level] = new Array();
		}
	    }
	    current_colour = saved_colour.pop();
	    if (debug) {
		debug_out('restore C' + current_colour, true);
	    }
	    first = true;
	    break;

	case '.': // Record polygon vertex
	    if (debug) {
		debug_out('.', true);
	    }
	    first = false;
	    poly[poly_level].push(turtle_x);
	    poly[poly_level].push(turtle_y);
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
	    if (debug) {
		debug_out('C' + current_colour, true);
	    }
	    break;
	}
    }
    // End last strip
    if (mode_poly) {
	if (debug) {
	    debug_out('Last ' + poly[poly_level].length + ' vertices', true);
	}
	if (poly[poly_level].length > 0) {
	    poly_arr.push(poly[poly_level]);
	    colours_arr.push(poly_col_arr[poly_level]);
	}
    } else {
	if (coords.length > 0) {
	    coords_arr.push(coords);
	    colours_arr.push(colours);
	}
    }
};

function angles_to_radians() {
    angle *= 0.01745329251994;
    turn_angle *= 0.01745329251994;
    pipe_angle = -180 * 0.01745329251994;
}

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
	      ' Mouse      ; Click/drag to rotate' + '\n' +
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
    if (!debug) {
	debug_out('L-system: ' + lsys_name, true);
	debug_out('Level: ' + level, true);
	vertices = 0;
	cols = 0;
	if (mode_poly) {
	    for (i = 0; i < poly_arr.length; i++) {
		vertices += poly_arr[i].length / 2;
		cols += colours_arr[i].length / 4;
	    }
	} else {
	    for (i = 0; i < coords_arr.length; i++) {
		vertices += coords_arr[i].length / 2;
		cols += colours_arr[i].length / 4;
	    }
	}
	debug_out('Number of vertices: ' + vertices + ' (' + cols + ')', true);
	out_keys();
    }
    init_buffers();
    rendered = true;
};

// Called only once, at startup
function init_lindenmayer() {
    // L-systems will appear in this order
    lsystems.push(lsys_sphinx_polygon);
    lsystems.push(lsys_penrose1);
    lsystems.push(lsys_penrose_polygon);
    lsystems.push(lsys_penrose_polygon_2);
    lsystems.push(lsys_dragon);
    lsystems.push(lsys_twindragon);
    lsystems.push(lsys_sierpinski);
    lsystems.push(lsys_sierpinski_polygon);
    lsystems.push(lsys_plant);
    lsystems.push(lsys_lace3060);
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

    // Rotate camera
    mat4.multiply(mv_matrix, rot_mat);

    // Draw line segments (if render is finised)
    if (rendered) {
	var i;

	set_mv_uniform();

	if (mode_poly) {
	    for (i = 0; i < poly_arr.length; i++) {
		gl.bindBuffer(gl.ARRAY_BUFFER, polybuf[i]);
		gl.vertexAttribPointer(shader_program.vertex_pos, polybuf_sz, gl.FLOAT, false, 0, 0);
		gl.bindBuffer(gl.ARRAY_BUFFER, colbuf[i]);
		gl.vertexAttribPointer(shader_program.vertex_col, colbuf_sz, gl.FLOAT, false, 0, 0);
		gl.drawArrays(gl.TRIANGLE_FAN, 0, polybuf[i].num_items);
	    }
	} else {
	    for (i = 0; i < coords_arr.length; i++) {
		gl.bindBuffer(gl.ARRAY_BUFFER, linebuf[i]);
		gl.vertexAttribPointer(shader_program.vertex_pos, linebuf_sz, gl.FLOAT, false, 0, 0);
		gl.bindBuffer(gl.ARRAY_BUFFER, colbuf[i]);
		gl.vertexAttribPointer(shader_program.vertex_col, colbuf_sz, gl.FLOAT, false, 0, 0);
		gl.drawArrays(gl.LINE_STRIP, 0, linebuf[i].num_items);
	    }
	}
    }
};
