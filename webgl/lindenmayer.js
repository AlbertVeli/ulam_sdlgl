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
var linelen;
var lendiv;
var turtle_x;
var turtle_y;

var coords = [];
var colours = [];
var current_colour = 15;
var multicolour = true;

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
}

// clear() method for Array class
Array.prototype.clear = function() {
  while (this.length > 0) {
    this.pop();
  }
};

function render_arrays() {
    coords.clear();
    colours.clear();
    var first = true;
    var c;

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
};

function angles_to_radians() {
    angle *= 0.01745329251994;
    turn_angle *= 0.01745329251994;
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
    current_colour = 15;
    max_level = 19;
    // Initial turtle position and angle
    turtle_x = -1.5;
    turtle_y = -1.0;
    angle = 0;
    angles_to_radians();
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
    angle_offset = -30;
    lendiv = Math.sqrt(3);
    current_colour = 15;
    max_level = 12;
    turtle_x = -1.5;
    turtle_y = 0.5;
    angles_to_radians();
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
    angle_offset = 0;
    linelen = 3.0;
    lendiv = 3;
    current_colour = 15;
    max_level = 9;
    turtle_x = -1.5;
    turtle_y = 0.85;
    angle = 0;
    angles_to_radians();
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
    angle_offset = 0;
    lendiv = 2.65;
    current_colour = 15;
    max_level = 7;
    turtle_x = -1.5;
    turtle_y = 0.85;
    angle += 19.3 * level;
    angles_to_radians();
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
    angle_offset = 0;
    lendiv = 2.0;
    current_colour = 15;
    max_level = 12;
    turtle_x = -1.5;
    turtle_y = -1.33;
    angle = 0;
    angles_to_radians();
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
    angle_offset = 0;
    lendiv = 1.732;
    current_colour = 15;
    max_level = 11;
    turtle_x = -1.732;
    turtle_y = -1.0;
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
	    if (i >= pattern.length) {
		break; // finished
	    }
	}
	// Normal rule
	new_pattern += rules[c];
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
    var i;

    for (i = 0; i < level; i++) {
	expand_lsys();
    }
    render_arrays();
    clear_debug();
    debug_out('L-system: ' + lsys_name, true);
    debug_out('Level: ' + level, true);
    debug_out('Number of vertices: ' + coords.length / 2, true);
    out_keys();
    init_buffers();
    rendered = true;
};

// Called only once, at startup
function init_lindenmayer() {
    // L-systems will appear in this order
    lsystems.push(lsys_church);
    lsystems.push(lsys_sierpinski);
    lsystems.push(lsys_terdragon);
    lsystems.push(lsys_levy);
    lsystems.push(lsys_snowflake);
    lsystems.push(lsys_flowsnake);
    current_lsystem = 0;
    init_lsystem();
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
