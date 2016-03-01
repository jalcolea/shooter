#include <iostream>
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

using namespace std;
using namespace Ogre;

template<> IntroState *Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter()
{
  _root = Ogre::Root::getSingletonPtr();
  createScene();
  _exitGame = false;
  _deltaT = 0;
}

void IntroState::exit()
{
    destroyMyGui();
}

void IntroState::pause()
{
}

void IntroState::resume()
{

}

bool IntroState::frameStarted(const Ogre::FrameEvent &evt)
{
  _deltaT = evt.timeSinceLastFrame;
  return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent &evt)
{
    return true;
}

bool IntroState::keyPressed(const OIS::KeyEvent &e)
{

    // Transición al siguiente estado.
    // Espacio --> PlayState
    if (e.key == OIS::KC_SPACE)
    {
        changeState(MenuState::getSingletonPtr());
    }
    return true;

}

bool IntroState::keyReleased(const OIS::KeyEvent &e)
{
    if (e.key == OIS::KC_ESCAPE)
    {

        _exitGame = true;
    }
    return true;
}

bool IntroState::mouseMoved(const OIS::MouseEvent &e)
{
    return true;
}

bool IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

bool IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

IntroState *IntroState::getSingletonPtr()
{
    return msSingleton;
}

IntroState &IntroState::getSingleton()
{
    assert(msSingleton);
    return *msSingleton;
}

IntroState::~IntroState()
{
}


void IntroState::createScene()
{
  createMyGui();
}

void IntroState::destroyMyGui()
{
}

void IntroState::createMyGui()
{
}

