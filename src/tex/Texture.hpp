

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

class Texture
{
public:
  Texture(std::string filename, float alpha, GLint mode, 
	  bool repeat, GLenum target = GL_TEXTURE0); 
  
  void PrepareTexture() const;
  void DisableTexture() const;

  void SetAlpha(float alpha);
  float GetAlpha() const;
  
  void SetMode(GLint m_mode);
  GLint GetMode() const;

  void SetTranslate(Vector trans);
  void SetScale(Vector scale);
  void SetRotate(float deg, Vector axis);
  

protected:
private:
  GLenum m_target;
  GLuint m_texId;
  GLint  m_mode;
  
  std::string m_handle;
  
  float m_alpha;

  Vector m_trans;
  Vector m_scale;
  
  float m_rot_deg;
  Vector m_rot_axis;
  
};


#endif
