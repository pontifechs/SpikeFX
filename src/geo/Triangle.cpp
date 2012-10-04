
#include "geo/Triangle.hpp"


// Libs
#ifdef __APPLE__ 
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include <stdio.h>


// Classes
#include "math/Vector.hpp"
#include "math/Color.hpp"



Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

Triangle::Triangle(Vector pt1, Vector pt2, Vector pt3)
  : m_pt1(pt1), m_pt2(pt2), m_pt3(pt3)
{
}

Triangle::Triangle(Vector pt1, Vector pt2, Vector pt3, Color col)
  : m_pt1(pt1), m_pt2(pt2), m_pt3(pt3), m_col1(col), m_col2(col), m_col3(col)

{
}

Triangle::Triangle(Vector pt1, Vector pt2, Vector pt3, 
		   Color col1, Color col2, Color col3)
  : m_pt1(pt1), m_pt2(pt2), m_pt3(pt3), m_col1(col1), m_col2(col2), m_col3(col3)
{
}

void Triangle::PushTexCoords(Vector pt1, Vector pt2, Vector pt3)
{
  m_tex1_stack.push_back(pt1);
  m_tex2_stack.push_back(pt2);
  m_tex3_stack.push_back(pt3);
}

void Triangle::ClearTexCoords()
{
  m_tex1_stack.clear();
  m_tex2_stack.clear();
  m_tex3_stack.clear();
}

void Triangle::SetNormals(Vector norm1, Vector norm2, Vector norm3)
{
  m_norm1 = norm1;
  m_norm2 = norm2;
  m_norm3 = norm3;
}

Vector Triangle::pt1() const
{
  return m_pt1;
}

Vector Triangle::pt2() const
{
  return m_pt2;
}

Vector Triangle::pt3() const
{
  return m_pt3;
}

void Triangle::Draw(TexStack* override) const
{
  TexStack texStack = m_texStack;
  if (override != NULL)
  {
    texStack = *override;
  }
  
  texStack.EnableTexStack();
  
  glBegin(GL_TRIANGLES);

  // Point 1
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

  // Point 2
  glColor4f(m_col2.r(), m_col2.g(), m_col2.b(), m_col2.a());
  for (int i = 0; i < texStack.TexCount(); i++)
  {
    Vector tex;
    if ( i < m_tex2_stack.size())
    {
      tex = m_tex2_stack[i];
    }
    else
    {
      tex = m_tex2_stack[0];
    }

    glMultiTexCoord2f(GL_TEXTURE0 + i, tex.x(), tex.y());
  }
  glNormal3f(m_norm2.x(), m_norm2.y(), m_norm2.z());
  glVertex3f(m_pt2.x(), m_pt2.y(), m_pt2.z());

  // Point 3
  glColor4f(m_col3.r(), m_col3.g(), m_col3.b(), m_col3.a());
  for (int i = 0; i < texStack.TexCount(); i++)
  {
    Vector tex;
    if (i < m_tex3_stack.size())
    {
      tex = m_tex3_stack[i];
    }
    else
    {
      tex = m_tex3_stack[0];
    }
    glMultiTexCoord2f(GL_TEXTURE0 + i, tex.x(), tex.y());
  }
  glNormal3f(m_norm3.x(), m_norm3.y(), m_norm3.z());
  glVertex3f(m_pt3.x(), m_pt3.y(), m_pt3.z());
  glEnd();

  texStack.DisableTexStack();

}
