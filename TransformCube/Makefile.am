#######################################
# The list of executables we are building seperated by spaces
# A 'bin_' prefix indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#
# The 'noinst_' prefix indicates that the following targets are to be built,
# but not installed.
bin_PROGRAMS=TransformCube

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4

# Sources for the a.out 
TransformCube_SOURCES= TransformCube.cpp

# Libraries for a.out
TransformCube_LDADD = $(top_srcdir)/lib/libOpenGLCommon.la \
                      $(top_srcdir)/lib/libCPPMisc.la

# Linker options for a.out
TransformCube_LDFLAGS = -rpath `cd $(top_srcdir);pwd`/lib/.libs \
                        -lGL -lGLEW -lglfw -lSOIL

# Compiler options for a.out
TransformCube_CPPFLAGS = -I$(top_srcdir)/include \
                         -I/usr/include/eigen3
