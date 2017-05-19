//============================================================================
// Name        : TextureTriangle.cpp
// Author      : James L. Makela
// Version     : 0.0.1
// Copyright   : LGPL v3.0
// Description : OpenGL triangle in C++ 11 for Legacy OpenGL 2.1.
//               This is another spinoff of the simple triangle demo.
//               What we are now doing is exploring the methods for
//               applying textures to our shaders.
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

// We will initially use GLM for our OpenGL Mathematics.
// But since this is a bit of a learning exercise, I would also
// like to try out the Eigen Math library to see how it can
// be used with OpenGL.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Eigen/Dense>
using Eigen::Matrix4f;
using Eigen::Affine3f;
using Eigen::Translation3f;
using Eigen::AngleAxisf;
using Eigen::Vector3f;
using Eigen::Vector2f;
using Eigen::Scaling;

// Simple OpenGL Image Library
#include <SOIL/SOIL.h>

#include "CmdOptionParser.hpp"
#include "OGLCommon.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "KeyHandler.hpp"
#include "MouseHandler.hpp"

// forward declarations defined after main()
// I like organizing my functions in a top-down fashion
void ConfigureGLFW();
void report_error(int code, const char * description);

void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mode);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window,
                           int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void handle_events(GLfloat deltaTime);

// set the camera as a global
Camera camera;

// set the key handler as a global
KeyHandler keyHandler;
MouseHandler mouseHandler;

// quick & dirty flag to tell the application whether to animate or not
bool animateCube = true;


