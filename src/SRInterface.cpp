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
/// \date   February 2013

#include "SRInterface.h"
#include "SciBall.h"
#include "SRCamera.h"

#include "Spire/Core/Hub.h"
#include "Spire/Core/InterfaceImplementation.h"

using namespace std::placeholders;

namespace Spire {
namespace SCIRun {

//------------------------------------------------------------------------------
SRInterface::SRInterface(std::shared_ptr<Context> context,
                         const std::vector<std::string>& shaderDirs,
                         bool createThread, LogFunction logFP) :
    Interface(context, shaderDirs, createThread, logFP),
    mCamDistance(7.0f),
    mScreenWidth(640),
    mScreenHeight(480),
    mCamAccumPosDown(0.0f, 0.0f, 0.0f),
    mCamAccumPosNow(0.0f, 0.0f, 0.0f),
    mCamera(new SRCamera(*this)),                       // Should come after all vars have been initialized.
    mSciBall(new SciBall(V3(0.0f, 0.0f, 0.0f), 1.0f))   // Should come after all vars have been initialized.
{
  buildAndApplyCameraTransform();
}

//------------------------------------------------------------------------------
SRInterface::~SRInterface()
{
}

//------------------------------------------------------------------------------
void SRInterface::eventResize(size_t width, size_t height)
{
  mScreenWidth = width;
  mScreenHeight = height; 

  // Ensure glViewport is called appropriately.
  Hub::RemoteFunction resizeFun =
      std::bind(InterfaceImplementation::resize, _1, width, height);
  mHub->addFunctionToThreadQueue(resizeFun);
}

//------------------------------------------------------------------------------
V2 SRInterface::calculateScreenSpaceCoords(const glm::ivec2& mousePos)
{
  float windowOriginX = 0.0f;
  float windowOriginY = 0.0f;

  // Transform incoming mouse coordinates into screen space.
  V2 mouseScreenSpace;
  mouseScreenSpace.x = 2.0f * (static_cast<float>(mousePos.x) - windowOriginX) 
      / static_cast<float>(mScreenWidth) - 1.0f;
  mouseScreenSpace.y = 2.0f * (static_cast<float>(mousePos.y) - windowOriginY)
      / static_cast<float>(mScreenHeight) - 1.0f;

  // Rotation with flipped axes feels much more natural.
  mouseScreenSpace.y = -mouseScreenSpace.y;

  return mouseScreenSpace;
}

//------------------------------------------------------------------------------
void SRInterface::inputMouseDown(const glm::ivec2& pos, MouseButton btn)
{
  // Translation variables.
  mCamAccumPosDown  = mCamAccumPosNow;
  mTransClick       = calculateScreenSpaceCoords(pos);

  if (btn == MOUSE_LEFT)
  {
    V2 mouseScreenSpace = calculateScreenSpaceCoords(pos);
    mSciBall->beginDrag(mouseScreenSpace);
  }
  else if (btn == MOUSE_RIGHT)
  {
    // Store translation starting position.
  }
  mActiveDrag = btn;
}

//------------------------------------------------------------------------------
void SRInterface::inputMouseMove(const glm::ivec2& pos, MouseButton btn)
{
  if (mActiveDrag == btn)
  {
    if (btn == MOUSE_LEFT)
    {
      V2 mouseScreenSpace = calculateScreenSpaceCoords(pos);
      mSciBall->drag(mouseScreenSpace);

      buildAndApplyCameraTransform();
    }
    else if (btn == MOUSE_RIGHT)
    {
      V2 curTrans = calculateScreenSpaceCoords(pos);
      V2 delta = curTrans - mTransClick;
      /// \todo This 2.5f value is a magic number, and it's real value should
      ///       be calculated based off of the world space position of the
      ///       camera. This value could easily be calculated based off of
      ///       mCamDistance.
      V2 trans = (-delta) * 2.5f;

      M44 camRot = mSciBall->getTransformation();
      V3 translation =   static_cast<V3>(camRot[0].xyz()) * trans.x
                       + static_cast<V3>(camRot[1].xyz()) * trans.y;
      mCamAccumPosNow = mCamAccumPosDown + translation;

      buildAndApplyCameraTransform();
    }
  }
}

//------------------------------------------------------------------------------
void SRInterface::inputMouseWheel(int32_t delta)
{
  // Reason why we subtract: Feels more natural to me =/.
  mCamDistance -= static_cast<float>(delta) / 100.0f;
  buildAndApplyCameraTransform();
}

//------------------------------------------------------------------------------
void SRInterface::inputMouseUp(const glm::ivec2& /*pos*/, MouseButton /*btn*/)
{
}

//------------------------------------------------------------------------------
void SRInterface::buildAndApplyCameraTransform()
{
  M44 camRot      = mSciBall->getTransformation();
  M44 finalTrafo  = camRot;

  // Translation is a post rotation operation where as zoom is a pre transform
  // operation. We should probably ensure the user doesn't scroll passed zero.
  // Remember, we are looking down NEGATIVE z.
  finalTrafo[3].xyz() = mCamAccumPosNow + static_cast<V3>(camRot[2].xyz()) * mCamDistance;

  mCamera->setViewTransform(finalTrafo);
}

} // namespace SCIRun
} // namespace Spire
