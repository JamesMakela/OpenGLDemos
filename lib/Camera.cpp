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
   Vector3f D = (position - target).normalized();
   Vector3f U = up.normalized();
   Vector3f R = U.cross(D).normalized();
   U = D.cross(R);

   mView << R.x(), R.y(), R.z(), -R.dot(position),
            U.x(), U.y(), U.z(), -U.dot(position),
            D.x(), D.y(), D.z(), -D.dot(position),
            0, 0, 0, 1;
}


// move the camera to a new position.
void Camera::move(const Vector3f& newPosition, bool keepLookingAtTarget)
{
    if (keepLookingAtTarget == false)
        target += newPosition;

    position += newPosition;
    lookAt();
}


// move the camera target.  Position stays the same
void Camera::moveTarget(const Vector3f& newTarget)
{
    target += newTarget;
    lookAt();
}


// rotate the camera.  Position stays the same
void Camera::rotate(const Vector3f& YPR)
{
    GLfloat yaw = to_radians(YPR[0]);
    GLfloat pitch = to_radians(YPR[1]);
    GLfloat roll = to_radians(YPR[2]);

    Vector4f targetDiff, newTarget;
    newTarget.topLeftCorner<3, 1>() = position - target;
    newTarget[3] = 1.0f;

    Affine3f xform;
    xform.setIdentity();

    targetDiff = xform.matrix() * newTarget;

    xform *= AngleAxisf(roll, Vector3f::UnitZ())
           * AngleAxisf(yaw, Vector3f::UnitY())
           * AngleAxisf(pitch, Vector3f::UnitX());

    targetDiff -= xform.matrix() * newTarget;

    target += targetDiff.topLeftCorner<3, 1>();
    lookAt();
}


// move the camera sideways.  distance can be positive or negative
void Camera::strafe(const GLfloat distance)
{
    Vector3f D = (position - target).normalized();
    Vector3f U = up.normalized();
    Vector3f R = U.cross(D).normalized();

    move(R * distance);
}


// move the camera straight towards target.
// distance can be positive or negative
void Camera::moveStraight(const GLfloat distance)
{
    Vector3f D = (position - target).normalized();

    move(D * distance);
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


