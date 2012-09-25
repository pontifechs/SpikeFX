
#include "tex/Texture.hpp"

// Libs
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif
#include <stdio.h>


#include "tex/readrgba.h"




Texture::Texture(std::string filename, float alpha, GLint mode, 
		 bool repeat, GLenum target) 
  : m_target(target),
    m_alpha(alpha),
    m_mode(mode),
    m_handle(filename)
{
  static unsigned *image;
  static int width, height, components;
    
  image = read_texture(filename.c_str(), &width, &height, &components);
  
  GLuint texId; glGenTextures(1, &texId);
  m_texId = texId;

  glBindTexture(GL_TEXTURE_2D, m_texId);
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


  printf("Created Texture for: %s. ID = %d\n", filename.c_str(), m_texId);
}

void Texture::PrepareTexture() const
{
//  printf("Preparing Texture %d\n", m_texId);
  // Apply Texture transforms


  glActiveTexture(m_target);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texId);


  if (m_target == GL_TEXTURE0)
  {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_mode);
  }
  else {
    GLfloat constColor[4] = {0.0, 0.0, 0.0, m_alpha };
    glBindTexture(GL_TEXTURE_2D, m_texId);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
  }
}

void Texture::DisableTexture() const
{
  glActiveTexture(m_target);
  glDisable(GL_TEXTURE_2D);
}

void Texture::SetAlpha(float alpha)
{
  m_alpha = alpha;
}

void Texture::SetMode(GLint mode)
{
  m_mode = mode;
}
