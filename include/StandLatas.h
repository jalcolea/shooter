#ifndef STANDLATAS_H
#define STANDLATAS_H


#include "Stand.h"

class StandLatas:public Stand{


  
protected:

  void startGame();

public:

  StandLatas( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world ):Stand(position, sceneMgr,name,world){
    
  }


   
};
#endif
