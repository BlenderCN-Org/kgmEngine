#pragma once
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmOctTree.h"

#include "kgmIPhysics.h"
#include "kgmBody.h"
#include "kgmCollision.h"
#include "kgmIntersection.h"

class kgmPhysics: public kgmObject, public kgmIPhysics
{
  KGM_OBJECT(kgmPhysics);

public:
  float  m_gravity;
  bool   m_isgravity;

  kgmList<kgmBody*>       m_bodies;
  kgmList<triangle3>      m_triangles;
  kgmOctTree<triangle3*>  m_treestatic;

  vec3  m_ptCurrent;
  vec3  m_ptDestination;
  vec3  m_ptCollision;
  vec3  m_nrCollision;
  box3  m_space;

  kgmCollision    m_collision;
  kgmIntersection m_intersection;

public:
  kgmPhysics();
  ~kgmPhysics();

  //virtual
  virtual void clear();
  virtual void build();
  virtual void update(float time);
  virtual void collision(kgmBody* cbody, kgmBody* tobody);

  // static objects
  void add(vec3& a, vec3& b, vec3& c);
  void add(kgmCollision::Shape* shape);

  // dynamic objects
  void add(kgmBody*);   //add body
  bool remove(kgmBody*);//remove body

  //collision detection
  bool checkCollision(vec3& spos, vec3& epos, float& rad, vec3& rpos);
  // bool checkCollision(vec3& spos, vec3& epos, box& bound, vec3& rpos);

protected:
  void doCollision(float);
  void getTriangles(kgmList<triangle3>& triangles, sphere& s);
  void getBodies(kgmList<kgmBody*>& bodies, sphere& s);
};
