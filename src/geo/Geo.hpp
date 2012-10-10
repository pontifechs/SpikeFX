
#ifndef __GUARD_GEO_HPP__
#define __GUARD_GEO_HPP__

#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "tex/TexStack.hpp"
//#include "geo/Asset.hpp"

class TexIF;

/// Provides an abstract base class which all geometry inherits from. 
class Geo
{
public:
  Geo();
  virtual void Draw(TexStack* override=0) const = 0;

  void PushTex(TexIF* tex);
  void ClearTexes();

  void SetTranslate(Vector trans);
  void SetScale(Vector scale);
  void SetRotate(float* rotation);
  
  void SetCenter(Vector center);
  Vector GetCenter() const;

  virtual Geo* GetTransformed(Vector light, Vector origin, Vector normal) = 0;

protected:
  Vector m_trans;
  Vector m_scale;

  float* m_rotation;

  Vector m_center;

  // Texture members
  TexStack m_texStack;

private:

    
};








#endif
