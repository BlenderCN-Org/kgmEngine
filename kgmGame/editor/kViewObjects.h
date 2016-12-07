#ifndef KVIEWOBJECTS_H
#define KVIEWOBJECTS_H

#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiList.h"
#include "../../kgmGraphics/kgmGuiButton.h"

namespace kgmGameEditor
{

class kViewObjects : public kgmGuiFrame
{
public:
  Signal<u32> sigClose;
  Signal<kgmString> sigSelect;

private:
  kgmEvent* target;
  kgmGuiList* list;

  Slot<kViewObjects, u32> slotSelect;

public:
  kViewObjects(kgmEvent* tar = null, int x = 100, int y = 100, int w = 150, int h = 300);

  void onClose();

  __stdcall void onSelectItem(u32);

  void onAction(kgmGui* g, u32 id)
  {
    target->onAction(this, id);
  }

  bool addItem(kgmString item)
  {
    if(hasItem(item))
      return false;

    list->addItem(item);

    return true;
  }

  bool hasItem(kgmString item)
  {
    for(int i = 0; i < list->m_items.length(); i++)
    {
      if(item == list->m_items[i])
        return true;
    }

    return false;
  }
};

}

#endif // KVIEWOBJECTS_H
