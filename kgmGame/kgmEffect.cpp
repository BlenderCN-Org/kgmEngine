#include "kgmEffect.h"

kgmEffect::kgmEffect(kgmIGame *g)
:kgmUnit(g)
{
  m_body = new kgmBody();

  m_body->m_collision = false;

  m_type = EFFECT;
}

void kgmEffect::init()
{
}

void kgmEffect::exit()
{
}
