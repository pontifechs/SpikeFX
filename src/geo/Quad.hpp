#ifndef __GUARD_QUAD_HPP__
#define __GUARD_QUAD_HPP__

#include "geo/Geo.hpp"

#include "math/Vector.hpp"
#include "math/Color.hpp"

class Quad : public Geo
{

public:
  Quad();
  Quad(const Quad& rhs);
  Quad& operator=(const Quad& rhs);

  Quad(Vector pt1, Vector pt2, Vector pt3, Vector pt4);
  Quad(Vector pt1, Vector pt2, Vector pt3, Vector pt4, Color col);

  void SetColors(Color col);
  void SetColors(Color col1, Color col2, Color col3, Color col4);

  void SetTex1Coords(Vector pt1, Vector pt2, Vector pt3, Vector pt4);
  void SetTex2Coords(Vector pt1, Vector pt2, Vector pt3, Vector pt4);

  void SetNormals(Vector norm1, Vector norm2, Vector norm3, Vector norm4);
  
  virtual void Draw() const;

protected:
private:
  Vector m_pt1;
  Vector m_pt2;
  Vector m_pt3;
  Vector m_pt4;
  
  Vector m_tex1_pt1;
  Vector m_tex1_pt2;
  Vector m_tex1_pt3;
  Vector m_tex1_pt4;

  Vector m_tex2_pt1;
  Vector m_tex2_pt2;
  Vector m_tex2_pt3;
  Vector m_tex2_pt4;

  Vector m_norm1;
  Vector m_norm2;
  Vector m_norm3;
  Vector m_norm4;

  Color m_col1;
  Color m_col2;
  Color m_col3;
  Color m_col4;
  

};



#endif
