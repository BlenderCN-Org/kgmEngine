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

  kgmList<AbstractSlot<>*>   slots;
  kgmMap<kgmGui*, kgmString> slotters;

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

  __stdcall void onSlotGuiButton(kgmGui*, int);

 private:
  void free();

  static void kgmLog(void*);
  static void kgmGameExit(void*);
  static void kgmGameLoad(void*);
  static void kgmGameEdit(void*);
  static void kgmGamePlay(void*);
  static void kgmGamePause(void*);
  static void kgmGameState(void*);
  static void kgmGuiLoad(void*);
  static void kgmGuiShow(void*);
};
