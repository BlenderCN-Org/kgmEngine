#ifndef KVIEWOBJECTS_H
#define KVIEWOBJECTS_H
#include "../../kgmGraphics/kgmGui.h"
#include "../../kgmGraphics/kgmGuiList.h"

class kViewObjects : public kgmGui
{
  kgmEvent* target;
  kgmGuiList* list;

public:
  kViewObjects(kgmEvent* tar, int x, int y, int w, int h);

  kgmGuiList* getGuiList()
  {
    return list;
  }

  void onAction(kgmGui* g, u32 id)
  {
    target->onAction(this, id);
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
#endif // KVIEWOBJECTS_H
