
// Libs
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif


#include "geo/Box.hpp"
#include "geo/Asset.hpp"

#include "math/Vector.hpp"
#include "math/Color.hpp"


Box::Box(Vector center, float x_len, float y_len, float z_len)
{
  if (x_len < 0)
  {
    x_len *= -1;
  }
  if (y_len < 0)
  {
    x_len *= -1;
  }

  if (z_len < 0)
  {
    x_len *= -1;
  }
  float min_x = center.x() - (x_len / 2);
  float max_x = center.x() + (x_len / 2);
  float min_y = center.y() - (y_len / 2);
  float max_y = center.y() + (y_len / 2);
  float min_z = center.z() - (z_len / 2);
  float max_z = center.z() + (z_len / 2);
  BuildQuads(min_x, max_x, min_y, max_y, min_z, max_z);

}

Box::Box(float min_x, float max_x, 
	 float min_y, float max_y, 
	 float min_z, float max_z)
{
  BuildQuads(min_x, max_x, min_y, max_y, min_z, max_z);
}

void Box::BuildQuads(float min_x, float max_x, float min_y, 
		     float max_y, float min_z, float max_z)
{
  Vector top1(min_x, max_y, min_z);
  Vector top2(min_x, max_y, max_z);
  Vector top3(max_x, max_y, max_z);
  Vector top4(max_x, max_y, min_z);

  Vector bot1(min_x, min_y, min_z);
  Vector bot2(min_x, min_y, max_z);
  Vector bot3(max_x, min_y, max_z);
  Vector bot4(max_x, min_y, min_z);

  Vector up(0, 1, 0);
  Vector down(0, -1, 0);
  Vector left(-1, 0, 0);
  Vector right(1, 0, 0);
  Vector out(0, 0, -1);
  Vector in(0, 0, 1);
  
  m_top = Quad(top1, top2, top3, top4, m_t_col);  
  m_top.SetNormals(up, up, up, up);
  m_bot = Quad(bot1, bot2, bot3, bot4, m_b_col);
  m_bot.SetNormals(down, down, down, down);
  m_left = Quad(bot1, top1, top2, bot2, m_l_col);
  m_left.SetNormals(left, left, left, left);
  m_right = Quad(bot4, top4, top3, bot3, m_r_col);
  m_right.SetNormals(right, right, right, right);
  m_near = Quad(bot1, top1, top4, bot4, m_n_col);
  m_near.SetNormals(out, out, out, out);
  m_far = Quad(bot2, top2, top3, bot3, m_f_col);
  m_far.SetNormals(in, in, in, in);

  float mid_x = (min_x + max_x) /2;
  float mid_y = (min_y + max_y) /2;
  float mid_z = (min_z + max_z) /2;
  m_center = Vector(mid_x, mid_y, mid_z);
}


void Box::SetColor(Color col, Face face)
{
  switch (face)
  {
  case TOP:
    m_t_col = col;
    m_top.SetColors(m_t_col);
    break;

  case BOTTOM:
    m_b_col = col;
    m_bot.SetColors(m_b_col);
    break;

  case LEFT:
    m_l_col = col;
    m_left.SetColors(m_l_col);
    break;

  case RIGHT:
    m_r_col = col;
    m_right.SetColors(m_r_col);
    break;

  case NEAR:
    m_n_col = col;
    m_near.SetColors(m_n_col);
    break;

  case FAR:
    m_f_col = col;
    m_far.SetColors(m_f_col);
    break;

  case ALL:
    m_t_col = col;
    m_b_col = col;
    m_l_col = col;
    m_r_col = col;
    m_n_col = col;
    m_f_col = col;
    m_top.SetColors(m_t_col);
    m_bot.SetColors(m_b_col);
    m_left.SetColors(m_l_col);
    m_right.SetColors(m_r_col);
    m_near.SetColors(m_n_col);
    m_far.SetColors(m_f_col);
    break;
  }
}

Color Box::GetColor(Face face) const
{
  switch (face)
  {
  case ALL:
  case TOP:
    return m_t_col;
  case BOTTOM:
    return m_b_col;
  case LEFT:
    return m_l_col;
  case RIGHT:
    return m_r_col;
  case NEAR:
    return m_n_col;
  case FAR:
    return m_f_col;
  }
}


void Box::Draw(TexStack* override) const
{
  TexStack texStack = m_texStack;
  if (override != NULL)
  {
    texStack = *override;
  }


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


  m_top.Draw(&texStack);
  m_bot.Draw(&texStack);
  m_left.Draw(&texStack);
  m_right.Draw(&texStack);
  m_near.Draw(&texStack);
  m_far.Draw(&texStack);

  glPopMatrix();
}

Geo* Box::GetTransformed(Vector light, Vector origin, Vector normal)
{
  Asset* ret = new Asset();
  m_top.SetTranslate(m_trans);
  Geo* top_proj = m_top.GetTransformed(light, origin, normal);
  m_top.SetTranslate(Vector(0.0, 0.0, 0.0));
  if (top_proj != NULL)
  {
    ret->AddGeo(top_proj);
  }

  m_bot.SetTranslate(m_trans);
  Geo* bot_proj = m_bot.GetTransformed(light, origin, normal);
  m_bot.SetTranslate(Vector(0.0, 0.0, 0.0));
  if (bot_proj != NULL)
  {
    ret->AddGeo(bot_proj);
  }

  m_left.SetTranslate(m_trans);
  Geo* left_proj = m_left.GetTransformed(light, origin, normal);
  m_left.SetTranslate(Vector(0.0, 0.0, 0.0));
  if (left_proj != NULL)
  {
    ret->AddGeo(left_proj);
  }

  m_right.SetTranslate(m_trans);
  Geo* right_proj = m_right.GetTransformed(light, origin, normal);
  m_right.SetTranslate(Vector(0.0, 0.0, 0.0));
  if (right_proj != NULL)
  {
    ret->AddGeo(right_proj);
  }

  m_near.SetTranslate(m_trans);
  Geo* near_proj = m_near.GetTransformed(light, origin, normal);
  m_near.SetTranslate(Vector(0.0, 0.0, 0.0));
  if (near_proj != NULL)
  {
    ret->AddGeo(near_proj);
  }

  m_far.SetTranslate(m_trans);
  Geo* far_proj = m_far.GetTransformed(light, origin, normal);
  m_far.SetTranslate(Vector(0.0, 0.0, 0.0));
  if (far_proj != NULL)
  {
    ret->AddGeo(far_proj);
  }

  return ret;
}
