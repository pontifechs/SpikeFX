

// Libs
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif
#include <stdio.h>


#include "tex/BaseTex.hpp"


#include "tex/readrgba.h"




BaseTex::BaseTex(std::string filename, GLint mode, bool repeat)
  : m_mode(mode),
    m_handle(filename)
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

void BaseTex::EnableTex() const
{
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texID);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_mode);

}

void BaseTex::DisableTex() const
{
  glActiveTexture(m_texUnit);
  glDisable(GL_TEXTURE_2D);
}


void BaseTex::SetMode(GLint mode)
{
  m_mode = mode;
}

GLint BaseTex::GetMode() const
{
  return m_mode;
}
