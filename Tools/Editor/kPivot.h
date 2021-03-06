#ifndef KGMPIVOT_H
#define KGMPIVOT_H

#include "../../kgmGraphics/kgmMesh.h"

class kPivot: public kgmMesh
{
public:
  enum
  {
    AXIS_NONE,
    AXIS_X,
    AXIS_Y,
    AXIS_Z
  };

public:
  vec3 pos, rot;
  u32  axis;

public:
  kPivot();

  u32 peekAxis(ray3 r);
};

#endif // KGMPIVOT_H
