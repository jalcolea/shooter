#ifndef STANDLATAS_H
#define STANDLATAS_H


#include "Stand.h"
#include "PlayState.h"

class StandLatas:public Stand{


  
  
protected:



  void reacomodateCamera();

public:
  void enter();
  void drawCans();
  void drawCan(Vector3 position,string name);
  StandLatas( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world ):Stand(position, sceneMgr,name,world){
    
  }



   
};
#endif
