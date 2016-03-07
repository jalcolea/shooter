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

  createScene();
  _exitGame = false;
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
    return true;
}

bool testwiimoteState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
    { 
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
void testwiimoteState::createScene(){}
void testwiimoteState::createLight(){}