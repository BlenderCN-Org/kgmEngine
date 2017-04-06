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
  enum UnitType
  {
    Unit,
    Light,
    Actor,
    Visual,
    Camera,
    Effect,
    Sensor,
    Trigger
  };

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

  u32       m_id;
  kgmString m_name;

  bool m_lock;
  bool m_valid;
  bool m_remove;
  bool m_culled;
  bool m_visible;

  u32 m_group;
  u32 m_birth;
  u32 m_living;

  vec3 m_position;
  vec3 m_rotation;
  quat m_quaternion;

  Action m_action;

  static kgmTab<kgmString, ActionCallback> g_list_action;

  kgmBody*    m_body   = null;

  union
  {
    kgmLight*   m_light  = null;
    kgmCamera*  m_camera;
    kgmVisual*  m_visual;
  };

  UnitType m_type;

public:
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

  virtual void init()
  { }

  virtual void exit()
  { }

  virtual void event(kgmObject*, kgmString)
  { }

  virtual void update(u32);

  void remove();
  u32  timeout();

private:
  virtual void clear()
  { }

public:
  kgmIGame* game() const
  {
    return m_game;
  }

  UnitType getType() const
  {
    return m_type;
  }

  kgmBody* body() const
  {
    return m_body;
  }

  kgmVisual* visual() const
  {
    return m_visual;
  }

  bool valid() const
  {
    return m_valid;
  }

  bool culled()  const
  {
    return m_culled;
  }

  bool visible() const
  {
    return m_visible;
  }

  bool removed() const
  {
    return m_remove;
  }

  void enable()
  {
    m_valid  = true;
  }

  void disable()
  {
    m_valid  = false;
  }

  void show()
  {
    m_visible = true;
  }

  void hide()
  {
    m_visible = false;
  }

  bool lock() const
  {
    return m_lock;
  }

  void lock(bool l)
  {
    m_lock = l;
  }

  void setId(u32 c)
  {
    m_id = c;
  }

  u32 getId()
  {
    return m_id;
  }

  void setName(kgmString s)
  {
    m_name = s;
  }

  kgmString getName()
  {
    return m_name;
  }

  void setGroup(u32 g)
  {
    m_group = g;
  }

  u32 getGroup()
  {
    return m_group;
  }

  void setLiving(u32 l)
  {
    m_living = l;
  }

  u32 getLiving() const
  {
    return m_living;
  }

  vec3 position()
  {
    if(m_body)
      return m_body->position();

    return m_position;
  }

  void position(vec3& v)
  {
    if(m_body)
      m_body->translate(v.x, v.y, v.z);
    else
      m_position = v;
  }

  vec3 rotation()
  {
    if(m_body)
      return m_body->rotation();

    return m_rotation;
  }

  void rotation(vec3& r)
  {
    if(m_body)
      m_body->rotate(r.x, r.y, r.z);
    else
      m_rotation = r;
  }

  quat quaternion()
  {
    if(m_body)
      return m_body->quaternion();

    return quat();
  }

  void quaternion(quat& q)
  {
    if(m_body)
      m_body->rotate(q);
  }

  u32 birth()
  {
    return m_birth;
  }

  UnitType type()
  {
    return m_type;
  }

  kgmLight* light() const
  {
    if(m_type != Light)
      return null;

    return m_light;
  }

  kgmCamera* camera() const
  {
    if(m_type != Camera)
      return null;

    return m_camera;
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
};

#endif // KGMGAMEOBJECT_H
