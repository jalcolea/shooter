#ifndef STANDLATAS_H
#define STANDLATAS_H


#include "Stand.h"

class StandLatas:public Stand{


  
protected:

  void startGame();

public:
  StandLatas( Vector3 position, SceneManager* sceneMgr);
   
};
#endif
