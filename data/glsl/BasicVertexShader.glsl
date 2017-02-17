#if __VERSION__ >= 140
    in vec3 position;
    in vec3 vertex_color;
    out vec3 color;
#else
    attribute vec3 position;
    attribute vec3 vertex_color;
    varying vec3 color;
#endif

void main() {
    color = vertex_color;
    gl_Position = vec4(position, 1.0);
}
