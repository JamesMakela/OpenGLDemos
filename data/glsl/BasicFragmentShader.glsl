#version 300 es

#ifdef GL_ES
precision mediump float;
#endif

in vec3 color;
out vec4 out_color;

void main() {
    out_color = vec4(color, 1.0f);
}
