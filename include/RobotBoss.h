#ifndef ROBOTBOSS_H
#define ROBOTBOSS_H

#include "Robot.h"

#define NOMBRE_ROBOT_BOSS "BOSS"

class RobotBoss : public Robot
{
public:
    RobotBoss(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, SceneManager* sceneMgr, string nombre) : Robot(world,direccion,posicion,sceneMgr){};
    ~RobotBoss();

public:

    virtual void mueve(Vector3 direccion, Real deltaT);

    virtual TipoRobot getTipo();
};

#endif // ROBOTBOSS_H
