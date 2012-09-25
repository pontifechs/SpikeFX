
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

void Triangle::SetTexCoords(Vector pt1, Vector pt2, Vector pt3)
{
  SetTex1Coords(pt1,pt2,pt3);
}


void Triangle::SetTex1Coords(Vector pt1, Vector pt2, Vector pt3)
{
  m_pt1_tex1 = pt1;
  m_pt2_tex1 = pt2;
  m_pt3_tex1 = pt3;
}


void Triangle::SetTex2Coords(Vector pt1, Vector pt2, Vector pt3)
{
  m_pt1_tex2 = pt1;
  m_pt2_tex2 = pt2;
  m_pt3_tex2 = pt3;
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

void Triangle::Draw() const
{
  if (m_pTex1 != NULL)
  {
    m_pTex1->PrepareTexture();
  }
  if (m_pTex2 != NULL)
  {
    m_pTex2->PrepareTexture();  
  }

//  printf("%d, %d, %d\n", m_norm1.x(), m_norm1.y(), m_norm1.z());

  glBegin(GL_TRIANGLES);
  glColor4f(m_col1.r(), m_col1.g(), m_col1.b(), m_col1.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_pt1_tex1.x(), m_pt1_tex1.y());   
  glMultiTexCoord2f(GL_TEXTURE1, m_pt1_tex2.x(), m_pt1_tex2.y());   
  glNormal3f(m_norm1.x(), m_norm1.y(), m_norm1.z());
  glVertex3f(m_pt1.x(), m_pt1.y(), m_pt1.z());

  glColor4f(m_col2.r(), m_col2.g(), m_col2.b(), m_col2.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_pt2_tex1.x(), m_pt2_tex1.y());   
  glMultiTexCoord2f(GL_TEXTURE1, m_pt2_tex2.x(), m_pt2_tex2.y());   
  glNormal3f(m_norm2.x(), m_norm2.y(), m_norm2.z());
  glVertex3f(m_pt2.x(), m_pt2.y(), m_pt2.z());

  glColor4f(m_col3.r(), m_col3.g(), m_col3.b(), m_col3.a());
  glMultiTexCoord2f(GL_TEXTURE0, m_pt3_tex1.x(), m_pt3_tex1.y());   
  glMultiTexCoord2f(GL_TEXTURE1, m_pt3_tex2.x(), m_pt3_tex2.y());   
  glNormal3f(m_norm3.x(), m_norm3.y(), m_norm3.z());
  glVertex3f(m_pt3.x(), m_pt3.y(), m_pt3.z());
  glEnd();

  if (m_pTex1 != NULL)
  {
    m_pTex1->DisableTexture();
  }
  if (m_pTex2 != NULL)
  {
    m_pTex2->DisableTexture();  
  }



}
