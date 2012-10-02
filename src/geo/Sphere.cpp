
// Libs
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif


#include <cmath>

#include "geo/Sphere.hpp"



Sphere::~Sphere() 
{
}

void Sphere::Generate(Vector center, float radius, float ang_inc)
{

  m_center = center;
  // Clip angle_increment to their maximum (90) and minimum (1)
  if (ang_inc  > 90)
  {
    ang_inc = 90;
  }
  else if (ang_inc < 1)
  {
    ang_inc = 1;
  }

  // The number of segments must be a whole number
  int segs = 180 / ang_inc;
  ang_inc = 180 / segs;

  // Iterate over phi and theta, generating quads, then triangles.
  for (float phi = ang_inc; phi <= 360.0; phi += ang_inc)
  {
    for (float theta = -90.0f + ang_inc; theta <= 90.0; theta += ang_inc)
    {

      // Going clock-wise
      // pt1 = (phi-ang_inc,theta-ang_inc)
      // pt2 = (phi-ang_inc,theta)
      // pt3 = (phi, theta)
      // pt4 = (phi, theta-ang_inc)

      // Given phi and theta, the coordinates of that point on the sphere are::
      // x = sin(theta) cos(phi)
      // y = cos(theta)
      // z = sin(theta) sin(phi)
      
      float phi_hi = phi * M_PI / 180;
      float phi_lo = (phi - ang_inc) * M_PI / 180;
      float theta_hi = theta * M_PI / 180;
      float theta_lo = (theta - ang_inc) * M_PI / 180;

      float x1 = (radius * cos(theta_lo) * cos(phi_lo)) + center.x();
      float y1 = (radius * sin(theta_lo)) + center.y();
      float z1 = (radius * cos(theta_lo) * sin(phi_lo)) + center.z();
      Vector pt1(x1, y1, z1);
      Vector tex1( 1- (phi / 360.0), (theta + 90) / 180.0, 0.0);
      Vector norm1 = pt1 - center;
      norm1.normalize();

      float x2 = (radius * cos(theta_hi) * cos(phi_lo)) + center.x();
      float y2 = (radius * sin(theta_hi)) + center.y();
      float z2 = (radius * cos(theta_hi) * sin(phi_lo)) + center.z();
      Vector pt2(x2, y2, z2);
      Vector tex2( 1- (phi / 360.0), (theta + 90 + ang_inc) / 180.0, 0.0);
      Vector norm2 = pt2 - center;
      norm2.normalize();
      
      float x3 = (radius * cos(theta_hi) * cos(phi_hi)) + center.x();
      float y3 = (radius * sin(theta_hi)) + center.y();
      float z3 = (radius * cos(theta_hi) * sin(phi_hi)) + center.z();
      Vector pt3(x3, y3, z3);
      Vector tex3(1 - ((phi + ang_inc) / 360.0), (theta + 90 + ang_inc) / 180.0, 0.0);
      Vector norm3 = pt3 - center;
      norm3.normalize();

      float x4 = (radius * cos(theta_lo) * cos(phi_hi)) + center.x();
      float y4 = (radius * sin(theta_lo)) + center.y();
      float z4 = (radius * cos(theta_lo) * sin(phi_hi)) + center.z();
      Vector pt4(x4, y4, z4);
      Vector tex4(1 - ((phi + ang_inc) / 360.0), (theta + 90) / 180.0, 0.0);
      Vector norm4 = pt4 - center;
      norm4.normalize();
      
      // T1 is pt1, pt2, pt3, T2 is pt1, pt3, pt4
      Triangle t1(pt1, pt2, pt3, SOLID_WHITE);
      Triangle t2(pt1, pt3, pt4, SOLID_WHITE);

      t1.SetNormals(norm1, norm2, norm3);
      t2.SetNormals(norm1, norm3, norm4);
      
      t1.PushTexCoords(tex1, tex2, tex3);
      t2.PushTexCoords(tex1, tex3, tex4);
      t1.PushTexCoords(tex1, tex2, tex3);
      t2.PushTexCoords(tex1, tex3, tex4);
      t1.PushTexCoords(tex1, tex2, tex3);
      t2.PushTexCoords(tex1, tex3, tex4);

      tris.push_back(t1);
      tris.push_back(t2);     
    }
  }  
}


Sphere::Sphere()
{
}

void Sphere::Draw() const
{  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Translate
  glTranslatef(m_trans.x(), m_trans.y(), m_trans.z());
  // Translate back
  glTranslatef(m_center.x(), m_center.y(), m_center.z());
  // Rotate
  if (m_rotation != NULL)
  {
    glMultMatrixf(m_rotation);
  }
  // Translate to Center
  glTranslatef(-m_center.x(), -m_center.y(), -m_center.z());

  m_texStack.EnableTexStack();

  for (int i = 0; i < tris.size(); i++)
  {    
    tris[i].Draw();
  }

  m_texStack.DisableTexStack();

  glPopMatrix();
}


// std::vector<Triangle> Sphere::getTris()
// {
//   return tris;
// }
