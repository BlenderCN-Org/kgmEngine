#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmIGame;
class kgmSound;

class kgmUnit : public kgmObject
{
  KGM_OBJECT(kgmUnit);

public:
  struct Action;

  typedef void (*ActionCallback)(kgmIGame*, kgmUnit*, Action*);

  struct Action
  {
    kgmString id;
    u32       time;
    u32       state;

    kgmList<kgmVariable> variables;

    ActionCallback callback = null;
  };

private:
  kgmIGame* m_game = null;

  kgmString m_id;
  kgmString m_class;

  bool  m_valid;
  bool  m_remove;
  bool  m_culled;
  bool  m_visible;

  u32  m_group;
  u32  m_birth;
  u32  m_timeout;

  vec3 m_position;
  vec3 m_rotation;
  quat m_quaternion;

  Action m_action;

  static kgmTab<kgmString, ActionCallback> g_list_action;

protected:
  kgmBody*    m_body   = null;
  kgmVisual*  m_visual = null;

public:
  enum Type
  {
    Unit,
    Mesh,
    Actor,
    Camera,
    Effect,
    Sensor,
    Trigger
  };

  typedef kgmUnit* (*Generate)(kgmIGame*);

  static kgmTab<kgmString, Generate> g_typ_objects;

  kgmList<kgmVariable> m_variables;

#ifdef EDITOR
  virtual void eupdate()
  {

  }

  virtual void resetToVariables()
  {
    for(int i = 0; i < m_variables.length(); i++)
      m_variables[i].sync(kgmVariable::SyncToLink);
  }
#endif

public:
  kgmUnit(kgmIGame* g = null);
  ~kgmUnit();

  virtual void         init(){}
  virtual void         exit(){}
  virtual void         update(u32 mls);
  virtual void         event(kgmObject*, kgmString){ }
  virtual kgmBody*     getBody(){   return m_body;   }
  virtual kgmVisual*   getVisual(){ return m_visual; }

  kgmIGame* game() const { return m_game;   }

  bool valid()   const { return m_valid;    }
  bool culled()  const { return m_culled;   }
  bool visible() const { return m_visible;  }
  bool removed() const { return m_remove;   }

  void enable()  { m_valid  = true;   }
  void disable() { m_valid  = false;  }
  void show()    { m_visible = true;  }
  void hide()    { m_visible = false; }

  void remove();

  void timeout(u32 t){ m_timeout = t; }

  void setId(kgmString s)
  {
    m_id = s;
  }

  kgmString getId()
  {
    return m_id;
  }

  void setClass(kgmString c)
  {
    m_class = c;
  }

  kgmString getClass()
  {
    return m_class;
  }

  void setGroup(u32 g)
  {
    m_group = g;
  }

  u32 getGroup()
  {
    return m_group;
  }

  vec3 getPosition()
  {
    if(m_body)
      return m_body->position();

    return m_position;
  }

  void setPosition(vec3& v)
  {
    if(m_body)
      m_body->translate(v.x, v.y, v.z);
    else
      m_position = v;
  }

  vec3 getRotation()
  {
    if(m_body)
      return m_body->rotation();

    return m_rotation;
  }

  void setRotation(vec3& r)
  {
    if(m_body)
      m_body->rotate(r.x, r.y, r.z);
    else
      m_rotation = r;
  }

  void setQuaternion(quat& q)
  {
    if(m_body)
      m_body->rotate(q);
  }

  u32 birth()
  {
    return m_birth;
  }

  kgmVisual* visual() const
  {
    return m_visual;
  }

  void visual(kgmVisual* v)
  {
    m_visual = v;
  }

  kgmBody* body() const
  {
    return m_body;
  }

  void body(kgmBody* b)
  {
    m_body = b;
  }

  kgmString action() const
  {
    return m_action.id;
  }

  void action(kgmString a)
  {
    m_action.id = a;
  }

  void setParameter(kgmString name, kgmString value)
  {
    for(int i = 0; i < m_variables.length(); ++i)
    {
      kgmVariable& var = m_variables[i];

      if(var.getName() == name)
        var.fromString(value);
    }
  }

  static bool getAvailableActions(kgmList<kgmString>& actions)
  {
    if(g_list_action.length() < 1)
      return false;

    for(int i = 0; i < g_list_action.length(); i++)
    {
      kgmString id;
      ActionCallback call;

      g_list_action.get(i, id, call);

      actions.add(id);
    }

    return true;
  }

  static void setActionCallback(kgmString action, kgmUnit::ActionCallback callback)
  {
    if(g_list_action.hasKey(action))
    {
      g_list_action[action] = callback;

      return;
    }

    g_list_action.add(action, callback);
  }

  static kgmUnit::ActionCallback getActionCallback(kgmString action)
  {
    if(!g_list_action.hasKey(action))
      return null;

    return g_list_action[action];
  }

private:
  virtual void clear(){}
};

#endif // KGMGAMEOBJECT_H
