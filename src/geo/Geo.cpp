


#include "geo/Geo.hpp"


Geo::Geo()
  : m_pTex1(NULL),
    m_pTex2(NULL),
    m_trans(0.0, 0.0, 0.0),
    m_scale(1.0, 1.0, 1.0),
    m_rotation(NULL)
{
}

void Geo::RegisterTex(Texture* tex)
{
  RegisterTex1(tex);
}

void Geo::RegisterTex1(Texture* tex)
{
  m_pTex1 = tex; 
}

void Geo::RegisterTex2(Texture* tex)
{
  m_pTex2 = tex;
}

void Geo::SetTranslate(Vector trans)
{
  m_trans = trans;
}

void Geo::SetScale(Vector scale)
{
  m_scale = scale;
}

void Geo::SetRotate(float* matrix)
{
  m_rotation = matrix;
}

void Geo::SetCenter(Vector center)
{
  m_center = center;
}

Vector Geo::GetCenter() const
{
  return m_center;
}
