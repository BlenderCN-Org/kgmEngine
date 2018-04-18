#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

kgmMaterial::kgmMaterial()
{
  m_flags = MF_Textures | MF_Depth | MF_Color;

  m_color        = Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_specular     = Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_shininess    = 0.0;

  //m_alpha  = false;
  m_depth  = true;
  m_cull   = true;

  m_blend  = Blend_None;
}

kgmMaterial::kgmMaterial(const kgmMaterial& mtl)
{
  m_color     = mtl.m_color;
  m_specular  = mtl.m_specular;
  m_shininess = mtl.m_shininess;
  m_shader    = mtl.m_shader;

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
  return (m_tex_color && m_tex_color->texture());
}

bool kgmMaterial::hasTexNormal()
{
  return (m_tex_normal && m_tex_normal->texture());
}

bool kgmMaterial::hasTexSpecular()
{
  return (m_tex_specular && m_tex_specular->texture());
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

kgmString kgmMaterial::blendToString(Blend blend)
{
  switch(blend)
  {
  case Blend_None:
    return "none";
  case Blend_Add:
    return "add";
  case Blend_Mul:
    return "mul";
  case Blend_Sub:
    return "sub";
  case Blend_Inter:
    return "inter";
  case Blend_CBurn:
    return "cburn";
  case Blend_LBurn:
    return "lburn";
  case Blend_CDodge:
    return "cdodge";
  case Blend_LDodge:
    return "ldodge";
  case Blend_Screen:
    return "screen";
  case Blend_Darken:
    return "darken";
  case Blend_Lighten:
    return "lighten";
  }

  return "";
}

kgmMaterial::Blend kgmMaterial::stringToBlend(kgmString blend)
{
  if(blend == "none")
    return Blend_None;
  else if(blend == "add")
    return Blend_Add;
  else if(blend == "mul")
    return Blend_Mul;
  else if(blend == "sub")
    return Blend_Sub;
  else if(blend == "inter")
    return Blend_Inter;
  else if(blend == "cburn")
    return Blend_CBurn;
  else if(blend == "lburn")
    return Blend_LBurn;
  else if(blend == "cdodge")
    return Blend_CDodge;
  else if(blend == "ldodge")
    return Blend_LDodge;
  else if(blend == "screen")
    return Blend_Screen;
  else if(blend == "darken")
    return Blend_Darken;
  else if(blend == "lighten")
    return Blend_Lighten;

  return Blend_None;
}


