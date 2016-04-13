#ifndef STAND_H
#define STAND_H

#include <Ogre.h>
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletCollisionsShape.h"
#include "OgreBulletDynamicsWorld.h"

using namespace Ogre;
using namespace std;
/**
 * Representa un puesto de feria
 */
class Stand {

private:
  Stand(){};

protected:
  Vector3 _position;
  SceneManager* _sceneMgr;
  std::string _name;
  SceneNode * _activator;
  Vector3 _activatorPosition;
  void buildActivator();
  OgreBulletDynamics::RigidBody *_rigidStand;
  OgreBulletCollisions::CollisionShape *_shapeStand;
  shared_ptr<OgreBulletDynamics::DynamicsWorld>_world;
  OgreBulletDynamics::RigidBody *_rigidActivator;
  OgreBulletCollisions::CollisionShape *_shapeActivator;




public:
  Stand( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world);

  

  virtual void buildGame();
  virtual void startGame(){};

};

#endif
