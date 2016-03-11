#include "testwiimoteState.h"

template<> testwiimoteState* Ogre::Singleton<testwiimoteState>::msSingleton = 0;
 
void testwiimoteState::enter()
{
  _root = Ogre::Root::getSingletonPtr();
  
      _root = Ogre::Root::getSingletonPtr();
    try
    {
        _sceneMgr = _root->getSceneManager("SceneManager");
    }
    catch (...)
    {
        cout << "SceneManager no existe, creándolo \n";
        _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
    }

    try
    {
        _camera = _sceneMgr->getCamera("IntroCamera");
    }
    catch (...)
    {
        cout << "IntroCamera no existe, creándola \n";
        _camera = _sceneMgr->createCamera("IntroCamera");
    }

    try
    {
        _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
    }
    catch (...)
    {
        _viewport = _root->getAutoCreatedWindow()->getViewport(0);
    }
    
    //Fondo del test a rojo
    _viewport->setBackgroundColour(Ogre::ColourValue(1.0, 0.0, 0.0));

    //Configuramos la camara
    double width = _viewport->getActualWidth();
    double height = _viewport->getActualHeight();
    _camera->setAspectRatio(width / height);
    //_camera->setPosition(Vector3(0, 12, 18));
    _camera->setPosition(Vector3(0, 0, 18));
    _camera->lookAt(_sceneMgr->getRootSceneNode()->getPosition());
    _camera->lookAt(0,0,0);
    _camera->setNearClipDistance(0.1);
    _camera->setFarClipDistance(100);
    
    //Plano para hacer rayos contra él desde la cámara y saber la equivalencia
    //de la posición del ratón con respecto al mundo 3d. Ha de ser paralelo, en cualquier
    //posición, a la cámara. 
    //_plane = Ogre::Plane(_camera->getDerivedDirection(),Ogre::Vector3(0,0,0));
    
    createScene();
    _exitGame =  false;
    _deltaT = 0;

}

void testwiimoteState::exit(){}
void  testwiimoteState::pause(){}
void testwiimoteState::resume(){}

bool testwiimoteState::keyPressed (const OIS::KeyEvent &e)
{ 
    if (e.key == OIS::KC_W)
    {
        GameManager::getSingletonPtr()->usarWiimote();
    }    
    return true;
}

bool testwiimoteState::keyReleased (const OIS::KeyEvent &e)
{ 
    if (e.key == OIS::KC_ESCAPE)
        _exitGame = true;
    
    return true;
}


bool testwiimoteState::mouseMoved (const OIS::MouseEvent &e)
{ 
    //cout << "Posicion raton: [" << e.state.X.abs << "," << e.state.Y.abs << "] Area del ratón: [" << e.state.width << "," << e.state.height << "]" << endl;

    const size_t& xMouse = e.state.X.abs + 6;// por alguna razón que desconozco el ratón está desplazado -6 unidades
    const size_t& yMouse = e.state.Y.abs + 6;
    
    const size_t& xRel = e.state.X.rel;
    const size_t& yRel = e.state.Y.rel;

    const Ogre::Real& wWindow = e.state.width;
    const Ogre::Real& hWindow = e.state.height;

/*
    //Le pedimos a la cámara un rayo (devuelto en el mismo parámetro _mouseray
    //_mouseray es un unique_ptr, y la función espera un puntero, de modo que usamos _mouseray.get() para que
    //el unique_ptr nos devuelva el puntero de verdad.
    _camera->getCameraToViewportRay(xMouse/wWindow,yMouse/hWindow,_mouseray.get()); 
   
    //Comprobamos que el rayo devuelto por la camara "choca" con el plano en el que la cámara se proyecta
    std::pair<bool,Ogre::Real> intersectionResult = _mouseray->intersects(_plane);
    //El par que devuelve tiene como primer elemento un booleano indicando si ha habido "choque" con el plano
    if(intersectionResult.first) 
        //Si ha habido choque pues podemos pedirle al rayo que nos devuelva el vector (getPoint) para colocar 
        //el objeto (el punto de mira) en el lugar donde está el ratón.
        //auto WORLDPOS = _mouseray->getPoint(intersectionResult.second);
        _nodeCrosshair->setPosition(_mouseray->getPoint(intersectionResult.second));
*/
    
    _crosshair.get()->setActualHitPoint(Ogre::Real(xMouse/wWindow),Ogre::Real(yMouse/hWindow));
    _nodeWeapon->lookAt(_nodeWeapon->getPosition().reflect(_crosshair.get()->getActualHitPoint()),Ogre::Node::TS_WORLD);
    
    
    

    return true;
}


bool testwiimoteState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
    if (id == OIS::MouseButtonID::MB_Left)
        _crosshair.get()->setMaterialCrosshair("circle-02.png");
    
    return true;
}

bool testwiimoteState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
    return true;
}


bool testwiimoteState::frameStarted (const Ogre::FrameEvent& evt)
{ 
    _deltaT = evt.timeSinceLastFrame;
    
    if (_exitGame)
        return false;
    
    return true;
}

bool testwiimoteState::frameEnded (const Ogre::FrameEvent& evt)
    { 
    return true;
}


bool testwiimoteState::WiimoteButtonUp(const WiimoteEvent & e)
{ 
    return true;
}

bool testwiimoteState::WiimoteButtonDown(const WiimoteEvent & e)
{ 
    std::cout << "testWiimoteState wiimoteButtonDown" << endl;
    if (e._wiimote.keys.bits & WIIMOTE_KEY_A)
        std::cout << "BOTON A PRESIONADO" << endl;
    return true;
}

