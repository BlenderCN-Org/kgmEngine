#include "kgmGameLogic.h"
#include "kgmIGame.h"
#include "../kgmBase/kgmLog.h"

kgmGameLogic::kgmGameLogic()
{
  m_gameplayer = null;
}

kgmGameLogic::~kgmGameLogic()
{
  clear();
}

void kgmGameLogic::clear()
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    (*i)->remove();
    (*i)->release();
  }

  m_objects.clear();

  m_gameplayer = null;
}

bool kgmGameLogic::add(kgmUnit *u)
{
  if(u)
  {
    m_objects.push_back(u);
    u->increment();

    u->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgmActor *a)
{
  if(a)
  {
    m_objects.push_back(a);
    a->increment();

    a->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgmEffect *e)
{
  if(e)
  {
    m_objects.push_back(e);
    e->increment();

    e->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgmSensor *sn)
{
  if(sn)
  {
    m_objects.push_back(sn);
    sn->increment();

    sn->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgmTrigger *tr)
{
  if(tr)
  {
    m_objects.push_back(tr);
    tr->increment();

    tr->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::remove(kgmGameObject *o)
{
  if(!o)
    return false;

  for(int i = m_objects.length(); i > 0; i--)
  {
    if(o == m_objects[i - 1])
    {
      m_objects.erase(i - 1);

      o->release();

      if(o == m_gameplayer)
        m_gameplayer = null;

      return true;
    }
  }

  return false;
}

bool kgmGameLogic::isValid(kgmGameObject *go)
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); i.next())
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::build()
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);
    go->init();
  }
}

void kgmGameLogic::update(u32 milliseconds)
{
#ifdef EDITOR
  u32 state = kgmIGame::getGame()->gState();

  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play &&
        kgmIGame::getGame()->gState() != kgmIGame::State_Edit)
    return;
#else
  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play)
    return;
#endif

  kgmList<kgmGameObject*>::iterator i = m_objects.begin();

  gcount = 0;

  while(i != m_objects.end())
  {
    kgmGameObject* go = (*i);

    if(!kgmObject::isValid(go))
    {
      ++i;

      continue;
    }
    else if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer = null;

      i = m_objects.erase(i);
      go->release();
    }
    else if(go->valid())
    {
      go->update(milliseconds);
      ++i;
    }
  }

  static int tm = kgmTime::getTicks();

  if(kgmTime::getTicks() - tm > 100)
  {
    tm = kgmTime::getTicks();
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer)
    m_gameplayer->input(btn, state);
}

void kgmGameLogic::action(ACTION, kgmObject*, kgmString)
{

}

void kgmGameLogic::collide(kgmGameObject* src, kgmGameObject* dst)
{

}

kgmGameObject* kgmGameLogic::getObjectById(kgmString id)
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(go->removed() || !go->valid())
    {
      continue;
    }
    else if(id == go->getId())
    {
      return go;
    }
  }

  return null;
}

u32 kgmGameLogic::getObjects(kgmList<kgmGameObject*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    objs.add((*i));
    count++;
  }

  return count;
}

u32 kgmGameLogic::getObjectsByType(kgmRuntime& t, kgmList<kgmGameObject*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isType(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}

u32 kgmGameLogic::getObjectsByClass(kgmRuntime& t, kgmList<kgmGameObject*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isClass(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}
