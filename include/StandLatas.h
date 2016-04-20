#ifndef STANDLATAS_H
#define STANDLATAS_H


#include "Stand.h"
#include "PlayState.h"
#include "StandLatas.h"
#include "Crosshair.h"
#include "OgreOverlayContainer.h"
#include "OgreTextAreaOverlayElement.h"
#include "PlayWidget.h"

class StandLatas:public Stand{


  
  
protected:

  int _numCans;
  int _mousex,_mousey;
  unique_ptr<Crosshair> _crosshair;
  Ogre::RaySceneQuery* _rayScnQuery;
  int _numBolas = 4;
  Ogre::Vector3 calculateDirShoot();
  void shootBall(Ogre::Vector3 dir);
  void reacomodateCamera();

  std::vector<btRigidBody*> _ballBodys;

  std::vector<btRigidBody*> _canBodys;
  SceneNode * _canStack;
  SceneNode * _ballStack;
  bool _loadingShoot;
  double _timeLoadingShoot;
  double _timeWithoutBalls;
  //  Ogre::OverlayContainer* _panel;
  int _puntos;
  //  TextAreaOverlayElement* _textArea;
  PlayWidget * _playWidget;

  
  
  

public:
  void enter();
  void drawCans();
  void drawCan(Vector3 position,string name);
  bool mouseMoved (const OIS::MouseEvent &e);
  bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void checkCollisions();
  double getCollisionForce(btPersistentManifold* maniFold);
  void deleteFallenCans();
  void drawHud();
  void paintBallsHud();
  void endGame();
  void exit();
  

  StandLatas( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world ):Stand(position, sceneMgr,name,world){
    
  }



   
};
#endif
