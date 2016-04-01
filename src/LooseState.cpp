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
  createScene();
  _exitGame = false;
}

void LooseState::exit()
{
 _root->getAutoCreatedWindow()->removeAllViewports();
  destroyMyGui();
}

void LooseState::pause()
{
}

void LooseState::resume()
{
}

bool LooseState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame;
  return true;
}

bool LooseState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}

bool LooseState::keyPressed(const OIS::KeyEvent &e) 
{
  // Tecla p --> Estado anterior.
      if (e.key == OIS::KC_P)  // Con  P otra vez reanudamos el PlayState
      {
        popState();
      }
  
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
  int x = e.state.X.abs;
  int y = e.state.Y.abs;
  if (btn_resume->_checkPoint(x,y))
  {
cout << __FUNCTION__<<endl;
    popState();
  }
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

void LooseState::destroyMyGui()
{
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

void LooseState::createMyGui()
{
  layout = MyGUI::LayoutManager::getInstance().loadLayout("shooter_loose.layout");
  btn_resume = MyGUI::Gui::getInstance().findWidget<MyGUI::Button>("btn_resume");

}

void LooseState::createScene()
{
 createMyGui();
}

bool LooseState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool LooseState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool LooseState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e)
{return true;}
