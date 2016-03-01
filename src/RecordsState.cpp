#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "records.h"

template<> RecordsState* Ogre::Singleton<RecordsState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void RecordsState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  createScene();
  loadRecords();
  _exitGame = false;
}

void RecordsState::loadRecords()
{
          int result = 0;
          int cont=0;
          string name;
          int points;
          string tmp_users="";
          string tmp_points="";
          char tmp_char [64];
          int max_records = 10;
          result = records::getInstance()->getNext(name,points,true);
          cont++;
          while (result == 0)
          {

            if (result == 0)
            {
              sprintf(tmp_char,"\%s\n",name.c_str());
              tmp_users += string(tmp_char);
              sprintf(tmp_char,"\%d\n",points);
              tmp_points += string(tmp_char);
            }
            if (max_records!=0 && cont >= max_records) break;
            result = records::getInstance()->getNext(name,points);
            cont++;
          }
  //        score_names_txt->setCaption(tmp_users);
  //        score_points_txt->setCaption(tmp_points);
}
void RecordsState::exit ()
{
  destroyMyGui();
}

void RecordsState::pause()
{
}

void RecordsState::resume()
{
}

bool RecordsState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool RecordsState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}


bool RecordsState::keyPressed(const OIS::KeyEvent &e)
{
  popState();
  
  return true;
}

bool RecordsState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool RecordsState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool RecordsState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool RecordsState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

RecordsState* RecordsState::getSingletonPtr()
{
    return msSingleton;
}

RecordsState& RecordsState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

RecordsState::~RecordsState()
{
}


void RecordsState::createScene()
{
  createMyGui();
}

void RecordsState::destroyMyGui()
{
}

void RecordsState::createMyGui()
{
}
