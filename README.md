# OpenGLDemos

## Expanding upon the simple HelloTriangle demo programs.

The simple demo programs are nice in that they are self contained pieces of code
that do one thing.  But to start really exploring the features of OpenGL, we would
like to start reusing code for the more common things.

So this project contains a library area for making reusable class libraries.
New executable targets can then be added to explore new OpenGL features, using the
libraries.

This project is setup to use the GNU autoconf utilities, so in order to configure it
for your platform, you need autoconf, automake, libtools, etc.

It also contains Eclipse configurations, so if you use Eclipse, you can import the
project using the EGit plugin.  You will probably also need the Autotools plugin to
manage the project's autoconf setup within Eclipse.
