#ifndef __GUARD_VEC_HPP__
#define __GUARD_VEC_HPP__

// Libs


// Classes


class Vector {
private:
  float m_x;
  float m_y;
  float m_z;
  float m_w;
  
public:
  float x() const;
  float y() const;
  float z() const;
  float w() const;
  
  void setX(float f);
  void setY(float f);
  void setZ(float f);
  void setW(float f);
  
  bool operator==(const Vector& other) const;
  bool operator!=(const Vector& other) const;

  const Vector operator-(const Vector& rhs) const;
  const Vector operator+(const Vector& rhs) const;
  const Vector operator*(float scalar) const;
  

  
  Vector& operator-=(const Vector& rhs);
  Vector& operator+=(const Vector& rhs);
  Vector& operator*=(float scalar);
  
  
  Vector(float _x, float _y, float _z, float _w = 0);
  Vector();
  
  // 3-space functions
  float dot(const Vector& rhs) const;
  Vector cross(const Vector& rhs) const;
  
  // 4-space functions
  float dot4(const Vector& rhs) const;
  //Vector& cross4(const Vector& rhs); // Will create it if I need it.
  
  void normalize();
  Vector getNormalized() const;
  float length() const;
};


// Non-Member Operators
const Vector operator*(float lhs, const Vector& rhs);



#endif
