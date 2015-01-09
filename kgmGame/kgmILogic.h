#ifndef KGMILOGIC_H
#define KGMILOGIC_H

#include "../kgmBase/kgmString.h"

template <class T> class kgmList;

class kgmUnit;
class kgmActor;
class kgmEffect;
class kgmSensor;
class kgmTrigger;
class kgmGameObject;

class kgmILogic
{
public:
  enum ACTION
  {
    ACTION_NONE,
    ACTION_OBJECT,
    ACTION_GAMEOBJECT
  };

public:
  virtual bool add(kgmUnit*)                           = 0;
  virtual bool add(kgmActor*)                          = 0;
  virtual bool add(kgmEffect*)                         = 0;
  virtual bool add(kgmSensor*)                         = 0;
  virtual bool add(kgmTrigger*)                        = 0;

  virtual bool remove(kgmGameObject*)                  = 0;

  virtual bool addGameplayer(kgmActor*)                = 0;
  virtual bool delGameplayer(kgmActor*)                = 0;

  virtual void clear()                                 = 0;
  virtual void build()                                 = 0;
  virtual void update(u32)                             = 0;
  virtual void action(ACTION, kgmObject*, kgmString)   = 0;
  virtual void collide(kgmGameObject*, kgmGameObject*) = 0;

  virtual kgmGameObject* getObjectById(kgmString)                    = 0;
  virtual u32            getObjects(kgmList<kgmGameObject*>&)        = 0;
  virtual u32            getObjectsByType(kgmRuntime&,
                                          kgmList<kgmGameObject*>&)  = 0;
  virtual u32            getObjectsByClass(kgmRuntime&,
                                           kgmList<kgmGameObject*>&) = 0;
};

#endif // KGMILOGIC_H
