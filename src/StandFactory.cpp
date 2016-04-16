#include "StandFactory.h"
#include "StandLatas.h"
#include "StandRobots.h"

std::unique_ptr<Stand>
StandFactory::buildStand(StandTypes type, Vector3 position,
                         SceneManager *sceneMgr,
                         shared_ptr<OgreBulletDynamics::DynamicsWorld> world) {

  std::unique_ptr<Stand> stand(
      new StandLatas(position, sceneMgr, "Puesto", world));

  stand->buildGame();
  /*    switch(type){
    case LATAS: return stand;
    break;
    //  case PORTER: return StandPortero();
    //    break;
    }*/

  return stand;
}

vectorStand StandFactory::buildFestival(
    SceneManager *sceneMgr,
    shared_ptr<OgreBulletDynamics::DynamicsWorld> world) {

  vectorStand stands;
  float x = 0, y = 0, z = 0;
  std::cout << "creando festival" << std::endl;
  for (StandTypes type : _standsConf) {
    std::cout << "creando puesto" << std::endl;
    Vector3 position(x, y, z);

    stands.push_back(std::move(buildStand(type, position, sceneMgr, world)));

    x = x + 10;
  }
  
  stands.push_back(unique_ptr<Stand>(new StandRobots(Vector3(x,0,0),sceneMgr,"StandRobots", world)));
  stands.back()->buildGame();
  
  return stands;
}
