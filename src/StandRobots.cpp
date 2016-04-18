#include "StandRobots.h"

template<> StandRobots* Ogre::Singleton<StandRobots>::msSingleton = 0;

StandRobots::~StandRobots()
{
    cout << "ME HAN MATAO :D" << endl;
}

void StandRobots::enter()
{
    try
    {
        _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
    }
    catch (...)
    {
        _viewport = _root->getAutoCreatedWindow()->getViewport(0);
    }
    
    //Plano para hacer rayos contra él desde la cámara y saber la equivalencia
    //de la posición del ratón con respecto al mundo 3d. Ha de ser paralelo, en cualquier
    //posición, a la cámara. 
    _plane = Ogre::Plane(_camera->getDerivedDirection(),Ogre::Vector3(0,0,0));
    
    _sentidoAccionPuerta = AccionPuerta::PARAR;
    
    sounds::getInstance()->play_music("robotsmusic");
    
    _retrasoInicio = RETRASO_INICIO;
     
    
    createScene();
    
    createOleada();

//    _robotBodys.clear();
//    std::vector<TipoRobot> tipos(_numRobots,TipoRobot::PEON);
//    
//    _robFact = RobotFactory(Ogre::Vector3(10,0,0),Ogre::Vector3(0,0,0));
//    auto robots = _robFact.createRobotSet(_world,_robFact.getDireccionInicial(),_robFact.getPosicionInicial(),tipos,_sceneMgr);
//    
//    for (auto it = robots->begin(); it != robots->end(); ++it)
//    {
//        (*it)->setAnim("Walk",true);
//        (*it)->startAnim();
//        _robotBodys.push_back((*it)->getBulletRigidBody());
//    }
    
    _exitGame =  false;
    _deltaT = 0;
    _lapsusTime = CADENCIA_DE_TIRO;
    _inicio = true;
    
    reacomodateCamera();

}

void StandRobots::exit()
{
    _activatorActive = false;
    _crosshair.reset();
    resetCamera();
    ActivaPuerta(AccionPuerta::CERRAR);
    OgreUtil::destroyAllAttachedMovableObjects(_nodeEscudo);
    OgreUtil::destroyAllAttachedMovableObjects(_nodeWeapon);
    OgreUtil::destroySceneNode(_nodeEscudo);
    OgreUtil::destroySceneNode(_nodeWeapon);
    OgreUtil::destroyAllAttachedMovableObjects(_sceneMgr->getSceneNode("nodoFondo"));
    OgreUtil::destroySceneNode(_sceneMgr->getSceneNode("nodoFondo"));

}

void StandRobots::pause(){}
void StandRobots::resume(){}

bool StandRobots::keyPressed (const OIS::KeyEvent &e)
{ 
    if (e.key == OIS::KC_W)
    {
        GameManager::getSingletonPtr()->usarWiimote();
    }    
    
    if (e.key == OIS::KC_C)
    {
        ActivaPuerta(AccionPuerta::CERRAR);
        sounds::getInstance()->play_effect("puerta",0);
    }
    
    if (e.key == OIS::KC_A)
    {
        ActivaPuerta(AccionPuerta::ABRIR);
        sounds::getInstance()->play_effect("puerta",0);
        
    }
        
    if (e.key == OIS::KC_P)
        ActivaPuerta(AccionPuerta::PARAR);
        
    return true;
}

bool StandRobots::keyReleased (const OIS::KeyEvent &e)
{ 
    if (e.key == OIS::KC_ESCAPE)
        _exitGame = true;
    
    return true;
}


bool StandRobots::mouseMoved (const OIS::MouseEvent &e)
{ 
    //cout << "Posicion raton: [" << e.state.X.abs << "," << e.state.Y.abs << "] Area del ratón: [" << e.state.width << "," << e.state.height << "]" << endl;

    const size_t& xMouse = e.state.X.abs + 6;// por alguna razón que desconozco el ratón está desplazado -6 unidades
    const size_t& yMouse = e.state.Y.abs + 6;
    
    _xMouse = xMouse;
    _yMouse = yMouse;
    
    const size_t& xRel = e.state.X.rel;
    const size_t& yRel = e.state.Y.rel;

    const Ogre::Real& wWindow = e.state.width;
    const Ogre::Real& hWindow = e.state.height;

    _crosshair.get()->setActualHitPoint(Ogre::Real(xMouse/wWindow),Ogre::Real(yMouse/hWindow));
    //_nodeWeapon->lookAt(_nodeWeapon->getPosition().reflect(_crosshair.get()->getActualHitPoint()),Ogre::Node::TS_WORLD);
    
    
    

    return true;
}

