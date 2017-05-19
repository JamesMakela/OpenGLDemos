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


// Handle the mouse movement
void MouseHandler::position_callback(const Vector2f& pos)
{
    if (firstTimePosition)
        firstTimePosition = false;
    else
        deltaPosition += oldPosition - pos;

    oldPosition = pos;
}


Vector2f MouseHandler::pop_position()
{
    Vector2f ret;

    ret = deltaPosition;
    deltaPosition.setZero();

    return ret;
}


// Handle the button presses
// Note: We will not concern ourselves with the modifiers at this time.
void MouseHandler::button_callback(int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        buttons[button] = true;
    else if (action == GLFW_RELEASE)
        buttons[button] = false;
}


bool MouseHandler::is_button(int buttoncode)
{
    if (buttons[buttoncode])
        return true;
    else
        return false;
}


void MouseHandler::reset_button(int buttoncode)
{
    buttons[buttoncode] = false;
}


bool MouseHandler::is_button_left()
{
    if (buttons[GLFW_MOUSE_BUTTON_LEFT])
        return true;
    else
        return false;
}


bool MouseHandler::is_button_middle()
{
    if (buttons[GLFW_MOUSE_BUTTON_MIDDLE])
        return true;
    else
        return false;
}


bool MouseHandler::is_button_right()
{
    if (buttons[GLFW_MOUSE_BUTTON_RIGHT])
        return true;
    else
        return false;
}


// Handle the mouse scroll
void MouseHandler::scroll_callback(const Vector2f& scrollPos)
{
    deltaScroll += scrollPos;
}


Vector2f MouseHandler::pop_scroll()
{
    Vector2f ret;

    ret = deltaScroll;
    deltaScroll.setZero();

    return ret;
}

