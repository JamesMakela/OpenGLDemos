//============================================================================
// Name        : JoystickHandler.hpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : The GLFW support for joysticks seems a bit spotty to me.
//               There are events for connection/disconnection, and that's it.
//               Also, with versions up to 3.2.1, the disconnection event
//               doesn't reliably work.  All other communication is polled.
//
//============================================================================

#ifndef JOYSTICKHANDLER_HPP_
#define JOYSTICKHANDLER_HPP_

#include <GLFW/glfw3.h>

#include <Eigen/Dense>
using Eigen::Vector2f;

#include "SpookyV2.h"


struct Joystick
{
    char *name;
    uint64 nameHash;

    bool connected;

    int numAxes;
    float *axes;

    int numButtons;
    unsigned char *buttons;
};


class JoystickHandler
{
public:
    void connection_callback(int joy, int event);

    void poll_connected();

    void enable_joystick(int joy);
    void disable_joystick(int joy);
    void get_joystick_state(int joy);

    void print_connected();
    void print_joystick_info(int joy);

private:
    Joystick joysticks[GLFW_JOYSTICK_LAST] {};

};


#endif /* JOYSTICKHANDLER_HPP_ */
