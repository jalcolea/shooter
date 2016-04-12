#include "StandFactory.h"
#include "StandLatas.h"

std::unique_ptr<Stand> StandFactory::buildStand(StandTypes type,Vector3 position, SceneManager* sceneMgr,shared_ptr<OgreBulletDynamics::DynamicsWorld> world){

  std::unique_ptr<Stand> stand(new StandLatas(position,sceneMgr,"Puesto",world));
  
  stand->buildGame();
  /*    switch(type){
    case LATAS: return stand;
    break;
    //  case PORTER: return StandPortero();
    //    break;
    }*/
  this->_stands.push_back(std::move(stand));
  return stand;
}

void StandFactory::buildFestival(SceneManager* sceneMgr,shared_ptr<OgreBulletDynamics::DynamicsWorld> world) {

  float x=0,y=0,z=0;
  std::cout << "creando festival" << std::endl;
  for(StandTypes type:_standsConf){
      std::cout << "creando puesto" << std::endl;
    Vector3 position(x,y,z);
     buildStand(type,position,sceneMgr, world);

    x= x + 10;
  }


}
