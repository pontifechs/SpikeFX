

#ifndef __GUARD_TEXT_MGR_HPP__
#define __GUARD_TEXT_MGR_HPP__



#ifdef __APPLE__ 
  #include <GLUT/glut.h>
  #include <GLUI/glui.h>
#else
  #include <GL/glut.h>
  #include <GL/glui.h>
#endif

#include <string>
#include <map>


class TexMgr
{

public:
  static TexMgr& Instance();

  void LoadTex(std::string filename);
  bool GetTex(std::string handle, GLuint& texId);

protected:
private:
  TexMgr();
  TexMgr(const TexMgr&);
  void operator=(const TexMgr&);
  
  std::map<std::string,GLuint> dict;

};




#endif
