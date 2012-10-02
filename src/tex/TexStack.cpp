#include "tex/TexStack.hpp"




TexStack::TexStack()
{
}

void TexStack::EnableTexStack() const
{
  for (int i = 0; i < m_stack.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    m_stack[i]->EnableTex();
  }
}

void TexStack::DisableTexStack() const
{
  for (int i = 0; i < m_stack.size(); i++)
  {
    m_stack[i]->DisableTex();
  }
}

void TexStack::PushTex(TexIF* tex)
{
  m_stack.push_back(tex);
}


void TexStack::ClearTexes()
{
  m_stack.clear();
}
