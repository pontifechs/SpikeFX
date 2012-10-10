

#ifndef __GUARD_BOX_HPP__
#define __GUARD_BOX_HPP__

#include "geo/Geo.hpp"

#include "math/Color.hpp"
#include "geo/Quad.hpp"

class Vector;

class Box : public Geo
{

public:
  enum Face 
  {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    NEAR,
    FAR,
    ALL,
  };


  Box(Vector center, float x_len, float y_len, float z_len);
  Box(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z);
  
  void SetColor(Color col, Face face = ALL);
  Color GetColor(Face face) const;

  virtual void Draw(TexStack* override) const;
  virtual Geo* GetTransformed(Vector light, Vector origin, Vector normal);

protected:
private:

  void BuildQuads(float min_x, float max_x, 
		  float min_y, float max_y, 
		  float min_z, float max_z);
  Quad m_top;
  Quad m_bot;
  Quad m_left;
  Quad m_right;
  Quad m_near;
  Quad m_far;  

  Color m_t_col;
  Color m_b_col;
  Color m_l_col;
  Color m_r_col;
  Color m_n_col;
  Color m_f_col;

};


#endif
