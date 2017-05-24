#ifndef LIGHTRENDER_H
#define LIGHTRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "BaseRender.h"

class LightRender: public BaseRender
{
public:
  LightRender(kgmGraphics* gr);

  void render(kgmList<kgmLight*>&, kgmArray<kgmVisual*>&);
};

#endif // LIGHTRENDER_H
