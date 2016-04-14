#include "RobotPeon.h"

RobotPeon::~RobotPeon()
{
}

TipoRobot RobotPeon::getTipo() 
{ 
    return TipoRobot::PEON;
}

void RobotPeon::mueve(Vector3 direccion, Real deltaT)
{
}