RigidBody* StandRobots::pickBody (Vector3 &puntoColision, Ray &rayo, float x, float y) {
    rayo = _camera->getCameraToViewportRay (x, y);
    CollisionClosestRayResultCallback cQuery = CollisionClosestRayResultCallback (rayo, _world.get(), 10000);
    _world->launchRay(cQuery);
    if (cQuery.doesCollide()) 
    {
        RigidBody* body = (RigidBody *) (cQuery.getCollidedObject());
        puntoColision = cQuery.getCollisionPoint();
        return body;
    }
    return NULL;
}

Vector3 StandRobots::CalculaDireccionTiro()
{
    std::cout << "Disparando" << std::endl;
    Vector3 dirShoot;

    Ogre::Ray ray  = _camera->
                       getCameraToViewportRay(_xMouse/float(_viewport->getActualWidth()),
                                              _yMouse/float(_viewport->getActualHeight()));
    dirShoot = ray.getDirection();
    std::cout << "Direccion "<< dirShoot << std::endl;
    return dirShoot*10;

}


bool StandRobots::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
    if (id == OIS::MouseButtonID::MB_Left)
    {
        if (_numBalas)
        {
            if (_lapsusTime <= 0)
            {    
                bala = new Bala(_world,CalculaDireccionTiro(), _cameraNode->getPosition(),_sceneMgr,std::to_string(_numBalas));
                Vector3 dir = CalculaDireccionTiro();
                Vector3 rel(0,0,0);
                bala->shoot(dir,2,rel);
                sounds::getInstance()->play_effect("laser");
                --_numBalas;
                _lapsusTime = CADENCIA_DE_TIRO;
            }
        }
    }
    
    return true;
}

bool StandRobots::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
    return true;
}


bool StandRobots::frameStarted (const Ogre::FrameEvent& evt)
{ 
    _deltaT = evt.timeSinceLastFrame;
    
    _world->stepSimulation(_deltaT);
    
    checkCollisions();
    
    _lapsusTime -= _deltaT;
    
    for (auto it = _robFact.getRobots()->begin(); it != _robFact.getRobots()->end(); ++it)
    {
        (*it)->anima(_deltaT);
        if (_inicio)
        {
            _retrasoInicio -= _deltaT;
            if (_retrasoInicio <= 0)
            {
                _inicio = false;
                ActivaPuerta(AccionPuerta::CERRAR);
            }
        }
//        else
//            (*it)->getBulletRigidBody()->setLinearVelocity(convert(Vector3(0,0.1,0.75)));
    }

        
    
    if (_animPuerta)
        _animPuerta->addTime(_deltaT * 0.5 * Ogre::Real(_sentidoAccionPuerta));
        
    
    
    if (_exitGame)
    {
        popState();
    }
        
    
    return true;
}

bool StandRobots::frameEnded (const Ogre::FrameEvent& evt)
{ 
    _deltaT = evt.timeSinceLastFrame;
    _world->stepSimulation(_deltaT);
    
    return true;
}


bool StandRobots::WiimoteButtonUp(const WiimoteEvent & e)
{ 
    return true;
}

bool StandRobots::WiimoteButtonDown(const WiimoteEvent & e)
{ 
    std::cout << "StandRobots wiimoteButtonDown" << endl;
    if (e._wiimote.keys.bits & WIIMOTE_KEY_A)
        std::cout << "BOTON A PRESIONADO" << endl;
    return true;
}

bool StandRobots::WiimoteIRMove(const WiimoteEvent & e)
{ 
    std::cout << "StandRobots wiimoteIRMove" << endl;
    
    std::cout << "IR1: "<< e._wiimote.ir1.x << "," << e._wiimote.ir1.y << endl;
    std::cout << "IR2: "<< e._wiimote.ir2.x << "," << e._wiimote.ir2.y << endl;    
    std::cout << "IR3: "<< e._wiimote.ir3.x << "," << e._wiimote.ir3.y << endl;
    std::cout << "IR4: "<< e._wiimote.ir4.x << "," << e._wiimote.ir4.y << endl;
    
    const Ogre::Real & width = _viewport->getActualWidth();
    const Ogre::Real & height = _viewport->getActualHeight();
    
    size_t x = width - (e._wiimote.ir1.x * (width / IR_X_MAX)); // Restamos el resultado de la conversión a Width pq el wiimote tiene invertida la x.
    size_t y = e._wiimote.ir1.y * (height / IR_Y_MAX);
    
    cout << "x calculada: " << x << endl;
    cout << "y calculada: " << y << endl;
    
    
    _crosshair.get()->setActualHitPoint(Ogre::Real(x/width),Ogre::Real(y/height));
    
    return true;
}

