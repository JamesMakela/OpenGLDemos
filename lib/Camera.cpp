//============================================================================
// Name        : Camera.cpp
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

#include "Camera.hpp"


void Camera::lookAt(const Vector3f& position,
                    const Vector3f& target,
                    const Vector3f& up)
{
    this->position = position;
    this->target = target;
    this->up = up;

    lookAt();
}


void Camera::lookAt()
{
    mView.setIdentity();

    Matrix3f R;
    R.col(2) = (position - target).normalized();
    R.col(0) = up.cross(R.col(2)).normalized();
    R.col(1) = R.col(2).cross(R.col(0));
    R.transposeInPlace();

    mView.topLeftCorner<3, 3>() = R;
    mView.topRightCorner<3, 1>() = -R * position;
}


void Camera::move(const Vector3f& position, bool keepLookingAtTarget)
{
    if (keepLookingAtTarget == false)
        this->target += position;

    this->position += position;

    lookAt();
}


void Camera::setPerspective(GLfloat fovDegrees,
                            GLfloat width, GLfloat height,
                            GLfloat near, GLfloat far)
{
    this->fovDegrees = fovDegrees;

    this->width = width;
    this->height = height;

    this->near = near;
    this->far = far;

    setPerspective();
}


void Camera::setPerspective()
{
    // Define our projection transformation
    //
    // This seems odd.  I looked at the GLM code for the perspective
    // transformation, and it does nothing to convert the FOV (in degrees)
    // to radians.  Why not???  It seems you would need to use radians
    // on any trig functions you were using in order to get valid results.
    //
    // Update: OK, it seems that as of GLM 0.9.6.0, radians are used by
    //         default.  So this means that most of the GLM examples found
    //         online will probably not reflect this.
    GLfloat fov = to_radians(fovDegrees);
    GLfloat aspect = width / height;

    GLfloat tanHalfFovy = tan(fov / 2.0);
    GLfloat xScale = 1.0 / (aspect * tanHalfFovy);
    GLfloat yScale = 1.0 / tanHalfFovy;

    // using the comma initializer just makes it easier to read.
    mProjection << xScale, 0, 0, 0,
                   0, yScale, 0, 0,
                   0, 0, -(far + near) / (far - near), -1,
                   0, 0, -2 * far * near / (far - near), 0;
    mProjection.transposeInPlace();
}


