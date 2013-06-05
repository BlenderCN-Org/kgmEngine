#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmGameObject : public kgmObject
{
  KGM_OBJECT(kgmGameObject);

private:
  bool    m_enable;
  bool    m_active;
  bool    m_remove;
  bool    m_culled;
  bool    m_visible;

protected:
  kgmBody*    m_body;
  kgmVisual*  m_visual;

  kgmGameObject* 	         m_parent;
  kgmList<kgmGameObject*>  m_childs;


public:
  kgmGameObject();
  virtual ~kgmGameObject();

  virtual void         update(u32 mls){}
  virtual kgmBody*     getBody(){ return m_body; }
  virtual kgmVisual*   getVisual(){ return m_visual; }

  bool isEnable()   { return m_enable;   }
  bool isActive()   { return m_active;   }
  bool isRemove()   { return m_remove;   }
  bool isCulled()   { return m_culled;   }
  bool isVisible()  { return m_visible;  }

  void remove()     { m_remove = true;   }
  void enable()     { m_enable = true;   }
  void disable()    { m_enable = false;  }
  void activate()   { m_active = true;   }
  void deactivate() { m_active = false;  }
  void show()       { m_visible = true;  }
  void hide()       { m_visible = false; }

  void setParent(kgmGameObject* a)
  {
    if(!a)
      return;

    if(m_parent)
    {
      m_parent->removeChild(this);
    }

    m_parent = a;
  }

  void addChild(kgmGameObject* a)
  {
    if(!a)
      return;

    m_childs.add(a);
    a->increment();
  }

  void removeChild(kgmGameObject* a)
  {
    if(!a)
      return;

    for(int i = 0; i < m_childs.length(); i++)
    {
      if(a == m_childs[i])
      {
        a->release();
        m_childs.erase(i);

        return;
      }
    }
  }
};

#endif // KGMGAMEOBJECT_H
