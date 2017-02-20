#if __VERSION__ >= 140
    in vec3 color;
    in vec2 TexCoord;

    varying out vec4 out_color;
#else
    attribute vec3 color;
    attribute vec2 TexCoord;
#endif

    uniform sampler2D ourTexture;

void main() {
#if __VERSION__ >= 140
    out_color = texture(ourTexture, TexCoord);
#else
    // gl_FragColor = texture(ourTexture, TexCoord);
    gl_FragColor = texture(ourTexture, TexCoord) * vec4(color, 1.0f);  
#endif
}
