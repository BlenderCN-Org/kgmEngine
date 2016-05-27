#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmBase/kgmPointer.h"

#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmVisual.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmIGame.h"
#include "../../kgmGame/kgmUnit.h"
#include "../../kgmGame/kgmActor.h"
#include "../../kgmGame/kgmEffect.h"
#include "../../kgmGame/kgmSensor.h"
#include "../../kgmGame/kgmTrigger.h"

#include "../../kgmPhysics/kgmObstacle.h"

namespace kgmGameEditor
{

#define BODY_ICO     "body_ico.tga"
#define UNIT_ICO     "unit_ico.tga"
#define LIGHT_ICO    "light_ico.tga"
#define ACTOR_ICO    "actor_ico.tga"
#define VISUAL_ICO   "visual_ico.tga"
#define EFFECT_ICO   "effect_ico.tga"
#define SENSOR_ICO   "sensor_ico.tga"
#define TRIGGER_ICO  "trigger_ico.tga"
#define OBSTACLE_ICO "obstacle_ico.tga"

class kNode : public kgmObject
{
  KGM_OBJECT(kNode);
public:
  enum Type
  {
    NONE,
    UNIT,
    LIGHT,
    ACTOR,
    VISUAL,
    EFFECT,
    SENSOR,
    TRIGGER,
    OBSTACLE,
    MATERIAL
  };

  union
  {
    kgmUnit*      unt;
    kgmLight*     lgt;
    kgmActor*     act;
    kgmVisual*    vis;
    kgmEffect*    eff;
    kgmSensor*    sns;
    kgmTrigger*   trg;
    kgmObstacle*  obs;
    kgmMaterial*  mtl;

    kgmObject*    obj = null;
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

public:
  kNode()
  {
    typ  = NONE;
    obj  = null;
    col  = false;
    lock = false;

    icn = null;
    mtl = null;
  }

  kNode(kgmVisual* v)
  {
    typ = VISUAL;
    vis = v;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) VISUAL_ICO));
  }

  kNode(kgmUnit* u)
  {
    typ  = UNIT;
    unt  = u;
    col  = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) UNIT_ICO));
  }

  kNode(kgmLight* l)
  {
    typ  = LIGHT;
    lgt  = l;
    col  = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) LIGHT_ICO));
  }

  kNode(kgmActor* a)
  {
    typ = ACTOR;
    act = a;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) ACTOR_ICO));
  }

  kNode(kgmEffect* e)
  {
    typ = EFFECT;
    eff = e;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) EFFECT_ICO));
  }

  kNode(kgmSensor* s)
  {
    typ = SENSOR;
    sns = s;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) SENSOR_ICO));
  }

  kNode(kgmTrigger* t)
  {
    typ = TRIGGER;
    trg = t;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) TRIGGER_ICO));
  }

  kNode(kgmObstacle* o)
  {
    typ = OBSTACLE;
    obs = o;
    col = true;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture((char*) OBSTACLE_ICO));
  }

  kNode(kgmMaterial* m)
  {
    typ = MATERIAL;
    mtl = m;
    col = true;
    lock = false;
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

    if(n.icn != null)
      icn = new kgmIcon(*n.icn);

    if(n.obj)
    {
      switch(n.typ)
      {
      case UNIT:
        unt = new kgmUnit(*n.unt);
        break;
      case LIGHT:
        lgt = new kgmLight(*n.lgt);
        break;
      case ACTOR:
        act = new kgmActor(*n.act);
        break;
      case VISUAL:
        vis = new kgmVisual(*n.vis);
        break;
      case EFFECT:
        eff = new kgmEffect(*n.eff);
        break;
      case SENSOR:
        sns = new kgmSensor(*n.sns);
        break;
      case TRIGGER:
        trg = new kgmTrigger(*n.trg);
        break;
      case OBSTACLE:
        obs = new kgmObstacle(*n.obs);
        break;
      };
    }
  }

  ~kNode()
  {
    delete obj;
    delete icn;
  }

  kgmObject* clone()
  {
    kNode* n = new kNode();

    n->typ = typ;
    n->col = col;
    n->shp = shp;
    n->pos = pos;
    n->rot = rot;
    n->bnd = bnd;
    n->lnk = lnk;
    n->lock = lock;

    if(icn != null)
      n->icn = new kgmIcon(*icn);

    if(obj)
    {
      switch(typ)
      {
      case UNIT:
        n->unt = new kgmUnit(*unt);
        break;
      case LIGHT:
        n->lgt = new kgmLight(*lgt);
        break;
      case ACTOR:
        n->act = new kgmActor(*act);
        break;
      case VISUAL:
        n->vis = new kgmVisual(*vis);
        break;
      case EFFECT:
        n->eff = new kgmEffect(*eff);
        break;
      case SENSOR:
        n->sns = new kgmSensor(*sns);
        break;
      case TRIGGER:
        n->trg = new kgmTrigger(*trg);
        break;
      case OBSTACLE:
        n->obs = new kgmObstacle(*obs);
        break;
      case MATERIAL:
        n->mtl = new kgmMaterial(*mtl);
        break;
      };
    }

    return n;
  }

  void update();

  mtx4 getMatrix();
  vec3 getPosition();
  vec3 getRotation();
  void setPosition(vec3 v);
  void setRotation(vec3 r);
};

}

#endif // KNODE_H
