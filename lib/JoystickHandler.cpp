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

#include "JoystickHandler.hpp"

#include <iostream>

// this is just to make printing stuff a bit more concise
// everything else in std, we can call explicitly
using std::cout;
using std::cin;
using std::endl;


void JoystickHandler::connection_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED) {
        cout << "Joystick " << joy << " connected";
        enable_joystick(joy);
    }
    else if (event == GLFW_DISCONNECTED) {
        // We can't really rely on the disconnection event.  This sort of
        // invalidates the notion we can even use this callback for anything,
        // since we will have to poll anyway for most of our joystick
        // behaviors.  Kinda annoying.
        cout << "Joystick " << joy << " disconnected";
        disable_joystick(joy);
    }
}


void JoystickHandler::poll_connected()
{
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
        if (glfwJoystickPresent(i))
            enable_joystick(i);
        else
            disable_joystick(i);
    }
}


void JoystickHandler::enable_joystick(int joy)
{
    joysticks[joy].name = (char *)glfwGetJoystickName(joy);
    joysticks[joy].nameHash = SpookyHash::Hash64(joysticks[joy].name,
                                                 strlen(joysticks[joy].name),
                                                 0);
    joysticks[joy].connected = true;
}


void JoystickHandler::disable_joystick(int joy)
{
    joysticks[joy].connected = false;

    joysticks[joy].name = nullptr;
    joysticks[joy].nameHash = 0;
}


void JoystickHandler::get_joystick_state(int joy)
{
    if (joysticks[joy].connected) {
        joysticks[joy].axes = (float *)glfwGetJoystickAxes(joy, &joysticks[joy].numAxes);
        joysticks[joy].buttons = (unsigned char *)glfwGetJoystickButtons(joy, &joysticks[joy].numButtons);
    }
}


void JoystickHandler::print_connected()
{
    cout << "Joysticks connected [" << endl;
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
        get_joystick_state(i);
        print_joystick_info(i);
    }
    cout << "]" << endl;
}


void JoystickHandler::print_joystick_info(int joy)
{
    if (joysticks[joy].connected) {
        int numAxes = joysticks[joy].numAxes;
        int numButtons = joysticks[joy].numButtons;

        cout << "\t(joystick: " << joy << ", "
             //<< "Name: " << joysticks[joy].name << ", "
             << "Hash: " << joysticks[joy].nameHash << ", "
             << "#axes: " << numAxes << ", "
             << "#buttons: " << numButtons << ", "
             << ")," << endl;

        cout << "\tAxes: (";
        for (int a = 0; a < numAxes; a++) {
            cout << joysticks[joy].axes[a] << ", ";
        }
        cout << ")" << endl;

        cout << "\tButtons: (";
        for (int b = 0; b < numButtons; b++) {
            cout << (unsigned int)joysticks[joy].buttons[b] << ", ";
        }
        cout << ")" << endl;
    }
}

