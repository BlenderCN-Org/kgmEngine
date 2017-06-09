#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

kgmMaterial::kgmMaterial()
{
  m_flags = MF_Textures | MF_Depth | MF_Color;

  m_color        = Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_specular     = Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_shininess    = 0.0;

  m_alpha  = false;
  m_blend  = false;
  m_depth  = true;
  m_cull   = true;

  m_srcblend = gcblend_srcalpha;
  m_dstblend = gcblend_one;
}

kgmMaterial::kgmMaterial(const kgmMaterial& mtl)
{
  m_color     = mtl.m_color;
  m_specular  = mtl.m_specular;

  m_shininess    = mtl.m_shininess;

  m_shader = mtl.m_shader;

  m_gc = mtl.m_gc;

  if(mtl.m_tex_color)
    mtl.m_tex_color->assign((kgmResource *&) m_tex_color);

  if(mtl.m_tex_normal)
    mtl.m_tex_normal->assign((kgmResource *&) m_tex_normal);

  if(mtl.m_tex_specular)
    mtl.m_tex_specular->assign((kgmResource *&) m_tex_specular);
}

kgmMaterial::~kgmMaterial()
{
  if(m_tex_color)
    m_tex_color->release();

  if(m_tex_normal)
    m_tex_normal->release();

  if(m_tex_specular)
    m_tex_specular->release();
}

void kgmMaterial::setTexColor(kgmTexture* t)
{
  m_tex_color = t;
}

void kgmMaterial::setTexNormal(kgmTexture* t)
{
  m_tex_normal = t;
}

void kgmMaterial::setTexSpecular(kgmTexture* t)
{
  m_tex_specular = t;
}

bool kgmMaterial::hasTexColor()
{
  return (m_tex_color && m_tex_color->m_texture);
}

bool kgmMaterial::hasTexNormal()
{
  return (m_tex_normal && m_tex_normal->m_texture);
}

bool kgmMaterial::hasTexSpecular()
{
  return (m_tex_specular && m_tex_specular->m_texture);
}

kgmTexture* kgmMaterial::getTexColor()
{
  return m_tex_color;
}

kgmTexture* kgmMaterial::getTexNormal()
{
  return m_tex_normal;
}

kgmTexture* kgmMaterial::getTexSpecular()
{
  return m_tex_specular;
}

void kgmMaterial::setShader(kgmShader* shader)
{
  m_shader = shader;
}

kgmString kgmMaterial::blendToString(u32 blend)
{
  switch(blend)
  {
  case gcblend_one:
    return "one";
  case gcblend_zero:
    return "zero";
  case gcblend_srcalpha:
    return "srcalpha";
  case gcblend_dstalpha:
    return "dstalpha";
  case gcblend_srcialpha:
    return "isrcalpha";
  case gcblend_dstialpha:
    return "idstalpha";
  }

  return "";
}

u32 kgmMaterial::stringToBlend(kgmString blend)
{
  if(blend == "one") return gcblend_one;
  else if(blend == "zero") return gcblend_zero;
  else if(blend == "srcalpha") return gcblend_srcalpha;
  else if(blend == "dstalpha") return gcblend_dstalpha;
  else if(blend == "isrcalpha") return gcblend_srcialpha;
  else if(blend == "idstalpha") return gcblend_dstialpha;

  return 0;
}


