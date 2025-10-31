#shader vertex
#version 300 es

precision highp float;
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 vColor;

out vec4 fColor;

uniform vec4 u_camera;

void main() {
  vec4 newPosition = vec4(position, 0, 1.0);
  newPosition.xy -= u_camera.xy;
  newPosition.xy /= u_camera.zw / vec2(2.0, 2.0);
  newPosition.xy -= vec2(1.0, 1.0);
  gl_Position = newPosition;
  fColor = vec4(vColor, 1.0);
}


#shader fragment
#version 300 es

precision mediump float;
layout(location = 0) out vec4 color;

in vec4 fColor;

void main() {
  color = fColor;
}
