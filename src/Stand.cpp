
#include "Stand.h"
#include <iostream>
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreUtil.h"
#include "PlayState.h"



Stand::Stand( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world){
    _position = position;
    _sceneMgr = sceneMgr;
    _name = name;
    _activatorPosition = position + Vector3(0,0,5);
    _world= world;
    _root = Ogre::Root::getSingletonPtr();
    _camera = sceneMgr->getCamera("IntroCamera");
    _cameraNode = sceneMgr->getSceneNode("nodeCamera");
    _cameraBody = PlayState::getSingleton().getCameraBody();
    _activatorActive = true;
    startGame();


}

void Stand::buildGame(){

  StaticGeometry *stage = _sceneMgr->createStaticGeometry(_name);
  Entity* entStand = _sceneMgr->createEntity("puesto.mesh");
  entStand->setQueryFlags(COL_STAND);
  entStand->setCastShadows(true);
  //Asociar forma y cuerpo rígido
  OgreBulletCollisions::StaticMeshToShapeConverter trimeshConverter = OgreBulletCollisions::StaticMeshToShapeConverter(entStand);
  _shapeStand = trimeshConverter.createTrimesh();
  _rigidStand = new OgreBulletDynamics::RigidBody(_name, _world.get(), COL_STAND,  COL_CAMERA | COL_FLOOR | COL_ACTIVATOR | COL_CAN | COL_BALL);
  _rigidStand->setStaticShape(_shapeStand, 1, 1, _position, Quaternion::IDENTITY);

  entStand->setCastShadows(true);
  stage->addEntity(entStand, Vector3(_position));
  stage->build();
  buildActivator();

}


void Stand::buildActivator(){

  std::stringstream str;
  str << "Activator" << _name;
  _activator = _sceneMgr->createSceneNode(str.str());
  Entity* entActivator = _sceneMgr->createEntity("Activador.mesh");
  _activator->attachObject(entActivator);

  _activator->setPosition(_activatorPosition);
  _sceneMgr->getRootSceneNode()->addChild(_activator);
  _rigidActivator = new OgreBulletDynamics::RigidBody(str.str(), _world.get(),COL_ACTIVATOR, COL_STAND  | COL_FLOOR | COL_CAMERA);
  _shapeActivator = new OgreBulletCollisions::CylinderCollisionShape(Vector3(0.5,0.05,0.05),Vector3::UNIT_Y);
  _rigidActivator->setShape(_activator, _shapeActivator, 0,1,1000,_activatorPosition,Quaternion::IDENTITY);
}


void Stand::enter (){
  



}
void Stand::exit(){}
void Stand::pause (){}
void Stand::resume (){}

bool Stand::keyPressed (const OIS::KeyEvent &e){

  keysArePressed.erase(e.key);
  keysArePressed[e.key] = true;
  if (e.key == OIS::KC_P) {
    // pushState(PauseState::getSingletonPtr());
  } else if (e.key == OIS::KC_G) {
    //    game_over();
  } else if (e.key == OIS::KC_W) {
    //    win();
  }

  return true;}
bool Stand::keyReleased (const OIS::KeyEvent &e){ return true;}

bool Stand::mouseMoved (const OIS::MouseEvent &e){return true;}
bool Stand::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id){return true;}
bool Stand::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id){return true;}

bool Stand::frameStarted (const Ogre::FrameEvent& evt){
  _deltaT = evt.timeSinceLastFrame;
  _world.get()->stepSimulation(evt.timeSinceLastFrame);

  if (keysArePressed[OIS::KC_ESCAPE]) {
    return false;
  }
  checkCollisions();

  return true;

}
bool Stand::frameEnded (const Ogre::FrameEvent& evt){


  return true;


}
  
/* WIIMOTE *********************************************************************/  
bool Stand::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e){return true;};
bool Stand::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e){return true;};
bool Stand::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e){return true;};
