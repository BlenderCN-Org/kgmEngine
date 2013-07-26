#include "kGui.h"
#include "../kgmBase/kgmLog.h"

kGui::kGui(kgmGameBase* game)
{
  m_guiMain = null;
  kgmString text;

  if(!game)
    return;
  else
    m_game = game;

  int x, y, w, h;

  game->getRect(x, y, w, h);
  kgm_log() << "GameRect: " << w << " " << h << ".";

  m_guiMain = new kgmGui(null, 0, 0, w, h);
  m_guiMain->addListener(this);

  kgmGui* gui = new kgmGui(m_guiMain, 0, 0, w, 50);

  kgmGuiButton* btn = new kgmGuiButton(gui, 0, 0, w/3, 50);
  text = "Game";
  btn->setSid("game");
  btn->setText(text);
  btn = new kgmGuiButton(gui, w/3, 0, w/3, 50);
  text = "Settings";
  btn->setSid("settings");
  btn->setText(text);
  btn = new kgmGuiButton(gui, 2 * w/3, 0, w/3, 50);
  text = "Quit";
  btn->setText(text);
  btn->setSid("quit");

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("guiSettings");
  gui->hide();
  m_guiSettings = gui;

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("guiGame");
  m_guiGame = gui;
  btn = new kgmGuiButton(gui, 1, 10, 100, 30);
  text = "Start";
  btn->setText(text);
  btn = new kgmGuiButton(gui, 1, 50, 100, 30);
  text = "Continue";
  btn->setText(text);

  kgmGuiList* lst = new kgmGuiList(gui, w / 2, 10, w / 2 - 2, h - 100);
  lst->m_itemHeight = 40;
  lst->setSid("levels");
  lst->addItem("level01.kgm");
  lst->addItem("map001.kgm");
  lst->addItem("map002.kgm");
  lst->addItem("map003.kgm");
  lst->addItem("map004.kgm");
  lst->addItem("map005.kgm");
  gui->show();

  game->guiAdd(m_guiMain);


  m_guiPause = new kgmGui(null, w / 2 - 50, h / 2 - 50, 100, 100);
  m_guiPause->addListener(this);
  m_guiPause->hide();

  btn = new kgmGuiButton(m_guiPause, 1, 1, 98, 48);
  btn->setText("Resume");
  btn->setSid("gameResume");

  btn = new kgmGuiButton(m_guiPause, 1, 51, 98, 48);
  btn->setText("Exit");
  btn->setSid("gameExit");

  game->guiAdd(m_guiPause);
}

kGui::~kGui()
{
}

void kGui::onAction(kgmEvent* e, int a)
{
  kgmGui* gui = (kgmGui*)e;

  kgmString sid = gui->getSid();

  if(sid == "quit")
  {
    m_game->gQuit();
#ifdef ANDROID
    extern kgm_android_exit();

    kgm_android_exit();
#endif
  }
  else if(sid == "game")
  {
    m_guiGame->show();
    m_guiSettings->hide();
  }
  else if(sid == "settings")
  {
    m_guiGame->hide();
    m_guiSettings->show();
  }
  else if(sid == "levels")
  {
    kgmGuiList* lst = (kgmGuiList*)gui;
    int i = lst->getSel();

    kgmString map = lst->getItem(i);
    int res = m_game->gLoad(map);

    if(res == kgmIGame::State_Play)
    {
      m_guiMain->hide();
      m_game->m_msAbs = true;
    }
  }
  else if(sid == "gameExit")
  {
    m_game->gUnload();
    m_guiPause->hide();
    m_guiMain->show();
    m_game->m_msAbs = false;
  }
  else if(sid == "gameResume")
  {
    m_guiPause->hide();
    m_game->gPause(false);
    m_game->m_msAbs = true;
  }
}

void kGui::viewAgain()
{
  m_guiMain->show();
  m_game->m_msAbs = false;
}
