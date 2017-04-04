//============================================================================
// Name        : KeyHandler.hpp
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

#ifndef KEYHANDLER_HPP_
#define KEYHANDLER_HPP_

#include <GLFW/glfw3.h>

class KeyHandler
{
public:
    void callback(int key, int scancode, int action, int mode);

    bool is_key(int keycode);
    void reset_key(int keycode);

    bool is_up();
    bool is_down();
    bool is_left();
    bool is_right();

private:
    bool keys[1024];
};


#endif /* KEYHANDLER_HPP_ */