int main(int argc, const char **argv)
{
    GLenum err;

    CmdOptionParser options(argc, argv);

    std::string vertexFile = "glsl/TransTexVertexShader.glsl";
    std::string fragmentFile = "glsl/TextureFragmentShader.glsl";
    std::string textureFile1 = "img/container.jpg";
    std::string textureFile2 = "img/awesomeface.png";

    const std::string &filePath = options.getCmdOption("-p");
    if (!filePath.empty()) {
        if (filePath.back() != '/') {
            vertexFile.insert(0, "/");
            fragmentFile.insert(0, "/");
            textureFile1.insert(0, "/");
            textureFile2.insert(0, "/");
        }

        vertexFile.insert(0, filePath);
        fragmentFile.insert(0, filePath);
        textureFile1.insert(0, filePath);
        textureFile2.insert(0, filePath);

        cout << "Our vertex shader file: " << vertexFile << endl;
        cout << "Our fragment shader file: " << fragmentFile << endl;
        cout << "Our first texture file: " << textureFile1 << endl;
        cout << "Our second texture file: " << textureFile2 << endl;
    }
    else {
        cout << "Usage: " << argv[0]
             << " -p <path_to_resource_folder>" << endl;
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
                                          "OpenGL Textured Cube",
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

    glEnable(GL_DEPTH_TEST);  // for z-buffer clipping

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    // Here is where we build and compile our shader program
    Shader ourShader(vertexFile.c_str(), fragmentFile.c_str());

    // Setup our textures
    Texture ourTexture1(textureFile1.c_str());
    Texture ourTexture2(textureFile2.c_str());

    // Setup our vertex data
    GLfloat vertices[] = {
                          -0.5f, -0.5f,  0.5f,
                           0.5f, -0.5f,  0.5f,
                           0.5f,  0.5f,  0.5f,
                          -0.5f,  0.5f,  0.5f,

                          -0.5f, -0.5f, -0.5f,
                           0.5f, -0.5f, -0.5f,
                           0.5f,  0.5f, -0.5f,
                          -0.5f,  0.5f, -0.5f
                          };

    // Note: texture coordinates are mapped to vertices, and not indices
    GLfloat texCoords[] = {
                           0.0f, 0.0f,  // Lower-left corner
                           1.0f, 0.0f,  // Lower-right corner
                           1.0f, 1.0f,  // Top-right corner
                           0.0f, 1.0f,  // Top-left corner

                           0.0f, 1.0f,  // Top-left corner
                           1.0f, 1.0f,  // Top-right corner
                           1.0f, 0.0f,  // Lower-right corner
                           0.0f, 0.0f,  // Lower-left corner
                           };

    // Note: color values are mapped to vertices, and not indices
    GLfloat colors[] = {
                        0.f, 0.f, 1.f,  // Lower-left-near corner
                        1.f, 0.f, 1.f,  // Lower-right-near corner
                        1.f, 1.f, 1.f,  // Top-right-near corner
                        0.f, 1.f, 1.f,  // Top-left-near corner

                        0.f, 0.f, 0.f,  // Lower-left-far corner
                        1.f, 0.f, 0.f,  // Lower-right-far corner
                        1.f, 1.f, 0.f,  // Top-right-far corner
                        0.f, 1.f, 0.f,  // Top-left-far corner
                        };

    GLuint indices[] = {
                        0, 1, 2,  // near face
                        2, 3, 0,

                        4, 5, 6,  // far face
                        6, 7, 4,

                        3, 2, 6,  // top face
                        6, 7, 3,

                        0, 1, 5,  // bottom face
                        5, 4, 0,

                        0, 4, 7,  // left face
                        7, 3, 0,

                        1, 5, 6,  // right face
                        6, 2, 1,
                        };

    // Setup our transformations. We are using Eigen here.
    Affine3f rot, scale, modelTrans;

    // Define our model transformation
    rot = AngleAxisf(to_radians(-65.0f), Vector3f::UnitX());
    //scale = Scaling(Vector3f(0.8, 0.8, 0.8));
    modelTrans = rot;

    // Define our view and projection transformation
    camera.lookAt(Vector3f(0.0, 0.0, 3.0),
                  Vector3f(0.0, 0.0, 0.0),
                  Vector3f(0.0, 1.0, 0.0));

    camera.setPerspective(45.0f, (float)width, (float)height, 0.1f, 100.0f);

    cout << "Our Model matrix:\n"<< modelTrans.matrix() << endl;
    cout << "Our View matrix:\n"<< camera.View() << endl;
    cout << "Our Projection matrix:\n"<< camera.Projection() << endl;

    // glm::mat4 glmProjection;
    // glmProjection = glm::perspective(glm::radians(45.0f),
    //                                  (float)width / (float)height,
    //                                  0.1f, 100.0f);
    // cout << "GLM Projection:\n" << glm::to_string(glmProjection) << endl;

    // glm::mat4 glmView;
    // glmView = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
    //                       glm::vec3(0.0f, 0.0f, 0.0f),
    //                       glm::vec3(0.0f, 1.0f, 0.0f));
    // cout << "glm::lookAt view:\n" << glm::to_string(glmView) << endl;

    // Initialize our Vertex Array Object and buffer objects
    GLuint VAO = 0;
    GLuint vertexVBO = 0;
    GLuint vertexEBO = 0;
    GLuint colorVBO = 0;
    GLuint textureVBO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &vertexEBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &textureVBO);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Note that this is allowed, the call to glVertexAttribPointer
    // registered VBO as the currently bound vertex buffer object so
    // afterwards we can safely unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Note: Remember, do NOT unbind the EBO, keep it bound to this VAO

    // Unbind the Vertex Array Object.
    // (It is always good to unbind any buffer/array to prevent strange bugs)
    glBindVertexArray(0);

    // our main loop
    GLfloat prevTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        // get the time elapsed since last iteration
        GLfloat deltaTime = glfwGetTime() - prevTime;
        prevTime += deltaTime;

        // check input events(kbd, mouse, etc.)
        glfwPollEvents();
        handle_events(deltaTime);

        if (animateCube) {
            // rotate the image at about 60 degrees/sec
            modelTrans *= AngleAxisf(to_radians(deltaTime * 60.0f),
                                     Vector3f::UnitZ())
                        * AngleAxisf(to_radians(deltaTime * 30.0f),
                                     Vector3f::UnitY())
                        * AngleAxisf(to_radians(deltaTime * 30.0f),
                                     Vector3f::UnitX());
        }

        //
        // rendering routines
        //
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // grab our graphics pipeline context
        ourShader.Use();
        glBindVertexArray(VAO);

        // set our transformation matrices as uniforms
        Affine3f tempModelTrans;
        tempModelTrans = modelTrans;

        ourShader.UseTransform(tempModelTrans.data(), 0);
        ourShader.UseTransform(camera.View().data(), 1);
        ourShader.UseTransform(camera.Projection().data(), 2);


        // grab our textures
        ourShader.UseTexture(ourTexture1.ID, 0);
        ourShader.UseTexture(ourTexture2.ID, 1);

        // draw our cube
        // Note: we are using vertex indices, so it is impossible
        //       to draw the last two faces of our cube with a reasonable
        //       texture.  So the strategy is to draw the first four faces,
        //       rotate 90 degrees, and then draw the last two faces.
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        tempModelTrans *= AngleAxisf(to_radians(90.0f), Vector3f::UnitY());
        ourShader.UseTransform(tempModelTrans.data(), 0);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

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


void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mode)
{
    keyHandler.callback(key, scancode, action, mode);

    if (keyHandler.is_key(GLFW_KEY_ESCAPE)) {
        // When a user presses the escape key, we set the WindowShouldClose
        // property to true, closing the application
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}


void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouseHandler.position_callback(Vector2f(xpos, ypos));
}


