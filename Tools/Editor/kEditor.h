#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGraphics/kgmGuiMenu.h"

#include "kPivot.h"
#include "kGridline.h"

class kEditor: public kgmGameBase
{

private:
  bool ms_click[3];
  vec3 cam_pos;
  f32  cam_rot;

  kGridline* gridline;
  kPivot*    pivot;

  kgmGuiMenu* menu;
public:
  kEditor();
  ~kEditor();

  void onKeyUp(int k);
  void onKeyDown(int k);
  void onMsMove(int k, int x, int y);
  void onMsWheel(int k, int x, int y, int z);
  void onMsLeftUp(int k, int x, int y);
  void onMsLeftDown(int k, int x, int y);
  void onMsRightUp(int k, int x, int y);
  void onMsRightDown(int k, int x, int y);
};

#endif // KEDITOR_H