void StandRobots::buildGame() 
{
    cout << _name << endl;
    StaticGeometry *stage = nullptr;
    stage = _sceneMgr->createStaticGeometry(_name);
    _entStand = _sceneMgr->createEntity("ent" + _name,"puestoRobots.mesh");
    _entStand->setQueryFlags(COL_STAND);
    //Asociar forma y cuerpo rígido
    OgreBulletCollisions::StaticMeshToShapeConverter trimeshConverter = OgreBulletCollisions::StaticMeshToShapeConverter(_entStand);
    _shapeStand = trimeshConverter.createTrimesh();
    _rigidStand = new OgreBulletDynamics::RigidBody(_name, _world.get(), COL_STAND, COL_CAMERA | COL_FLOOR | COL_ROBOT | COL_BALA);
    _rigidStand->setStaticShape(_shapeStand, 1, 1,_position);
    _entStand->setCastShadows(true);
    //stage->addEntity(_entStand, Vector3::ZERO);
    stage->addEntity(_entStand, _position);
    stage->build();
    buildActivator();

    _rigidStand->setDebugDisplayEnabled(true);

    _entPuerta = _sceneMgr->createEntity("entPuerta","Puerta.mesh");
    _nodePuerta = _sceneMgr->createSceneNode("nodePuerta");
    _nodePuerta->attachObject(_entPuerta);
    _sceneMgr->getRootSceneNode()->addChild(_nodePuerta);
    //_nodePuerta->translate(10,0,-0.01);
    _nodePuerta->translate(_position + Vector3(0,0,-0.01));
    _animPuerta = _entPuerta->getAnimationState("AbrirPuerta");
    _animPuerta->setLoop(false);
    
//    RigidBody* bodyPuerta = new RigidBody("bodyPuerta",_world.get(),  COL_PUERTA, COL_ROBOT | COL_FLOOR | COL_BALA);
//    BoxCollisionShape* shapePuerta = new BoxCollisionShape(_entPuerta->getBoundingBox().getSize());
//    bodyPuerta->showDebugShape(true);
//    bodyPuerta->setShape(_nodePuerta,                    // SceneNode que manejará Bullet
//                  shapePuerta,                        // Forma geométrica para manejar colisiones (o eso creo)
//                  0.0,                           // Dynamic body restitution
//                  0,                        // Dynamic body friction
//                  0,                           // Dynamic body mass   
//                  //_position + Vector3(0,0,-0.01),                    // Posicion inicial del shape
//                  _nodePuerta->getPosition(),
//                  Quaternion::IDENTITY);         // Orientacion del shape
    
    _numBalas = 100;
    
    _numRobots = NUM_ROBOTS_INICIAL;
    _oleada = 0;

}

void StandRobots::createMyGui(){}

void StandRobots::destroyMyGui(){}

void StandRobots::createScene()
{
    //createFloor();
    
    Ogre::Entity* entPistola =  _sceneMgr->createEntity("entPistola","m1911.mesh");
    _nodeWeapon = _sceneMgr->createSceneNode("nodePistola");
    _nodeWeapon->attachObject(entPistola);
    _sceneMgr->getRootSceneNode()->addChild(_nodeWeapon);
    _nodeWeapon->setPosition(Ogre::Vector3(3.25,-5.0,18));

    Ogre::Light* light = _sceneMgr->createLight("LightWeaponStandRobots");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(-1,-1,0));
    _nodeWeapon->attachObject(light);
    
    _crosshair = unique_ptr<Crosshair>(new Crosshair(_sceneMgr,_camera));
    _crosshair.get()->createCrossHairManual("circle-01.png");
    

    Ogre::Light* lightStand = _sceneMgr->createLight("LightStandRobots");
    lightStand->setType(Ogre::Light::LT_POINT);
    
    //_camera->setPosition(Vector3(0, 0.5, 18));
    //_camera->lookAt(_nodeStand->getPosition());
    
