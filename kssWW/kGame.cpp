// kGame.cpp: implementation of the kGame class.
//
//////////////////////////////////////////////////////////////////////

#include "kGame.h"
#include "../kgmMain/kgmString.h"
#include "../kgmMain/kgmMemory.h"
#include "../kgmMain/kgmStack.h"
#include "../kgmMain/kgmMap.h"
#include "../kgmMain/kgmXml.h"
#include "../kgmMain/kgmFile.h"
#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmGameResources.h"

//map name, map id
const char* g_maps[] = {
  "map01", "map01.map",
  "map02", "map01.map",
  "map03", "map01.map",
  "map04", "map01.map",
  "map05", "map01.map",
  0, 0,
};

//useful for xml values reading
int   toInt(const char* s){
 return atoi(s);
}

double toFloat(const char* s){
 return atof(s);
}

inline void xmlAttrFloat(kgmXml::Node* node, const char* attr, float* val){
 kgmString s;

 if(!node)
   return;

 s = attr;
 for(int i = 0; i < node->attributes(); i++){
  kgmString a, v;
  node->attribute(i, a, v);
  if(s == a){
   *val = atof(v.data());
  }
 }
}

inline void xmlAttrInt(kgmXml::Node* node, const char* attr, float* val){
 kgmString s;

 if(!node)
   return;

 s = attr;
 for(int i = 0; i < node->attributes(); i++){
  kgmString a, v;
  node->attribute(i, a, v);
  if(s == a){
   *val = atoi(v.data());
  }
 }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class kGame::Gui: public kgmGameBase::Gui{
 typedef struct{
  kgmGui* gui;
  kgmString id;
 }	GUI;
public:
 static const char class_name[];
 static kgmLuaBind<kGame::Gui>::Reg Register[];

 kgmMap<kgmString, kgmGui*>   m_guis;
 kgmMap<kgmGui*, kgmString>   m_guis_r;

 kgmGui*	  m_gui;     //current activated gui
 kgmString        m_sid;
public:
 Gui()
 :kgmGameBase::Gui(0, 0, 0, 100, 100){
 }
 Gui(lua_State* l)
 :kgmGameBase::Gui(0, 0, 0, 100, 100){
  printf("Gui\n");
 }
 ~Gui(){
  m_guis.clear();
 }
 int _class(lua_State* l){
  lua_pushlightuserdata(l, this);
  return 1;
 }
 int _show(lua_State* l){
  kgmString   id = (char*)lua_tostring(l, 2);
  int         st = (int)lua_tointeger(l, 3);
  kgmGui*     gu = 0;
  if(id == "self")
   gu = this;
  else
   m_guis.get(id, gu);

  if(gu)
   if(st)
	gu->show();
   else
	gu->hide();

  return 0;
 }
 int load(lua_State* l){
  kgmXml*     xml;
  kgmFile*    file;
  kgmString   id = (char*)lua_tostring(l, 2);
  file = new kgmFile();
  if( !file->open(id, kgmFile::Read) ){
   file->close();
   delete file;
   return false;
  }
  void* m = file->mmap();
  xml = new kgmXml(m, file->length());

  m_gui = 0;
  read(xml->m_node, this);
  delete xml;
  file->munmap(m);
  file->close();
  delete file;

  if(!m_gui)
   return false;
  m_gui = 0;
  return true;
 }

  void read(kgmXml::Node* n, kgmGui* par){
  if(!n)
   return;
  kgmGui *gui = 0;
  kgmString id, val;
  n->id(id);
  if(id == "kgmGui"){
   gui = new kgmGui();
   gui->setParent(par);
  }else if(id == "kgmGuiButton"){
   gui = new kgmGuiButton();
   gui->setParent(par);
  }else if(id == "kgmGuiList"){
   gui = new kgmGuiList();
   gui->setParent(par);
   gui->setColor(0, 0xffffff00);
  }else if(id == "kgmListItem"){
    gui = par;
  }
  
  for(int i = 0; i < n->attributes(); i++){
   n->attribute(i, id, val);
   if(id == "id"){
    m_guis.add(val, gui);
    m_guis_r.add(gui, val);
   }else if(id == "rect"){
    int r[4];
    sscanf(val.data(), "%i %i %i %i", &r[0], &r[1], &r[2], &r[3]);
    gui->setRect(r[0], r[1], r[2], r[3]);
   }else if(id == "text"){
    gui->setText(val);
   }else if(id == "show"){
    int sh = 0;
    sscanf(val.data(), "%i", &sh);
    if(sh)
      gui->show();
    else
      gui->hide();
   }else if(id == "item"){
    ((kgmGuiList*)gui)->addItem(val);
   }else if(id == "maps"){
    for(int i = 0; g_maps[i]; i+=2){
     kgmString s;
     s = g_maps[i];
     ((kgmGuiList*)gui)->addItem(s);
    }
   }
  }

  for(int i = 0; i < n->nodes(); i++)
    read(n->node(i), gui);
 }
 void align(kgmGui* gui, double w, double h){
  Rect rc;
  gui->getRect(rc);
  rc.x *= w;
  rc.y *= h;
  rc.w *= w;
  rc.h *= h;
  gui->setRect(rc);
  for(int i = 0; i < gui->m_childs.length(); i++)
   align(gui->m_childs[i], w, h);
 }
 void onResize(int w, int h){
  Rect rc;
  getRect(rc);
  double x = (double)w / (double)rc.w;
  double y = (double)h / (double)rc.h;
  align(this, x, y);
 }
 void onAction(kgmGui* g, uint a){
  if(!g)
   return;
  kgmString s;
  if(m_guis_r.get(g, s))
   kGame::onAction(this, s, a);
 }
};
const char kGame::Gui::class_name[] = "Gui";
kgmLuaBind<kGame::Gui>::Reg kGame::Gui::Register[] = {
 {"class", &kGame::Gui::_class},
 {"show",  &kGame::Gui::_show},
 {"load",  &kGame::Gui::load},
 {0, 0}
};


class kGame::Actor: public kgmActor{
public:
 static const char class_name[];
 static kgmLuaBind<kGame::Actor>::Reg Register[];
public:
 Actor(lua_State* l):kgmActor(){

 }
 ~Actor(){

 }
 int _class(lua_State* l){
  lua_pushlightuserdata(l, this);
  return 1;
 }
};
const char kGame::Actor::class_name[] = "Actor";
kgmLuaBind<kGame::Actor>::Reg kGame::Actor::Register[] = {
 {"class", &kGame::Actor::_class},
 {0, 0}
};

class kGame::Game: public kgmGameBase{
 struct Node{
  uint type;
  union{
   kgmMesh* mesh;
   kgmLight* light;
   kgmActor* actor;
  };
 };
public:
 static const char class_name[];
 static kgmLuaBind<kGame::Game>::Reg Register[];

 kgmList<kgmMaterial*> m_materials;
 kgmList<kgmLight*>    m_lights;
 kgmList<kgmMesh*>     m_meshs;
  kgmList<kgmActor*>   m_actors;
public:
 Game(lua_State* l){
 }
 ~Game(){

 }
 int quit(lua_State* l){
  gQuit();
  m_loop = false;
  return 0;
 }
 int start(lua_State* l){
  loop();
  return 0;
 }
 void clear(){
  
 }
 int load(lua_State* l){
  printf("loading...\n");
  int i = lua_tointeger(l, 2);
  kgmString s;
  kgmMemory<char> m;
  s = g_maps[2 * i];
  ((kgmGameResources*)m_resources)->getFile(s, m);
  if(m.length() < 1){
   printf("Can't load map %s\n", (char*)s);
   return 0;
  }

  kgmXml xml(m.data(), m.length());
  if(!xml.m_node){
   printf("no or not valid xml data\n");
   return 0;
  }

  return 0;
 }
 int addGui(lua_State* l){
  kGame::Gui* gui = (kGame::Gui*)lua_topointer(l, 2);
  kgmGameBase::addGui((kgmGameBase::Gui*)gui);
  return 0;
 }
 void addNode(kgmXml::Node* node){
 }
 kgmActor* spawn(kgmString& id){
  return 0;
 }
};


/////////////////////////////////////////////
//////
const char kGame::Game::class_name[] = "Game";
kgmLuaBind<kGame::Game>::Reg kGame::Game::Register[] = {
 {"quit",   &kGame::Game::quit},
 {"start",  &kGame::Game::start},
 {"load",   &kGame::Game::load},
 {"addGui", &kGame::Game::addGui},
 {0, 0}
};


kGame* kGame::m_game = 0;

kGame::kGame(){
 m_game = this;
 kgmLuaBind<Gui>::init(m_lua);
 kgmLuaBind<Actor>::init(m_lua);
 kgmLuaBind<Game>::init(m_lua);
 load("main.lua");
}

kGame::~kGame(){

}

void kGame::onAction(Gui* gui, kgmString& g, uint a){
 if(!m_game || !m_game->m_lua)
  return;
 m_game->call("onAction", 0, "vsi", gui, (char*)g, a);
}