bool testwiimoteState::WiimoteIRMove(const WiimoteEvent & e)
{ 
    std::cout << "testWiimoteState wiimoteIRMove" << endl;
    
    std::cout << "IR1: "<< e._wiimote.ir1.x << "," << e._wiimote.ir1.y << endl;
    std::cout << "IR2: "<< e._wiimote.ir2.x << "," << e._wiimote.ir2.y << endl;    
    std::cout << "IR3: "<< e._wiimote.ir3.x << "," << e._wiimote.ir3.y << endl;
    std::cout << "IR4: "<< e._wiimote.ir4.x << "," << e._wiimote.ir4.y << endl;
    
    return true;
}

testwiimoteState& testwiimoteState::getSingleton()
{
    return *msSingleton;
}

testwiimoteState* testwiimoteState::getSingletonPtr()
{ 
    assert(msSingleton);
    return msSingleton;
}

void testwiimoteState::createMyGui(){}

void testwiimoteState::destroyMyGui(){}

void testwiimoteState::createScene()
{
    Ogre::Entity* entPistola =  _sceneMgr->createEntity("entPistola","m1911.mesh");
    Ogre::SceneNode* nodePistola = _sceneMgr->createSceneNode("nodePistola");
    nodePistola->attachObject(entPistola);
    _sceneMgr->getRootSceneNode()->addChild(nodePistola);
    //nodePistola->yaw(Ogre::Degree(-165));
    nodePistola->setPosition(Ogre::Vector3(3.25,-5.0,1.75));
    //nodePistola->lookAt(Ogre::Vector3(0.0,0.0,0.0),Ogre::Node::TransformSpace::TS_PARENT);
    _nodeWeapon = nodePistola;

    Ogre::Light* light = _sceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(-1,-1,0));
    nodePistola->attachObject(light);
    
    _crosshair = unique_ptr<Crosshair>(new Crosshair(_sceneMgr,_camera));
    _crosshair.get()->createCrossHairManual("circle-01.png");
    
//    Ogre::Entity* entCrossHairOut = _sceneMgr->createEntity("entCrossHairOut","MediumCrossHair.mesh");
//    Ogre::Entity* entCrossHairIn = _sceneMgr->createEntity("entCrossHairIn", "MiniCrossHair.mesh");
//    Ogre::SceneNode* nodeCrossHairOut = _sceneMgr->createSceneNode("nodeCrossHairOut");
//    Ogre::SceneNode* nodeCrossHairIn = _sceneMgr->createSceneNode("nodeCrossHairIn");
//    nodeCrossHairIn->attachObject(entCrossHairIn);
//    nodeCrossHairOut->attachObject(entCrossHairOut);
//    nodeCrossHairOut->addChild(nodeCrossHairIn);
//    nodeCrossHairIn->setInheritScale(false);
//    //nodeCrossHairOut->scale(0.30,0.30,0);
//    _sceneMgr->getRootSceneNode()->addChild(nodeCrossHairOut);
//    nodeCrossHairOut->setPosition(0,0,0);
//    _nodeCrosshair = nodeCrossHairOut;
//    _nodeCrosshair = createCrossHair("circle-01.png");
    
}

Ogre::SceneNode* testwiimoteState::createCrossHair(const std::string & crosshairImg)
{

    MaterialPtr material = MaterialManager::getSingleton().create("crosshair", "General",true);
    material->getTechnique(0)->getPass(0)->createTextureUnitState(crosshairImg);
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    material->getTechnique(0)->getPass(0)->setCullingMode(CullingMode::CULL_NONE);
    material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    
    // Creamos un objeto manual para el Crosshair
    ManualObject* crossHair = _sceneMgr->createManualObject("crossHair");
     
    // Matriz identidad para que se vea en 2D
    crossHair->setUseIdentityProjection(true);
    crossHair->setUseIdentityView(true);
     
    //Esto es como un wrapper de openGl
    crossHair->begin("crosshair", RenderOperation::OT_TRIANGLE_STRIP);  // begin(nombreMaterial, tipoPrimitiva)
        crossHair->position(-0.2, -0.2, 0.0);           //position creat un vértice en la posicion dada
        crossHair->textureCoord(0,1);                   //con textureCoord hacemos mapeado UV
        crossHair->position( 0.2, -0.2, 0.0);
        crossHair->textureCoord(1,1);
        crossHair->position( 0.2,  0.2, 0.0);
        crossHair->textureCoord(1,0);
        crossHair->position(-0.2,  0.2, 0.0);
        crossHair->textureCoord(0,0);
        crossHair->index(0);                            //Indexamos  para indicar como se va dibujando la primitiva.
        crossHair->index(1);
        crossHair->index(2);
        crossHair->index(3);
        crossHair->index(0);
    crossHair->end();
    
    Ogre::Entity* entCrossHair = _sceneMgr->createEntity(crossHair->convertToMesh("crossHairMesh"));
    //entCrossHair->setMaterial(material);
    
    // Uso de una Aligned Axis Box (AAB) infinita para estar siempre visible. O eso es 
    // lo que pone en el tutorial que he mirado
    AxisAlignedBox aabInf;
    aabInf.setInfinite();
    crossHair->setBoundingBox(aabInf);
     
    // Renderizado antes que naide :D
    crossHair->setRenderQueueGroup(RENDER_QUEUE_OVERLAY - 1);
    
    SceneNode* aux = _sceneMgr->getRootSceneNode()->createChildSceneNode("CrossHair");
    aux->attachObject(entCrossHair);
    aux->scale(2.0,2.0,2.0);
    
    return aux;
}



void testwiimoteState::createLight()
{

}