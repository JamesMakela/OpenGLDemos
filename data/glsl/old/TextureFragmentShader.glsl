#if __VERSION__ >= 140
    in vec3 color;
    in vec2 TexCoord;

    varying out vec4 out_color;
#else
    attribute vec3 color;
    attribute vec2 TexCoord;
#endif

    uniform sampler2D ourTexture0;
    uniform sampler2D ourTexture1;

void main() {
#if __VERSION__ >= 140
    out_color = mix(texture(ourTexture0, TexCoord),
                    texture(ourTexture1, TexCoord),
                    0.2);
#else
    gl_FragColor = mix(texture(ourTexture0, TexCoord),
                       texture(ourTexture1, TexCoord),
                       0.2);

    // we could further mix a color on top of our textures
    // if we wanted to
    // gl_FragColor *= vec4(color, 0.1f);
#endif
}
