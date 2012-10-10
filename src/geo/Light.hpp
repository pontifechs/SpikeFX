#ifndef __GUARD_LIGHT_HPP__
#define __GUARD_LIGHT_HPP__


#include "math/Vector.hpp"
#include "math/Color.hpp"


class Light
{

public:
  Light(Vector pos, Color color, GLenum target);

  void PrepareLight() const;
  void DisableLight() const;
  
  Vector GetPosition() const;
  void SetPosition(Vector pos);
  
  void SetTranslate(Vector trans);
  Vector GetTranslate() const;

  void SetColor(Color color);
  Color GetColor() const;

protected:
private:
  Vector m_pos;  
  Vector m_trans;
  Color m_color;
  GLenum m_target;
};



#endif
