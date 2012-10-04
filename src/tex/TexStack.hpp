#ifndef __GUARD_TEX_STACK_HPP__
#define __GUARD_TEX_STACK_HPP__


#include <vector>

#include "tex/TexIF.hpp"


class TexStack
{
public:
  TexStack();
  void EnableTexStack() const;
  void DisableTexStack() const;
  
  void PushTex(TexIF* tex);
  void ClearTexes();

  int TexCount() const;
  
protected:
private:

  std::vector<TexIF*> m_stack;  

};















#endif
