#include "StandRobots.h"

template<> StandRobots* Ogre::Singleton<StandRobots>::msSingleton = 0;

StandRobots::~StandRobots()
{
}

void StandRobots::enter()
{
//    _root = Ogre::Root::getSingletonPtr();
//    
//    try
//    {
//        _sceneMgr = _root->getSceneManager("SceneManager");
//    }
//    catch (...)
//    {
//        cout << "SceneManager no existe, creándolo \n";
//        _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
//    }
//
//    try
//    {
//        _camera = _sceneMgr->getCamera("IntroCamera");
//    }
//    catch (...)
//    {
//        cout << "IntroCamera no existe, creándola \n";
//        _camera = _sceneMgr->createCamera("IntroCamera");
//    }
//
    try
    {
        _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
    }
    catch (...)
    {
        _viewport = _root->getAutoCreatedWindow()->getViewport(0);
    }
//    
//    //Fondo del test a rojo
//    _viewport->setBackgroundColour(Ogre::ColourValue(1.0, 0.0, 0.0));
//
//    //Configuramos la camara
//    double width = _viewport->getActualWidth();
//    double height = _viewport->getActualHeight();
//    _camera->setAspectRatio(width / height);
//    _camera->setPosition(Vector3(0, 5, 18));
//    _camera->lookAt(_sceneMgr->getRootSceneNode()->getPosition());
//    _camera->lookAt(0,0,0);
//    _camera->setNearClipDistance(0.1);
//    _camera->setFarClipDistance(100);
    
    //Plano para hacer rayos contra él desde la cámara y saber la equivalencia
    //de la posición del ratón con respecto al mundo 3d. Ha de ser paralelo, en cualquier
    //posición, a la cámara. 
    _plane = Ogre::Plane(_camera->getDerivedDirection(),Ogre::Vector3(0,0,0));
    
    _sentidoAccionPuerta = AccionPuerta::PARAR;
    
    sounds::getInstance()->play_music("robotsmusic");
    
//    //Activacion de bullet RECUERDA QUITAR ESTO CUANDO ESTE ESTADO SEA LLAMADO
//    // HABRÁ QUE RECUPERAR LA INSTANCIA, NO CREARLA
//    AxisAlignedBox boundBox =  AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),Ogre::Vector3 (10000,  10000,  10000));
//    _world = shared_ptr<OgreBulletDynamics::DynamicsWorld>(new DynamicsWorld(_sceneMgr, boundBox, Vector3(0, -9.81, 0), true,true, 15000));
//    
//    _debugDrawer = new OgreBulletCollisions::DebugDrawer();
//    _debugDrawer->setDrawWireframe(true);
//    SceneNode *node = _sceneMgr->getRootSceneNode()->createChildSceneNode("debugNodeStandRobots", Vector3::ZERO);
//    node->attachObject(static_cast<SimpleRenderable *>(_debugDrawer));
//    _world.get()->setDebugDrawer(_debugDrawer);
//    _world.get()->setShowDebugShapes (true);

    
    createScene();


    std::vector<TipoRobot> tipos(5,TipoRobot::PEON);
    
    _robFact = RobotFactory(Ogre::Vector3(10,0,0),Ogre::Vector3(0,0,0));
    auto robots = _robFact.createRobotSet(_world,_robFact.getDireccionInicial(),_robFact.getPosicionInicial(),tipos,_sceneMgr);
    
    for (auto it = robots->begin(); it != robots->end(); ++it)
    {
        (*it)->setAnim("Walk",true);
        (*it)->startAnim();
    }
    
    _exitGame =  false;
    _deltaT = 0;

}

void StandRobots::exit(){}
void  StandRobots::pause(){}
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


bool StandRobots::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
    if (id == OIS::MouseButtonID::MB_Left)
    {
        _crosshair.get()->setMaterialCrosshair("circle-02.png");
        
        bala = new Bala(_world,Vector3(0,0,-1),Vector3(0,2,10),_sceneMgr);
        
        if (bala)
        {
            Vector3 puntoColision; Ray rayo; RigidBody* body;
            body = pickBody(puntoColision,rayo,e.state.X.abs/float(_viewport->getActualWidth()),e.state.Y.abs/float(_viewport->getActualHeight()));
            if (body)
            {
                body->enableActiveState();
                Vector3 relPos(puntoColision - body->getCenterOfMassPosition());
                Vector3 impulse (rayo.getDirection());
                bala->shoot(impulse, 100, relPos);
                
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
    
    for (auto it = _robFact.getRobots()->begin(); it != _robFact.getRobots()->end(); ++it)
    {
        (*it)->anima(_deltaT);
    }

    
    if (_animPuerta)
        _animPuerta->addTime(_deltaT * 0.5 * Ogre::Real(_sentidoAccionPuerta));
    
    
    if (_exitGame)
        return false;
    
    return true;
}

bool StandRobots::frameEnded (const Ogre::FrameEvent& evt)
{ 
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

//StandRobots& StandRobots::getSingleton()
//{
//    return *msSingleton;
//}
//
//StandRobots* StandRobots::getSingletonPtr()
//{ 
//    assert(msSingleton);
//    return msSingleton;
//}

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
    _rigidStand->setStaticShape(_shapeStand, 1, 1);
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
    _sceneMgr->getRootSceneNode()->addChild(_nodeEscudo);
    //_nodeEscudo->translate(0,0,0.1);
    _nodeEscudo->translate(_position + Vector3(0,0,-5));
    
    
}

void StandRobots::ActivaPuerta(AccionPuerta accion)
{
    _animPuerta->setEnabled(true);
 
    switch (accion)
    {
        case AccionPuerta::ABRIR:   _animPuerta->setTimePosition(0.0);
                                    _sentidoAccionPuerta = AccionPuerta::ABRIR; 
                                    _nodeEscudo->setVisible(true); 
                                    sounds::getInstance()->play_effect_loop("escudo",1); break;
        case AccionPuerta::CERRAR:  _animPuerta->setTimePosition(_animPuerta->getLength());
                                    _sentidoAccionPuerta = AccionPuerta::CERRAR; 
                                    _nodeEscudo->setVisible(false); 
                                    sounds::getInstance()->halt_effect(1); break;
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

      _cameraBody->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(0)), Vector3(0, 1, 0)));
      _cameraBody->getBulletRigidBody()->forceActivationState(DISABLE_SIMULATION);
//      btTransform bt = _cameraBody->getBulletRigidBody()->getWorldTransform();
//      bt.setOrigin(convert(_activatorPosition-Vector3(0,0,-10) ));
//      _cameraBody->getBulletRigidBody()->setWorldTransform(bt);
//      _cameraNode->translate(_activatorPosition-Vector3(0,0,2));
      _cameraNode->translate(Vector3(0, 1, -3));
  
}