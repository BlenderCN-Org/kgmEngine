#ifndef KGMGAMELOGIC_H
#define KGMGAMELOGIC_H

#include "../kgmBase/kgmObject.h"
#include "kgmActor.h"

class kgmGameLogic : public kgmObject
{
public:

  class AI
  {
  public:
    virtual void logic(kgmActor* a){}
  };

  class Sensor
  {
    kgmActor* actor;

  public:
    Sensor(kgmActor* a)
    {
      actor = a;
    }

    virtual void sense(){}
  };

public:
    kgmGameLogic();

    void clear();
};

#endif // KGMGAMELOGIC_H
