//============================================================================
// Name        : Texture.cpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the code into
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

// Simple OpenGL Image Library
#include <iostream>
#include <map>

// this is just to make printing stuff a bit more concise
// everything else in std, we can call explicitly
using std::cout;
using std::cin;
using std::endl;

#include "Texture.hpp"


Texture::Texture(const char *imagePath)
{
    // Load and generate the texture
    GLenum err = GL_NO_ERROR;
    int width = 0;
    int height = 0;

    unsigned char *image = ReadFile(imagePath, width, height);
    if (image == nullptr)
        return;

    GLuint texture = GenTexture();
    if (texture == 0) {
        SOIL_free_image_data(image);

        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    if (SetPixelStorageModes() != GL_NO_ERROR) {
        // cleanup our local texture objects
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return;
    }


    if (SetTextureWrappingModes() != GL_NO_ERROR) {
        // cleanup our local texture objects
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return;
    }

    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if ((err = glGetError()) != GL_NO_ERROR) {
        cout << "glTexImage2D(): error: " << err << endl;

        // cleanup our local texture objects
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    if ((err = glGetError()) != GL_NO_ERROR) {
        cout << "glGenerateMipmap(): error: " << err << endl;

        // cleanup our local texture objects
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return;
    }

    this->TextureID = texture;

    // cleanup our local texture objects
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}


unsigned char *Texture::ReadFile(const char *path,
                                 int &width, int &height)
{
    unsigned char *image = SOIL_load_image(path,
                                           &width, &height,
                                           0, SOIL_LOAD_RGB);
    if (image == nullptr) {
        cout << "No loaded image!!" << endl
             << "libSOIL result: " << SOIL_last_result() << endl;
    }

    return image;
}


GLuint Texture::GenTexture()
{
    GLenum err;
    GLuint texture = 0;

    glGenTextures(1, &texture);
    if ((err = glGetError()) != GL_NO_ERROR) {
        cout << "glGenTextures(): error: " << err << endl;
        return 0;
    }

    if (texture == 0)
        cout << "Failed to generate texture!!" << endl;

    return texture;
}


GLenum Texture::SetPixelStorageModes()
{
    GLenum err;

    // Some images have weird sizes and attributes, and I have
    // had some problems with them.  So let's try to set the
    // pixel storage modes to something kind of general
    std::map<GLenum, GLint> pstorei_params = {
            {GL_UNPACK_ROW_LENGTH, 0},  // tightly packed
            {GL_UNPACK_SKIP_PIXELS, 0},  // tightly packed
            {GL_UNPACK_SKIP_ROWS, 0},  // tightly packed
            {GL_UNPACK_ALIGNMENT, 1},  // tightly aligned
    };

    for (auto param : pstorei_params) {
        GLenum pname = param.first;
        GLint value = param.second;

        // cout << "glPixelStorei("
        //      << param.first << ", "
        //      << param.second << ");" << endl;

        glPixelStorei(pname, value);
        if ((err = glGetError()) != GL_NO_ERROR) {
            cout << "glPixelStorei(): error: " << err << endl;

            return err;
        }
    }

    return GL_NO_ERROR;
}


GLenum Texture::SetTextureWrappingModes()
{
    GLenum err;

    // Set the texture wrapping/filtering options (on the currently bound
    // texture object)
    // Note: We will want to make these configurable in the future
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Required if we are clamping to border
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

    // we might want to check for errors after each parameter setting
    if ((err = glGetError()) != GL_NO_ERROR)
        cout << "glTexParameter(): error: " << err << endl;

    return err;
}


void Texture::Use(GLuint texture_unit_idx = 0, GLuint program = 0)
{
    std::string uniformName = "ourTexture" + std::to_string(texture_unit_idx);

    // Activate the texture unit first before binding texture
    glActiveTexture(GL_TEXTURE0 + texture_unit_idx);
    glBindTexture(GL_TEXTURE_2D, this->TextureID);


    glUniform1i(glGetUniformLocation(program, uniformName.c_str()),
                texture_unit_idx);
}

