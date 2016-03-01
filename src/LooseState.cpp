#include "LooseState.h"
#include "IntroState.h"
#include "MenuState.h"
#include "PlayState.h"

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#define TIME_RESUCITANDO 3

template<> LooseState* Ogre::Singleton<LooseState>::msSingleton = 0;

void LooseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _exitGame = false;
  createScene();
}

void LooseState::createScene()
{
  createMyGui();
}

void LooseState::exit()
{
  destroyMyGui();
}

void LooseState::createMyGui()
{
}

void LooseState::destroyMyGui()
{
}
void LooseState::pause()
{
}

void LooseState::resume()
{
}

bool LooseState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool LooseState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}

bool LooseState::keyPressed(const OIS::KeyEvent &e) 
{
  return true;
}

bool LooseState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool LooseState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool LooseState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool LooseState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

LooseState* LooseState::getSingletonPtr ()
{
    return msSingleton;
}

LooseState& LooseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

