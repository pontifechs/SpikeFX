
#ifndef __GUARD_GEO_HPP__
#define __GUARD_GEO_HPP__

#include "tex/Texture.hpp"
#include "tex/TexMgr.hpp"

#include "math/Vector.hpp"



/// Provides an abstract base class which all geometry inherits from. 
class Geo 
{
public:

  Geo();
  virtual void Draw() const = 0;

  void RegisterTex(Texture* tex);
  
  void RegisterTex1(Texture* tex);
  void RegisterTex2(Texture* tex);

  void SetTranslate(Vector trans);
  void SetScale(Vector scale);
  void SetRotate(float* rotation);
  
  void SetCenter(Vector center);
  Vector GetCenter() const;

protected:
  Texture* m_pTex1;
  Texture* m_pTex2;
  
  Vector m_trans;
  Vector m_scale;

  float* m_rotation;

  Vector m_center;
  
private:


    
};








#endif
