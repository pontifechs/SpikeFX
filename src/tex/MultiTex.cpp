
#include "tex/MultiTex.hpp"


#include "tex/readrgba.h"


MultiTex::MultiTex(std::string filename, float alpha, bool repeat)
  : m_alpha(alpha)
{
  static unsigned *image;
  static int width, height, components;
    
  image = read_texture(filename.c_str(), &width, &height, &components);
  
  GLuint texId; glGenTextures(1, &texId);
  m_texID = texId;

  glBindTexture(GL_TEXTURE_2D, m_texID);
  gluBuild2DMipmaps( GL_TEXTURE_2D, components, width, height, 
  		     GL_RGBA, GL_UNSIGNED_BYTE, image );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if (repeat) 
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }
  else 
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);    
  }
}

void MultiTex::EnableTex() const
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texID);

  GLfloat constColor[4] = {0.0, 0.0, 0.0, m_alpha };
  glBindTexture(GL_TEXTURE_2D, m_texID);
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);

}

void MultiTex::DisableTex() const
{
  glActiveTexture(m_texUnit);
  glDisable(GL_TEXTURE_2D);
}

void MultiTex::SetAlpha(float alpha)
{
  m_alpha = alpha; 
}

float MultiTex::GetAlpha() const
{
  return m_alpha;
}
