#include "Robot.h"

Robot::~Robot() 
{ 
}

void Robot::setAnim(string nombreAnim, bool loop /* = false */)
{
    _anim = _entRobot->getAnimationState(nombreAnim);
    _anim->setLoop(loop);
}

void Robot::anima(Real deltaT, Real velocidad /* = 1 */)
{
    if (_anim)
        _anim->addTime(deltaT * velocidad);
}