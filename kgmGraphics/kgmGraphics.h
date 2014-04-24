#pragma once
#include "../kgmBase/kgmInterface.h"
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmSystem/kgmWindow.h"

#include "kgmNode.h"
#include "kgmGui.h"
#include "kgmGuiTab.h"
#include "kgmGuiList.h"
#include "kgmGuiText.h"
#include "kgmGuiButton.h"
#include "kgmGuiScroll.h"
#include "kgmGuiProgress.h"
#include "kgmGuiStyle.h"
#include "kgmIGuiDraw.h"

#include "kgmMesh.h"
#include "kgmText.h"
#include "kgmLight.h"
#include "kgmSprite.h"
#include "kgmCamera.h"
#include "kgmVisual.h"
#include "kgmMaterial.h"

#ifdef DEBUG
#include "../kgmPhysics/kgmBody.h";
#endif

class kgmGraphics: public kgmObject
{
  KGM_OBJECT(kgmGraphics);
public:

  enum GraphicsQuality
  {
    GraphicsQualityLow,
    GraphicsQualityMedium,
    GraphicsQualityHight,
    GraphicsQualitySuper
  };

  static GraphicsQuality   textureQuality;
  static GraphicsQuality   shadowQuality;

  struct Mesh
  {
    kgmMaterial* material;
    kgmMesh*     mesh;
    mtx4         mtx;
  };

  class Icon: public kgmObject
  {
    kgmTexture* icon;
    float       width,
                height;
    vec3        position;

    bool        remove;

  public:
    Icon()
    {
      icon = null;
      width = 1;
      height = 1;
      position = vec3(0, 0, 0);
      remove = false;
    }

    Icon(kgmTexture* c, float w, float h, vec3 v)
    {
      icon = c;
      width = w;
      height = h;
      position = v;
      remove = false;

      if(c)
        c->increment();
    }

    ~Icon()
    {
      if(icon)
        icon->release();
    }

    kgmTexture* getIcon() { return icon; }
    float       getWidth() { return width; }
    float       getHeight() { return height; }
    vec3        getPosition() { return position; }
    void        setPosition(vec3 v) { position = v; }

    bool        getRemove() { return remove; }
    void        setRemove(bool r) { remove = r; }
  };

private:
  kgmIGC* gc;
  kgmIResources* rc;

  kgmFont* font;

  iRect       m_viewport;
  kgmCamera   m_camera;

  kgmList<Mesh>         m_meshes;
  kgmList<kgmMaterial*> m_materials;
  kgmList<kgmLight*>    m_lights;
  kgmList<kgmVisual*>   m_visuals;
  kgmList<kgmGui*>      m_guis;
  kgmList<Icon*>        m_icons;

#ifdef DEBUG
  kgmList<kgmBody*>     m_bodies;
#endif

  kgmGuiStyle* gui_style;

  void* tcolor;
  void* tnormal;
  void* tspecular;
  void* tdepth;
  void* shader;
  mtx4  location;

  kgmTab<u16, kgmShader*>  shaders;
  kgmTab<u16, kgmTexture*> textures;

  bool  m_has_shaders;
  bool  m_has_buffers;

  bool  m_depth;
  bool  m_alpha;
  bool  m_culling;

  bool  m_editor;

  uint32 m_max_faces;

  u32    m_bg_color;
public:
  kgmGraphics(kgmIGC*, kgmIResources*);
  ~kgmGraphics();

  void clear();
  void build();
  void render();
  void update();
  void resize(float, float);
  void setEditor(bool);

  void setGuiStyle(kgmGuiStyle*);
  void setDefaultFont(kgmFont*);

  s32  getShaderId(kgmString);

private:
  void render(kgmGui*);
  void render(kgmMesh*);
  void render(kgmVisual*);
  void render(kgmSprite*);
  void render(kgmShader*);
  void render(kgmMaterial*);
  void render(kgmParticles*);
  void render(Icon*);

  void renderGuiMenuItem(void*);

  void gc2DMode();
  void gc3DMode();
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);
  void gcDrawBillboard(box b, u32 col);

  void trash();

  void setProjMatrix(mtx4&);
  void setViewMatrix(mtx4&);
  void setWorldMatrix(mtx4&);

public:
  void add(kgmMaterial* mtl)
  {
    if(mtl)
    {
      m_materials.add(mtl);
      mtl->increment();
    }
  }

  void add(kgmLight* lgt)
  {
    if(!lgt)
      return;

    lgt->increment();
    m_lights.add(lgt);
  }

  void add(kgmMesh* mesh, kgmMaterial* mtl, mtx4* mtx = null)
  {
    Mesh m;

    if(!mesh)
      return;

    if(mesh) mesh->increment();
    if(mtl)  mtl->increment();

    m.mesh = mesh;
    m.material = mtl;

    if(mtx)
      m.mtx = *mtx;
    else
      m.mtx.identity();

    m_meshes.add(m);
  }

  void add(kgmVisual* a)
  {
    if(!a)
      return;

    a->increment();

    m_visuals.add(a);
  }

  void add(Icon* ico)
  {
    if(!ico)
      return;

    ico->increment();
    m_icons.add(ico);
  }

  void add(kgmGui* gui)
  {
    if(gui)
    {
      gui->increment();

      m_guis.add(gui);
    }
  }

#ifdef DEBUG
  void add(kgmBody* a)
  {
    if(a)
    {
      m_bodies.add(a);
      a->increment();
    }
  }
#endif

  bool get(kgmString name, kgmMaterial** mtl)
  {
    for(int i = 0; i < m_materials.size(); i++)
    {
      kgmMaterial* m = m_materials[i];

      if(m && m->m_id.length() && (m->m_id == name))
      {
        *mtl = m;

        return true;
      }
    }

    return false;
  }

  bool set(kgmMesh* msh, kgmMaterial* mtl)
  {
    if(!msh)
      return false;

    for(int i = 0; i < m_meshes.length(); i++)
    {
      Mesh* mesh = &m_meshes[i];

      if(mesh->mesh == msh)
      {
        if(mesh->material)
          mesh->material->release();

        if(mtl)
          mtl->increment();

        mesh->material = mtl;

        return true;
      }
    }

    return false;
  }

  bool set(kgmMesh* msh, mtx4 m)
  {
    if(!msh)
      return false;

    for(int i = 0; i < m_meshes.length(); i++)
    {
      Mesh* mesh = &m_meshes[i];

      if(mesh->mesh == msh)
      {
        mesh->mtx = m;

        return true;
      }
    }

    return false;
  }

  iRect viewport()
  {
    return m_viewport;
  }

  kgmCamera& camera()
  {
    return m_camera;
  }

  void setBgColor(u32 col)
  {
    m_bg_color = col;
  }
};
