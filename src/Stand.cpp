#include "Stand.h"

void Stand::buildGame(){

  StaticGeometry *stage = _sceneMgr->createStaticGeometry("Stand1");
  Entity* entStand = _sceneMgr->createEntity("puesto.mesh");

  entStand->setCastShadows(true);
  stage->addEntity(entStand, Vector3(_position));
  stage->build();


}
