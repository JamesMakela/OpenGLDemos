//============================================================================
// Name        : OGLCommon.hpp
// Author      : James L. Makela
// Version     : 0.1.1
// Copyright   : LGPL v3.0
// Description : What we are trying to do is refactor the OpenGL demo code
//               into classes that are easier to use.
//               This is just a place where we will define things that are
//               common to all of the OpenGL library stuff that we are
//               building.
//============================================================================

#ifndef OGLCOMMON_HPP_
#define OGLCOMMON_HPP_

#include <cmath>


// In C++ there seem to be no good builtin constant for PI.
// And of course everyone has an opinion on how to define it.
// So I chose the one I felt was best for this OpenGL app.
// constexpr is new since C++11, and indicates evaluation at compile time.
constexpr GLfloat pi() { return std::acos(-1.0); }


// GLM has a conversion routine from degrees to radians,
// but Eigen does not seem to have one.
// Let's just build a simple one so we don't have to rely upon
// GLM if we are using Eigen
#define to_radians(degrees) (degrees * 4.0 * atan(1.0) / 180.0)
#define to_degrees(radians) (radians * 180.0 / (4.0 * atan(1.0)))

#endif /* OGLCOMMON_HPP_ */
