#ifndef STAND_FACTORY
#define STAND_FACTORY

#include "Stand.h"

//Enumeración del tipo de puestos
enum StandTypes {LATAS,PORTERO};

//Clase que pinta la feria. Patrón Factoria para generar puestos
class StandFactory {
private:
  StandTypes _standsConf[2];
  std::vector<std::unique_ptr<Stand>> _stands;

public:

  StandFactory():_standsConf {LATAS,LATAS}{
  };
  /**
   * Construye un Puesto
   */
  std::unique_ptr<Stand> buildStand(StandTypes type,Vector3 position, SceneManager* sceneMgr);

  /***
   * Construye la feria
   */ 
   void buildFestival(SceneManager* sceneMgr);

};
  


#endif
