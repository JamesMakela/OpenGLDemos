//============================================================================
// Name        : Shader.hpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the OpenGL demo
//               code into classes that are easier to use, starting with
//               a Shader class.  The Shader class tries to take the
//               GLSL source files for the vertex and the fragment
//               shader and then compile and link them into a shader
//               program that encapsulates everything.
//
//               Right now this is a fairly simple case.  We have a
//               vertex buffer configuration and a vertex color buffer
//               configuration.
//
//               More complex configurations will likely require
//               sub-classing this Shader, as it will be difficult
//               to anticipate what a new Shader design may need.
//============================================================================

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Include glew to get all the required OpenGL headers

class Shader
{
public:
    // The program ID
    GLuint Program = 0;

    // Constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    std::string ReadSourceFile(const GLchar *path);
    GLuint CreateVertexShader(const GLchar *code);
    GLuint CreateFragmentShader(const GLchar *code);
    void CreateShaderProgram();

    // Use the program
    void Use() { glUseProgram(this->Program); }

private:
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
};

#endif /* SHADER_HPP_ */
