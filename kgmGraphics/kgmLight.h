#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"

#include "kgmTexture.h"
#include "kgmIcon.h"

class kgmLight: public kgmObject
{
  KGM_OBJECT(kgmLight);

public:
    static float LIGHT_RANGE;

    enum Type
    {
      TypePoint,
      TypeSpot,
      TypeSun
    };

public:
  Type  type;

  vec3  position;   //light position
  vec3  direction;  //light direction, if 0 then light is omni
  vec4  color;

  float intensity;  //light intensity, 1 mean 10 meter range
  float angle;      //light spot angle if directional

  bool  active;     //light switched on
  bool  shadows;    //cast shadows, need shadow map. not for point type.

  kgmString m_id;
  u32       m_group;     // object group id

#ifdef DEBUG
  kgmIcon* m_icon = null;
#endif

public:
  kgmLight()
  {
    type = TypePoint;

    position  = vec3(0, 0, 0);
    direction = vec3(0, 0, 0);

    color     = vec4(.7, .7, .7, 1);

    intensity = 1.0f;
    angle = 0.0f;

    shadows = false;
    active = true;

    m_group = 0;
  }

  kgmLight(const kgmLight& light)
  {
    type = light.TypePoint;

    position  = light.position;
    direction = light.direction;

    color     = light.color;

    intensity = light.intensity;
    angle     = light.angle;

    shadows   = light.shadows;
    active    = light.active;

    m_group   = light.m_group;

#ifdef DEBUG
    setIcon(light.m_icon);
#endif
  }

  ~kgmLight()
  {
  }

  kgmObject* clone()
  {
    kgmLight* l = new kgmLight();

    l->position  = position;
    l->direction = direction;
    l->color     = color;
    l->intensity = intensity;
    l->angle     = angle;
    l->shadows   = shadows;
    l->active    = active;
    l->m_group   = m_group;

#ifdef DEBUG
    l->setIcon(m_icon);
#endif
  }

  vec3 getPosition() const
  {
    return position;
  }

  vec3 getDirection() const
  {
    return direction;
  }

  void setPosition(vec3 v)
  {
    position = v;
  }

  void setDirection(vec3 v)
  {
    direction = v;
  }

#ifdef DEBUG
  void setIcon(kgmIcon* icon)
  {
    m_icon = icon;
  }

  kgmIcon* getIcon()
  {
    return m_icon;
  }
#endif
};

typedef kgmList<kgmLight*> kgmLights;



