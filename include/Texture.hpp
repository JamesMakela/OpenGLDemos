//============================================================================
// Name        : Texture.hpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the OpenGL demo
//               code into classes that are easier to use.
//               In addition to the Shaders, it seems that a Texture
//               class would simplify things a bit.
//               This Texture class tries to take the OpenGL texture
//               generation and binding methods and package them into
//               something that is more easy to use.
//
//               Right now this is a fairly simple case.  The constructor
//               performs the initial generation of the texture, and the
//               Texture::Use() function will bind the texture when it is
//               needed.
//
//               More complex implementations may include the handling of
//               multiple textures at once, rotations, flipping, etc.
//               But right now let's keep it simple.
//============================================================================

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Include glew to get all the required OpenGL headers

#include <SOIL/SOIL.h>

class Texture
{
public:
    GLuint TextureID = 0;

    // Constructor reads and builds the texture
    Texture(const char *imagePath);

    unsigned char *ReadFile(const char *path,
                            int &width, int &height);
    GLuint GenTexture();
    GLenum SetPixelStorageModes();
    GLenum SetTextureWrappingModes();

    // Use the texture
    void Use(GLuint texture_unit_idx, GLuint program);

private:
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
};

#endif /* TEXTURE_HPP_ */
