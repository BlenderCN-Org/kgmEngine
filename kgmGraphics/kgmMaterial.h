#pragma once
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmPointer.h"
#include "kgm4Color.h"
#include "kgmShader.h"

class kgmIGC;
class kgmTexture;
class kgmShader;

#define KGM_TEXTURES_PER_MATERIAL 8

class kgmMaterial: public kgmObject
{
  KGM_OBJECT(kgmMaterial);

protected:
  kgmIGC*  m_gc;

public:
  enum Flags
  {
    MF_Color     = 1 << 0L,
    MF_Depth     = 1 << 1L,
    MF_Stencil   = 1 << 2L,
    MF_Blend     = 1 << 3L,
    MF_Alpha     = 1 << 4L,
    MF_Cull      = 1 << 5L,
    MF_Textures  = 1 << 6L,
  };

  class Color
  {
  public:
    union
    {
      struct { float r, g, b, a; };
      float c[4];
    };

    Color()
    {
      r = g = b = a = 1.0;
    }

    Color(float rx, float gx, float bx, float ax)
    {
      r = rx, g = gx, b = bx, a = ax;
    }

    vec4 get()
    {
      return vec4(r, g, b, a);
    }

    void get(uchar &r, uchar &g, uchar &b, uchar &a)
    {
      r = 255.0 * this->r;
      g = 255.0 * this->g;
      b = 255.0 * this->b;
      a = 255.0 * this->a;
    }

    void set(vec4 v)
    {
      r = v.x; g = v.y; b = v.z; a = v.w;
    }

    void set(uchar r, uchar g, uchar b, uchar a)
    {
      this->r = (float)(r / 255.0);
      this->g = (float)(g / 255.0);
      this->b = (float)(b / 255.0);
      this->a = (float)(a / 255.0);
    }    
  };

protected:
  kgmTexture* m_tex_color = null;
  kgmTexture* m_tex_normal = null;
  kgmTexture* m_tex_specular = null;

  float  m_shininess;

public:
  Color  m_color, m_specular, m_emision;

  kgmShader*  m_shader = null;

  u32         m_flags;

  bool        m_cull;
  bool        m_alpha;
  bool        m_blend;
  bool        m_depth;

  u32         m_srcblend, m_dstblend;

  kgmString   m_type;

public:
  kgmMaterial();

  kgmMaterial* clone();

  ~kgmMaterial();

  void setTexColor(kgmTexture* t);
  void setTexNormal(kgmTexture* t);
  void setTexSpecular(kgmTexture* t);

  bool hasTexColor();
  bool hasTexNormal();
  bool hasTexSpecular();

  kgmTexture* getTexColor();
  kgmTexture* getTexNormal();
  kgmTexture* getTexSpecular();

  void setShader(kgmShader* shader);

  kgmShader* getShader() const
  {
    return m_shader;
  }

  float transparency() const
  {
    return m_color.a;
  }

  void transparency(float t)
  {
    m_color.a = t;

    if(t > 0.0)
      m_alpha = true;
  }

  float shininess() const
  {
    return m_shininess;
  }

  void shininess(float s)
  {
    m_shininess = s;
  }

  bool alpha() const
  {
    return m_alpha;
  }

  void alpha(bool a)
  {
    if(m_alpha == a)
      return;

    if(!a && m_color.a > 0.0)
      return;

    m_alpha = a;
  }
};

typedef kgmList<kgmMaterial*> kgmMaterialList;
