#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmUnit.h"
#include "../../kgmGame/kgmActor.h"
#include "../../kgmGame/kgmEffect.h"
#include "../../kgmGame/kgmSensor.h"
#include "../../kgmGame/kgmTrigger.h"

#include "../../kgmPhysics/kgmObstacle.h"

namespace kgmGameEditor
{

class kNode : public kgmObject
{
  KGM_OBJECT(kNode);
public:
  enum Type
  {
    NONE,
    MESH,
    UNIT,
    LIGHT,
    ACTOR,
    EFFECT,
    SENSOR,
    TRIGGER,
    OBSTACLE
  };

  union
  {
    kgmUnit*      unt;
    kgmLight*     lgt;
    kgmActor*     act;
    kgmVisual*    msh;
    kgmEffect*    eff;
    kgmSensor*    sns;
    kgmTrigger*   trg;
    kgmObstacle*  obs;

    kgmObject*    obj;
  };

  Type typ;

  vec3 pos;
  vec3 rot;

  box3 bnd;

  kgmString nam;
  kgmString lnk;
  kgmString shp;
  kgmString ini;

  bool col, lock;

  kgmIcon*   icn = null;
  kgmVisual* geo = null;

private:
  ~kNode()
  {
    kgmObject* o = (kgmObject*)msh;

    if(o)
      o->release();

    if(icn)
      icn->release();

    if(geo)
      geo->release();
  }

public:
  kNode()
  {
    typ  = NONE;
    msh  = null;
    icn  = null;
    geo  = null;
    col  = false;
    lock = false;
  }

  kNode(kgmMesh* m)
  {
    typ = MESH;
    msh = new kgmVisual();
    msh->set(m);
    icn = null;
    geo = null;
    col = false;
    lock = false;

    if(m)
    {
      kgmMaterial* mtl = new kgmMaterial();
      msh->set(mtl);
      mtl->release();
    }
  }

  kNode(kgmUnit* u)
  {
    typ  = UNIT;
    unt  = u;
    icn  = null;
    geo  = null;
    col  = false;
    lock = false;

    if(u)
      u->increment();
  }

  kNode(kgmLight* l)
  {
    typ  = LIGHT;
    lgt  = l;
    icn  = null;
    geo  = null;
    col  = false;
    lock = false;

    if(l)
      l->increment();
  }

  kNode(kgmActor* a)
  {
    typ = ACTOR;
    act = a;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    if(a)
      a->increment();
  }

  kNode(kgmEffect* e)
  {
    typ = EFFECT;
    eff = e;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    if(e)
      e->increment();
  }

  kNode(kgmSensor* s)
  {
    typ = SENSOR;
    sns = s;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    if(s)
      s->increment();
  }

  kNode(kgmTrigger* t)
  {
    typ = TRIGGER;
    trg = t;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    if(t)
      t->increment();
  }

  kNode(kgmObstacle* o)
  {
    typ = OBSTACLE;
    obs = o;
    icn = null;
    geo = null;
    col = true;
    lock = false;

    if(o)
      o->increment();
  }

  kNode(const kNode& n)
  {
    typ = n.typ;
    col = n.col;
    shp = n.shp;
    pos = n.pos;
    rot = n.rot;
    bnd = n.bnd;
    lnk = n.lnk;
    lock = n.lock;

    obj = null;
    icn = null;
    geo = null;

    if(n.obj)
      obj = (kgmUnit*)n.obj->clone();
  }

  mtx4 getMatrix();
  void setPosition(vec3 v);
  void setRotation(vec3 r);
  void setConvex(kgmString s);

  void setMaterial(kgmMaterial* m);
  kgmMaterial* getMaterial();
};

}

#endif // KNODE_H
