#ifndef __GUARD_LIGHT_HPP__
#define __GUARD_LIGHT_HPP__


#include "math/Vector.hpp"
#include "math/Color.hpp"


class Light
{

public:
  Light(Vector pos, Color color, GLenum target);

  void PrepareLight();
protected:
private:
  Vector m_pos;  
  Color m_color;
  GLenum m_target;
};



#endif
