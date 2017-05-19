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


void Camera::lookAt(bool setOrientation)
{
    Vector3f D = (position - target).normalized();
    Vector3f U = up.normalized();
    Vector3f R = U.cross(D).normalized();
    U = D.cross(R);

    Matrix3f rotMx;
    rotMx.col(0) = R;
    rotMx.col(1) = U;
    rotMx.col(2) = D;
    rotMx.transposeInPlace();

    mView.setIdentity();
    mView.topLeftCorner<3,3>() = rotMx;
    mView.topRightCorner<3,1>() = -rotMx * position;

    if (setOrientation)
        SetOrientation(rotMx);
}


// Based on the online document:
//   "Computing Euler Angles from a Rotation Matrix"
//   by Gregory G. Slabough
// The original algorithm computes two possible sequences of rotations
// depending on the angle inputs.  But they both result in the same
// orientation of the object.
void Camera::SetOrientation(const Matrix3f& rotMx)
{
    GLfloat vecLength;
    GLfloat psi, psi1, psi2;
    GLfloat theta, theta1, theta2;
    GLfloat phi, phi1, phi2;

    if (rotMx(2, 0) == -1.0) {
        phi = 0.0;  // could be anything
        theta = pi() / 2.0;
        psi = phi + atan2(rotMx(0, 1), rotMx(0, 2));
    }
    else if (rotMx(2, 0) == 1.0) {
        phi = 0.0;  // could be anything
        theta = -pi() / 2.0;
        psi = -phi + atan2(-rotMx(0, 1), -rotMx(0, 2));
    }
    else {
        theta1 = -asin(rotMx(2, 0));
        theta2 = pi() - theta1;

        psi1 = atan2(rotMx(2, 1) / cos(theta1),
                     rotMx(2, 2) / cos(theta1));
        psi2 = atan2(rotMx(2, 1) / cos(theta2),
                     rotMx(2, 2) / cos(theta2));

        phi1 = atan2(rotMx(1, 0) / cos(theta1),
                     rotMx(0, 0) / cos(theta1));
        phi2 = atan2(rotMx(1, 0) / cos(theta2),
                     rotMx(0, 0) / cos(theta2));

        // we have two possible choices for an angle set
        // and we probably want to choose the one with the smallest
        // angular path.
        vecLength = sqrt((psi1 * psi1) + (theta1 * theta1) + (phi1 * phi1));
        vecLength -= sqrt((psi2 * psi2) + (theta2 * theta2) + (phi2 * phi2));

        if (vecLength <= 0.0) {
            // PYR1 is the smallest
            psi = psi1;
            theta = theta1;
            phi = phi1;
        }
        else {
            // PYR2 is the smallest
            psi = psi2;
            theta = theta2;
            phi = phi2;
        }

    }

    PYR[0] = psi;
    PYR[1] = theta;
    PYR[2] = phi;
}


// Primitive function to cumulatively apply angles to our class.
// - deltaPYR contains angles in degrees.
// - Checks the range of values and limits them to one revolution.
void Camera::applyAngles(const Vector3f& deltaPYR)
{
    PYR += to_radians(deltaPYR);

    PYR[0] -= floor(PYR[0] / (pi() * 2.0)) * pi() * 2.0;
    PYR[1] -= floor(PYR[1] / (pi() * 2.0)) * pi() * 2.0;
    PYR[2] -= floor(PYR[2] / (pi() * 2.0)) * pi() * 2.0;
}


// move the camera to a new position.
void Camera::move(const Vector3f& newPosition, bool keepLookingAtTarget)
{
    position += newPosition;

    if (keepLookingAtTarget)
        lookAt();
    else {
        // Target tracks with position.
        // Rotation and orientation don't need to be updated.
        target += newPosition;
        lookAt(false);
    }
}


// move the camera target.
// - rotation and orientation need to be updated
void Camera::moveTarget(const Vector3f& newTarget)
{
    target += newTarget;
    lookAt();
}


// Rotate the camera.
// - Position stays the same.
// - deltaPYR contains Euler angles in degrees.
void Camera::rotate(const Vector3f& deltaPYR)
{
    applyAngles(deltaPYR);

    GLfloat pitch = PYR[0];
    GLfloat yaw = PYR[1];
    GLfloat roll = PYR[2];

    Matrix3f rotMx;

    // first get the current positional angle matrix
    Vector3f D = (target - position).normalized();
    Vector3f U = up.normalized();
    Vector3f R = U.cross(D).normalized();
    U = D.cross(R);

    rotMx.col(0) = R;
    rotMx.col(1) = U;
    rotMx.col(2) = D;
    rotMx.transposeInPlace();

    target = rotMx * (target - position);

    // next get the PYR angle matrix
    rotMx = AngleAxisf(pitch, Vector3f::UnitX())
          * AngleAxisf(yaw,  Vector3f::UnitY())
          * AngleAxisf(roll, Vector3f::UnitZ());
    target = position - rotMx.inverse() * target;

    lookAt(false);
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


// Change the field of view
// - assumes the perspective has been initialized
void Camera::setFOV(GLfloat fovDegrees)
{
    this->fovDegrees -= fovDegrees;
    this->fovDegrees = std::max(std::min(this->fovDegrees, 45.0f), 1.0f);

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


