#ifndef TESTWIIMOTESTATE_H
#define TESTWIIMOTESTATE_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <string>
#include "GameState.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include <vector>
#include "OgreUtil.h"
#include "Wiimote.h"
#include "Crosshair.h"
#include "sounds.h"

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

class testwiimoteState : public Ogre::Singleton<testwiimoteState>, public GameState
{
    
public:
    testwiimoteState() /*: _mouseray(new Ogre::Ray)*/ {};
    ~testwiimoteState(){};

    void enter ();
    void exit();
    void pause ();
    void resume ();

    bool keyPressed (const OIS::KeyEvent &e);
    bool keyReleased (const OIS::KeyEvent &e);

    bool mouseMoved (const OIS::MouseEvent &e);
    bool mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

    bool frameStarted (const Ogre::FrameEvent& evt);
    bool frameEnded (const Ogre::FrameEvent& evt);
    
/* WIIMOTE *********************************************************************/  
    bool WiimoteButtonDown(const wiimWrapper::WiimoteEvent &e);
    bool WiimoteButtonUp(const wiimWrapper::WiimoteEvent &e);
    bool WiimoteIRMove(const wiimWrapper::WiimoteEvent &e);
/*******************************************************************************/  


    // Heredados de Ogre::Singleton.
    static testwiimoteState& getSingleton ();
    static testwiimoteState* getSingletonPtr ();

protected:
    Ogre::Root* _root;
    Ogre::SceneManager* _sceneMgr;
    Ogre::Viewport* _viewport;
    Ogre::Camera* _camera;
    MyGUI::VectorWidgetPtr layout;
    
    Ogre::AnimationState* _animTiro;
    bool _exitGame;
    int lives;
    int score;
    bool paused;
    Ogre::Real _deltaT;
    unique_ptr<Crosshair> _crosshair;

    void ActivaPuerta(AccionPuerta accion);
    
private:
  void createMyGui();
  void destroyMyGui();
  void createScene();
  void createLight();
  std::unique_ptr<Ogre::Ray> _mouseray;// {new Ogre::Ray};
  Ogre::Plane _plane;
  Ogre::SceneNode* _nodeCrosshair;
  Ogre::SceneNode* _nodeWeapon;
  Ogre::SceneNode* _nodeStand;
  Ogre::SceneNode* _nodeEscudo;
  Ogre::AnimationState* _animPuerta;
  Ogre::Entity* _entStand;
  AccionPuerta _sentidoAccionPuerta;
  
};

#endif // TESTWIIMOTESTATE_H

