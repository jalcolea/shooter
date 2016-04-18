
#include "StandLatas.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include <iostream>
#include "Crosshair.h"
#include "sounds.h"
#include "OgreUtil.h"
#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"
#include "OgreTextAreaOverlayElement.h"
#include "Carrusel.h"

#define NUM_BOLAS 3
#define MIN_FORCE_SOUND 2
#define MAX_TIME_NO_BALLS 3
using namespace Ogre;
using namespace std;



void StandLatas::enter() {

  _loadingShoot =false;
  _timeLoadingShoot =0;

  reacomodateCamera();
  _numCans=0;
  drawCans();
  
  _crosshair = unique_ptr<Crosshair>(new Crosshair(_sceneMgr,_camera));
  _crosshair.get()->createCrossHairManual("circle-01.png");
  _rayScnQuery =_sceneMgr->createRayQuery(Ogre::Ray(), 0);
  _ballStack = _sceneMgr->createSceneNode("ballStack");
  _sceneMgr->getRootSceneNode()->addChild(_ballStack);
  drawHud();
   Carrusel c;
   c.go();
    _timeWithoutBalls=0;

    Ogre::Light* light = _sceneMgr->createLight("LightStandLatas");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(0,0,-1));
    light->setPosition(_cameraNode->getPosition());
    light->setCastShadows(true);

    




}


/**
 * Dibuja una lata en la posición adecuada
*/
void StandLatas::drawCan(Vector3 position, string name) {


  SceneNode *canNode = _sceneMgr->createSceneNode(name);
 
  Entity *canEnt = _sceneMgr->createEntity("lata.mesh");
  canEnt->setQueryFlags(COL_CAN);
  canEnt->setCastShadows(true);
  canNode->attachObject(canEnt);
  canNode->setPosition(_activatorPosition + Vector3(0, 2, 2));
  canNode->scale(Vector3(0.2, 0.2, 0.2));
  
  _canStack->addChild(canNode);
  OgreBulletDynamics::RigidBody *rigidCan = new OgreBulletDynamics::RigidBody(
      name, _world.get(), COL_CAN,
      COL_STAND | COL_FLOOR | COL_CAMERA | COL_CAN | COL_BALL);
  OgreBulletCollisions::CollisionShape *shapeCan =
      new OgreBulletCollisions::CylinderCollisionShape(Vector3(0.1, 0.21, 0.21),
                                                       Vector3::UNIT_Y);
  rigidCan->setShape(canNode, shapeCan, 0, 1, 5, position,
                      Quaternion::IDENTITY);

  _canBodys.push_back(rigidCan->getBulletRigidBody());
  _numCans++;
}

/**
 * dibuja una pila de latas
 */
void StandLatas::drawCans() {

  _canStack = _sceneMgr->createSceneNode("canStack");
  _sceneMgr->getRootSceneNode()->addChild(_canStack);
  Vector3 startPosition = _position + Vector3(0, 2.2, 0.1);

  float gap = 0.24;
  float height = 0.37;
  int numRows = 3;

  for (int i = numRows; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      float x = j * gap;
      x = x + (gap / 2.0f) * (numRows - i);
      stringstream str;
      str << "Can" << i << j;
      drawCan(startPosition + Vector3(x, (numRows - i) * height, 0), str.str());
    }
  }
  _timeWithoutBalls = 0;
}

/**
 * Detecta la dirección del disparo
 */
bool StandLatas::mouseMoved (const OIS::MouseEvent &e)
{ 


    const size_t& xMouse = e.state.X.abs + 6;// por alguna razÃ³n que desconozco el ratÃ³n estÃ¡ desplazado -6 unidades
    const size_t& yMouse = e.state.Y.abs + 6;
    
    const size_t& xRel = e.state.X.rel;
    const size_t& yRel = e.state.Y.rel;

    const Ogre::Real& wWindow = e.state.width;
    const Ogre::Real& hWindow = e.state.height;

        _crosshair.get()->setActualHitPoint(Ogre::Real(xMouse/wWindow),Ogre::Real(yMouse/hWindow));

    
  _mousex = e.state.X.abs;
  _mousey = e.state.Y.abs;
    
    

    return true;
}


