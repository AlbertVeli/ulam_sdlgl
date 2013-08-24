/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * GL stuff, partly from lesson 2/6 at http://learningwebgl.com/
 *
 *
 * Boomtime, the 13th day of Bureaucracy in the YOLD 3179
 *
 * Albert Veli
 */

var gl;
var canvas;

var rot_mat;

function init_gl() {
    try {
	gl = WebGLUtils.setupWebGL(canvas);
	gl.viewportWidth = canvas.width;
	gl.viewportHeight = canvas.height;
    } catch (e) {
    }
    if (!gl) {
	alert('Could not initialise WebGL');
    }
}

function get_shader(gl, id) {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
	return null;
    }

    var str = '';
    var k = shaderScript.firstChild;
    while (k) {
	if (k.nodeType == 3) {
	    str += k.textContent;
	}
	k = k.nextSibling;
    }

    var shader;
    if (shaderScript.type == 'x-shader/x-fragment') {
	shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == 'x-shader/x-vertex') {
	shader = gl.createShader(gl.VERTEX_SHADER);
    } else {
	return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
	alert(gl.get_shaderInfoLog(shader));
	return null;
    }

    return shader;
}

var shader_program;

function init_shaders() {
    var frag_sh = get_shader(gl, 'shader-fs');
    var vert_sh = get_shader(gl, 'shader-vs');

    shader_program = gl.createProgram();
    gl.attachShader(shader_program, vert_sh);
    gl.attachShader(shader_program, frag_sh);
    gl.linkProgram(shader_program);

    if (!gl.getProgramParameter(shader_program, gl.LINK_STATUS)) {
	alert('Could not initialise shaders');
    }

    gl.useProgram(shader_program);

    shader_program.vertex_pos = gl.getAttribLocation(shader_program, 'vx_pos');
    gl.enableVertexAttribArray(shader_program.vertex_pos);

    shader_program.vertex_col = gl.getAttribLocation(shader_program, 'vx_col');
    gl.enableVertexAttribArray(shader_program.vertex_col);

    shader_program.p_matrix = gl.getUniformLocation(shader_program, 'u_pmx');
    shader_program.mv_matrix = gl.getUniformLocation(shader_program, 'u_mvmx');
}

var mv_matrix = mat4.create();
var matrix_stack = [];
var p_matrix = mat4.create();

function push_matrix() {
    var copy = mat4.create();
    mat4.set(mv_matrix, copy);
    matrix_stack.push(copy);
}

function pop_matrix() {
    if (mv_matrix_stack.length == 0) {
	throw 'Invalid popMatrix!';
    }
    mv_matrix = matrix_stack.pop();
}

function set_mv_uniform() {
    gl.uniformMatrix4fv(shader_program.mv_matrix, false, mv_matrix);
}

// Camera key controls
var pressed_keys = {};
var zoom = -7.0;
var xpos = 0.0;
var ypos = 0.0;
var dz = 0.0;
var dx = 0.0;
var dy = 0.0;
var mdx = 0.0;
var mdy = 0.0;

function animate_keys_mouse(elapsed) {
    var maxx = 50.0;
    var xchange = 0.0001 * elapsed;
    if (pressed_keys[37]) {
	// Left
    } else  if (pressed_keys[39]) {
	// Right
	xchange *= -1;
    } else {
	xchange = 0;
    }
    dx += xchange * zoom;
    dx /= 1.1;
    xpos += dx;
    if (xpos < -maxx) {
    	xpos = -maxx;
    	dx = 0;
    } else if (xpos > maxx) {
    	xpos = maxx;
    	dx = 0;
    }

    var maxy = 50.0;
    var ychange = 0.0001 * elapsed;
    if (pressed_keys[70] || pressed_keys[85]) {
	// Down
	ychange *= -1;
    } else  if (pressed_keys[75] || pressed_keys[86]) {
	// Up
    } else {
	ychange = 0;
    }
    dy += ychange * zoom;
    dy /= 1.1;
    ypos += dy;
    if (ypos < -maxy) {
    	ypos = -maxy;
    	dy = 0;
    } else if (ypos > maxy) {
    	ypos = maxy;
    	dy = 0;
    }

    var zchange = 0.0002 * elapsed;
    if (pressed_keys[38]) {
	// Up
	zchange *= -1;
    } else  if (pressed_keys[40]) {
	// Down
    } else {
	zchange = 0;
    }
    dz += zchange * zoom;
    dz /= 1.1;
    zoom += dz;
    if (zoom > -0.01) {
	zoom = -0.01;
	dz = 0;
    } else if (zoom < -99.9) {
	zoom = -99.9;
	dz = 0;
    }

    if (elapsed > 1) {
	mdx /= elapsed * 0.07;
	mdy /= elapsed * 0.07;
	var new_rotmat = mat4.create();
	mat4.identity(new_rotmat);
	mat4.rotate(new_rotmat, mdx * 0.005, [0, 1, 0]);
	mat4.rotate(new_rotmat, mdy * 0.005, [1, 0, 0]);
	mat4.multiply(new_rotmat, rot_mat, rot_mat);
    }
}

