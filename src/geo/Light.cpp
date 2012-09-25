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


void Light::PrepareLight()
{
  GLfloat pos[] = { m_pos.x(), m_pos.y(), m_pos.z(), 0.0};
  GLfloat color[] = { m_color.r(), m_color.g(), m_color.b(), m_color.a()};

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  glLightfv(m_target, GL_POSITION, pos);
  glLightfv(m_target, GL_DIFFUSE, color);
  glLightfv(m_target, GL_SPECULAR, color);

  glEnable(GL_LIGHTING);
  glEnable(m_target);
  glEnable(GL_COLOR_MATERIAL);  
}