bool StandLatas::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    if (id == OIS::MB_Left ) {
      if(_loadingShoot){
	shootBall(calculateDirShoot());
	
  	_loadingShoot = false;
	_timeLoadingShoot=0;
      }

}
      return true;
  

}
bool StandLatas::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
      if (e.state.buttonDown(OIS::MB_Left) == true) {
	if(_numBolas > 0 ){
	  _loadingShoot = true;
	}


}
      return true;

}


/**
 * Calcula dirección del disparo
 */
Vector3 StandLatas::calculateDirShoot() {

  Vector3 dirShoot;
  _rayScnQuery->setSortByDistance(true);
  _rayScnQuery->setQueryMask(COL_STAND|COL_CAN);

    Ogre::Ray ray  = _camera->
                       getCameraToViewportRay(_mousex/float(1024),
                                              _mousey/float(768));
    dirShoot = ray.getDirection();


    std::cout << "Factor de Potencia "<< (_timeLoadingShoot/0.1)   << " tiempo Pulsado " <<_timeLoadingShoot << std::endl;
    return dirShoot*(_timeLoadingShoot/0.05);
}


/**
 * Dispara una bola a la dirección dada.
 */
 void StandLatas::shootBall(Vector3 dir){


  stringstream str;
  str << "Ball" << _numBolas;
  string name = str.str();  
  SceneNode *ballNode = _sceneMgr->createSceneNode(name);
  Entity *ballEnt = _sceneMgr->createEntity("ball.mesh");
  ballEnt->setCastShadows(true);
  ballNode->attachObject(ballEnt);
  OgreBulletDynamics::RigidBody *rigidBall = new OgreBulletDynamics::RigidBody(
      name, _world.get(), COL_BALL,
      COL_STAND | COL_FLOOR | COL_CAMERA | COL_CAN | COL_BALL);
  OgreBulletCollisions::CollisionShape *shapeBall =
    new OgreBulletCollisions::SphereCollisionShape(0.12);
  rigidBall->setShape(ballNode, shapeBall, 0, 1, 0.8, _cameraNode->getPosition()+ Vector3(0,0,-2),
                      Quaternion::IDENTITY);
  _ballStack->addChild(ballNode);
  rigidBall->getBulletRigidBody()->setLinearVelocity(convert(dir));
  

  _ballBodys.push_back(rigidBall->getBulletRigidBody());
  _numBolas--;


  

  stringstream str2;
  str2 << "BallHud" << _numBolas;
  OgreUtil::destroySceneNode(_sceneMgr->getSceneNode(str2.str()));
}

/**
 * Reacomodando Camara para que se vea bien el puesto
 */
void StandLatas::reacomodateCamera() {

  _cameraBody->setOrientation(
      Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));

  _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_SIMULATION);
  /*    btTransform bt = _cameraBody->getBulletRigidBody()->getWorldTransform();
  bt.setOrigin(convert(_activatorPosition-Vector3(0,0,-10) ));
  _cameraBody->getBulletRigidBody()->setWorldTransform(bt);
  _cameraNode->translate(_activatorPosition-Vector3(0,0,2));*/
  _cameraNode->translate(Vector3(0, 1, -3));
}


double StandLatas::getCollisionForce(btPersistentManifold* maniFold){

  double force =0;
  for(int i=0; i<  maniFold->getNumContacts(); i++){
    force+= maniFold->getContactPoint(i).getAppliedImpulse();
  }


  return force;

}

