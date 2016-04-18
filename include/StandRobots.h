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
#define CADENCIA_DE_TIRO 0.25
#define OLEADAS  3
#define NUM_ROBOTS_INICIAL 6
#define RETRASO_INICIO 3.0;


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

    virtual void buildGame() override;


    // Heredados de Ogre::Singleton.
//    static StandRobots& getSingleton ();
//    static StandRobots* getSingletonPtr ();

protected:
    Ogre::Viewport* _viewport;
    MyGUI::VectorWidgetPtr layout;
    Ogre::AnimationState* _animTiro;
    int lives;
    int _xMouse,_yMouse;
    unique_ptr<Crosshair> _crosshair;
    std::vector<btRigidBody*> _robotBodys;
    void ActivaPuerta(AccionPuerta accion);
    void reacomodateCamera();
    void resetCamera();
    Vector3 CalculaDireccionTiro();
    void checkCollisions();
    
private:
  void createMyGui();
  void destroyMyGui();
  void createScene();
  void createLight();
  void createOleada();
  RigidBody* pickBody (Vector3 &puntoColision, Ray &rayo, float x, float y);
  RigidBody* _bodyEscudo;
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
  Bala* bala;
  size_t _numBalas;
  Real _lapsusTime;
  Real _retrasoInicio;
  size_t _oleada;
  size_t _numRobots;
  bool _inicio;
  


  //DebugDrawer* _debugDrawer;

};

#endif // STANDROBOTS_H
