

#include "geo/Geo.hpp"
#include "tex/TexIF.hpp"

Geo::Geo()
  : m_trans(0.0, 0.0, 0.0),
    m_scale(1.0, 1.0, 1.0),
    m_rotation(NULL)
{
}

void Geo::PushTex(TexIF* tex)
{
  m_texStack.PushTex(tex);
}

void Geo::ClearTexes()
{
  m_texStack.ClearTexes();
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

Geo* Geo::GetTransformed(Vector light, Vector origin, Vector normal)
{
  return NULL;
}