void StandLatas::deleteFallenCans(){



  for (std::vector <btRigidBody*>::iterator it = _canBodys.begin(); it != _canBodys.end();  ) {
    
    if((*it)->getCenterOfMassPosition().getY() < 0.5){
      _world.get()->removeObject(_world.get()->findObject(*it));

      _world.get()->getBulletDynamicsWorld()->removeRigidBody(*it);

      delete (*it)->getMotionState();
      delete (*it)->getCollisionShape();
      delete (*it);

      it = _canBodys.erase(it);
      
      _numCans--;
     }
    else{
      it++;
    }

  }

  for (std::vector <btRigidBody*>::iterator it = _ballBodys.begin(); it != _ballBodys.end();  ) {
    
    if((*it)->getCenterOfMassPosition().getY() < 0.5){

      _world.get()->removeObject(_world.get()->findObject(*it));
      _world.get()->getBulletDynamicsWorld()->removeRigidBody(*it);
      delete (*it)->getMotionState();
      delete (*it)->getCollisionShape();
      delete (*it);
      it = _ballBodys.erase(it);
     }
    else{
      it++;
    }

  }



  if(_numCans <= 0){

    //_canBodys.clear();
    // _ballBodys.clear();
    OgreUtil::destroySceneNode(_canStack);
    OgreUtil::destroySceneNode(_ballStack);
    OgreUtil::destroySceneNode(_sceneMgr->getSceneNode("ballsHud"));
    _ballStack = _sceneMgr->createSceneNode("ballStack");
   _sceneMgr->getRootSceneNode()->addChild(_ballStack);
   
   drawCans();
   paintBallsHud();


  }


}
void StandLatas::checkCollisions() {

  btCollisionWorld *collisionWorld = _world.get()->getBulletCollisionWorld();
  btDynamicsWorld *dynamicWorld = _world.get()->getBulletDynamicsWorld();

  int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
  bool collide = false;
  for (int i = 0; i < numManifolds; i++) {
    btPersistentManifold *contactManifold =
        collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject *obA = (btCollisionObject *)contactManifold->getBody0();
    btCollisionObject *obB = (btCollisionObject *)contactManifold->getBody1();



    std::vector<btRigidBody*>::iterator it = std::find_if(
            _canBodys.begin(), _canBodys.end(),
            [contactManifold](btRigidBody* canBody) -> bool {
              return contactManifold->getBody0()->getCollisionShape() == canBody->getCollisionShape() ||
		contactManifold->getBody1()->getCollisionShape() == canBody->getCollisionShape();
            });

	
        if (it != _canBodys.end() && getCollisionForce(contactManifold)> MIN_FORCE_SOUND) {
	 
	          sounds::getInstance()->play_effect("can");

        }

     
    
  }

  deleteFallenCans();

  if(_loadingShoot){


    double width = 0.1+ _timeLoadingShoot/0.001;
    if(width<1000){
    _panel->setWidth(width);
    _timeLoadingShoot += _deltaT;
    }
  }

  if(_numBolas == 0){
    _timeWithoutBalls += _deltaT;
    
    if(_timeWithoutBalls > MAX_TIME_NO_BALLS){
      endGame();
    }
  }

  
}


void StandLatas::drawHud(){

  Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

 
// Create a panel
   _panel = (OverlayContainer*)(
    overlayManager.createOverlayElement("Panel", "Panelaco"));
_panel->setMetricsMode(Ogre::GMM_PIXELS);
_panel->setPosition(10, 10);
_panel->setDimensions(100, 10);
_panel->setMaterialName("Material.002"); // Optional background material
 

// Create an overlay, and add the panel
Overlay* overlay = overlayManager.create("OverlayName");
overlay->add2D(_panel);
 
// Add the text area to the panel
//panel->addChild(textArea);

// Show the overlay
overlay->show();

 paintBallsHud();



}

void StandLatas::paintBallsHud(){

  _numBolas = NUM_BOLAS;
  SceneNode *ballsHud = _sceneMgr->createSceneNode("ballsHud");
  

  for(int i=0; i< _numBolas;i++){
    
    stringstream str;
    str << "BallHud" << i;

    string name = str.str();  
    SceneNode *ballNode = _sceneMgr->createSceneNode(name);
    Entity *ballEnt = _sceneMgr->createEntity("ball.mesh");
    ballNode->attachObject(ballEnt);
    ballNode->scale(Vector3(0.3,0.3,0.3));
    ballNode->translate(Vector3(((float)i/(float)10)-0.4,-0.35,0));
    std::cout << str.str() << " - " << ballNode->getPosition() << std::endl;
    ballsHud->addChild(ballNode);
  }
  _sceneMgr->getRootSceneNode()->addChild(ballsHud);
  ballsHud->setPosition(_cameraNode->getPosition() + Vector3(0,0,-1));

}

void StandLatas::endGame(){

  //Pedir Record

  _crosshair.reset();
  _activatorActive = false;
  std::cout <<"End Game" << std::endl;
  _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
  _cameraBody->getBulletRigidBody()->setLinearVelocity(convert(Vector3(0,0,0)));
   _cameraBody->getBulletRigidBody()->setAngularVelocity(convert(Vector3(0,0,0)));
   _cameraBody->getBulletRigidBody()->clearForces();
   // _cameraNode->translate(Vector3(0, -1, 3));
  popState();


}
