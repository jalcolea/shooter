#ifndef STAND_H
#define STAND_H

#include <Ogre.h>

using namespace Ogre;
/**
 * Representa un puesto de feria
 */
class Stand {

private:

  Vector3 _position;
  SceneManager* _sceneMgr;

protected:


public:
  Stand( Vector3 position, SceneManager* sceneMgr){
    _position = position;
    _sceneMgr = sceneMgr;
  }
  
  Stand(){};
  virtual void buildGame();
  virtual void startGame(){};

};

#endif
