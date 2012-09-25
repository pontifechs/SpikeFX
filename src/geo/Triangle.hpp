
#ifndef __GUARD_TRIANGLE_HPP__
#define __GUARD_TRIANGLE_HPP__

#include <vector>

#include "geo/Geo.hpp"

#include "math/Vector.hpp"
#include "math/Color.hpp"
#include "tex/Texture.hpp"

class Triangle : public Geo 
{
public:
  Triangle();
  ~Triangle();
  Triangle(Vector pt1, Vector pt2, Vector pt3);

  Triangle(Vector pt1, Vector pt2, Vector pt3, Color col);

  Triangle(Vector pt1, Vector pt2, Vector pt3, 
	   Color col1, Color col2, Color col3);

  void SetTexCoords(Vector pt1, Vector pt2, Vector pt3);

  void SetTex1Coords(Vector pt1, Vector pt2, Vector pt3);
  void SetTex2Coords(Vector pt1, Vector pt2, Vector pt3);

  void SetNormals(Vector norm1, Vector norm2, Vector norm3);

  Vector pt1() const;
  Vector pt2() const;
  Vector pt3() const;

  virtual void Draw() const;

protected:
private:
  
  Vector m_pt1;
  Vector m_pt2;
  Vector m_pt3;

  Vector m_norm1;
  Vector m_norm2;
  Vector m_norm3;

  Vector m_pt1_tex1;
  Vector m_pt2_tex1;
  Vector m_pt3_tex1;

  Vector m_pt1_tex2;
  Vector m_pt2_tex2;
  Vector m_pt3_tex2;

  Color m_col1;
  Color m_col2;
  Color m_col3;

};





#endif

