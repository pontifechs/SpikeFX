// Libs
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif



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
  
  m_tex1_stack = rhs.m_tex1_stack;
  m_tex2_stack = rhs.m_tex2_stack;
  m_tex3_stack = rhs.m_tex3_stack;
  m_tex4_stack = rhs.m_tex4_stack;

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
  
  // Calculate normals based on CCW winding
  
  Vector pt4_to_pt1 = pt4 - pt1;
  Vector pt1_to_pt2 = pt2 - pt1;
  Vector normal = pt4_to_pt1.cross(pt1_to_pt2);
  m_norm1 = normal;
  m_norm2 = normal;
  m_norm3 = normal;
  m_norm4 = normal;

}

Quad::Quad(Vector pt1, Vector pt2, Vector pt3, Vector pt4, Color col)
  : m_pt1(pt1), m_pt2(pt2), m_pt3(pt3), m_pt4(pt4),
    m_col1(col), m_col2(col), m_col3(col), m_col4(col)
{
  m_center = (pt1 + pt2 + pt3 + pt4);
  m_center *= 0.25;
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

void Quad::PushTexCoords(Vector pt1, Vector pt2, Vector pt3, Vector pt4)
{
  m_tex1_stack.push_back(pt1);
  m_tex2_stack.push_back(pt2);
  m_tex3_stack.push_back(pt3);
  m_tex4_stack.push_back(pt4);
}

void Quad::ClearTexCoords()
{
  m_tex1_stack.clear();
  m_tex2_stack.clear();
  m_tex3_stack.clear();
  m_tex4_stack.clear();
}

void Quad::Draw(TexStack* override) const
{

  if (m_norm1.length() == 0 || 
      m_norm2.length() == 0 || 
      m_norm3.length() == 0 ||
      m_norm4.length() == 0)
  {
    printf("Quad 0-length normal!\n");
  }
  
  TexStack texStack = m_texStack;
  if (override != NULL)
  {
    texStack = *override;
  }
  texStack.EnableTexStack();

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
  
  glBegin(GL_QUADS);
  
  glColor4f(m_col1.r(), m_col1.g(), m_col1.b(), m_col1.a());
  for (int i = 0; i < texStack.TexCount(); i++)
  {
    Vector tex;
    if (i < m_tex1_stack.size())
    {
      tex = m_tex1_stack[i];
    }
    else {
      tex = m_tex1_stack[0];
    }
    glMultiTexCoord2f(GL_TEXTURE0 + i, tex.x(), tex.y());
  }
  glNormal3f(m_norm1.x(), m_norm1.y(), m_norm1.z());
  glVertex3f(m_pt1.x(), m_pt1.y(), m_pt1.z());


  glColor4f(m_col2.r(), m_col2.g(), m_col2.b(), m_col2.a());
  for (int i = 0; i < texStack.TexCount(); i++)
  {
    Vector tex;
    if (i < m_tex2_stack.size())
    {
      tex = m_tex2_stack[i];
    }
    else {
      tex = m_tex2_stack[0];
    }
    glMultiTexCoord2f(GL_TEXTURE0 + i, tex.x(), tex.y());
  }
  glNormal3f(m_norm2.x(), m_norm2.y(), m_norm2.z());
  glVertex3f(m_pt2.x(), m_pt2.y(), m_pt2.z());


  glColor4f(m_col3.r(), m_col3.g(), m_col3.b(), m_col3.a());
  for (int i = 0; i < texStack.TexCount(); i++)
  {
    Vector tex;
    if (i < m_tex3_stack.size())
    {
      tex = m_tex3_stack[i];
    }
    else {
      tex = m_tex3_stack[0];
    }
    glMultiTexCoord2f(GL_TEXTURE0 + i, tex.x(), tex.y());
  }
  glNormal3f(m_norm3.x(), m_norm3.y(), m_norm3.z());
  glVertex3f(m_pt3.x(), m_pt3.y(), m_pt3.z());


  glColor4f(m_col4.r(), m_col4.g(), m_col4.b(), m_col4.a());
  for (int i = 0; i < texStack.TexCount(); i++)
  {
    Vector tex;
    if (i < m_tex4_stack.size())
    {
      tex = m_tex4_stack[i];
    }
    else {
      tex = m_tex4_stack[0];
    }
    glMultiTexCoord2f(GL_TEXTURE0 + i, tex.x(), tex.y());
  }
  glNormal3f(m_norm4.x(), m_norm4.y(), m_norm4.z());  
  glVertex3f(m_pt4.x(), m_pt4.y(), m_pt4.z());

  glEnd();

  glPopMatrix();

  texStack.DisableTexStack();
}

