
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
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "OgreBulletDynamicsWorld.h"
#include "StandFactory.h"
#include <string>
#include <vector>
#include "records.h"
#include "OgreUtil.h"
#include <ctime>
#include "OgreOverlayManager.h"
#include "OgreOverlaySystem.h"

#define CAMSPEED 20
#define CAMROTATESPEED 0.1
template <> PlayState *Ogre::Singleton<PlayState>::msSingleton = 0;

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

void PlayState::enter() {
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");

  _camera = _sceneMgr->getCamera("IntroCamera");
  // _camera->setPosition(Vector3(0,0,5));
  _cameraNode = _sceneMgr->createSceneNode("nodeCamera");
  _camera->setPosition(Vector3::ZERO);
  _cameraNode->attachObject(_camera);
  //_cameraNode->setPosition(Vector3(0, 2, -10));

  _sceneMgr->getRootSceneNode()->addChild(_cameraNode);

  _exitGame = false;
  paused = false;
  
  _deltaT = 0;
    std::cout << "OverlayManager " <<  new Ogre::OverlayManager() << "PlayState" << std::endl;
  _sceneMgr->addRenderQueueListener(new Ogre::OverlaySystem());


  // Activar Bullet
  AxisAlignedBox boundBox =
      AxisAlignedBox(Ogre::Vector3(-10000, -10000, -10000),
                     Ogre::Vector3(10000, 10000, 10000));
  _world = shared_ptr<OgreBulletDynamics::DynamicsWorld>(new DynamicsWorld(
      _sceneMgr, boundBox, Vector3(0, -9.81, 0), true, true, 15000));


  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  //  _debugDrawer->setDrawWireframe(true);
  SceneNode *node = _sceneMgr->getRootSceneNode()->createChildSceneNode(
      "debugNode", Vector3::ZERO);
  node->attachObject(static_cast<SimpleRenderable *>(_debugDrawer));
  _world.get()->setDebugDrawer(_debugDrawer);
  //_world.get()->setShowDebugShapes(true);
  _cameraBody = new RigidBody("cameraBody", _world.get(), COL_CAMERA,
                              COL_ACTIVATOR | COL_STAND | COL_FLOOR);

  _cameraShape = new SphereCollisionShape(2);
  _cameraBody->setShape(_cameraNode, _cameraShape, 0, 0, 0.5,
                        Vector3(0, 2, 10), Quaternion::IDENTITY);
  _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
  createScene();
}

void PlayState::exit() { destroyMyGui(); }

void PlayState::pause() { paused = true; }

void PlayState::resume() { paused = false; }

bool PlayState::frameStarted(const Ogre::FrameEvent &evt) {

  _deltaT = evt.timeSinceLastFrame;
  _world.get()->stepSimulation(evt.timeSinceLastFrame);

  moveCamera();

  checkCollisions();

  if (keysArePressed[OIS::KC_ESCAPE]) {
    return false;
  }
  return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent &evt) {
  if (keysArePressed[OIS::KC_ESCAPE]) {
    return false;
  }
  return true;
}

bool PlayState::keyPressed(const OIS::KeyEvent &e) {

  std::cout << "presionando " << std::endl;
  keysArePressed.erase(e.key);
  keysArePressed[e.key] = true;
  if (e.key == OIS::KC_P) {
    // pushState(PauseState::getSingletonPtr());
  } else if (e.key == OIS::KC_G) {
    //    game_over();
  } else if (e.key == OIS::KC_W) {
    //    win();
  }
 
  return true;
}
/**
 * Move the camera around the world if arrows are pressed
 */
void PlayState::moveCamera() {

  if (keysArePressed[OIS::KC_UP]) {
    btQuaternion quat = _cameraBody->getBulletRigidBody()->getOrientation();
    btVector3 aux(0, 0, -CAMSPEED);
    aux = aux.rotate(quat.getAxis(), quat.getAngle());
    _cameraBody->getBulletRigidBody()->setLinearVelocity(aux);

  } else if (keysArePressed[OIS::KC_DOWN]) {
    btQuaternion quat = _cameraBody->getBulletRigidBody()->getOrientation();
    btVector3 aux(0, 0, CAMSPEED);
    aux = aux.rotate(quat.getAxis(), quat.getAngle());
    _cameraBody->getBulletRigidBody()->setLinearVelocity(aux);

  } else if (keysArePressed[OIS::KC_RIGHT]) {
    btQuaternion quat = _cameraBody->getBulletRigidBody()->getOrientation();
    btVector3 aux(CAMSPEED, 0, 0);
    aux = aux.rotate(quat.getAxis(), quat.getAngle());
    _cameraBody->getBulletRigidBody()->setLinearVelocity(aux);

  } else if (keysArePressed[OIS::KC_LEFT]) {
    btQuaternion quat = _cameraBody->getBulletRigidBody()->getOrientation();
    btVector3 aux(-CAMSPEED, 0, 0);
    aux = aux.rotate(quat.getAxis(), quat.getAngle());

    _cameraBody->getBulletRigidBody()->setLinearVelocity(aux);
  }
}

bool PlayState::keyReleased(const OIS::KeyEvent &e) {
  _cameraBody->setLinearVelocity(Ogre::Vector3(0, 0, 0));
  keysArePressed.erase(e.key);
  keysArePressed[e.key] = false;
  return true;
}

