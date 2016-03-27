#include "MenuState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "ControlsState.h"
#include "RecordsState.h"
#include "CreditsState.h"
#include "PauseState.h"
#include "records.h"


//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> MenuState* Ogre::Singleton<MenuState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void MenuState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  createScene();
  _exitGame = false;
}

void MenuState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void MenuState::pause()
{
}

void MenuState::resume()
{
   enter();
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool MenuState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame;
  return true;
}

bool MenuState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}

bool MenuState::keyPressed(const OIS::KeyEvent &e)
{

  if (e.key == OIS::KC_S) {
    //MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    pushState(PlayState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_C) {
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    //pushState(ControlsState::getSingletonPtr());
    pushState(CreditsState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_R) {
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    pushState(RecordsState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_P) {
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    pushState(PauseState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_ESCAPE) 
  {
    _exitGame = true;
  }
  return true;
}

bool MenuState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool MenuState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool MenuState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool MenuState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

MenuState* MenuState::getSingletonPtr()
{
    return msSingleton;
}

MenuState& MenuState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

MenuState::~MenuState()
{
}

void MenuState::destroyMyGui()
{
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

void MenuState::createMyGui()
{
  layout = MyGUI::LayoutManager::getInstance().loadLayout("shooter_main.layout");
}

void MenuState::createScene()
{
 createMyGui();
}

MenuState::MenuState()
{
}

bool MenuState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool MenuState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool MenuState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e)
{return true;}
