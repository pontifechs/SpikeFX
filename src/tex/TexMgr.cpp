
#include "readrgba.h"

#include "tex/TexMgr.hpp"

TexMgr& TexMgr::Instance()
{
  static TexMgr texMgr;
  return texMgr;
}

TexMgr::TexMgr()
{
}

void TexMgr::LoadTex(std::string filename)
{
  static unsigned *image;
  static int width, height, components;
    
  image = read_texture(filename.c_str(), &width, &height, &components);

  GLuint texId; glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);
  glTexImage2D(GL_TEXTURE_2D, 0, components, width,
	       height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	       image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


  dict[filename] = texId;
  printf("%s :: %d", filename.c_str(), texId);
  free(image); 
}

bool TexMgr::GetTex(std::string handle, GLuint& texId)
{
  if (dict.find(handle) == dict.end())
  {
    texId = dict[handle];
    return true;
  }
  else {
    return false;
  }
}

