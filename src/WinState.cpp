#include "WinState.h"
#include "IntroState.h"
#include "MenuState.h"
#include "PlayState.h"

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#define TIME_RESUCITANDO 3

template<> WinState* Ogre::Singleton<WinState>::msSingleton = 0;

void WinState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _exitGame = false;
  createScene();
}

void WinState::createScene()
{
  createMyGui();
}

void WinState::createMyGui()
{
}

void WinState::destroyMyGui()
{
}

void WinState::exit()
{
  destroyMyGui();
}

void WinState::pause()
{
}

void WinState::resume()
{
}

bool WinState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool WinState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}

bool WinState::keyPressed(const OIS::KeyEvent &e) 
{
  // Tecla p --> Estado anterior.
      if (e.key == OIS::KC_P)  // Con  P otra vez reanudamos el PlayState
      {
        popState();
      }
  
  return true;
}

bool WinState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool WinState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool WinState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool WinState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

WinState* WinState::getSingletonPtr ()
{
    return msSingleton;
}

WinState& WinState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

bool WinState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool WinState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool WinState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e)
{return true;}