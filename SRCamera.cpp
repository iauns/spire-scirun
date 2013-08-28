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

#include "SRCamera.h"
#include "SRCommonUniforms.h"

namespace Spire {
namespace SCIRun {

//------------------------------------------------------------------------------
SRCamera::SRCamera(SRInterface& iface) :
    mTrafoSeq(0),
    mPerspective(true),
    mFOV(getDefaultFOVY()),
    mZNear(getDefaultZNear()),
    mZFar(getDefaultZFar()),
    mInterface(iface)
{
  setAsPerspective();

  Spire::M44 cam;
  cam[3] = (Spire::V4(0.0f, 0.0f, 7.0f, 1.0f));
  
  setViewTransform(cam);
}

//------------------------------------------------------------------------------
SRCamera::~SRCamera()
{
}

//------------------------------------------------------------------------------
void SRCamera::setAsPerspective()
{
  mPerspective = true;

  float aspect = static_cast<float>(mInterface.getScreenWidthPixels()) / 
                 static_cast<float>(mInterface.getScreenHeightPixels());
  mP = glm::perspective(mFOV, aspect, mZNear, mZFar);
}

//------------------------------------------------------------------------------
void SRCamera::setAsOrthographic(float halfWidth, float halfHeight)
{
  mPerspective = false;

	mP = glm::ortho(-halfWidth, halfWidth, 
                  -halfHeight, halfHeight, 
                  mZNear, mZFar);
}

//------------------------------------------------------------------------------
void SRCamera::setViewTransform(const Spire::M44& trafo)
{
  ++mTrafoSeq;

  //std::cout << "Transform: " << std::endl;
  //printM44(trafo);

  mV    = trafo;
  mIV   = glm::affineInverse(trafo);
  mPIV  = mP * mIV;

  // Update appropriate uniforms.
  mInterface.addGlobalUniform(std::get<0>(SRCommonUniforms::getToCameraToProjection()), mPIV);
  mInterface.addGlobalUniform(std::get<0>(SRCommonUniforms::getToProjection()), mP);
  mInterface.addGlobalUniform(std::get<0>(SRCommonUniforms::getCameraToWorld()), mV);

  // Projection matrix is oriented down negative z. So we are looking down 
  // negative z, which is -V3(mV[2].xyz()).
  mInterface.addGlobalUniform(std::get<0>(SRCommonUniforms::getCameraViewVec()), -V3(mV[2].xyz()));
  mInterface.addGlobalUniform(std::get<0>(SRCommonUniforms::getCameraUpVec()), V3(mV[1].xyz()));
}

} // namespace SCIRun
} // namespace Spire
