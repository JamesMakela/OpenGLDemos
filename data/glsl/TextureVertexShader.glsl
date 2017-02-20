#if __VERSION__ >= 140
    in vec3 position;
    in vec3 vertex_color;
    in vec2 texCoord;
    
    out vec3 color;
    out vec2 TexCoord;
#else
    attribute vec3 position;
    attribute vec3 vertex_color;
    attribute vec2 texCoord;
    
    varying vec3 color;
    varying vec2 TexCoord;
#endif

void main() {
    color = vertex_color;
    gl_Position = vec4(position, 1.0);

    TexCoord = texCoord;
}
