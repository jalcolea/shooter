#include "ControlsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include <stdlib.h>
#include "records.h"

template<> ControlsState* Ogre::Singleton<ControlsState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void ControlsState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  createScene();
  _exitGame = false;
}

void ControlsState::destroyMyGui()
{
}

void ControlsState::createMyGui()
{
}

void ControlsState::exit ()
{
  destroyMyGui();
}

void ControlsState::pause()
{
}

void ControlsState::resume()
{
}

bool ControlsState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame;
  return true;
}

bool ControlsState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}


bool ControlsState::keyPressed(const OIS::KeyEvent &e)
{
  popState();
  return true;
}

bool ControlsState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool ControlsState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool ControlsState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool ControlsState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

ControlsState* ControlsState::getSingletonPtr()
{
    return msSingleton;
}

ControlsState& ControlsState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

ControlsState::~ControlsState()
{
}

void ControlsState::createScene()
{
 createMyGui();
}

bool ControlsState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool ControlsState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool ControlsState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e)
{return true;}
