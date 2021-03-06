#include "../kgmScript/kgmIScript.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmList.h"

class kgmIGame;
class kgmGui;

class kgmGameScript: public kgmObject
{
  kgmIScript* handler;
  kgmIGame*   game;

  bool script = false;

  kgmMap<kgmGui*, kgmString> slotters;
  kgmList< Slot<kgmGameScript>* > slots;

 public:
  kgmGameScript(kgmIGame*);
  ~kgmGameScript();

  void init();
  void update();

  void setSlot(kgmGui*, kgmString);

  kgmIScript* getHandler() const
  {
    return handler;
  }

  __stdcall void onSlotGuiMenu(kgmGui*, u32);
  __stdcall void onSlotGuiList(kgmGui*, u32);
  __stdcall void onSlotGuiText(kgmGui*, kgmString);
  __stdcall void onSlotGuiCheck(kgmGui*, bool);
  __stdcall void onSlotGuiButton(kgmGui*, int);

 private:
  void free();

  static s32 kgmLog(void*);

  static s32 kgmImport(void*);

  static s32 kgmGameExit(void*);
  static s32 kgmGameLoad(void*);
  static s32 kgmGameEdit(void*);
  static s32 kgmGamePlay(void*);
  static s32 kgmGamePause(void*);
  static s32 kgmGameState(void*);

  static int kgmGuiLoad(void*);
  static s32 kgmGuiShow(void*);
  static s32 kgmGuiResize(void*);
  static s32 kgmGuiResolution(void*);

  static s32 kgmScreenResolution(void*);
};
