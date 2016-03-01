#include "PauseState.h"
#include "IntroState.h"
#include "MenuState.h"
#include "PlayState.h"

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#define TIME_RESUCITANDO 3

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void PauseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _exitGame = false;
}

void PauseState::exit()
{
}

void PauseState::pause()
{
}

void PauseState::resume()
{
}

bool PauseState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool PauseState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}

bool PauseState::keyPressed(const OIS::KeyEvent &e) 
{
  // Tecla p --> Estado anterior.
      if (e.key == OIS::KC_P)  // Con  P otra vez reanudamos el PlayState
      {
        popState();
      }
  
  return true;
}

bool PauseState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool PauseState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool PauseState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool PauseState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

PauseState* PauseState::getSingletonPtr ()
{
    return msSingleton;
}

PauseState& PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

