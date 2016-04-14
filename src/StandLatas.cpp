#include "StandLatas.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include <iostream>

using namespace Ogre;
using namespace std;
void StandLatas::enter() {

  std::cout << "Empezando juego de las latas" << std::endl;
  reacomodateCamera();
  drawCans();
}

void StandLatas::drawCan(Vector3 position, string name) {

  std::cout << position << std::endl;

  SceneNode *stackCanNode = _sceneMgr->getSceneNode("canStack");
  SceneNode *canNode = _sceneMgr->createSceneNode(name);
  Entity *canEnt = _sceneMgr->createEntity("lata.mesh");
  canNode->attachObject(canEnt);
  canNode->setPosition(_activatorPosition + Vector3(0, 2, 2));
  canNode->scale(Vector3(0.2, 0.2, 0.2));
  stackCanNode->addChild(canNode);

  OgreBulletDynamics::RigidBody *_rigidCan = new OgreBulletDynamics::RigidBody(
      name, _world.get(), COL_CAN, COL_STAND | COL_FLOOR | COL_CAMERA | COL_CAN | COL_BALL);
  OgreBulletCollisions::CollisionShape *_shapeCan =
      new OgreBulletCollisions::CylinderCollisionShape(Vector3(0.1, 0.2, 0.2),
                                                       Vector3::UNIT_Y);
  _rigidCan->setShape(canNode, _shapeCan, 0, 1, 10,
                      position, Quaternion::IDENTITY);
}

void StandLatas::drawCans() {

  SceneNode *stackCanNode = _sceneMgr->createSceneNode("canStack");
  _sceneMgr->getRootSceneNode()->addChild(stackCanNode);
  Vector3 startPosition = _position + Vector3(0, 2.2, -0.5);

  float gap = 0.25;
  float height = 0.5;
  int numRows = 2;

  for (int i = numRows; i > 0; i--) {


    for (int j = 0; j < i; j++) {

  float x = j *gap;
  if(i%2 != 0){
  x=x+ gap/2;
}
      stringstream str;
      str << "can" << i << j;
      std::cout << "pintando lata en " << (j*gap) << std::endl;
      drawCan(startPosition + Vector3(x, (numRows - i) * height, 0),
              str.str());
    }
  }
}

void StandLatas::reacomodateCamera() {

  _cameraBody->setOrientation(
      Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));

  _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_SIMULATION);
  /*    btTransform bt = _cameraBody->getBulletRigidBody()->getWorldTransform();
  bt.setOrigin(convert(_activatorPosition-Vector3(0,0,-10) ));
  _cameraBody->getBulletRigidBody()->setWorldTransform(bt);
  _cameraNode->translate(_activatorPosition-Vector3(0,0,2));*/
  //_cameraNode->translate(Vector3(0,0,-5));
}
