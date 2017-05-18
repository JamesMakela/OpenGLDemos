//============================================================================
// Name        : Camera.hpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the OpenGL demo code
//               into classes that are easier to use.
//               Instead of handling primitive view and perspective matrices
//               using ad-hoc math instructions, it seems that a Camera class
//               could contain that functionality.
//               This Camera class uses Eigen vectors and matrices instead of
//               GLM.
//               It persists its own position, the position of what it is
//               looking at, and the 'up' vector.
//
//               More complex implementations may include ???
//               But right now let's keep it simple.
//============================================================================

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <Eigen/Dense>

using Eigen::Matrix4f;
using Eigen::Matrix3f;
using Eigen::Affine3f;

using Eigen::Vector3f;
using Eigen::Vector4f;
using Eigen::AngleAxisf;

#include "OGLCommon.hpp"

class Camera
{
public:
    void lookAt(const Vector3f& position,
                const Vector3f& target,
                const Vector3f& up);
    void lookAt(bool setOrientation = true);

    void SetOrientation(const Matrix3f& rotMx);
    void applyAngles(const Vector3f& YPR);

    void move(const Vector3f& position,
              bool keepLookingAtTarget = false);
    void moveTarget(const Vector3f& position);

    void rotate(const Vector3f& YPR);

    void strafe(const GLfloat distance);
    void moveStraight(const GLfloat distance);

    void setPerspective(GLfloat fovDegrees,
                        GLfloat width, GLfloat height,
                        GLfloat near, GLfloat far);
    void setPerspective();

    Matrix4f View() {return this->mView; }
    Matrix4f Projection() {return this->mProjection; }

private:
    Vector3f position;
    Vector3f target;
    Vector3f up;
    Vector3f PYR;  // Euler angle orientation (pitch, yaw, roll)

    GLfloat fovDegrees;
    GLfloat width; GLfloat height;
    GLfloat near; GLfloat far;

    Matrix4f mView;
    Matrix4f mProjection;
};

#endif /* CAMERA_HPP_ */
