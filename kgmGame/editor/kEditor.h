#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGraphics/kgmGuiMenu.h"

#include "kNode.h"
#include "kLine.h"
#include "kArrow.h"
#include "kPivot.h"
#include "kGridline.h"
#include "kFileDialog.h"
#include "kViewObjects.h"
#include "kViewOptions.h"

class kgmGameBase;

namespace kgmGameEditor
{

class kEditor: public kgmEvent
{
private:

  enum
  {
    ViewPerspective,
    ViewFront,
    ViewLeft,
    ViewTop
  };

  kgmGameBase* game;

  bool ms_click[3];

  vec3 cam_pos;
  f32  cam_rot;

  vec3 cam_pos_bk;
  vec3 cam_dir_bk;

  //kGridline* gridline;
  //kPivot*    pivot;
  kgmVisual *gridline, *pivot;
  vec3       pv_pos;
  vec3       pv_rot;

  //kMenu*     menu;

  //kFileDialog* fdd;

  u32  oquered;
  u32  view_mode;

  //kViewOptions* vop;


  kgmList<kNode*> nodes;

  kNode* selected;

  kgmMaterial *mtlLines, *mtlPivot;

  kgmVisual*     logView;

  bool  move_camera;

private:
  ~kEditor();

public:
  kEditor(kgmGameBase* game);

  void clear();

  void select(int x, int y);
  void select(kgmString name);

  kgmRay3d<float> getPointRay(int x, int y);

  __stdcall bool fdMapSave(kFileDialog*);
  __stdcall bool fdMapOpen(kFileDialog*);
  __stdcall bool fdAddMesh(kFileDialog*);
  __stdcall bool addUnit(kgmString);
  __stdcall bool addActor(kgmString);
  __stdcall bool addEffect(kgmString);
  __stdcall bool addSensor(kgmString);

  bool mapSave(kgmString);
  bool mapOpen(kgmString);
  bool addMesh(kgmString);

  void initPhysics();
  void initLogic();

  void onIdle();
  void onEvent(kgmEvent::Event*);
  void onAction(kgmEvent*,int);
  void onKeyUp(int k);
  void onKeyDown(int k);
  void onMsMove(int k, int x, int y);
  void onMsWheel(int k, int x, int y, int z);
  void onMsLeftUp(int k, int x, int y);
  void onMsLeftDown(int k, int x, int y);
  void onMsRightUp(int k, int x, int y);
  void onMsRightDown(int k, int x, int y);

  __stdcall void onQuit();
  __stdcall void onEditClone();
  __stdcall void onEditRemove();
  __stdcall void onEditOptions();
  __stdcall void onMapOpen();
  __stdcall void onMapSave();
  __stdcall void onAddMesh();
  __stdcall void onAddUnit();
  __stdcall void onAddLight();
  __stdcall void onAddActor();
  __stdcall void onAddEffect();
  __stdcall void onAddSensor();
  __stdcall void onAddTrigger();
  __stdcall void onAddObstacle();
  __stdcall void onViewObjects();
  __stdcall void onViewPerspective();
  __stdcall void onViewFront();
  __stdcall void onViewLeft();
  __stdcall void onViewTop();
  __stdcall void onOptionsDatabase();
  __stdcall void onRunRun();
  __stdcall void onSelectObject(kgmString);
  __stdcall void onCloseVop();
};

}

#endif // KEDITOR_H
