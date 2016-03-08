#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "OgreBulletDynamicsWorld.h"
#include <string>
#include <vector>
#include "records.h"
#include "OgreUtil.h"
#include <ctime>

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

void PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");
  createScene();
  _exitGame = false;
  paused=false;
  _deltaT = 0;
}

void PlayState::exit ()
{
  destroyMyGui();
}

void PlayState::pause()
{
        paused = true;
}

void PlayState::resume()
{
    paused=false;
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame;
  return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}

bool PlayState::keyPressed(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_G) {
    game_over();
  }
  else if (e.key == OIS::KC_W) {
    win();
  }
  return true;
}
 
bool PlayState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool PlayState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

PlayState* PlayState::getSingletonPtr()
{

  return msSingleton;
}

PlayState& PlayState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

PlayState::~PlayState()
{
}

void PlayState::createScene()
{
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  _sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
  _sceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
  createLight();
  createMyGui();
  StaticGeometry* stage =   _sceneMgr->createStaticGeometry("SG");
  Entity* entLevel = _sceneMgr->  createEntity("puesto.mesh");
  
  entLevel->setCastShadows(true);
  stage->addEntity(entLevel, Vector3(0,0,0));
  stage->build();

}

void PlayState::createMyGui()
{
}

void PlayState::destroyMyGui()
{
}

void PlayState::createLight()
  
{
  _sceneMgr->setShadowTextureCount(2);
  _sceneMgr->setShadowTextureSize(512);
  Light* light = _sceneMgr->createLight("Light1");
  light->setPosition(0, 12, 0);
  light->setType(Light::LT_SPOTLIGHT);
  light->setDirection(Vector3(0, -1, 0));
  light->setSpotlightInnerAngle(Degree(60.0f));
  light->setSpotlightOuterAngle(Degree(80.0f));
  light->setSpotlightFalloff(0.0f);
  light->setCastShadows(true);

}

void PlayState::win()
{
}

void PlayState::game_over()
{
}

void PlayState::set_lives (int lives)
{
  this->lives = lives;
}

int PlayState::get_lives ()
{
  return lives;
}

void PlayState::set_score (int score)
{
  this->score = score;
}

int PlayState::get_score ()
{
  return score;

}

void PlayState::handleCollision(btCollisionObject *body0, btCollisionObject *body1) 
{
}

bool PlayState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool PlayState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e)
{return true;}
bool PlayState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e)
{return true;}



