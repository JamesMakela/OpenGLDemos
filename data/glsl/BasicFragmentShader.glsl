#if __VERSION__ >= 140
    in vec3 color;
    varying out vec4 out_color;
#else
    attribute vec3 color;
#endif

void main() {
#if __VERSION__ >= 140
    out_color = vec4(color, 1.0f);
#else
    gl_FragColor = vec4(color, 1.0);
#endif
}
