#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec3 position;
in vec3 vertex_color;
out vec3 color;

void main() {
    color = vertex_color;
    gl_Position = vec4(position, 1.0);
}
