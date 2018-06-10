#include "kgmGameScript.h"
#include "kgmGameBase.h"
#include "../kgmScript/kgmLuaScript.h"

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  handler = new kgmLuaScript(g->getResources());
}

kgmGameScript::~kgmGameScript()
{
  delete handler;
}
