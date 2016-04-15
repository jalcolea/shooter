#ifndef STANDLATAS_H
#define STANDLATAS_H


#include "Stand.h"
#include "PlayState.h"
#include "Crosshair.h"

class StandLatas:public Stand{


  
  
protected:

  int _mousex,_mousey;
  unique_ptr<Crosshair> _crosshair;
  Ogre::RaySceneQuery* _rayScnQuery;
  int numBolas = 25;
  Ogre::Vector3 calculateDirShoot();
  void shootBall(Ogre::Vector3 dir);
  void reacomodateCamera();
  

public:
  void enter();
  void drawCans();
  void drawCan(Vector3 position,string name);
  bool mouseMoved (const OIS::MouseEvent &e);
  bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

  StandLatas( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world ):Stand(position, sceneMgr,name,world){
    
  }



   
};
#endif
