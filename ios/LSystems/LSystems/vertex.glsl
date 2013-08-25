// -*- mode: c; -*-
attribute vec2 Position;
attribute vec4 SourceColor;

varying vec4 DestinationColor;

uniform mat4 Projection;
uniform mat4 Modelview;

void main(void) {
  DestinationColor = SourceColor;
  gl_Position = Projection * Modelview * vec4(Position, 0.0, 1.0);
}
