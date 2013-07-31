#include "kgmParticles.h"
#include "kgmMaterial.h"
#include "../kgmBase/kgmTime.h"

KGMOBJECT_IMPLEMENT(kgmParticles, kgmObject);

kgmParticles::kgmParticles()
{
  m_count = 10;
  m_life = 2000;
  m_speed = 0.5f;
  m_color.color = 0xffffffff;
  m_fade  = true;
  m_fall  = false;
  m_alpha = false;
  m_blend = true;
  m_depth = false;
  m_loop  = true;

  volume    = vec3(0, 0, 0);
  location  = vec3(0, 0, 0);
  direction = vec3(0.0f, 0.0f, 1.0f);

  div_life  = 0.0f;
  div_speed = 0.0f;

  st_size   = 0.1f;
  en_size   = 1.0f;

  m_particles  = null;
  m_material   = null;
  m_typerender = RTypeBillboard;
}

kgmParticles::~kgmParticles()
{
  if(m_particles)
    delete [] m_particles;

  if(m_material)
    m_material->release();
}

void kgmParticles::build(){
  int i = 0;
  float ctime = kgmTime::getTicks();

  if(m_particles)
    delete [] m_particles;

  m_particles = new Particle[m_count];

  for(i = 0; i < m_count; i++)
    init(&m_particles[i]);
}

void kgmParticles::init(Particle* pr)
{
  if(!pr)
    return;

  pr->pos = location;
  pr->pos.x += volume.x * pow(-1.0, rand()%2) * (rand() % 100);
  pr->pos.y += volume.x * pow(-1.0, rand()%2) * (rand() % 100);
  pr->pos.z += volume.x * pow(-1.0, rand()%2) * (rand() % 100);
  pr->dir.x  = direction.x * pow(-1.0, rand()%2) * (rand() % 100);
  pr->dir.y  = direction.y * pow(-1.0, rand()%2) * (rand() % 100);
  pr->dir.z  = direction.z * pow( 1.0, rand()%2) * (rand() % 100);
  pr->dir.normalize();

  pr->speed = m_speed - div_speed * m_speed / (1 + rand() % 100);
  pr->life  = m_life  - div_life  * m_life  / (1 + rand() % 100);
  pr->time  = 0;
  pr->col   = m_color;
  pr->scale = st_size;
}

void kgmParticles::update(u32 t)
{
  int i = 0;

  for(i = m_count; i > 0; i--)
  {
    Particle*  pr =  &m_particles[i - 1];

    pr->pos = pr->pos + pr->dir * (pr->speed * t * 0.001f);
    pr->pos.y = 0;
    pr->time += t;

    if(st_size != en_size)
    {
      float d_size = (st_size - en_size) / pr->life;
      pr->scale = st_size - (d_size * pr->time);
    }

    if(m_fade)
    {
      uchar a = (uchar)(255.0 - 255.0 * pr->time / pr->life);

      if(m_alpha)
      {
        pr->col.a = a;
      }
      else
      {
        pr->col.r = pr->col.g = pr->col.b = a;
      }
    }

    if(pr->time > pr->life)
    {
      if(m_loop)
      {
        init(pr);
      }
    }
  }
}

void kgmParticles::set(kgmMaterial *m)
{
  if(m_material)
    m_material->release();

  m_material = m;

  if(m)
    m->increment();
}