var last_time = 0;
function animate() {
    var now = new Date().getTime();
    if (last_time != 0) {
	var elapsed = now - last_time;
	animate_keys_mouse(elapsed);
    }
    last_time = now;
}

function key_down(event) {
    if (!pressed_keys[event.keyCode]) {
	switch(event.keyCode) {
	case 32:
	    // Space, reset camera
	    dz = 0;
	    dx = 0;
	    zoom = -7;
	    xpos = 0;
	    ypos = 0;
	    mat4.identity(rot_mat);
	    mdx = 0;
	    mdy = 0;
	    break;

	case 38:
	    // Up
	    dz = zoom * -0.002;
	    break;

	case 40:
	    // Down
	    dz = zoom * 0.002;
	    break;

	case 37:
	    // Left
	    dx = zoom * -0.002;
	    break;

	case 39:
	    // Right
	    dx = zoom * 0.002;
	    break;

	case 70:
	case 85:
	    // - Move down
	    dy = zoom * -0.002;
	    break;

	case 75:
	case 86:
	    // - Move up
	    dy = zoom * 0.002;
	    break;

	case 107:
	case 67:
	case 74:
	    // + or c
	    if (level < max_level) {
		// recalculate coords
		level++;
		init_lsystem();
	    }
	    break;

	case 109:
	case 81:
	case 88:
	    // - or x
	    if (level > 0) {
		// recalculate coords
		level--;
		init_lsystem();
	    }
	    break;

	case 79:
	case 82:
	    // o
	    if (current_lsystem > 0) {
		current_lsystem--;
		init_lsystem();
	    }
	    break;

	case 80:
	case 76:
	    // p
	    if (current_lsystem < lsystems.length - 1) {
		current_lsystem++;
		init_lsystem();
	    }
	    break;

	case 68:
	    // d
	    clear_debug();
	    debug_out(pattern, true);
	    for (var j = 0; j < coords_arr.length; j++) {
	    debug_out('Line ' + j + ': ');
		for (var i = 0; i < coords_arr[j].length; i++) {
		    if (i % 2 == 0) {
			debug_out(' (' + coords_arr[j][i] + ',');
		    } else {
			debug_out(coords_arr[j][i] + ')');
		    }
		}
		debug_out(' end', true);
	    }
	    for (var j = 0; j < poly_arr.length; j++) {
	    debug_out('Poly ' + j + ': ');
		for (var i = 0; i < poly_arr[j].length; i++) {
		    if (i % 2 == 0) {
			debug_out(' (' + poly_arr[j][i] + ',');
		    } else {
			debug_out(poly_arr[j][i] + ')');
		    }
		}
		debug_out(' end', true);
		if (debug) {
		    debug = false;
		} else {
		    debug = true;
		}
	    }
	    break;

	case 77:
	    // m, toggle multicolour
	    if (multicolour) {
		multicolour = false;
	    } else {
		multicolour = true;
	    }
	    init_lsystem();
	    break;

	}
	pressed_keys[event.keyCode] = true;
    }
};

function key_up(event) {
    pressed_keys[event.keyCode] = false;
};

var last_x = 0;
var last_y = 0;
var mouse_pressed = false;

function mouse_scroll(event) {
    var delta = 0;
    if (!event) {
        event = window.event;
    }
    if (event.wheelDelta) {
        delta = event.wheelDelta/120;
    } else if (event.detail) {
        delta = -event.detail/3;
    }
    if (delta > 0) {
	dz = zoom * -0.03;
    } else {
	dz = zoom * 0.03;
    }
};

function mouse_down(event) {
    mouse_pressed = true;
    last_x = event.clientX;
    last_y = event.clientY;
};

function mouse_up(event) {
    mouse_pressed = false;
};

function mouse_move(event) {
    var new_x = event.clientX;
    var new_y = event.clientY;

    if (!mouse_pressed) {
	last_x = new_x;
	last_y = new_y;
	return;
    }

    mdx = new_x - last_x;
    mdy = new_y - last_y;

    last_x = new_x;
    last_y = new_y;
};

function render() {
    requestAnimFrame(render, canvas);
    draw_scene();
    animate();
}

function start_webgl() {
    canvas = document.getElementById('main_canvas');
    init_gl();
    init_shaders();
    init_lindenmayer();

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST);

    mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.01, 100.0, p_matrix);
    // p_matrix is never changed, only need to set it once
    gl.uniformMatrix4fv(shader_program.p_matrix, false, p_matrix);

    rot_mat = mat4.create();
    mat4.identity(rot_mat);

    // kbd callbacks
    document.onkeydown = key_down;
    document.onkeyup = key_up;

    // mouse callbacks
    if (window.addEventListener) {
        window.addEventListener('DOMMouseScroll', mouse_scroll, false);
    } else {
	window.onmousewheel = document.onmousewheel = mouse_scroll;
    }
    canvas.onmousedown = mouse_down;
    document.onmouseup = mouse_up;
    document.onmousemove = mouse_move;

    render();
}
