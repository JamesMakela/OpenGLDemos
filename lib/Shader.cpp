//============================================================================
// Name        : Shader.cpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the code into
//               classes that are easier to use, starting with a
//               Shader class.  The Shader class tries to take the
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

#include "Shader.hpp"


Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    std::string vShaderCode = this->ReadSourceFile(vertexPath);
    if (vShaderCode.length() == 0)
        return;

    std::string fShaderCode = this->ReadSourceFile(fragmentPath);
    if (fShaderCode.length() == 0)
        return;

    // 2. Compile shaders
    GLuint vertex = this->CreateVertexShader(vShaderCode.c_str());
    if (vertex == 0)
        return;

    GLuint fragment = this->CreateFragmentShader(fShaderCode.c_str());
    if (fragment == 0) {
        glDeleteShader(vertex);
        return;
    }

    this->CreateShaderProgram();

    // Delete the shaders as they're linked into our program now
    // and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


std::string Shader::ReadSourceFile(const GLchar *path)
{
    std::string codeBuffer = "";

    std::ifstream sourceFile(path);
    if (!sourceFile) {
        // The file does not exist. Stop.
        std::cout << "ERROR::SHADER::FILE_DOES_NOT_EXIST" << std::endl
                  << "\tFile Path: " << path << std::endl;
        return codeBuffer;
    }

    sourceFile.exceptions(std::ifstream::eofbit |
                          std::ifstream::failbit |
                          std::ifstream::badbit);

    try {
        // Read file's buffer contents into stream
        std::stringstream fileStream;
        fileStream << sourceFile.rdbuf();

        sourceFile.close();

        // Convert stream into a string
        codeBuffer = fileStream.str();
    }
    catch(const std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
                  << std::endl
                  << "\tFile Path: " << path << std::endl
                  << "\tError: " << e.what() << std::endl;
        return codeBuffer;
    }

    // std::cout << "our code buffer:" << std::endl
    //           << codeBuffer << std::endl;
    return codeBuffer;
}


GLuint Shader::CreateVertexShader(const GLchar *code) {
    GLchar infoLog[512];
    GLint success;

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex, 1, &code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n\t"
                  << infoLog << std::endl;
        return 0;
    }
    else {
        this->vertexShader = vertex;
        return vertex;
    }
}


GLuint Shader::CreateFragmentShader(const GLchar *code) {
    GLchar infoLog[512];
    GLint success;

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment, 1, &code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n\t"
                  << infoLog << std::endl;
        return 0;
    }
    else {
        this->fragmentShader = fragment;
        return fragment;
    }
}


void Shader::CreateShaderProgram() {
    // Shader Program
    GLchar infoLog[512];
    GLint success;

    this->Program = glCreateProgram();


    glAttachShader(this->Program, this->vertexShader);
    glAttachShader(this->Program, this->fragmentShader);

    glLinkProgram(this->Program);

    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n\t"
                  << infoLog << std::endl;
        this->Program = 0;
    }
}
