#ifndef KGMNODELIGHT_H
#define KGMNODELIGHT_H

#include "kgmNode.h"
#include "kgmLight.h"

class kgmNodeLight: public kgmNode
{
  kgmLight* m_light;

public:
  kgmNodeLight();
  ~kgmNodeLight();

  kgmIGraphics::TypeNode getNodeType()
  {
    return kgmIGraphics::NodeLight;
  }
};

#endif // KGMNODELIGHT_H
