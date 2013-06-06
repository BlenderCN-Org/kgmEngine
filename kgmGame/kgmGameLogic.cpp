#include "kgmGameLogic.h"
#include "kgmIGame.h"

kgmGameLogic::kgmGameLogic()
{
}

kgmGameLogic::~kgmGameLogic()
{
  clear();
}

void kgmGameLogic::clear()
{
  for(kgmList<kgmActor*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i)
  {

    (*i)->exit();
    (*i)->release();
  }

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {

    (*i)->exit();
    (*i)->release();
  }

  m_actors.clear();
  m_objects.clear();

  m_gameplayer = null;
}

bool kgmGameLogic::add(kgmActor *a)
{
  if(a)
  {
    if(a->m_gameplayer)
      m_gameplayer = a;

    m_actors.add(a);
    a->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgmGameObject *o)
{
  if(o)
  {
    m_objects.add(o);
    o->init();

    return true;
  }

  return false;
}

void kgmGameLogic::update(u32 milliseconds)
{
  /*for(kgmList<Sensor*>::iterator i = m_sensors.begin(); i != m_sensors.end(); ++i)
  {
    (*i)->sense();
  }*/

  for(kgmList<kgmActor*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i)
  {

    (*i)->update(milliseconds);
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer)
  {
    m_gameplayer->input(btn, state);
  }
  else
  {
  }
}

kgmGameObject* kgmGameLogic::createGameObject(kgmString t)
{
  return null;
}
