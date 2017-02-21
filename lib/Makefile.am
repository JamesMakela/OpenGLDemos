#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libOpenGLCommon.la libCPPMisc.la

#######################################
# libOpenGLCommon options
# Shared libraries built with libtool use the current.revision.age format
# where:
#    - current:  The most recent interface number that the library implements.
#    - revision: The implementation number of the current interface.
#    - age:      The difference between the newest and oldest interfaces
#                that the library implements. 
#######################################
libOpenGLCommon_la_SOURCES = Shader.cpp \
                             Texture.cpp

libOpenGLCommon_la_LDFLAGS = -version-info 1:0:0

libOpenGLCommon_la_LIBADD = -lGL -lGLEW -lglfw -lSOIL

libOpenGLCommon_la_CPPFLAGS = -I$(top_srcdir)/include

#######################################
# libCPPMisc options
#######################################
libCPPMisc_la_SOURCES = CmdOptionParser.cpp

libCPPMisc_la_LDFLAGS = -version-info 1:0:0

libCPPMisc_la_CPPFLAGS = -I$(top_srcdir)/include