//    _entPuerta = _sceneMgr->createEntity("entPuerta","Puerta.mesh");
//    _nodePuerta = _sceneMgr->createSceneNode("nodePuerta");
//    _nodePuerta->attachObject(_entPuerta);
//    _sceneMgr->getRootSceneNode()->addChild(_nodePuerta);
//    _nodePuerta->translate(0,0,-0.01);
//    _animPuerta = _entPuerta->getAnimationState("AbrirPuerta");
//    _animPuerta->setLoop(false);
    
    _nodeEscudo = _sceneMgr->createSceneNode("nodeEscudo");
    Ogre::Entity* entEscudo = _sceneMgr->createEntity("entEscudo","Escudo.mesh");
    _nodeEscudo->attachObject(entEscudo);
    _nodeEscudo->setVisible(false);
    _nodeEscudo->scale(5.0,2.0,0.0);
    Vector3 caja = entEscudo->getBoundingBox().getHalfSize();
    caja.x *= 5; caja.y *= 2; // Escalamos para la cáscara de bullet para el escudo y que coincida con él
    _sceneMgr->getRootSceneNode()->addChild(_nodeEscudo);
    _nodeEscudo->translate(_position + Vector3(0,0,-5));
    //_shape = new BoxCollisionShape(entEscudo->getBoundingBox().getHalfSize() * 0.2);
    BoxCollisionShape* shapeEscudo = new BoxCollisionShape(caja);
    _bodyEscudo = new RigidBody("Escudo",_world.get(), COL_ESCUDO, COL_BALA);
    _bodyEscudo->setShape(_nodeEscudo,            // SceneNode que manejará Bullet
                  shapeEscudo,                   // Forma geométrica para manejar colisiones (o eso creo)
                  0.0,                           // Dynamic body restitution
                  1.0,                           // Dynamic body friction
                  0,                             // Dynamic body mass   
                  _position + Vector3(0,0,-5),   // Posicion inicial del shape 5 ARRIBA Y 5 ATRAS
                  Quaternion::IDENTITY);         // Orientacion del shape
    
    
    Entity* entFondo = _sceneMgr->createEntity("entFondo","Fondo.mesh");
    SceneNode* nodeFondo = _sceneMgr->createSceneNode("nodoFondo");
    nodeFondo->attachObject(entFondo);
    _sceneMgr->getRootSceneNode()->addChild(nodeFondo);
    nodeFondo->translate(_position + Vector3(0,0,-0.01));
    
    ActivaPuerta(AccionPuerta::ABRIR);
}

void StandRobots::ActivaPuerta(AccionPuerta accion)
{
    _animPuerta->setEnabled(true);
 
    btTransform transform;
    transform.setIdentity();
    transform = _bodyEscudo->getBulletRigidBody()->getWorldTransform();
    
    switch (accion)
    {
        case AccionPuerta::ABRIR:   _animPuerta->setTimePosition(0.0);
                                    _sentidoAccionPuerta = AccionPuerta::ABRIR; 
                                    _nodeEscudo->setVisible(true); 
                                    sounds::getInstance()->play_effect_loop("escudo",1); 
                                    transform.setOrigin(convert(_position + Vector3(0,0,-5)));
                                    _bodyEscudo->getBulletRigidBody()->setWorldTransform(transform);
                                    break;
        case AccionPuerta::CERRAR:  _animPuerta->setTimePosition(_animPuerta->getLength());
                                    _sentidoAccionPuerta = AccionPuerta::CERRAR; 
                                    _nodeEscudo->setVisible(false); 
                                    sounds::getInstance()->halt_effect(1); 
                                    transform.setOrigin(convert(_position + Vector3(0,5,-5)));
                                    _bodyEscudo->getBulletRigidBody()->setWorldTransform(transform);
                                    break;
        case AccionPuerta::PARAR:   _animPuerta->setEnabled(false);
                                    _sentidoAccionPuerta = AccionPuerta::PARAR; break;
        default:;
    }
}


void StandRobots::createLight()
{

}

void StandRobots::reacomodateCamera() 
{

//      _cameraNode->translate(Vector3(0, 1, -2));
//      _cameraNode->setDirection(Vector3(0,0,-1));
//      _cameraNode->setOrientation(Quaternion::IDENTITY);
//      _camera->setPosition(0,1,-2);
//      _camera->setDirection(0,0,-1);
//      _camera->lookAt(_position + Vector3(0,1,0));
//      _cameraNode->lookAt(_position + Vector3(0,1,0),Node::TS_LOCAL);
//      cout << "camara mirando a " << _position << endl;
//      btTransform bt;
//      bt.setIdentity();  
//      bt = _cameraBody->getBulletRigidBody()->getWorldTransform();
//      bt.setRotation(convert(_cameraNode->getOrientation()));  

      _cameraBody->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));
      _cameraBody->setPosition(Vector3(_activatorPosition.x,1.5,_activatorPosition.z));
      _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_SIMULATION);
      _camera->setPosition(Vector3(_activatorPosition.x-_position.x,1.5,_activatorPosition.z));
      _camera->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));
      _cameraNode->setPosition(Vector3(_activatorPosition.x,1.5,_activatorPosition.z));
      _cameraNode->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));

