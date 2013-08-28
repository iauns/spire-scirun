/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2013 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/// \author James Hughes
/// \date   March 2013

#ifndef SPIRE_APPSPECIFIC_SCIRUN_SRCOMMONUNIFORMS_H
#define SPIRE_APPSPECIFIC_SCIRUN_SRCOMMONUNIFORMS_H

#include <tuple>

#include "Spire/Core/ShaderUniformStateManTemplates.h"

namespace Spire
{

/// Common uniforms used by Spire.
/// \todo Look into the ability to rename these uniforms in the future. This is
/// so that Spire can adapt to other code bases.
class SRCommonUniforms
{
public:
  SRCommonUniforms()          {}
  virtual ~SRCommonUniforms() {}
  
  //----------------------------------------------------------------------------
  // Camera Uniforms (View)
  //----------------------------------------------------------------------------

  /// Inverse view to world matrix.
  static std::tuple<const char*, UNIFORM_TYPE> getToCameraToProjection()
  {return std::make_pair("uProjIV", UNIFORM_FLOAT_MAT4);}

  /// Projection matrix.
  static std::tuple<const char*, UNIFORM_TYPE> getToProjection()
  {return std::make_pair("uProj", UNIFORM_FLOAT_MAT4);}

  /// View to world.
  static std::tuple<const char*, UNIFORM_TYPE> getCameraToWorld()
  {return std::make_pair("uView", UNIFORM_FLOAT_MAT4);}

  /// Viewing vector for the camera (used in lighting calculations).
  /// Depends on the projection matrix -- in world space.
  static std::tuple<const char*, UNIFORM_TYPE> getCameraViewVec()
  {return std::make_pair("uCamViewVec", UNIFORM_FLOAT_VEC3);}

  /// 'Up' vector for the camera -- in world space.
  static std::tuple<const char*, UNIFORM_TYPE> getCameraUpVec()
  {return std::make_pair("uCamUp", UNIFORM_FLOAT_VEC3);}

  //----------------------------------------------------------------------------
  // Object and Combined Object/Camera Uniforms
  //----------------------------------------------------------------------------
  /// Object -> World -> Camera -> Projection
  static std::tuple<const char*, UNIFORM_TYPE> getObjectToCameraToProjection()   
  {return std::make_pair("uProjIVObject", UNIFORM_FLOAT_MAT4);}

  /// Object -> World to view transformation.
  static std::tuple<const char*, UNIFORM_TYPE> getObjectToView()
  {return std::make_pair("uViewObject", UNIFORM_FLOAT_MAT4);}

  /// Object -> World transformformation.
  static std::tuple<const char*, UNIFORM_TYPE> getObject()
  {return std::make_pair("uObject", UNIFORM_FLOAT_MAT4);}

};

} // namespace Spire

#endif 
