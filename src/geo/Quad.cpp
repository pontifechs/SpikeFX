
#include "geo/Quad.hpp"

Quad::Quad()
{
  m_center = Vector(0, 0, 0);
}

Quad::Quad(const Quad& rhs)
{
  (*this) = rhs;
}

Quad& Quad::operator=(const Quad& rhs)
{
  if (this == &rhs) 
  {
    return *this;
  }
  m_pt1 = rhs.m_pt1;
  m_pt2 = rhs.m_pt2;
  m_pt3 = rhs.m_pt3;
  m_pt4 = rhs.m_pt4;
  
  m_tex1_pt1 = rhs.m_tex1_pt1;
  m_tex1_pt2 = rhs.m_tex1_pt2;
  m_tex1_pt3 = rhs.m_tex1_pt3;
  m_tex1_pt4 = rhs.m_tex1_pt4;

  m_tex2_pt1 = rhs.m_tex2_pt1;
  m_tex2_pt2 = rhs.m_tex2_pt2;
  m_tex2_pt3 = rhs.m_tex2_pt3;
  m_tex2_pt4 = rhs.m_tex2_pt4;

  m_norm1 = rhs.m_norm1;
  m_norm2 = rhs.m_norm2;
  m_norm3 = rhs.m_norm3;
  m_norm4 = rhs.m_norm4;

  m_col1 = rhs.m_col1;
  m_col2 = rhs.m_col2;
  m_col3 = rhs.m_col3;
  m_col4 = rhs.m_col4;
  
  return *this;
}

Quad::Quad(Vector pt1, Vector pt2, Vector pt3, Vector pt4)
  : m_pt1(pt1), m_pt2(pt2), m_pt3(pt3), m_pt4(pt4)
{
  m_center = (pt1 + pt2 + pt3 + pt4);
  m_center *= 0.25;
}

Quad::Quad(Vector pt1, Vector pt2, Vector pt3, Vector pt4, Color col)
  : m_pt1(pt1), m_pt2(pt2), m_pt3(pt3), m_pt4(pt4),
    m_col1(col), m_col2(col), m_col3(col), m_col4(col)
{
  m_center = (pt1 + pt2 + pt3 + pt4);
  m_center *= 0.25;
}

void Quad::SetTex1Coords(Vector pt1, Vector pt2, Vector pt3, Vector pt4)
{
  m_tex1_pt1 = pt1;
  m_tex1_pt2 = pt2;
  m_tex1_pt3 = pt3;
  m_tex1_pt4 = pt4;
}


void Quad::SetTex2Coords(Vector pt1, Vector pt2, Vector pt3, Vector pt4)
{
  m_tex2_pt1 = pt1;
  m_tex2_pt2 = pt2;
  m_tex2_pt3 = pt3;
  m_tex2_pt4 = pt4;
}

void Quad::SetNormals(Vector pt1, Vector pt2, Vector pt3, Vector pt4)
{
  m_norm1 = pt1;
  m_norm2 = pt2;
  m_norm3 = pt3;
  m_norm4 = pt4;
}

void Quad::SetColors(Color col)
{
  m_col1 = col;
  m_col2 = col;
  m_col3 = col;
  m_col4 = col;
}

void Quad::SetColors(Color col1, Color col2, Color col3, Color col4)
{
  m_col1 = col1;
  m_col2 = col2;
  m_col3 = col3;
  m_col4 = col4;
}


void Quad::Draw() const
{
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glScalef(m_scale.x(), m_scale.y(), m_scale.z());
  
  // Translate
  glTranslatef(m_trans.x(), m_trans.y(), m_trans.z());
  // Translate back
  glTranslatef(m_center.x(), m_center.y(), m_center.z());

  // Rotate
  if (m_rotation != NULL)
  {
    printf("About to use m_rotation Quad\n");
    glMultMatrixf(m_rotation);
  }
  // Translate to Center
  glTranslatef(-m_center.x(), -m_center.y(), -m_center.z());

  if (m_pTex1 != NULL)
  {
    m_pTex1->PrepareTexture();
  }
  if (m_pTex2 != NULL)
  {
    m_pTex2->PrepareTexture();  
  }

  glBegin(GL_QUADS);
  
  glColor4f(m_col1.r(), m_col1.g(), m_col1.b(), m_col1.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_tex1_pt1.x(), m_tex1_pt1.y());
  glMultiTexCoord2f(GL_TEXTURE1, m_tex2_pt1.x(), m_tex2_pt1.y());
  glNormal3f(m_norm1.x(), m_norm1.y(), m_norm1.z());
  glVertex3f(m_pt1.x(), m_pt1.y(), m_pt1.z());


  glColor4f(m_col2.r(), m_col2.g(), m_col2.b(), m_col2.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_tex1_pt2.x(), m_tex1_pt2.y());
  glMultiTexCoord2f(GL_TEXTURE1, m_tex2_pt2.x(), m_tex2_pt2.y());
  glNormal3f(m_norm2.x(), m_norm2.y(), m_norm2.z());
  glVertex3f(m_pt2.x(), m_pt2.y(), m_pt2.z());


  glColor4f(m_col3.r(), m_col3.g(), m_col3.b(), m_col3.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_tex1_pt3.x(), m_tex1_pt3.y());
  glMultiTexCoord2f(GL_TEXTURE1, m_tex2_pt3.x(), m_tex2_pt3.y());
  glNormal3f(m_norm3.x(), m_norm3.y(), m_norm3.z());
  glVertex3f(m_pt3.x(), m_pt3.y(), m_pt3.z());



  glColor4f(m_col4.r(), m_col4.g(), m_col4.b(), m_col4.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_tex1_pt4.x(), m_tex1_pt4.y());
  glMultiTexCoord2f(GL_TEXTURE1, m_tex2_pt4.x(), m_tex2_pt4.y());
  glNormal3f(m_norm4.x(), m_norm4.y(), m_norm4.z());  
  glVertex3f(m_pt4.x(), m_pt4.y(), m_pt4.z());

  glEnd();


  if (m_pTex1 != NULL)
  {
    m_pTex1->DisableTexture();
  }
  if (m_pTex2 != NULL)
  {
    m_pTex2->DisableTexture();  
  }

  glPopMatrix();
}

