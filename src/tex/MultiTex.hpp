#ifndef __GUARD_MULTI_TEX_HPP__
#define __GUARD_MULTI_TEX_HPP__

#include <string>

#include "tex/TexIF.hpp"


class MultiTex : public TexIF
{
public:
  MultiTex(std::string filename, float alpha, bool repeat);
  
  void EnableTex() const;
  void DisableTex() const;

  void SetAlpha(float alpha);
  float GetAlpha() const;
  
  


protected:
private:

  GLenum m_combinerFunc;

  float m_alpha;
  
  
  
};




#endif
