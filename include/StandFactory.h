#ifndef STAND_FACTORY
#define STAND_FACTORY

#include "Stand.h"
#include "OgreBulletDynamicsWorld.h"
#include "PlayState.h"

using namespace std;
//Enumeración del tipo de puestos
enum StandTypes {LATAS,PORTERO};

//Clase que pinta la feria. Patrón Factoria para generar puestos
class StandFactory {
private:
  StandTypes _standsConf[1];
  

public:

  StandFactory():_standsConf {LATAS}{
  };
  /**
   * Construye un Puesto
   */
  std::unique_ptr<Stand> buildStand(StandTypes type,Vector3 position, SceneManager* sceneMgr,shared_ptr<OgreBulletDynamics::DynamicsWorld> world);

  /***
   * Construye la feria
   */ 
  vectorStand  buildFestival(SceneManager* sceneMgr,shared_ptr<OgreBulletDynamics::DynamicsWorld> world);

};
  


#endif
