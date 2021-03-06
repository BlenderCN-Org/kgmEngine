#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

/* Artifisial Inteligence */
class kgmIInput: public kgmInterface
{
public:
  virtual void keyUp(u8 key)   = 0;
  virtual void keyDown(u8 key) = 0;
  virtual void movePoint(s16 x, s16 y, s16 z) = 0;
};
