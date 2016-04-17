#include "Bala.h"


Bala::~Bala()
{
}

void Bala::shoot(Vector3 & direccion, float fuerza, Vector3 & relPos)
{
    _body->enableActiveState();
    //_body->applyImpulse(direccion * fuerza, relPos);
    _body->setLinearVelocity(direccion * fuerza);
}

