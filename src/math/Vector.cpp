
// Libs
#include <math.h>

// Classes
#include "math/Vector.hpp"


// Equality Overloads
bool Vector::operator==(const Vector& other) const {
  return (m_x == other.x()) && (m_y == other.y()) && (m_z == other.z()) && (m_w == other.w());
}
bool Vector::operator!=(const Vector& other) const {
  return !(*this == other);
}

const Vector Vector::operator-(const Vector& rhs) const
{
  return Vector(*this) -= rhs;
}

const Vector Vector::operator+(const Vector& rhs) const
{
  return Vector(*this) += rhs;
}

const Vector Vector::operator*(float scalar) const
{
  return Vector(*this) *= scalar;
}


Vector& Vector::operator-=(const Vector& rhs)
{
  m_x -= rhs.m_x;
  m_y -= rhs.m_y;
  m_z -= rhs.m_z;
  m_w -= rhs.m_w;
  return *this;
}

Vector& Vector::operator+=(const Vector& rhs)
{
  m_x += rhs.m_x;
  m_y += rhs.m_y;
  m_z += rhs.m_z;
  m_w += rhs.m_w;
  return *this;
}

Vector& Vector::operator*=(float scalar)
{
  m_x *= scalar;
  m_y *= scalar;
  m_z *= scalar;
  m_w *= scalar;
  return *this;
}

// Getters
float Vector::x() const {
  return m_x;
}	
float Vector::y() const {
  return m_y;
}
float Vector::z() const {
  return m_z;
}
float Vector::w() const {
  return m_w;
}

// Setters
void Vector::setX(float f) {
  m_x = f;
}
void Vector::setY(float f) {
  m_y = f;
}
void Vector::setZ(float f) {
  m_z = f;
}
void Vector::setW(float f) {
  m_w = f;
}

// Constructor
Vector::Vector(float _x, float _y, float _z, float _w)
  : m_x(_x), m_y(_y), m_z(_z), m_w(_w)
{}

Vector::Vector()
  : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(0.0f)
{}



// Dot-Product (3-space only).
float Vector::dot(const Vector& rhs) const
{
  return (m_x * rhs.x()) + (m_y * rhs.y()) + (m_z * rhs.z());
}

// Cross-Product (3-space only).
Vector Vector::cross(const Vector& rhs) const
{
  float newX = (m_y * rhs.z()) - (m_z * rhs.y());
  float newY = (m_z * rhs.x()) - (m_x * rhs.z());
  float newZ = (m_x * rhs.y()) - (m_y * rhs.x());
  return Vector(newX, newY, newZ, 0.0);
}

// Dot-Product (4-space only).
float Vector::dot4(const Vector& rhs) const
{
  return (m_x * rhs.x()) + (m_y * rhs.y()) + (m_z * rhs.z()) + (m_w * rhs.w());
}

float Vector::length() const
{
  return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

void Vector::normalize()
{
  float len = this->length();
  m_x /= len;
  m_y /= len;
  m_z /= len;
}

Vector Vector::getNormalized() const
{
  Vector temp(m_x, m_y, m_z, m_w);
  temp.normalize();
  return temp;
}

const Vector operator*(float lhs, const Vector& rhs)
{
  Vector ret(rhs);
  ret *= lhs;
  return ret;
}

