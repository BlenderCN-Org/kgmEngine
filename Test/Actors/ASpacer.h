#ifndef ASPACER_H
#define ASPACER_H

class ASpacer: public kgmActor
{
  kgmIGame* game;

  float     c_dist;
  float     z_dist;

  bool      gbtns[65];
public:
  ASpacer(kgmIGame* g)
  {
    game = g;
    c_dist = 5.0f;
    z_dist = 2.0f;

    memset(gbtns, false, sizeof(gbtns));
  }

  ~ASpacer()
  {

  }

  void update(u32 mls)
  {
    kgmActor::update(mls);

    if(m_visual)
    {
      vec3 v(0, 0, 1);
      mtx4 m;
      m.rotate(0.5 * PI, v);
      m_visual->m_transform = m * m_visual->m_transform;

      for(int i = 0; i < m_dummies.length(); i++)
      {
        kgmDummy*       dm = m_dummies[i];
        kgmGameObject*  go = (kgmGameObject*)dm->m_linked;

        if(go && go->getVisual())
        {
          mtx4 m;
          vec3 v = dm->m_shift;

          //v.x -= 2.0f;
          m.identity();
          m.translate(v);

          go->getVisual()->m_transform = m * m_visual->m_transform;
        }
      }
    }

    if(m_gameplayer)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = m_body->m_position - m_body->m_direction * c_dist;
      cpos.z = z_dist;
      cam.mPos = cpos;
      cam.mDir = m_body->m_direction;
      cam.update();

      if(gbtns[gbtn_left])
      {
        vec3 vt = m_body->m_rotation;
        vt.z += (0.02f);
        m_body->rotate(0, 0, vt.z);
      }
      else if(gbtns[gbtn_right])
      {
        vec3 vt = m_body->m_rotation;
        vt.z -= (0.02f);
        m_body->rotate(0, 0, vt.z);
      }
    }
  }

  void input(u32 btn, int state)
  {
    static float alpha = 0.0f;
    vec3 vt;

    if(!m_gameplayer)
      return;

    kgmActor::input(btn, state);

    switch(btn)
    {
    case grot_x:
    {
      alpha += (0.005f * state);
      vt = m_body->m_rotation;
      vt.z += (0.002f * state);
      m_body->rotate(0, 0, vt.z);
      break;
    }
    case grot_y:
    {
      z_dist -= (state * 0.005f);
      z_dist = (z_dist > 2.0)?(2.0):(z_dist);
      z_dist = (z_dist < .1)?(.1):(z_dist);

      break;
    }
    case grot_z:
    {
      c_dist += (state * 0.1f);

      break;
    }
    case gbtn_down:
      if(state)
        m_body->m_velocity = -0.01f;
      else
        m_body->m_velocity = 0.0f;
      break;
    case gbtn_up:
      if(state)
        m_body->m_velocity = 0.01f;
      else
        m_body->m_velocity = 0.0f;
      break;
    case gbtn_left:
      gbtns[gbtn_left] = (state)?(true):(false);
      break;
    case gbtn_right:
      gbtns[gbtn_right] = (state)?(true):(false);
      break;
    }
  }

  void action(kgmString action)
  {
    if(action == "laser")
    {
      action_shoot_laser();
    }
    else if(action == "rocket")
    {
      action_shoot_rocket();
    }
  }

  void action_shoot_laser()
  {
    kgmGameObject* go = new ASp_LaserA(game, 1000, m_body->m_position, m_body->m_direction);
    game->gAppend(go);
  }

  void action_shoot_rocket()
  {

  }
};

#endif // ASPACER_H
