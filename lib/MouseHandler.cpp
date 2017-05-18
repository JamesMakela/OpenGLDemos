//============================================================================
// Name        : MouseHandler.hpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the OpenGL demo code
//               into classes that are easier to use.
//               The GLFW callback for mouse events handles the absolute
//               position of the mouse.  What we would like to do is
//               accumulate the relative position changes.  Then the
//               application can 'pop' the position changes when it is ready.
//
//============================================================================

#include "MouseHandler.hpp"

void MouseHandler::position_callback(const Vector2f& pos)
{
    if (firstTime) {
        firstTime = false;
    }
    else {
        delta += old - pos;
    }

    old = pos;
}


Vector2f MouseHandler::pop_position()
{
    Vector2f ret;

    ret = delta;
    delta.setZero();

    return ret;
}
