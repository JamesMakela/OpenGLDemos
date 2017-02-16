//============================================================================
// Name        : BetterTriangle.cpp
// Author      : James L. Makela
// Version     :
// Copyright   : LGPL v3.0
// Description : OpenGL triangle in C++ 11 for Legacy OpenGL 2.1.
//               This is another spinoff of the simple triangle demo.
//               What we are trying to do is refactor the code into
//               classes that are easier to use, starting with a
//               Shader class.
//============================================================================

#include <iostream>

// this is just to make printing stuff a bit more concise
// everything else in std, we can call explicitly
using std::cout;
using std::cin;
using std::endl;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "CmdOptionParser.hpp"

// forward declarations defined after main()
// I like organizing my functions in a top-down fashion
void ConfigureGLFW();
void report_error(int code, const char * description);
void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mode);

int main(int argc, const char **argv)
{
    CmdOptionParser options(argc, argv);

    std::string vertexFile = "BasicVertexShader.glsl";
    std::string fragmentFile = "BasicFragmentShader.glsl";

    const std::string &filePath = options.getCmdOption("-p");
    if (!filePath.empty()) {
        vertexFile.insert(0, "/");
        vertexFile.insert(0, filePath);

        fragmentFile.insert(0, "/");
        fragmentFile.insert(0, filePath);

        cout << "Our vertex shader file: " << vertexFile << endl;
        cout << "Our fragment shader file: " << fragmentFile << endl;
    }
    else {
        cout << "Usage: " << argv[0]
             << " -p <path_to_shader_files>" << endl;
        exit(1);
    }

    if (!glfwInit()) {
        // Initialization failed
        cout << "GLFW Initialization Failed!!" << endl;
        return -1;
    }

    glfwSetErrorCallback(&report_error);

    ConfigureGLFW();

    GLFWwindow* window = glfwCreateWindow(800, 600,
                                          "OpenGL Color Triangle",
                                          nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Note: glewInit() will fail if it is attempted before the current
    //       window context is made via glfwMakeContextCurrent()
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    cout << "OpenGL version supported by this platform: "
         << glGetString(GL_VERSION) << endl;
    cout << "GLSL version supported by this platform: "
         << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    cout << "Set the Viewport to size ("
         << width << ", " << height << ")"<< endl;
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);

    // Here is where we build and compile our shader program
    Shader ourShader(vertexFile.c_str(), fragmentFile.c_str());

    // Setup our vertex data
    GLfloat vertices[] = {-0.5f, -0.5f, 0.0f,
                          0.5f, -0.5f, 0.0f,
                          0.0f,  0.5f, 0.0f};

    GLfloat colors[] = {1.f, 0.f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f, 1.f};

    GLuint VAO;
    GLuint vertexVBO, colorVBO = 0;

    // Initialize our Vertex Array Object and buffer objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &colorVBO);

    // bind our Vertex Array Object first
    glBindVertexArray(VAO);

    // Initialize the vertex buffer and the color buffer objects so we can
    // Then bind and set our buffers
    // Note: the order in which things are done here is important. The order
    //       of operations that works for me is:
    //       - bind the buffer object
    //       - copy the data into the buffer
    //       - Set the attribute pointer
    //       - move on to the next buffer object
    //       I think the determining factor is that we can only operate
    //       on a single bound buffer at a time.
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Note that this is allowed, the call to glVertexAttribPointer
    // registered VBO as the currently bound vertex buffer object so
    // afterwards we can safely unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Note: Remember, do NOT unbind the EBO, keep it bound to this VAO

    // Unbind the Vertex Array Object.
    // (It is always good to unbind any buffer/array to prevent strange bugs)
    glBindVertexArray(0);

    // our main loop
    while(!glfwWindowShouldClose(window))
    {
        // check input events(kbd, mouse, etc.)
        glfwPollEvents();

        //
        // rendering routines
        //
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // grab our graphics pipeline context
        ourShader.Use();
        glBindVertexArray(VAO);

        // draw our colored triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // cleanup
        glBindVertexArray(0);

        //
        // done rendering
        //

        glfwSwapBuffers(window);
    }

    // Properly deallocate all resources once we are done.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &colorVBO);

    glfwTerminate();
    cout << "Terminated GLFW..." << endl;
    return 0;
}

void ConfigureGLFW() {

    // Note: I don't know if it is ever a good idea requiring a version
    //       It would be nice if we could query the version and provide
    //       backward compatibility, at least down to OpenGL version 2.0
    //       (GLSL version 1.x)
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Note: Context profiles are only defined for OpenGL version 3.2
    //       and above.  My old PC uses old NVidia linux drivers, so it is not
    //       possible to set this.
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}


void report_error(int code, const char * description)
{
    cout << "GLFW Error: "
         << "(" << code << ")" << endl
         << '\t' << description
         << endl;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
