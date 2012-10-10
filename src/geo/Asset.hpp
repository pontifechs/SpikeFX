#ifndef __GUARD_ASSET_HPP__
#define __GUARD_ASSET_HPP__

#include <vector>
#include <string>

#include "geo/Geo.hpp"
#include "geo/Triangle.hpp"


class Asset : public Geo
{
public:
  Asset();
  Asset(std::string filename);

  void AddGeo(Geo* geo);

  virtual void Draw(TexStack* override=0) const;
  virtual Geo* GetTransformed(Vector light, Vector origin, Vector normal);

protected:


private:
  std::vector<Geo*> m_geometry;
  
  

};



#endif
