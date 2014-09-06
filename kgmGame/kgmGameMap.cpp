#include "kgmGameMap.h"

KGMOBJECT_IMPLEMENT(kgmGameMap, kgmObject);

kgmGameMap::kgmGameMap(kgmIGame* g, OpenType ot)
{
  m_game = g;
  m_type = ot;

  m_xml = null;
  m_mem = null;
}

bool kgmGameMap::open(kgmXml& xml)
{
  if(m_mem)
    return false;

  m_xml = &xml;

  if(m_xml != null && m_xml->m_node == null)
  {
    m_xml->m_node = new kgmXml::Node(null);
    m_xml->m_node->m_name = "kgm";
  }
}

bool kgmGameMap::open(kgmMemory<u8>& mem)
{
  if(m_xml)
    return false;

  m_mem = &mem;
}


bool kgmGameMap::addMesh(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmMesh";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("link", n.lnk));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "kgmMaterial";
    snode->m_attributes.add(new kgmXml::Attribute("value", n.mtl));

    snode = new kgmXml::Node(node);
    snode->m_name = "kgmPosition";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "kgmRotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));
    if(n.col != 0)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Collision";
      snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.col)));
    }

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addLight(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmLight";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addMaterial(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmMaterial";
    node->m_attributes.add(new kgmXml::Attribute("name", ((kgmMaterial*)n.obj)->m_id));
  }
}

bool kgmGameMap::addActor(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

}

bool kgmGameMap::addSensor(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmSensor";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmSensor*)n.obj)->runtime().nClass));
    node->m_attributes.add(new kgmXml::Attribute("target", ((kgmSensor*)n.obj)->getTarget()));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addTrigger(Node n)
{
  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmTrigger";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("channels", kgmConvert::toString((s32) ((kgmTrigger*)n.obj)->getCount())));
    node->m_attributes.add(new kgmXml::Attribute("target", ((kgmTrigger*)n.obj)->getTarget()));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addGameObject(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmGameObjects";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmGameObject*)n.obj)->runtime().nClass));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));

    for(int i = 0; i < ((kgmGameObject*)n.obj)->m_variables.length(); ++i)
    {
      kgmVariable& var = ((kgmGameObject*)n.obj)->m_variables[i];

      snode = new kgmXml::Node(node);
      snode->m_name = "Parameter";
      snode->m_attributes.add(new kgmXml::Attribute(var.getName(), var.toString()));
    }

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

kgmGameMap::Node kgmGameMap::next()
{
  Node node;

  node.obj = null;

  kgmXml::XmlState xstate = m_xml->next();


  kgmString id, value, t;
  kgmString ntype;

  if(xstate == kgmXml::XML_ERROR)
  {
    return node;
  }
  else if(xstate == kgmXml::XML_FINISH)
  {
    return node;
  }

  if(xstate == kgmXml::XML_TAG_OPEN)
  {
    id = m_xml->m_tagName;

    if(id == "kgmCamera")
    {
      ntype = "camera";
    }
    else if(id == "kgmLight")
    {
      ntype = "light";

      kgmString id;
      m_xml->attribute("name", id);
      node.obj = new kgmLight();

      node.nam = id;
      node.bnd = box3(-1, -1, -1, 1, 1, 1);
    }
    else if(id == "kgmMesh")
    {
      ntype = "mesh";

      kgmString id, ln;
      xml.attribute("name", id);
      xml.attribute("link", ln);
      node.obj = game->getResources()->getMesh(ln);

      if(node.obj)
      {
        node.nam = id;
        node.lnk = ln;
        node.bnd = mesh->bound();
      }
    }
    else if(id == "kgmActor")
    {
      ntype = "actor";
    }
    else if(id == "kgmSensor")
    {
      ntype = "sensor";

      kgmString id, cls, trg;
      xml.attribute("name", id);
      xml.attribute("class", cls);
      xml.attribute("target", trg);

      kgmSensor* sns = null;

      if(kgmGameObject::g_typ_objects.hasKey(cls))
      {
        kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[cls];

        if(fn_new)
        {
          kgmSensor* sn = (kgmSensor*)fn_new(m_game);

          if(sn)
          {
            sn->setTarget(trg);

            node.obj = sn;
            node.bnd = box3(-1, -1, -1, 1, 1, 1);
            node.nam = id;
          }
        }
      }
    }
    else if(id == "kgmTrigger")
    {
      ntype = "trigger";

      kgmString id, chn, trg;
      m_xml->attribute("name", id);
      m_xml->attribute("channels", chn);
      m_xml->attribute("target", trg);

      kgmTrigger* tr= new kgmTrigger(m_game);
      tr->setCount(kgmConvert::toInteger(chn));
      tr->setTarget(trg);

      node.obj = tr;
      node.nam = id;
      node.bnd = box3(-1, -1, -1, 1, 1, 1);
    }
    else if(id == "kgmGameObject")
    {
      ntype = "gobject";

      kgmString id, cls, trg;
      m_xml->attribute("name", id);
      m_xml->attribute("class", cls);

      if(kgmGameObject::g_typ_objects.hasKey(cls))
      {
        kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[cls];

        if(fn_new)
        {
          kgmGameObject* go = (kgmGameObject*)fn_new(m_game);

          if(go)
          {
            node.obj = go;
            node.bnd = box3(-1, -1, -1, 1, 1, 1);
            node.nam = id;
          }
        }
      }
    }
  }
  else if(xstate == kgmXml::XML_TAG_CLOSE)
  {
    kgmString data;
    id = m_xml->m_tagName;

    if(id == "Position")
    {
      vec3 v;
      m_xml->attribute("value", value);
      sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

      node.pos = v;
    }
    else if(id == "Rotation")
    {
      vec3 v;
      m_xml->attribute("value", value);
      sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

      node.pos = v;
    }
    else if(id == "Direction")
    {
      vec3 v;
      m_xml->attribute("value", value);
      sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

      if(ntype == "camera")
      {
      }
    }
    else if(id == "Material")
    {
      value.clear();
      m_xml->attribute("value", value);

      if(value.length())
        node.mtl = value;
    }
    else if(id == "Collision")
    {
      value.clear();
      m_xml->attribute("value", value);

      u32 t = kgmConvert::toInteger(value);

      if(t != 0)
        node.col = true;
    }
    else if(id == "Locked")
    {
      value.clear();
      xml.attribute("value", value);

      u32 t = kgmConvert::toInteger(value);

      if(t != 0)
        node->lock = true;
    }
    else if(id == "Parameter")
    {
      kgmString name, type, value;
      kgmVariable var;

      xml.attribute("name",  name);
      xml.attribute("type",  type);
      xml.attribute("value", value);

      node->obj->setParameter(name, value);
    }
  }
  else if(xstate == kgmXml::XML_TAG_DATA)
  {
  }


  if(m_type == OpenWrite)
    return node;

  return node;
}
