#version 300 es

#ifdef GL_ES
    precision mediump float;
#endif

in vec3 color;
in vec2 TexCoord;
out vec4 out_color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main() {
    out_color = mix(texture(ourTexture0, TexCoord),
                    texture(ourTexture1, TexCoord),
                    0.2);
}
