#include "ControlsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
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

void ControlsState::exit ()
{
 //_sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();

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
  int x = e.state.X.abs;
  int y = e.state.Y.abs;
  if (btn_back->_checkPoint(x,y))
  {
    popState();
  }
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

void ControlsState::destroyMyGui()
{
  MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

void ControlsState::createMyGui()
{
  layout = MyGUI::LayoutManager::getInstance().loadLayout("shooter_controls.layout");
  btn_back = MyGUI::Gui::getInstance().findWidget<MyGUI::Button>("btn_back");
}

bool ControlsState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool ControlsState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool ControlsState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e)
{return true;}
