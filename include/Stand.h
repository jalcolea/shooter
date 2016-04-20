#ifndef STAND_H
#define STAND_H

#include <Ogre.h>
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletCollisionsShape.h"
#include "OgreBulletDynamicsWorld.h"
#include "GameState.h"

using namespace Ogre;
using namespace std;
/**
 * Representa un puesto de feria
 */
class Stand :public  GameState{

private:
  Stand(){};

protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Camera* _camera;
  Ogre::SceneNode* _cameraNode;
  OgreBulletDynamics::RigidBody* _cameraBody;
  std::map<OIS::KeyCode,bool> keysArePressed;

  bool _exitGame;
  int score;
  bool paused;
  Ogre::Real _deltaT;

  Vector3 _position;
  std::string _name;
  SceneNode * _activator;
  Vector3 _activatorPosition;
  void buildActivator();
  OgreBulletDynamics::RigidBody *_rigidStand;
  OgreBulletCollisions::CollisionShape *_shapeStand;
  shared_ptr<OgreBulletDynamics::DynamicsWorld>_world;
  OgreBulletDynamics::RigidBody *_rigidActivator;
  OgreBulletCollisions::CollisionShape *_shapeActivator;
  bool  _activatorActive = true;



  


public:

  Stand( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world);

  

  inline OgreBulletCollisions::CollisionShape* getActivatorShape(){return _shapeActivator;};
  inline  bool getActivatorActive(){return _activatorActive;};
  inline Vector3 getActivatorPosition(){return _activatorPosition;};
  virtual void checkCollisions(){};
  virtual void buildGame();
  virtual void startGame(){};
  
  virtual void enter ();
  virtual void exit();
  virtual void pause ();
  virtual void resume ();

  virtual bool keyPressed (const OIS::KeyEvent &e);
  virtual bool keyReleased (const OIS::KeyEvent &e);

  virtual bool mouseMoved (const OIS::MouseEvent &e);
  virtual bool mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  virtual bool mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  virtual bool frameStarted (const Ogre::FrameEvent& evt);
  virtual bool frameEnded (const Ogre::FrameEvent& evt);
  
/* WIIMOTE *********************************************************************/  
    virtual bool WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e);
    virtual bool WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e);
    virtual bool WiimoteIRMove(const wiimWrapper::WiimoteEvent &e);
/*******************************************************************************/  


};

#endif
