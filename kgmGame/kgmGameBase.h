#pragma once

#include "kgmIGame.h"
#include "kgmGameResources.h"
#include "kgmGameGraphics.h"
#include "kgmGamePhysics.h"
#include "kgmGameLogic.h"

#include "kgmGameObject.h"
#include "kgmActor.h"

#include "kgmGameSettings.h"

#include "../kgmBase/kgmQuadTree.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmSystem/kgmOGLWindow.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGraphics.h"

#ifdef WIN32
#endif
#ifdef LINUX
#endif

class kgmGameBase: public kgmIGame, public kgmOGLWindow
{
public:
 static kgmGameBase*	m_game;

protected:
 kgmIAudio*		    m_audio;
 kgmIVideo*		    m_video;
 kgmISpawner*		m_spawner;
 kgmIGraphics*		m_graphics;
 kgmGamePhysics*	m_physics;
 kgmGameResources*	m_resources;
 kgmGameSettings*   m_settings;

 kgmSystem*         m_system;

 u32		    m_width;
 u32		    m_height;
 u32		    m_bpp;
 u32		    m_fullscreen;

 kgmFont*       m_font;
 kgmString      m_paths;

 char           m_input[gbtn_end];
 char           m_keymap[150];

 bool       m_gamemode;
public:
 int		    m_state;	//game state
 bool		    m_result;	//game over result

 kgmGameLogic*         m_logic;
 kgmGameGraphics*      m_render;
 //kgmList<kgmObject*>   m_objects;
 kgmList<kgmGui*>      m_guis;     //game or nongame guis

 ////////////////
public:
 kgmGameBase();
 kgmGameBase(kgmString &conf);
 virtual ~kgmGameBase();

 kgmIGraphics*      getGraphics();
 kgmIPhysics*       getPhysics();
 kgmISpawner*       getSpawner();
 kgmIAudio*         getAudio();
 kgmIVideo*         getVideo();
 kgmIResources*     getResources();
 kgmSystem*         getSystem();
 kgmWindow*         getWindow();

 void quit();
 void log(const char* msg);

 void onEvent(kgmEvent::Event*);
 void onIdle();
 void onClose();
 void onKeyUp(int k);
 void onKeyDown(int k);
 void onMsMove(int k, int x, int y);
 void onMsWheel(int k, int x, int y, int z);
 void onResize(int w, int h);
 void onPaint(kgmIGraphics*);
 void onAction(kgmGui* e, u32 arg);

 void onInputAction(int action, int argument);

 int  gLoad(kgmString);     //load game level
 int  gUnload();            //load game level
 int  gCommand(kgmString);  //do string command
 int  gQuit();              //quit from level
 u32  gState();             //check game state
 void gPause(bool);         //render game scene
 void gRender();            //render game scene

 kgmActor*    gSpawn(kgmString); //spawns and add the actor

 bool gMapBinary(kgmString&);
 bool gMapAscii(kgmString&);
 bool gMapXml(kgmString&);

///////////////////////////////////////

private:
//init interfaces
 virtual void  initResources();
 virtual void  initGraphycs();
 virtual void  initPhysics();
 virtual void  initSystem();
 virtual void  initAudio();
 virtual void  initLogic();
protected:

// Game Resources
 kgmTexture*    getTexture(char*);
 kgmShader*	    getShader(char*);
 kgmMesh*	      getMesh(char*);
 kgmActor*	    getActor(kgmString);
 kgmAnimation*	getAnimation(char*);

 kgmGameNode*   loadXml(kgmString& path);
 bool           loadXml_II(kgmString& path);


public:
 void addGui(kgmGui* g)
 {
   if(g){
     bool is = false;
     for(int i = 0; i < m_guis.size(); i++)
       if(g == m_guis[i]) { is = true; break; }
     if(!is){
       m_guis.add(g);
       if(m_render)
         m_render->add(g);
     }
   }
 }
 
 kgmGraphics* getRender(){
     return 0; //m_render;
 }
};
