#ifndef ROBOT_H
#define ROBOT_H

#include <Ogre.h>
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletCollisionsShape.h"
#include "OgreBulletDynamicsWorld.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

enum class TipoRobot
{
    UNKNOWN = -1,
    PEON = 0,
    BOSS = 1
};

class Robot
{
public:
    Robot(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, SceneManager* sceneMgr) : 
          _world(world),
          _direccion(direccion),
          _posicion(posicion),
          _sceneMgr(sceneMgr),
          _anim(nullptr),
          _body(nullptr),
          _shape(nullptr){};
          
    virtual ~Robot();

    virtual TipoRobot getTipo() { return TipoRobot::UNKNOWN; }
    
    virtual void anima(Real deltaT, Real velocidad = 1);
    virtual void mueve(Vector3 direccion, Real deltaT) = 0;
    virtual void setAnim(string nombreAnim, bool loop = false);
    inline void startAnim() { _anim->setEnabled(true); }
    inline void stopAnim() { _anim->setEnabled(false); }
        
protected:
    shared_ptr<DynamicsWorld> _world;
    Vector3 _direccion;
    Vector3 _posicion;
    SceneManager* _sceneMgr;
    string _nombre;
    AnimationState* _anim;
    SceneNode* _nodoRobot;
    Entity* _entRobot;
    RigidBody* _body;
    BoxCollisionShape* _shape;
    unsigned char _vida;
    unsigned char _speed;

private:
    

};

#endif // ROBOT_H
