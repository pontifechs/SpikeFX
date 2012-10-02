#ifndef __GUARD_TEX_IF_HPP__
#define __GUARD_TEX_IF_HPP__

// OpenGL
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif




class TexIF
{
public:
  virtual void EnableTex() const = 0;
  virtual void DisableTex() const = 0;

  void SetTexUnit(GLenum texUnit)
  {
    m_texUnit = texUnit;
  }
  GLenum GetTexUnit() const
  {
    return m_texUnit;
  }

protected:
  GLuint m_texID;
  GLenum m_texUnit;

private:

  
};


#endif


