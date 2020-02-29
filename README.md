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


## Quick start for the impatient.

First, do you have the required GNU tools for this?  Run the following command to find out:

```
$ apt list --installed |egrep -i "autoconf|libtool"

autoconf/stable,now 2.69-11 all [installed]
libtool/stable,now 2.4.6-9 all [installed]
```

If you don't see autoconf & libtool installed, you need to install them.  Here's how:

```
$ sudo apt install autoconf libtool
```

Next, do you have the required libraries installed?  Here's how to find out:

```
$ apt list --installed |egrep -i "libglew-dev|libsoil-dev|libeigen3-dev|libglfw3-dev|libglm-dev"

libeigen3-dev/stable,now 3.3.7-1 all [installed]
libglew-dev/stable,now 2.1.0-4 armhf [installed]
libglfw3-dev/stable,now 3.2.1-1 armhf [installed]
libglm-dev/stable,now 0.9.9.3-2 all [installed]
libsoil-dev/stable,now 1.07~20080707.dfsg-4 armhf [installed]
```

