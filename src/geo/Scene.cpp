
#ifdef __APPLE__ 
  #include <GLUT/glut.h>
  #include <GLUI/glui.h>
#else
  #include <GL/glut.h>
  #include <GL/glui.h>
#endif



#include "geo/Scene.hpp"
#include "geo/Sphere.hpp"
#include "geo/Quad.hpp"

Scene::Scene()
  : m_shadow_mode(NONE)
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

void Scene::AddOccluder(Triangle* tri)
{
  m_geometry.push_back(tri);
  m_occluders.push_back(tri);
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

void Scene::DrawScene() const
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  switch (m_shadow_mode)
  {
  case NONE:
    RenderScene(true, true);
  case PROJECTIVE_SHADOWS:
    RenderScene(true, true);
  case SHADOW_VOLUMES:
    RenderScene(true, true);
//    DrawShadowVolume();
  }
}

void Scene::RenderScene(bool ambient, bool diffuse) const
{
  InitGLState();

  MVPTransform();
  if (ambient)
  {
    PrepareAmbient();
  }
  
  if (diffuse)
  {
    PrepareLights();
  }

  DrawGeometry();

  if (diffuse)
  {
    DisableLights();
  }
}

void Scene::DrawShadowVolume() const
{
  glEnable(GL_STENCIL_TEST);

  // ZPass Method
  // 1. Compute Shadow Volumes
  std::vector<Quad> shadow_quads;
  
  for (int i = 0; i < m_occluders.size(); i++)
  {
    for (int j = 0; j < m_lighting.size(); j++)
    {
      Triangle* t = m_occluders[i];
      Light l = m_lighting[j];
      std::vector<Quad> t_quads = t->GetShadowVolume(l.GetPosition());
      shadow_quads.insert(shadow_quads.end(), t_quads.begin(), t_quads.end());
    }
  }

  // 2. Clear Stencil Buffer
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);


  // 3. Render the scene ambient only
  //      - Sets the Depth and Color buffers
  RenderScene(true, false);

  // 4. Turn off color and depth updates (leave depth test on)
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  // 5. Render shadow volume front faces, increment depth buffer on zpass
  // 6. Render shadow volume back faces, decrement depth buffer on zpass
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR);
  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_INCR);
  
  for (int i = 0; i < shadow_quads.size(); i++)
  {
    shadow_quads[i].Draw();
  }


  // 7. Render scene with diffuse only where stencil = 0
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  
  glStencilFunc(GL_EQUAL, 0, 0xFFFF);


  // Need to clear the depth buffer or nothing will get drawn.
  glClear(GL_DEPTH_BUFFER_BIT);
  glCullFace(GL_BACK);

  PrepareLights();
  DrawGeometry();
  DisableLights();


  // Reset OpenGL state  
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glDisable(GL_STENCIL_TEST);
}


void Scene::PrepareLights() const
{
  glEnable(GL_LIGHTING);

  // Run through all of the lights
  for (int i = 0; i < m_lighting.size(); i++)
  {
    m_lighting[i].PrepareLight();
  }
}

void Scene::DisableLights() const
{
  for (int i = 0; i < m_lighting.size(); i++)
  {
    m_lighting[i].DisableLight();
  }

}

void Scene::PrepareAmbient() const
{
  // Ambient Lighting
  GLfloat ambLight[] = {m_amb.r(), m_amb.g(), m_amb.b(), m_amb.a() };

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);  

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);
}

void Scene::DrawGeometry() const
{
  // Run through all of the geometry 
  for (int i = 0; i < m_geometry.size(); i++)
  {
    m_geometry[i]->Draw();
  }
}

void Scene::InitGLState() const
{
  // Set up OpenGL State
  glEnable(GL_DEPTH_TEST);
//  glEnable(GL_CULL_FACE);
//  glCullFace(GL_BACK);
//  glEnable(GL_RESCALE_NORMAL); // Things may appear dark/odd without this
  glEnable(GL_NORMALIZE);
}

void Scene::MVPTransform() const
{
  // Projection Transform
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(m_left, m_right, m_bot, m_top, m_near, m_far);

  // Camera Transform
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(m_eye.x(),  m_eye.y(),  m_eye.z(),
  	    m_look.x(), m_look.y(), m_look.z(),
  	    m_up.x(),   m_up.y(),   m_up.z());
  

}

void Scene::SetShadowMode(ShadowMode mode)
{
  m_shadow_mode = mode;
}

Scene::ShadowMode Scene::GetShadowMode() const
{
  return m_shadow_mode;
}

