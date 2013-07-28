#include "kgmGamePhysics.h"

#include "../kgmPhysics/kgmCollision.h"
#include "../kgmPhysics/kgmIntersection.h"


#include "kgmIGame.h"
#include "kgmGameBase.h"

#ifdef WIN32
#include <windows.h>
#endif

kgmGamePhysics::kgmGamePhysics()
{
}

kgmGamePhysics::~kgmGamePhysics()
{
}

void kgmGamePhysics::update(float dtime)
{
  kgmPhysics::update(dtime);
}

void kgmGamePhysics::collision(kgmBody* body, kgmBody* tobody)
{
  if(!body || !tobody || !body->m_udata || !tobody->m_udata)
    return;

  kgmGameObject* go_body   = (kgmGameObject*)body->m_udata;
  kgmGameObject* go_tobody = (kgmGameObject*)tobody->m_udata;


  if(!kgmIGame::getGame()->getLogic() ||
     !kgmObject::isValid(go_body) ||
     !kgmObject::isValid(go_tobody))
    return;

  if(go_body->getParent() == go_tobody ||
     go_tobody->getParent() == go_body)
  {
  }
  else
  {
    kgmIGame::getGame()->getLogic()->collide(go_body, go_tobody);
  }
}

