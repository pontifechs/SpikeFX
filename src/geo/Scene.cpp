
#ifdef __APPLE__ 
  #include <GLUT/glut.h>
  #include <GLUI/glui.h>
#else
  #include <GL/glut.h>
  #include <GL/glui.h>
#endif



#include "geo/Scene.hpp"


Scene::Scene()
{
}

Scene::Scene(Vector eye, Vector look, Vector up)
  : m_eye(eye), m_look(look), m_up(up)
{
  
}

void Scene::AddGeometry(Geo* new_geo)
{
  m_geometry.push_back(new_geo);
}


void Scene::AddLight(Light new_light)
{
  m_lighting.push_back(new_light);
}


void Scene::SetFrustum(float left, float right,
		       float bot,  float top,
		       float near, float far)
{
  m_left = left; m_right = right;
  m_bot = bot;   m_top = top; 
  m_near = near; m_far = far;
}

void Scene::SetEye(Vector eye)
{
  m_eye = eye;
}

void Scene::SetLook(Vector look)
{
  m_look = look;
}

void Scene::SetUp(Vector up)
{
  m_up = up;
}

Vector Scene::GetEye() const
{
  return m_eye;
}

Vector Scene::GetLook() const
{
  return m_look;
}

Vector Scene::GetUp() const
{
  return m_up;
}

Color Scene::GetAmbient() const
{
  return m_amb;
}

void Scene::SetAmbient(Color color)
{
  m_amb = color;
}


void Scene::DrawScene()
{
  // Set up OpenGL State
  glEnable(GL_DEPTH_TEST);
  
  // Camera Transform
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(m_eye.x(),  m_eye.y(),  m_eye.z(),
	    m_look.x(), m_look.y(), m_look.z(),
	    m_up.x(),   m_up.y(),   m_up.z());
  
  // Projection Transform
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(m_left, m_right, m_bot, m_top, m_near, m_far);

  // Ambient Lighting
  GLfloat ambLight[] = {m_amb.r(), m_amb.g(), m_amb.b(), m_amb.a() };

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);

  // Run through all of the lights
  for (int i = 0; i < m_lighting.size(); i++)
  {
    m_lighting[i].PrepareLight();
  }

  // Run through all of the geometry 
  for (int i = 0; i < m_geometry.size(); i++)
  {
    m_geometry[i]->Draw();
    Geo* g = m_geometry[i];
  }
}
