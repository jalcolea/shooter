
#include "StandLatas.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include <iostream>
#include "Crosshair.h"

using namespace Ogre;
using namespace std;
void StandLatas::enter() {


  reacomodateCamera();
  drawCans();
  
  _crosshair = unique_ptr<Crosshair>(new Crosshair(_sceneMgr,_camera));
  _crosshair.get()->createCrossHairManual("circle-01.png");
  _rayScnQuery =_sceneMgr->createRayQuery(Ogre::Ray(), 0);



}

void StandLatas::drawCan(Vector3 position, string name) {

  SceneNode *stackCanNode = _sceneMgr->getSceneNode("canStack");
  SceneNode *canNode = _sceneMgr->createSceneNode(name);
 
  Entity *canEnt = _sceneMgr->createEntity("lata.mesh");
  canEnt->setQueryFlags(COL_CAN);
  canNode->attachObject(canEnt);
  canNode->setPosition(_activatorPosition + Vector3(0, 2, 2));
  canNode->scale(Vector3(0.2, 0.2, 0.2));
  stackCanNode->addChild(canNode);

  OgreBulletDynamics::RigidBody *_rigidCan = new OgreBulletDynamics::RigidBody(
      name, _world.get(), COL_CAN,
      COL_STAND | COL_FLOOR | COL_CAMERA | COL_CAN | COL_BALL);
  OgreBulletCollisions::CollisionShape *_shapeCan =
      new OgreBulletCollisions::CylinderCollisionShape(Vector3(0.1, 0.21, 0.21),
                                                       Vector3::UNIT_Y);
  _rigidCan->setShape(canNode, _shapeCan, 0, 1, 10, position,
                      Quaternion::IDENTITY);
}

void StandLatas::drawCans() {

  SceneNode *stackCanNode = _sceneMgr->createSceneNode("canStack");
  _sceneMgr->getRootSceneNode()->addChild(stackCanNode);
  Vector3 startPosition = _position + Vector3(0, 2.2, -0.1);

  float gap = 0.24;
  float height = 0.37;
  int numRows = 3;

  for (int i = numRows; i > 0; i--) {

    for (int j = 0; j < i; j++) {

      float x = j * gap;


        x = x + (gap / 2.0f) * (numRows - i);

       std::cout << i << "-" << j << " " << ((gap / 2.0f) * (numRows - i))
		 << " " << j * gap << " " << x <<std::endl ;

      stringstream str;

      str << "Can" << i << j;

      drawCan(startPosition + Vector3(x, (numRows - i) * height, 0), str.str());
    }
  }
}
bool StandLatas::mouseMoved (const OIS::MouseEvent &e)
{ 
    //cout << "Posicion raton: [" << e.state.X.abs << "," << e.state.Y.abs << "] Area del ratón: [" << e.state.width << "," << e.state.height << "]" << endl;

    const size_t& xMouse = e.state.X.abs + 6;// por alguna razón que desconozco el ratón está desplazado -6 unidades
    const size_t& yMouse = e.state.Y.abs + 6;
    
    const size_t& xRel = e.state.X.rel;
    const size_t& yRel = e.state.Y.rel;

    const Ogre::Real& wWindow = e.state.width;
    const Ogre::Real& hWindow = e.state.height;

        _crosshair.get()->setActualHitPoint(Ogre::Real(xMouse/wWindow),Ogre::Real(yMouse/hWindow));
    //_nodeWeapon->lookAt(_nodeWeapon->getPosition().reflect(_crosshair.get()->getActualHitPoint()),Ogre::Node::TS_WORLD);
    
  _mousex = e.state.X.abs;
  _mousey = e.state.Y.abs;
    
    

    return true;
}


bool StandLatas::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
      if (e.state.buttonDown(OIS::MB_Left) == true) {
  shootBall(calculateDirShoot());

}
      return true;

}


Vector3 StandLatas::calculateDirShoot() {
  std::cout << "Disparando" << std::endl;
  Vector3 dirShoot;
  _rayScnQuery->setSortByDistance(true);
  _rayScnQuery->setQueryMask(COL_STAND|COL_CAN);

    Ogre::Ray ray  = _camera->
                       getCameraToViewportRay(_mousex/float(1024),
                                              _mousey/float(768));
    dirShoot = ray.getDirection();
    /*  _rayScnQuery->setRay(ray);
  Ogre::RaySceneQueryResult &result = _rayScnQuery->execute();
  Ogre::RaySceneQueryResult::iterator it = result.begin();

  for ( ; it != result.end(); it++) {
    if ( it->movable ) {
  std::cout << "Nombre ------->" <<it->movable->getName() << " Nombre Nodo ---->" << it->movable->getParentNode()->getName() << std::endl;
 Vector3 pos = it->movable->getParentNode()->getPosition();
 dirShoot = _cameraNode->getPosition() - pos;

    }
    }*/
  std::cout << "Direccion "<< dirShoot << std::endl;
  return dirShoot*10;
}


 void StandLatas::shootBall(Vector3 dir){

  std::cout << "Disparando" << numBolas << std::endl;
  stringstream str;
  str << "ball" << numBolas;
  string name = str.str();
  SceneNode *ballNode = _sceneMgr->createSceneNode(name);
  Entity *ballEnt = _sceneMgr->createEntity("ball.mesh");
  ballNode->attachObject(ballEnt);
  OgreBulletDynamics::RigidBody *_rigidBall = new OgreBulletDynamics::RigidBody(
      name, _world.get(), COL_BALL,
      COL_STAND | COL_FLOOR | COL_CAMERA | COL_CAN | COL_BALL);
  OgreBulletCollisions::CollisionShape *_shapeBall =
    new OgreBulletCollisions::SphereCollisionShape(0.1);
  _rigidBall->setShape(ballNode, _shapeBall, 0, 1, 10, _cameraNode->getPosition()+ Vector3(0,0,-2),
                      Quaternion::IDENTITY);
  _sceneMgr->getRootSceneNode()->addChild(ballNode);
  //_rigidBall->getBulletRigidBody()->applyCentralForce(convert(dir));
  _rigidBall->getBulletRigidBody()->setLinearVelocity(convert(dir));
  numBolas--;

}

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
