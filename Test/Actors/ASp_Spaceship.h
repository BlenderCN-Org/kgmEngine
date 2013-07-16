#ifndef ASP_SPACESHIP_H
#define ASP_SPACESHIP_H
class ASp_Spaceship: public kgmActor
{
  KGM_OBJECT(ASp_Spaceship);

protected:
  kgmIGame* game;

  kgmVisual* vtext;

public:
  ASp_Spaceship(kgmIGame* g)
  {
    game = g;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-3, -3, -3);
    m_body->m_bound.max = vec3( 3,  3,  3);

    vtext = new kgmVisual();

    kgmText* text = new kgmText();
    text->m_rect  = uRect(10, 150, 500, 200);

    vtext->set(text);
    ((kgmGameBase*)game)->m_render->add(vtext);
    text->release();
  }

  virtual ~ASp_Spaceship()
  {
    vtext->remove();
    vtext->release();
  }

  void init()
  {
    kgmString sgo, saim;

    m_options.get("Aim", saim);
    m_options.get("Target", sgo);

    if(sgo.length() > 0 && m_dummies.size() > 0)
    {
      for(int i = 0; i < m_dummies.size(); i++)
      {
        kgmDummy* d = m_dummies[i];

        if(d->m_linked)
        {
          ((kgmActor*)d->m_linked)->setOption("Target", sgo);
        }
      }
    }
  }

  void exit()
  {

  }

  void update(u32 ms)
  {
    kgmActor::update(ms);

    if(m_visual)
    {
      vec3 vz(0, 0, 1), vy(0, 0, 0), vx(1, 0, 0);
      mtx4 mz, mx, my, mr;

      mz.rotate(0.5 * PI, vz);

      m_visual->m_transform = mr * m_visual->m_transform;

      for(int i = 0; i < m_dummies.length(); i++)
      {
        kgmDummy*       dm = m_dummies[i];
        kgmGameObject*  go = (kgmGameObject*)dm->m_linked;

        if(go && go->getBody())
        {
          vec3 v = getBody()->m_position +  dm->m_shift;
          vec3 r = getBody()->m_rotation +  dm->m_orient;

          go->getBody()->translate(v);
          go->getBody()->rotate(r);
        }
      }

      kgmString ts = "ASp_Spacer state: ";

      if(m_state)
        vtext->getText()->m_text = ts + m_state->id;
      else
        vtext->getText()->m_text = ts;
    }

    if(m_state)
    {
      if(m_state->id == "idle")
      {
      }

      logic(m_state->id);
    }
  }
};

class ASp_SpaceshipA: public ASp_Spaceship
{
  KGM_OBJECT(ASp_SpaceshipA);
public:
  ASp_SpaceshipA(kgmIGame* g)
    :ASp_Spaceship(g)
  {

  }

  void init()
  {
    ASp_Spaceship::init();
  }

  void exit()
  {
    ASp_Spaceship::exit();
  }
};
#endif // ASP_SPACESHIP_H
