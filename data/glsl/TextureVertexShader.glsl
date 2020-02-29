#version 300 es

#ifdef GL_ES
    precision mediump float;
#endif

in vec3 position;
in vec3 vertex_color;
in vec2 texCoord;

out vec3 color;
out vec2 TexCoord;

void main() {
    color = vertex_color;
    gl_Position = vec4(position, 1.0);

    // We are using SOIL_load_image, which creates a texture
    // with origin in the top-left instead of bottom-left.
    // So we have to flip it.
    TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
