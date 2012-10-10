
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

void Scene::AddReceiver(Quad* quad)
{
  m_receivers.push_back(quad);
}

void Scene::AddLight(Light* new_light)
{
  m_lighting.push_back(new_light);
}

void Scene::SetFrustum(float left, float right,
		       float bot,  float top,
		       float near, float far)
{
  m_left = left; m_right = right;
  m_bot  = bot;  m_top   = top; 
  m_near = near; m_far   = far;
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
    break;
  case PROJECTIVE_SHADOWS:
//    RenderScene(true, true);
    DrawProjectiveShadows();
    break;
  case SHADOW_VOLUMES:
//    RenderScene(true, true);
    DrawShadowVolume();
    break;
  case SHADOW_MAP:
    DrawShadowMap();
    break;
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
  // ZPass Method
  // 1. Compute Shadow Volumes
  std::vector<Quad> shadow_quads;
  
  for (int i = 0; i < m_occluders.size(); i++)
  {
    for (int j = 0; j < m_lighting.size(); j++)
    {
      Triangle* t = m_occluders[i];
      Light* l = m_lighting[j];
      std::vector<Quad> t_quads = t->GetShadowVolume(l->GetPosition());
      shadow_quads.insert(shadow_quads.end(), t_quads.begin(), t_quads.end());
    }
  }
  
  // !! WARNING !!
  // This is a huge hack because I don't have time to figure out the true issue.
  //
  // Since Something odd is going on with the normals, calc back-face by hand.
  // If dot product between normal and look vector is positive, front-face.
  // Otherwise, back-face.

  std::vector<Quad> front_faces;
  std::vector<Quad> back_faces;
  Vector lookat = m_look - m_eye;

  for (int i = 0; i < shadow_quads.size(); i++)
  {
    Vector norm = shadow_quads[i].GetNormal();

    // printf("lookat: %f %f %f\n", lookat.x(), lookat.y(), lookat.z());
    // printf("norm: %f %f %f\n", norm.x(), norm.y(), norm.z());
    // printf("dot: %f\n", norm.dot(lookat));

    if (norm.dot(lookat) > 0)
    {
      front_faces.push_back(shadow_quads[i]);
    }
    else 
    {
      back_faces.push_back(shadow_quads[i]);
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
  glEnable(GL_STENCIL_TEST);

  // 5. Render shadow volume front faces, increment depth buffer on zpass
  glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
  for (int i = 0; i < front_faces.size(); i++)
  {
    front_faces[i].Draw();
  }

  // 6. Render shadow volume back faces, decrement depth buffer on zpass
  glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
  for (int i = 0; i < back_faces.size(); i++)
  {
    back_faces[i].Draw();
  }

  // 7. Render scene with diffuse only where stencil = 0
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  

  // Need to clear the depth buffer or nothing will get drawn.
  glClear(GL_DEPTH_BUFFER_BIT);
  glStencilFunc(GL_EQUAL, 0, 0xFF);

  PrepareLights();
  DrawGeometry();
  DisableLights();

  // Reset OpenGL state  
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glDisable(GL_STENCIL_TEST);
}

void Scene::DrawShadowMap() const
{
  


}


void Scene::DrawProjectiveShadows() const
{
  glClear(GL_STENCIL_BUFFER_BIT);
  // 1. Calculate Shadow Geometry
  // Project each tri/quad to be shadowed onto all other tris/quads.

  std::vector<Geo*> shadow_geo;

  // Vector pt1(-3.0, -3.0, -4);
  // Vector pt2(-3.0,  3.0, -4);
  // Vector pt3( 3.0,  3.0, -4);
  // Vector pt4( 3.0, -3.0, -4);
  // Quad test(pt1, pt2, pt3, pt4);
  // shadow_geo.push_back(&test);


  for (int lights = 0; lights < m_lighting.size(); lights++)
  {
    Vector l = m_lighting[lights]->GetPosition();
    for (int i = 0; i < m_geometry.size(); i++)
    {
      Geo* g = m_geometry[i];
      for (int j = 0; j < m_receivers.size(); j++)
      {
  	Quad* q = m_receivers[j];
	
  	Vector o = q->GetOrigin();
  	Vector n = q->GetNormal();
	
  	Geo* ret = g->GetTransformed(l, o, n);
  	if (ret != NULL)
  	{
  	  shadow_geo.push_back(ret);
  	}
      }
    }
  }

  // 2. Draw Scene Ambient Only
  RenderScene(true, false);

  // 3. Render Shadow Geometry into Stencil Buffer; color, depth updates off
  glEnable(GL_DEPTH_TEST);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  glEnable(GL_STENCIL_TEST);

  glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
  for (int i = 0; i < shadow_geo.size(); i++)
  {
    shadow_geo[i]->Draw();
  }
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  // 4. Clear Depth Buffer, Draw Scene Diffuse/Ambient where stencil = 0
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);


  glStencilFunc(GL_EQUAL, 0, 0xFF);

  PrepareLights();

  DrawGeometry();
  
  DisableLights();

  // 5. Reset OpenGL state
  glDisable(GL_STENCIL_TEST);
}


void Scene::PrepareLights() const
{
  // Run through all of the lights
  for (int i = 0; i < m_lighting.size(); i++)
  {
    m_lighting[i]->PrepareLight();
  }
}

void Scene::DisableLights() const
{
  for (int i = 0; i < m_lighting.size(); i++)
  {
    m_lighting[i]->DisableLight();
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
  for (int i = 0; i < m_receivers.size(); i++)
  {
    m_receivers[i]->Draw();
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

