

#ifndef __GUARD_TEXTURE_HPP__
#define __GUARD_TEXTURE_HPP__


// Libs
#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif

#include <stdio.h>
#include <string>

#include "math/Vector.hpp"
#include "tex/TexIF.hpp"

class BaseTex : public TexIF
{
public:
  BaseTex(std::string filename,  GLint mode, bool repeat); 
  
  void EnableTex() const;
  void DisableTex() const;

  void SetMode(GLint m_mode);
  GLint GetMode() const;

  void SetTranslate(Vector trans);
  void SetScale(Vector scale);
  void SetRotate(float deg, Vector axis);
  

protected:
private:
  GLint  m_mode;
  
  std::string m_handle;
  

  Vector m_trans;
  Vector m_scale;
  
  float m_rot_deg;
  Vector m_rot_axis;
  
};


#endif
