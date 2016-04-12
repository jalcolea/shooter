#ifndef ROBOTFACTORY_H
#define ROBOTFACTORY_H

#include "Robot.h"
#include "RobotPeon.h"
#include "RobotBoss.h"
#include "OgreBulletDynamicsWorld.h"
#include <vector>

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;

class RobotFactory
{
public:
    RobotFactory(Vector3 posicionInicial,Vector3 direccionInicial) : _posicionInicial(posicionInicial), _direccionInicial(direccionInicial)
    {
        _robots = new std::vector< unique_ptr<Robot> >();
    };
    
    RobotFactory() : _posicionInicial(Vector3(0,0,0)), _direccionInicial(Vector3(0,0,0)){};
    ~RobotFactory();
    
    unique_ptr<Robot> createOneRobot(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, TipoRobot tipo, SceneManager* sceneMgr, string nombre);
    void createRobot(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, TipoRobot tipo, SceneManager* sceneMgr, string nombre);
    std::vector< unique_ptr<Robot> >* createRobotSet(shared_ptr<DynamicsWorld> world, Vector3 direccion, Vector3 posicion, std::vector<TipoRobot>& tipos, SceneManager* sceneMgr);
    
    inline void setPosicionInicial(Vector3 & posicionInicial)  { _posicionInicial = posicionInicial; }
    inline void setDireccionInicial(Vector3 & direccionInicial) { _direccionInicial = direccionInicial; }
    
    inline Vector3 & getDireccionInicial() { return _direccionInicial; }
    inline Vector3 & getPosicionInicial() { return _posicionInicial; }
    
private:
    Vector3 _posicionInicial;
    Vector3 _direccionInicial;
    std::vector< unique_ptr<Robot> >* _robots;
    
    

};

#endif // ROBOTFACTORY_H

