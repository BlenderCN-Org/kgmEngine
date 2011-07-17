#pragma once
#include "kgmBase.h"
#include "../kgmMath/kgmRect.h"
#include "../kgmMain/kgmEvent.h"
#include "../kgmMain/kgmBase.h"
#include "../kgmMain/kgmList.h"
#include "../kgmMain/kgmString.h"
#include "../kgmMain/kgmTab.h"


//Class of GUI
class   kgmGui;
class   kgmFont;
class	kgmTexture;
class	kgmMaterial;
class   kgmIGraphics;

class kgmGui: public kgmEvent
{
public:
 typedef kgmRect<int>	  Rect;
 enum Color{
  BACKGROUND,
  FOREGROUND,
  SELECTED,
  TEXT,
  EXTRA = 8,
 };

private:
 kgmIGraphics* m_gc;
 kgmFont*      m_font;	   

 // typedef struct{
  
 //} GC;

public:
typedef kgmList<kgmGui*> Guis;
 Guis	       m_childs;

public:
 uint       m_id;
 kgmGui*    m_parent;  //parent window whom send messages
 kgmGui*    m_focus;   //current child active window
 Rect       m_rect;    //rect of window
 bool       m_view;    //view status of window
 uint       m_colors[EXTRA];   //gui  colors
 kgmString  m_text;	   //gui text
 kgmTexture*  m_texture[EXTRA];
 kgmMaterial* m_material[EXTRA]; //selected material

 void*        m_xdata;
public:
 kgmGui();
 kgmGui(kgmGui *par, int x, int y, int w, int h);
 virtual ~kgmGui();

 void show();
 void hide();
 void scale(float, float);
 void resize(int, int);
 void repaint();
 void repaint(kgmIGraphics* gc);
 void setGC(kgmIGraphics* gc);
 void setParent(kgmGui*);
 void setRect(Rect&);
 void setRect(uint, uint, uint, uint);
 void getRect(Rect&);
 void getRect(uint&, uint&, uint&, uint&);
 void setText(kgmString&);
 void setFont(kgmFont*);
 void setColor(uint, uint);
 void setTexture(uint, kgmTexture*);
 void setMaterial(uint, kgmMaterial*);

 void  setXdata(void* x){ m_xdata = x; }
 void* getXdata(){ return m_xdata; }

 // MESSAGE MANAGER
 virtual void onEvent(kgmEvent::Event* e);
 virtual void onAction(kgmGui* from, uint type);
 virtual void onIdle();

protected:
 virtual void onMsWheel(int k, int x, int y, int z);
 virtual void onMsMove(int k, int x, int y);
 virtual void onMsLeftDown(int k, int x, int y);
 virtual void onMsLeftUp(int k, int x, int y);
 virtual void onMsRightDown(int k, int x, int y);
 virtual void onMsRightUp(int k, int x, int y);
 virtual void onKeyDown(int k);
 virtual void onKeyUp(int k);
 virtual void onPaint(kgmIGraphics* gc);

//useful functions
 long2         toScreen(long2 pt);
 void          addChild(kgmGui *e);
 void          delChild(kgmGui *e);
 bool          isChild(kgmGui * e);
 kgmGui*       getFocus(long2 pos);
 kgmFont*      getFont();
 kgmIGraphics* getGC();


 //*************** DRAWING ***************
 void gcDrawRect(kgmIGraphics* gc, Rect rc, uint col, void* tex);
 void gcDrawText(kgmIGraphics* gc, kgmFont* font, uint fwidth, uint fheight, uint fcolor, Rect clip, kgmString& text);
};




