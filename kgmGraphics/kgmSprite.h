#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"
#include "kgm4Color.h"

class kgmSprite: public kgmObject{
  KGM_OBJECT(kgmSprite);

public:
  uRect  m_rect;
  bool   m_alpha;
  color  m_color;

protected:
 ~kgmSprite();

public:
 kgmSprite();

 void setRect(u32 x, u32 y, u32 w, u32 h)
 {
   m_rect = uRect(x, y, w, h);
 }
};
