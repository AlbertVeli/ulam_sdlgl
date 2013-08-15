function debug_out(s) {
    var x=document.getElementById("debug_area");
    x.innerHTML += s + '<br />';
};

function clear_debug() {
    var x=document.getElementById("debug_area");
    x.innerHTML = '';
};

// L-systems

var current_lsystem;
var axiom;
var rules = {};
var pattern = '';
var level = 6;
var max_level = 9;

var angle;
var turn_angle;
var linelen;
var lendiv;
var turtle_x;
var turtle_y;

var coords = [];
var colours = [];
var current_colour = 15;

var palette = [
    // Bright 0
    0.0,  0.0,  0.0,  0.86, // 0 Black
    0.0,  0.0,  0.86, 0.86, // 1 Blue
    0.86, 0.0,  0.0,  0.86, // 2 Red
    0.86, 0.0,  0.86, 0.86, // 3 Magenta
    0.0,  0.86, 0.0,  0.86, // 4 Green
    0.0,  0.86, 0.86, 0.86, // 5 Cyan
    0.86, 0.86, 0.0,  0.86, // 6 Yellow
    0.86, 0.86, 0.86, 0.86, // 7 White
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
}

// clear() method for Array class
Array.prototype.clear = function() {
  while (this.length > 0) {
    this.shift();
  }
};

function render_arrays() {
    coords.clear();
    var first = true;

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
		// TODO: End this line strip
	    }
	    first = true;
	    turtle_x += linelen * Math.cos(angle);
	    turtle_y += linelen * Math.sin(angle);

	case '+':
	    angle += turn_angle;
	    break;

	case '-':
	    angle -= turn_angle;
	    break;
	}
    }
};

function angles_to_radians() {
    angle *= 0.01745329251994;
    turn_angle *= 0.01745329251994;
}

function set_levy() {
    current_lsystem = '<a href="http://en.wikipedia.org/wiki/L%C3%A9vy_C_curve" target="_blank">Levy C curve</a>';
    axiom = 'F';
    rules["F"] = '+F--F+';
    rules["+"] = '+';
    rules["-"] = '-';
    pattern = axiom;
    turn_angle = 45;
    linelen = 1.0;
    lendiv = Math.sqrt(2);
    current_colour = 15;
    max_level = 17;
    // Initial turtle position and angle
    turtle_x = -0.5;
    turtle_y = -0.5;
    angle = 0;
    angles_to_radians();
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
	new_pattern += rules[pattern[i]];
    }
    pattern = new_pattern;
    linelen /= lendiv;
};

function out_keys() {
    debug_out('<br />' + 'KEYS' + '<br />');
    debug_out(' minus, x - decrease level');
    debug_out(' plus, c - increase level' + '<br />');
    debug_out('CAMERA' + '<br />');
    debug_out(' Up - Zoom in');
    debug_out(' Down - Zoom out');
    debug_out(' Left - Move left');
    debug_out(' Right - Move right');
    debug_out(' f - Move up');
    debug_out(' v - Move down');
};

var rendered;

function init_lindenmayer() {
    clear_debug();
    debug_out('Calculating coordinates for ' + current_lsystem);
    // Wait 10ms so message will be output before crunching begins
    rendered = false;
    setTimeout(init_lindenmayer2, 10);
};

// Continue here after small delay
function init_lindenmayer2() {
    var i;

    set_levy();

    for (i = 0; i < level; i++) {
	expand_lsys();
    }
    render_arrays();
    clear_debug();
    debug_out('L-system: ' + current_lsystem);
    debug_out('Level: ' + level);
    debug_out('Number of vertices: ' + coords.length / 2);
    out_keys();
    init_buffers();
    rendered = true;
};

// Mostly OpenGL stuff below

var linebuf;
var colbuf;

function init_buffers() {
    linebuf = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, linebuf);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(coords), gl.STATIC_DRAW);
    linebuf.sz = 2;
    linebuf.num_items = coords.length / 2;
    colbuf = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colbuf);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colours), gl.STATIC_DRAW);
    colbuf.sz = 4;
}

function draw_scene() {
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    mat4.identity(mv_matrix);

    // Move camera
    mat4.translate(mv_matrix, [xpos, ypos, zoom]);

    // Draw line segments (if render is finised)
    if (rendered) {
	gl.bindBuffer(gl.ARRAY_BUFFER, linebuf);
	gl.vertexAttribPointer(shader_program.vertex_pos, linebuf.sz, gl.FLOAT, false, 0, 0);
	gl.bindBuffer(gl.ARRAY_BUFFER, colbuf);
	gl.vertexAttribPointer(shader_program.vertex_col, colbuf.sz, gl.FLOAT, false, 0, 0);
	set_mat_uniforms();
	gl.drawArrays(gl.LINE_STRIP, 0, linebuf.num_items);
    }
}
