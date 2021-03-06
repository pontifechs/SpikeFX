#ifndef __GUARD_QUAD_HPP__
#define __GUARD_QUAD_HPP__

#include <vector>

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

  void PushTexCoords(Vector pt1, Vector pt2, Vector pt3, Vector pt4);
  void ClearTexCoords();

  void SetNormals(Vector norm1, Vector norm2, Vector norm3, Vector norm4);
  Vector GetNormal() const;
  Vector GetOrigin() const;
  
  virtual void Draw(TexStack* override=0) const;
  virtual Geo* GetTransformed(Vector light, Vector origin, Vector normal);


protected:
private:
  Vector m_pt1;
  Vector m_pt2;
  Vector m_pt3;
  Vector m_pt4;
  
  Vector m_norm1;
  Vector m_norm2;
  Vector m_norm3;
  Vector m_norm4;

  Color m_col1;
  Color m_col2;
  Color m_col3;
  Color m_col4;
  
  std::vector<Vector> m_tex1_stack;
  std::vector<Vector> m_tex2_stack;
  std::vector<Vector> m_tex3_stack;
  std::vector<Vector> m_tex4_stack;

};



#endif
