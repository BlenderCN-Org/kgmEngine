#include "kgmUnit.h"
#include "kgmIGame.h"

kgmTab<kgmString, kgmUnit*(*)(kgmIGame*)> kgmUnit::g_typ_objects;

kgmTab<kgmString, kgmUnit::ActionCallback> kgmUnit::g_list_action;

kgmUnit::kgmUnit(kgmIGame* g)
{
  m_type = Unit;

  m_game = g;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::~kgmUnit()
{
  if(m_body)
    delete m_body;

  if(m_visual)
    delete m_visual;

  if(m_remove)
    remove();
}

void kgmUnit::remove()
{
  clear();

  m_remove  = true;
  m_valid   = false;
  m_visible = false;
}

u32 kgmUnit::timeout()
{
  return (kgmTime::getTicks() - m_birth);
}

void kgmUnit::update(u32 mls)
{
  u32 ct = kgmTime::getTicks();

  if(removed())
    return;

  if((getLiving() != -1) && (timeout() > getLiving()))
  {
    remove();

    return;
  }

  if(m_action.id.length() > 0)
  {
    if(!m_action.callback)
      m_action.callback = getActionCallback(m_action.id);

    if(m_action.callback)
      m_action.callback(game(), this, &m_action);
  }

  if(m_visual)
  {
    mtx4 tm;

    if(m_body)
    {
      m_body->transform(tm);
    }
    else
    {
      mtx4 rt, tr;

      tr.identity();
      //tr.translate(m_position);
      rt.identity();
      //rt.rotate(m_rotation);

      tm = rt * tr;
    }

    m_visual->getTransform() = tm;
    m_visual->update();
  }
}
