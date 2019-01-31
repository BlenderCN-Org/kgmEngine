// kgmLandscape.h: interface for the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmVector3d.h"
#include "../kgmMath/kgmPlane3d.h"
#include "../kgmMath/kgmTriangle3d.h"

class kgmPicture;
class kgmCamera;
class kgmMesh;
class kgmMaterial;

class kgmTerrain: public kgmObject
{
  KGM_OBJECT(kgmTerrain);

public:
  struct Mesh
  {
    kgmMesh*     msh;
    kgmMaterial* mtl;
  };

  struct Chunk
  {
    triangle chunk;

    Chunk* left;
    Chunk* right;
  };

  struct Heightmap
  {
    u16 width;
    u16 height;

    kgmArray<u16> map;
  };

  typedef kgmList<Mesh>::iterator MeshIt;

private:
  f32 m_width;
  f32 m_length;
  f32 m_height;

  Heightmap m_heightmap;

  kgmMesh* m_mesh;

  Chunk* m_chunks[2];

protected:
  ~kgmTerrain();

public:
  kgmTerrain();

  virtual void prepare(kgmCamera*);
  virtual MeshIt meshes();

  f32 width() const
  {
    return m_width;
  }

  void width(f32 w)
  {
    m_width = w;
  }

  f32 length() const
  {
    return m_length;
  }

  void length(f32 l)
  {
    m_length = l;
  }

  f32 height() const
  {
    return m_height;
  }

  void height(f32 h)
  {
    m_height = h;
  }

  bool heightmap(kgmPicture* map);

  kgmMesh* mesh();
};