void PlayState::createFloor() {
  SceneNode *floorNode = _sceneMgr->createSceneNode("floor");
  Plane planeFloor;
  planeFloor.normal = Vector3(0, 1, 0);
  planeFloor.d = 2;
  MeshManager::getSingleton().createPlane(
      "FloorPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      planeFloor, 200000, 200000, 20, 20, true, 1, 9000, 9000, Vector3::UNIT_Z);
  Entity *entFloor = _sceneMgr->createEntity("floor", "FloorPlane");
  entFloor->setCastShadows(false);
  entFloor->setMaterialName("floor");
  floorNode->attachObject(entFloor);
  _sceneMgr->getRootSceneNode()->addChild(floorNode);

  _floorShape = new StaticPlaneCollisionShape(Ogre::Vector3(0, 1, 0), 0);

  _floorBody = new RigidBody("rigidBodyPlane", _world.get(), COL_FLOOR,
                             COL_CAMERA | COL_ACTIVATOR);
  _floorBody->setStaticShape(_floorShape, 0.5, 0.8);
  floorNode->setPosition(Vector3(0, 2, 0));
  btCollisionShape *floorShape = _floorShape->getBulletShape();

}

bool PlayState::mouseMoved(const OIS::MouseEvent &e) {

  btTransform transform;
  transform.setIdentity();
  transform.setOrigin(
      _cameraBody->getBulletRigidBody()->getWorldTransform().getOrigin());

  Ogre::Quaternion camQuat(0, 0, e.state.X.abs * -0.1, 0);

  btQuaternion quat;

  quat.setEuler(e.state.X.abs * -0.01, 0, 0);

  transform.setRotation(quat);

  //_cameraBody->setOrientation(camQuat);

  _cameraBody->getBulletRigidBody()->setWorldTransform(transform);
  //_cameraNode->setOrientation(camQuat);

  /*
if (e.state.X.rel > 100 || e.state.X.rel < 100) {
std::cout << e.state.X.rel << std::endl;
_cameraBody->setAngularVelocity(
 Vector3(0, e.state.X.rel * -CAMROTATESPEED, 0));
 }*/
  //  _cameraNode->yaw(Radian(Degree(-CAMROTATESPEED * e.state.X.rel)),
  //             Node::TS_LOCAL);
  //  _cameraNode->pitch(Radian(Degree(-CAMROTATESPEED * e.state.Y.rel)),
  //  Node::TS_LOCAL);
  return true;
}

bool PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
  return true;
}

bool PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
  return true;
}

PlayState *PlayState::getSingletonPtr() { return msSingleton; }

PlayState &PlayState::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}

PlayState::~PlayState() {}

void PlayState::createScene() {

  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  _sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
  _sceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
  _sceneMgr->setSkyBox(true, "skybox");
  createLight();
  createMyGui();
  createFloor();
  StandFactory factory;
  _stands = factory.buildFestival(_sceneMgr, _world);
}

void PlayState::createMyGui() {}

void PlayState::destroyMyGui() {}

void PlayState::createLight()

{
  _sceneMgr->setShadowTextureCount(2);
  _sceneMgr->setShadowTextureSize(512);
  Light *light = _sceneMgr->createLight("Light1");
  light->setPosition(0, 12, 0);
  light->setType(Light::LT_SPOTLIGHT);
  light->setDirection(Vector3(0, -1, 0));
  light->setSpotlightInnerAngle(Degree(60.0f));
  light->setSpotlightOuterAngle(Degree(80.0f));
  light->setSpotlightFalloff(0.0f);
  light->setCastShadows(true);
}

void PlayState::win() {}

void PlayState::game_over() {}

void PlayState::set_lives(int lives) { this->lives = lives; }

int PlayState::get_lives() { return lives; }

void PlayState::set_score(int score) { this->score = score; }

int PlayState::get_score() { return score; }

void PlayState::handleCollision(btCollisionObject *body0,
                                btCollisionObject *body1) {}

bool PlayState::WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e) {
  return true;
}
bool PlayState::WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e) {
  return true;
}
bool PlayState::WiimoteIRMove(const wiimWrapper::WiimoteEvent &e) {
  return true;
}

void PlayState::checkCollisions() {
  btCollisionShape *cameraShape = _cameraShape->getBulletShape();
  btCollisionShape *floorShape = _floorShape->getBulletShape();
  btCollisionWorld *collisionWorld = _world.get()->getBulletCollisionWorld();
  btDynamicsWorld *dynamicWorld = _world.get()->getBulletDynamicsWorld();

  int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
  bool collide = false;
  for (int i = 0; i < numManifolds; i++) {
    btPersistentManifold *contactManifold =
        collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject *obA = (btCollisionObject *)contactManifold->getBody0();
    btCollisionObject *obB = (btCollisionObject *)contactManifold->getBody1();

    btCollisionShape *shapeA = obA->getCollisionShape();
    btCollisionShape *shapeB = obB->getCollisionShape();

    if (shapeA == cameraShape || shapeB == cameraShape) {
      btCollisionShape *other = (shapeA == cameraShape ? shapeB : shapeA);

      if (other != floorShape) {

        vectorStand::iterator it = std::find_if(
            _stands.begin(), _stands.end(),
            [other](unique_ptr<Stand> &stand) -> bool {
              return stand.get()->getActivatorShape()->getBulletShape() ==
		other && stand.get()->getActivatorActive();
            });
        if (it != _stands.end()) {
 	  _cameraBody->setLinearVelocity(Vector3(0,0,0));
	  pushState((*it).get());

        }

        /*

        std::cout << floorShape <<" Floor"
                      << obA->getCollisionShape()->getName() << std::endl;



        std::cout << other <<" Collision Body A: "
                      << obA->getCollisionShape()->getName() << std::endl;

        std::cout << floorShape<< " Collision Body B: "
                      << obB->getCollisionShape()->getName() << std::endl;

        */
      }
    }
  }
}
