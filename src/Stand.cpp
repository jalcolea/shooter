#include "Stand.h"
#include <iostream>
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreUtil.h"



Stand::Stand( Vector3 position, SceneManager* sceneMgr, std::string name,shared_ptr<OgreBulletDynamics::DynamicsWorld> world){
    _position = position;
    _sceneMgr = sceneMgr;
    _name = name;
    _activatorPosition = position + Vector3(0,0,4);
    _world= world;
}

void Stand::buildGame(){

  
  StaticGeometry *stage = _sceneMgr->createStaticGeometry( _name);
  Entity* entStand = _sceneMgr->createEntity("puesto.mesh");
  //Asociar forma y cuerpo rígido
  OgreBulletCollisions::StaticMeshToShapeConverter trimeshConverter = OgreBulletCollisions::StaticMeshToShapeConverter(entStand);
  _shapeStand = trimeshConverter.createTrimesh();
  _rigidStand = new OgreBulletDynamics::RigidBody(_name, _world.get(), COL_STAND, COL_CAMERA | COL_FLOOR);
  _rigidStand->setStaticShape(_shapeStand, 1, 1, _position, Quaternion::IDENTITY);

  entStand->setCastShadows(true);
  stage->addEntity(entStand, Vector3(_position));
  stage->build();
  buildActivator();

}


void Stand::buildActivator(){

  std::stringstream str;
  str << "Activator" << _name;
  _activator = _sceneMgr->createSceneNode(str.str());
  Entity* entActivator = _sceneMgr->createEntity("Activador.mesh");
  _activator->attachObject(entActivator);

  _activator->setPosition(_activatorPosition);
  _sceneMgr->getRootSceneNode()->addChild(_activator);
  _rigidActivator = new OgreBulletDynamics::RigidBody(str.str(), _world.get(),COL_ACTIVATOR, COL_CAMERA | COL_FLOOR);
  _shapeActivator = new OgreBulletCollisions::CylinderCollisionShape(Vector3(1,0.2,0.2),Vector3::UNIT_Y);
  _rigidActivator->setShape(_activator, _shapeActivator, 1,1,100,_activatorPosition,Quaternion::IDENTITY);
  
  



}