//      bt.setOrigin(convert(_activatorPosition-Vector3(0,0,-10) ));
//      bt.setOrigin(convert(Vector3(_activatorPosition.x,1,-3)));
//      bt.setRotation(convert(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3::UNIT_Y)));
//      _cameraBody->getBulletRigidBody()->setWorldTransform(bt);
      
      _crosshair->setCamera(_camera);

  
}

void StandRobots::resetCamera()
{
      _cameraBody->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));
      _cameraBody->setPosition(Vector3(_activatorPosition.x,1.5,_activatorPosition.z+1));
      _camera->setPosition(Vector3(_activatorPosition.x-_position.x,1.5,_activatorPosition.z+1));
      _camera->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));
      _cameraNode->setPosition(Vector3(_activatorPosition.x,1.5,_activatorPosition.z+1));
      _cameraNode->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));
      _cameraBody->getBulletRigidBody()->setLinearVelocity(convert(Vector3(0,0,0)));
      _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_DEACTIVATION);

}


void StandRobots::checkCollisions() 
{



    
    btCollisionWorld *collisionWorld = _world.get()->getBulletCollisionWorld();
    btDynamicsWorld *dynamicWorld = _world.get()->getBulletDynamicsWorld();

    int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
    bool collide = false;
    for (int i = 0; i < numManifolds; i++) 
    {
        btPersistentManifold *contactManifold = collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
        
        //Sacamos la lista de robotBodys que están colisionando con algo
        std::vector<btRigidBody*>::iterator it = std::find_if(_robotBodys.begin(), _robotBodys.end(),
                            [contactManifold](btRigidBody* robotBody)->bool 
                            {
                                return contactManifold->getBody0()->getCollisionShape() == robotBody->getCollisionShape() ||
                                       contactManifold->getBody1()->getCollisionShape() == robotBody->getCollisionShape();
                            });

        //Si it tiene más de 0 elementos, es que hay 1 o más robotBodys colisionando.
        if (it != _robotBodys.end()) 
        {
            // Ahora discernimos cual de la pareja de bodys que colisionan es el robot y cual es otro body
            // Si el body0 es el robot, devolvemos body0, de lo contrario el body1 (sino es burro, es burra)
            const btCollisionObject* robotBody = (contactManifold->getBody0() == (*it)) ? contactManifold->getBody0() : contactManifold->getBody1();
            // Del mismo modo nos guardamos el otro body, el que no sea un robotbody.
            const btCollisionObject* otherObject = (robotBody == contactManifold->getBody0()) ? contactManifold->getBody1():contactManifold->getBody0();
            
            if (!strcmp(otherObject->getCollisionShape()->getName(),"CylinderZ")) //Si la colision es con un cilindro, es una bala
            {
                //cout << "colision con una bala" << endl;
                sounds::getInstance()->play_effect("robotHit");
                //_world->getBulletDynamicsWorld()->removeCollisionObject((btCollisionObject*)otherObject);
            }
            else // Si no, pues será el escenario.
            {
                //cout << "colision con escenario" << endl;
                Quaternion quat = convert((*it)->getOrientation());
                if (Math::Abs((int)quat.getPitch().valueDegrees()) == 90) //Si lo han tumbao, se muere
                {
                    cout << "estoy tumbadito" << endl;
                    _world->getBulletDynamicsWorld()->removeCollisionObject((btCollisionObject*)otherObject);
                }
            }
            
        }
    }
}

void StandRobots::createOleada()
{
    _robotBodys.clear();
    std::vector<TipoRobot> tipos(_numRobots,TipoRobot::PEON);
    
    _robFact = RobotFactory(Ogre::Vector3(10,0,0),Ogre::Vector3(0,0,0));
    auto robots = _robFact.createRobotSet(_world,_robFact.getDireccionInicial(),_robFact.getPosicionInicial(),tipos,_sceneMgr);
    
    for (auto it = robots->begin(); it != robots->end(); ++it)
    {
        (*it)->setAnim("Walk",true);
        (*it)->startAnim();
        _robotBodys.push_back((*it)->getBulletRigidBody());
//        (*it)->getBulletRigidBody()->setLinearFactor(btVector3(0,1,1));
//        (*it)->getBulletRigidBody()->setAngularFactor(btVector3(1,0,0));
        //(*it)->getBulletRigidBody()->setAngularVelocity(convert(Vector3(0,0,0)));
        //(*it)->getBulletRigidBody()->setAngularVelocity(convert(Vector3(0,0,0)));
        //(*it)->getBulletRigidBody()->setLinearVelocity(convert(Vector3(0,0,1.5)));
    }
    
}

