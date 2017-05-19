//============================================================================
// Name        : KeyHandler.cpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the OpenGL demo code
//               into classes that are easier to use.
//               The GLFW callback for keyboard events only handles one key
//               event at a time, with no support for multiple key
//               combinations.  So we built a simple keyboard handler to
//               handle such things.
//
//               More complex implementations may include customisable key
//               configurations, but maybe we will just subclass for that.
//               Right now let's keep it simple.
//============================================================================

#include "KeyHandler.hpp"


void KeyHandler::callback(int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}


bool KeyHandler::is_key(int keycode)
{
    if (keys[keycode])
        return true;
    else
        return false;
}


void KeyHandler::reset_key(int keycode)
{
    keys[keycode] = false;
}


bool KeyHandler::is_up()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        return true;
    else
        return false;
}


bool KeyHandler::is_down()
{
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        return true;
    else
        return false;
}


bool KeyHandler::is_left()
{
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        return true;
    else
        return false;
}


bool KeyHandler::is_right()
{
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        return true;
    else
        return false;
}
