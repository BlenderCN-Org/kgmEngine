#ifndef KGMSNINPUTLISTENER_H
#define KGMSNINPUTLISTENER_H

#include "../kgmSensor.h"

class kgmSnInputListener : public kgmSensor
{
  KGM_UNIT(kgmSnInputListener);

public:
  kgmSnInputListener(kgmIGame* g)
  {

  }

  ~kgmSnInputListener()
  {

  }

  virtual void input(u32, int)
  {

  }
};
#endif // KGMSNINPUTLISTENER_H