void mouse_button_callback(GLFWwindow* window,
                           int button, int action, int mods)
{
    mouseHandler.button_callback(button, action, mods);
}


void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mouseHandler.scroll_callback(Vector2f(xoffset, yoffset));
}


void handle_events(GLfloat deltaTime)
{
    if (keyHandler.is_key(GLFW_KEY_SPACE)) {
        // toggle the animation
        animateCube ^= true;
        keyHandler.reset_key(GLFW_KEY_SPACE);
    }

    GLfloat deltaMovement = 1.0f * deltaTime;  // length of 1 cube per sec
    GLfloat deltaRotation = 90.0f * deltaTime;  // 90 degrees per sec

    if (keyHandler.is_key(GLFW_KEY_LEFT_SHIFT) ||
            keyHandler.is_key(GLFW_KEY_RIGHT_SHIFT))
    {
        // shift key selects left/right/up/down rotation
        if(keyHandler.is_left())
            camera.rotate(Vector3f(0.0f, deltaRotation, 0.0f));
        if(keyHandler.is_right())
            camera.rotate(Vector3f(0.0f, -deltaRotation, 0.0f));
        if(keyHandler.is_up())
            camera.rotate(Vector3f(deltaRotation, 0.0f, 0.0f));
        if(keyHandler.is_down())
            camera.rotate(Vector3f(-deltaRotation, 0.0f, 0.0f));
    }
    else if (keyHandler.is_key(GLFW_KEY_LEFT_CONTROL) ||
                 keyHandler.is_key(GLFW_KEY_RIGHT_CONTROL))
    {
        // control key selects left/right roll angle
        if(keyHandler.is_left())
            camera.rotate(Vector3f(0.0f, 0.0f, deltaRotation));
        if(keyHandler.is_right())
            camera.rotate(Vector3f(0.0f, 0.0f, -deltaRotation));
    }
    else if (keyHandler.is_key(GLFW_KEY_LEFT_ALT) ||
                 keyHandler.is_key(GLFW_KEY_RIGHT_ALT))
    {
        // basically this moves the camera target left or right
        // on the X axis.  This was mostly early diagnostic stuff,
        // and probably won't be used that much.
        if(keyHandler.is_left())
            camera.moveTarget(Vector3f(-deltaMovement, 0.0f, 0.0f));
        if(keyHandler.is_right())
            camera.moveTarget(Vector3f(deltaMovement, 0.0f, 0.0f));
    }
    else {
        if(keyHandler.is_up())
            camera.moveStraight(-deltaMovement);
        if(keyHandler.is_down())
            camera.moveStraight(deltaMovement);
        if(keyHandler.is_left())
            camera.strafe(-deltaMovement);
        if(keyHandler.is_right())
            camera.strafe(deltaMovement);
    }

    Vector2f deltaMousePosition, deltaScroll;
    deltaMousePosition = mouseHandler.pop_position();
    deltaScroll = mouseHandler.pop_scroll();

    if (mouseHandler.is_button_left()) {
        if (!deltaMousePosition.isZero()) {
            // Adjust the camera lateral movement.
            camera.strafe(deltaMousePosition[0] * deltaMovement * 0.5);
            camera.moveStraight(deltaMousePosition[1] * deltaMovement * 0.5);
        }
    }
    else if (mouseHandler.is_button_right()) {
        if (!deltaMousePosition.isZero()) {
            // Adjust the camera Yaw and Pitch.
            // So the vector we build will be (Y, X, 0.0)
            Vector3f newOrientation;
            newOrientation.topLeftCorner<2,1>() = (deltaMousePosition.reverse() *
                                                   deltaTime * 3.0);
            camera.rotate(newOrientation);
        }
    }

    if (!deltaScroll.isZero()) {
        camera.setFOV(deltaScroll.y());
    }
}
