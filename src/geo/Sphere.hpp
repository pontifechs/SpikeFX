
#ifndef __GUARD_SPHERE_HPP__
#define __GUARD_SPHERE_HPP__

#include "geo/Geo.hpp"

// Libs
#include <vector>

// Classes
#include "math/Vector.hpp"
#include "math/Color.hpp"
#include "geo/Triangle.hpp"


class Sphere : public Geo
{
public:
  ~Sphere();
  Sphere();

  void Generate(Vector center, float radius, float angle_increment = 5);

  virtual void Draw() const;

  // std::vector<Triangle> getTris(); 

protected:
private:

  std::vector<Triangle> tris;

};




#endif


