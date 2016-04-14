#ifndef STANDROBOTS_H
#define STANDROBOTS_H

#include "Stand.h"
#include <Ogre.h>
#include <OIS/OIS.h>
#include <string>
#include "GameState.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletCollisionsShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "OgreBulletCollisionsRay.h"


#include <vector>
#include "OgreUtil.h"
#include "Wiimote.h"
#include "Crosshair.h"
#include "sounds.h"
#include "RobotFactory.h"
#include "Bala.h"

#define NOMBRE_STAND_ROBOTS "StandRobots"

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;
using namespace wiimWrapper;

enum class AccionPuerta
{
    CERRAR = -1,
    PARAR = 0,
    ABRIR = 1
};


class StandRobots : public Stand //, public Ogre::Singleton<StandRobots>
{
public:
    StandRobots( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world ):Stand(position, sceneMgr,name,world){}
    virtual ~StandRobots();

    void enter() override;
    void exit() override;
    void pause() override;
    void resume() override;

    bool keyPressed (const OIS::KeyEvent &e) override;
    bool keyReleased (const OIS::KeyEvent &e) override;

    bool mouseMoved (const OIS::MouseEvent &e) override;
    bool mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id) override;
    bool mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id) override;

    bool frameStarted (const Ogre::FrameEvent& evt) override;
    bool frameEnded (const Ogre::FrameEvent& evt) override;
    
/* WIIMOTE *********************************************************************/  
    bool WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e) override;
    bool WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e) override;
    bool WiimoteIRMove(const wiimWrapper::WiimoteEvent &e) override;
/*******************************************************************************/  

    virtual void buildGame();// override;


    // Heredados de Ogre::Singleton.
//    static StandRobots& getSingleton ();
//    static StandRobots* getSingletonPtr ();

protected:
//    Ogre::Root* _root;
//    Ogre::SceneManager* _sceneMgr;
    Ogre::Viewport* _viewport;
//    Ogre::Camera* _camera;
    MyGUI::VectorWidgetPtr layout;
    
    Ogre::AnimationState* _animTiro;
//    bool _exitGame;
    int lives;
//    int score;
//    bool paused;
//    Ogre::Real _deltaT;
    unique_ptr<Crosshair> _crosshair;

    void ActivaPuerta(AccionPuerta accion);
    
private:
  void createMyGui();
  void destroyMyGui();
  void createScene();
  void createLight();
//  void createFloor();
  RigidBody* pickBody (Vector3 &puntoColision, Ray &rayo, float x, float y);
  std::unique_ptr<Ogre::Ray> _mouseray;// {new Ogre::Ray};
  RobotFactory _robFact;
  Ogre::Plane _plane; // Para manejar el crosshair
  Ogre::SceneNode* _nodeCrosshair;
  Ogre::SceneNode* _nodeWeapon;
  Ogre::SceneNode* _nodeStand;
  Ogre::SceneNode* _nodeEscudo;
  Ogre::SceneNode* _nodePuerta;
  Ogre::AnimationState* _animPuerta;
  Ogre::Entity* _entStand;
  Ogre::Entity* _entPuerta;
  AccionPuerta _sentidoAccionPuerta;
//  shared_ptr<DynamicsWorld> _world;
//  OgreBulletDynamics::RigidBody* _rigidStand;
//  OgreBulletCollisions::CollisionShape* _shapeStand;
//  string _name = NOMBRE_STAND_ROBOTS;
  Bala* bala;
  //OgreBulletCollisions::TriangleMeshCollisionShape* _shapeStand;
  //OgreBulletCollisions::ConvexHullCollisionShape* _shapeStand;

  DebugDrawer* _debugDrawer;

};

#endif // STANDROBOTS_H
