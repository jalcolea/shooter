#ifndef ROBOTPEON_H
#define ROBOTPEON_H

#include "Robot.h" // Base class: Robot
#include "OgreUtil.h"

#define NOMBRE_ROBOT_PEON "PEON"
#define MESH_PEON "RobotMinion.mesh"
#define VELOCIDAD_PEON 5

class RobotPeon : public Robot
{
public:

    RobotPeon(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, SceneManager* sceneMgr, string nombre) : Robot(world,direccion,posicion,sceneMgr)
    {
      _direccion = Vector3(0,1,0);
      _posicion = posicion;
      _nombre = nombre;
      _entRobot =  _sceneMgr->createEntity(_nombre+"ent",MESH_PEON);
      _nodoRobot = _sceneMgr->createSceneNode(_nombre+"nodo");
      _nodoRobot->attachObject(_entRobot);
      _nodoRobot->scale(0.2,0.2,0.2);
      _sceneMgr->getRootSceneNode()->addChild(_nodoRobot);
      _nodoRobot->translate(_posicion);
      _nodoRobot->setVisible(true);
      _body = new RigidBody(_nombre,_world.get(),COL_ROBOT,COL_STAND | COL_FLOOR);
      cout << "center " << _entRobot->getBoundingBox().getCenter() << endl;
      _shape = new BoxCollisionShape(_entRobot->getBoundingBox().getSize() * 0.04);
      cout << _entRobot->getBoundingBox().getSize() << endl;
      //_shape = OgreBulletCollisions::AnimatedMeshToShapeConverter()
    
#ifdef _DEBUG
      _body->showDebugShape(true);
#endif

      _body->setShape(_nodoRobot,                    // SceneNode que manejará Bullet
                      _shape,                        // Forma geométrica para manejar colisiones (o eso creo)
                      0.0,                           // Dynamic body restitution
                      1000.0,                        // Dynamic body friction
                      100,                           // Dynamic body mass   
                      Vector3(0,0,0),               // Posicion inicial del shape
                      Quaternion::IDENTITY);         // Orientacion del shape
      
      _body->setLinearVelocity(Vector3(0,0,0) * VELOCIDAD_PEON);
      _body->enableActiveState();
      
      btTransform transform; //Declaration of the btTransform
      transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
      transform = _body->getBulletRigidBody()->getWorldTransform();
      transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(_posicion)); //Set the new position/origin
      _body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/

    };
    
    ~RobotPeon();

    virtual void anima(string nombreAnim, Real deltaT);
    virtual void mueve(Vector3 direccion, Real deltaT);
    
    TipoRobot getTipo() override;
    
private:

};

#endif // ROBOTPEON_H
