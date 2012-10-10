#ifdef __APPLE__ 
  #include <GLUT/glut.h>
  #include <GLUI/glui.h>
#else
  #include <GL/glut.h>
  #include <GL/glui.h>
#endif



#include "geo/Light.hpp"



Light::Light(Vector pos, Color color, GLenum target)
  : m_pos(pos), m_color(color), m_target(target)
{
}


void Light::PrepareLight() const
{
  GLfloat pos[] = {m_pos.x() + m_trans.x(), 
		   m_pos.y() + m_trans.y(), 
		   m_pos.z() + m_trans.z(), 0.0};
  GLfloat color[] = {m_color.r(), m_color.g(), m_color.b(), m_color.a()};

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  glLightfv(m_target, GL_POSITION, pos);
  glLightfv(m_target, GL_DIFFUSE, color);
  glLightfv(m_target, GL_SPECULAR, color);

  glEnable(m_target);
}

void Light::DisableLight() const
{
  glDisable(m_target);
}

Vector Light::GetPosition() const
{
  return m_pos + m_trans;
}

void Light::SetPosition(Vector pos)
{
  m_pos = pos;
}

void Light::SetTranslate(Vector trans)
{
  m_trans = trans;
}

Vector Light::GetTranslate() const
{
  return m_trans;
}

void Light::SetColor(Color color)
{
  m_color = color;
}

Color Light::GetColor() const
{
  return m_color;
}
