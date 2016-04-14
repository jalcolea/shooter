#ifndef BALA_H
#define BALA_H

#include <Ogre.h>
#include "OgreUtil.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletCollisionsShape.h"
#include "OgreBulletDynamicsWorld.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"

#define MESH_BALA "Bala.mesh"

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


class Bala
{
public:
    Bala(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, SceneManager* sceneMgr) : 
      _world(world),
      _direccion(direccion),
      _posicion(posicion),
      _sceneMgr(sceneMgr)
    {
      _direccion = Vector3(0,1,0);
      _posicion = posicion;
      _entBala =  _sceneMgr->createEntity(MESH_BALA);
      _nodoBala = _sceneMgr->createSceneNode();
      _nodoBala->attachObject(_entBala);
      //_nodoBala->scale(0.1,0.1,0.1);
      _sceneMgr->getRootSceneNode()->addChild(_nodoBala);
      //_nodoRobot->showBoundingBox(true);
      _nodoBala->translate(_posicion);
      _nodoBala->setVisible(true);
      _body = new RigidBody(_nombre,_world.get(),COL_BALA,COL_STAND | COL_FLOOR | COL_ROBOT | COL_HIT_ZONE) ;
      _shape = new CylinderCollisionShape(_entBala->getBoundingBox().getHalfSize() * 0.2);
      
    
#ifdef _DEBUG
      _body->showDebugShape(true);
#endif

      _body->setShape(_nodoBala,                     // SceneNode que manejará Bullet
                      _shape,                        // Forma geométrica para manejar colisiones (o eso creo)
                      0.0,                           // Dynamic body restitution
                      1000.0,                        // Dynamic body friction
                      200,                           // Dynamic body mass   
                      Vector3(0,0,0),                // Posicion inicial del shape
                      Quaternion::IDENTITY);         // Orientacion del shape
      
      _body->setLinearVelocity(Vector3(0,0,0));
      _body->enableActiveState();
      
      btTransform transform; //Declaration of the btTransform
      transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
      transform = _body->getBulletRigidBody()->getWorldTransform();
      transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(_posicion + Vector3(0,0.1,0))); //Set the new position/origin
      _body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/
        
    
        
        
    };

    ~Bala();
        
    void shoot(Vector3 & direccion, float fuerza, Vector3 & relPos);


private:
    shared_ptr<DynamicsWorld> _world;
    Vector3 _direccion;
    Vector3 _posicion;
    SceneManager* _sceneMgr;
    string _nombre;
    //AnimationState* _anim;
    SceneNode* _nodoBala;
    Entity* _entBala;
    RigidBody* _body;
    CylinderCollisionShape* _shape;
    unsigned char _speed;


};

#endif // BALA_H
