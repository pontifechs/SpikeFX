#ifndef __GUARD_ASSET_HPP__
#define __GUARD_ASSET_HPP__

#include <vector>
#include <string>

#include "geo/Geo.hpp"
#include "geo/Triangle.hpp"


class Asset : public Geo
{
public:
  Asset(std::string filename);

  virtual void Draw(TexStack* override=0) const;

protected:


private:
  std::vector<Triangle> m_geometry;
  
  

};



#endif
