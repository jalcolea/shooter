#include "RobotFactory.h"

RobotFactory::~RobotFactory()
{
}

unique_ptr<Robot> RobotFactory::createOneRobot(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, TipoRobot tipo, SceneManager* sceneMgr, string nombre)
{
    switch (tipo)
    {
        case TipoRobot::BOSS:     return unique_ptr<Robot>(new RobotBoss(world,direccion,posicion,sceneMgr,nombre));
        case TipoRobot::PEON:     return unique_ptr<Robot>(new RobotPeon(world,direccion,posicion,sceneMgr,nombre));
        default:                  return nullptr;
    }
    
}

void RobotFactory::createRobot(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, TipoRobot tipo, SceneManager* sceneMgr, string nombre)
{
    switch (tipo)
    {
        case TipoRobot::BOSS:     _robots->push_back(std::move(unique_ptr<Robot>(new RobotBoss(world,direccion,posicion,sceneMgr,nombre))));
        case TipoRobot::PEON:     _robots->push_back(std::move(unique_ptr<Robot>(new RobotPeon(world,direccion,posicion,sceneMgr,nombre))));
        default:                  return;
    }
}

std::vector< unique_ptr<Robot> >* RobotFactory::createRobotSet(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, std::vector<TipoRobot>& tipos, SceneManager* sceneMgr)
{
    size_t i = 0;
    for (TipoRobot tipo : tipos)
       createRobot(world,direccion,posicion,tipo,sceneMgr,std::to_string(++i));
    
    return _robots;
}


