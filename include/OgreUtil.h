#ifndef OGREUTIL_H
#define OGREUTIL_H

#include "Ogre.h"

using namespace Ogre;
enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_FLOOR = 2, 
    COL_ACTIVATOR =4, 
    COL_STAND = 8, 
    COL_CAMERA = 16,
    COL_CAN = 32,
    COL_BALL = 64
};





class OgreUtil {



 public:

  static void destroyAllAttachedMovableObjects( SceneNode* node );
  static void destroySceneNode( Ogre::SceneNode* node );

};

#endif
