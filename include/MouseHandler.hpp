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

#ifndef MOUSEHANDLER_HPP_
#define MOUSEHANDLER_HPP_

#include <GLFW/glfw3.h>

#include <Eigen/Dense>
using Eigen::Vector2f;


class MouseHandler
{
public:
    void position_callback(const Vector2f& pos);

    Vector2f pop_position();

private:
    bool firstTime = true;

    Vector2f old;
    Vector2f delta;
};


#endif /* MOUSEHANDLER_HPP_ */
