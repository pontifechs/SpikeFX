
#ifndef __GUARD_SCENE_HPP__
#define __GUARD_SCENE_HPP__

#include <vector>

#include "geo/Geo.hpp"
#include "geo/Light.hpp"

#include "math/Color.hpp"

class Scene
{
public:
  Scene();
  Scene(Vector eye, Vector look, Vector up);
  
  void AddGeometry(Geo* new_geo);
  void AddLight(Light new_light);

  void SetFrustum(float left, float right, 
		  float top, float bot, 
		  float near, float far);

  void SetEye(Vector eye);
  void SetLook(Vector look);
  void SetUp(Vector up);

  Vector GetEye() const;
  Vector GetLook() const;
  Vector GetUp() const;

  void SetAmbient(Color color);
  Color GetAmbient() const;

  void DrawScene();
  
protected:
private:
  std::vector<Geo*> m_geometry;
  std::vector<Light> m_lighting;
  
  // Camera Coordinates
  Vector m_eye;
  Vector m_look;
  Vector m_up;

  // Frustum Coordinates
  float m_left;
  float m_right;
  float m_bot;
  float m_top;
  float m_near;
  float m_far;

  // Ambient lighting
  Color m_amb;

};



#endif
