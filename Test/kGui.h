#ifndef KGUI_H
#define KGUI_H

#include "../kgmBase/kgmObject.h"
#include "../kgmGame/kgmGameBase.h"

class kGui : public kgmEvent
{
public:
    kgmGameBase* m_game;
    kgmGui*      m_guiMain;
    kgmGui*      m_guiGame;
    kgmGui*      m_guiSettings;
    kgmGui*      m_guiPause;

public:
    kGui(kgmGameBase*);
    virtual ~kGui();
    void onAction(kgmEvent*, int);
};

#endif // KGUI_H
