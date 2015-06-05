#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmILogic.h"
#include "kgmUnit.h"
#include "kgmActor.h"
#include "kgmEffect.h"
#include "kgmSensor.h"
#include "kgmTrigger.h"

class kgmGameLogic : public kgmILogic, public kgmObject
{

  kgmActor* m_gameplayer;

public:
  kgmList<kgmUnit*> m_objects;

  s32 gcount;

public:
  kgmGameLogic();
  virtual ~kgmGameLogic();

  void clear();
  bool add(kgmUnit*);
  bool add(kgmActor*);
  bool add(kgmEffect*);
  bool add(kgmSensor*);
  bool add(kgmTrigger*);
  bool remove(kgmUnit*);
  bool chooseLogic(kgmString);
  bool isValid(kgmUnit*);

  virtual void build();
  virtual void input(int, int);
  virtual void update(u32 milliseconds);
  virtual void collide(kgmUnit*, kgmUnit*);

  virtual kgmUnit* getObjectById(kgmString&);

  virtual u32 getObjects(kgmList<kgmUnit*>&);
  virtual u32 getObjectsByType(kgmRuntime&, kgmList<kgmUnit*>&);
  virtual u32 getObjectsByClass(kgmRuntime&, kgmList<kgmUnit*>&);

  void      setPlayer(kgmActor* gp)
  {
    if(m_gameplayer)
      m_gameplayer->release();

    m_gameplayer = gp;

    if(m_gameplayer)
      m_gameplayer->increment();
  }

  kgmActor* getPlayer()
  {
    return m_gameplayer;
  }

private:
  void trush();
};

#endif // KGMGAMELOGIC_H
