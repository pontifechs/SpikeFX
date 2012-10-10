
#include "math/Matrix.hpp"


// Libs
#include <iostream>

// Classes




/// Constructs the Identity matrx.
Matrix::Matrix() 
{
  m_pData = new float[16];
  m_pData[0]  = 1.0f; m_pData[1]  = 0.0f; m_pData[2]  = 0.0f; m_pData[3]  = 0.0f;
  m_pData[4]  = 0.0f; m_pData[5]  = 1.0f; m_pData[6]  = 0.0f; m_pData[7]  = 0.0f;
  m_pData[8]  = 0.0f; m_pData[9]  = 0.0f; m_pData[10] = 1.0f; m_pData[11] = 0.0f;
  m_pData[12] = 0.0f; m_pData[13] = 0.0f; m_pData[14] = 0.0f; m_pData[15] = 1.0f;
}

/// Constructs a matrix in which all cells contain fill
/// @param[in] fill The value you wish all the cells to contain.
Matrix::Matrix(float fill)
{
  m_pData = new float[16];
  m_pData[0]  = fill; m_pData[1]  = fill; m_pData[2]  = fill; m_pData[3]  = fill;
  m_pData[4]  = fill; m_pData[5]  = fill; m_pData[6]  = fill; m_pData[7]  = fill;
  m_pData[8]  = fill; m_pData[9]  = fill; m_pData[10] = fill; m_pData[11] = fill;
  m_pData[12] = fill; m_pData[13] = fill; m_pData[14] = fill; m_pData[15] = fill;
}

/// Constructs a matrix and fills it with the float data specified.
Matrix::Matrix(float m00, float m01, float m02, float m03,
	       float m10, float m11, float m12, float m13,
	       float m20, float m21, float m22, float m23,
	       float m30, float m31, float m32, float m33)
{
  m_pData = new float[16];
  m_pData[0]  = m00; m_pData[1]  = m01; m_pData[2]  = m02; m_pData[3]  = m03;
  m_pData[4]  = m10; m_pData[5]  = m11; m_pData[6]  = m12; m_pData[7]  = m13;
  m_pData[8]  = m20; m_pData[9]  = m21; m_pData[10] = m22; m_pData[11] = m23;
  m_pData[12] = m30; m_pData[13] = m31; m_pData[14] = m32; m_pData[15] = m33;
}

/// Deconstructor
Matrix::~Matrix()
{
  delete[] m_pData;
}

/// Copy Constructor
Matrix::Matrix(const Matrix& original)
{
  m_pData = new float[16];
  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      m_pData[row * 4 + col] = original(row, col);
    }
  }
}
/// Assignment operator
Matrix& Matrix::operator= (const Matrix& rhs)
{
  // De-allocate m_pData
  delete[] m_pData;
  m_pData = new float[16];
  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      m_pData[row * 4 + col] = rhs(row, col);
    }
  }
  return *this;
}

/// Mutable Accessor
float& Matrix::operator() (int row, int col)
{
  return m_pData[row * 4 + col];
}
/// Const Accessor
float  Matrix::operator() (int row, int col) const
{
  return m_pData[row * 4 + col];
}
// += Operator overload
Matrix& Matrix::operator+= (const Matrix& rhs)
{
  for (int i = 0; i < 16; i++)
  {
    m_pData[i] += rhs.m_pData[i];
  }
  return *this;
}
// -= Operator overload
Matrix& Matrix::operator-= (const Matrix& rhs)
{
  for (int i = 0; i < 16; i++)
  {
    m_pData[i] -= rhs.m_pData[i];
  }
  return *this;
}
// *= Operator overload
Matrix& Matrix::operator*= (const Matrix& rhs) 
{
  float temp[16];
  temp[0] = (m_pData[0] * rhs.m_pData[0]) 
          + (m_pData[1] * rhs.m_pData[4])
          + (m_pData[2] * rhs.m_pData[8]) 
          + (m_pData[3] * rhs.m_pData[12]);
  temp[1] = (m_pData[0] * rhs.m_pData[1]) 
          + (m_pData[1] * rhs.m_pData[5])
          + (m_pData[2] * rhs.m_pData[9]) 
          + (m_pData[3] * rhs.m_pData[13]);
  temp[2] = (m_pData[0] * rhs.m_pData[2]) 
          + (m_pData[1] * rhs.m_pData[6])
          + (m_pData[2] * rhs.m_pData[10]) 
          + (m_pData[3] * rhs.m_pData[14]);
  temp[3] = (m_pData[0] * rhs.m_pData[3]) 
          + (m_pData[1] * rhs.m_pData[7])
          + (m_pData[2] * rhs.m_pData[11]) 
          + (m_pData[3] * rhs.m_pData[15]);

  temp[4] = (m_pData[4] * rhs.m_pData[0]) 
          + (m_pData[5] * rhs.m_pData[4])
          + (m_pData[6] * rhs.m_pData[8]) 
          + (m_pData[7] * rhs.m_pData[12]);
  temp[5] = (m_pData[4] * rhs.m_pData[1]) 
          + (m_pData[5] * rhs.m_pData[5])
          + (m_pData[6] * rhs.m_pData[9]) 
          + (m_pData[7] * rhs.m_pData[13]);
  temp[6] = (m_pData[4] * rhs.m_pData[2]) 
          + (m_pData[5] * rhs.m_pData[6])
          + (m_pData[6] * rhs.m_pData[10]) 
          + (m_pData[7] * rhs.m_pData[14]);
  temp[7] = (m_pData[4] * rhs.m_pData[3]) 
          + (m_pData[5] * rhs.m_pData[7])
          + (m_pData[6] * rhs.m_pData[11]) 
          + (m_pData[7] * rhs.m_pData[15]);

  temp[8] = (m_pData[8] * rhs.m_pData[0]) 
          + (m_pData[9] * rhs.m_pData[4])
          + (m_pData[10] * rhs.m_pData[8]) 
          + (m_pData[11] * rhs.m_pData[12]);
  temp[9] = (m_pData[8] * rhs.m_pData[1]) 
          + (m_pData[9] * rhs.m_pData[5])
          + (m_pData[10] * rhs.m_pData[9]) 
          + (m_pData[11] * rhs.m_pData[13]);
  temp[10]= (m_pData[8] * rhs.m_pData[2]) 
          + (m_pData[9] * rhs.m_pData[6])
          + (m_pData[10] * rhs.m_pData[10]) 
          + (m_pData[11] * rhs.m_pData[14]);
  temp[11]= (m_pData[8] * rhs.m_pData[3]) 
          + (m_pData[9] * rhs.m_pData[7])
          + (m_pData[10] * rhs.m_pData[11]) 
          + (m_pData[11] * rhs.m_pData[15]);

  temp[12]= (m_pData[12] * rhs.m_pData[0]) 
          + (m_pData[13] * rhs.m_pData[4])
          + (m_pData[14] * rhs.m_pData[8]) 
          + (m_pData[15] * rhs.m_pData[12]);
  temp[13]= (m_pData[12] * rhs.m_pData[1]) 
          + (m_pData[13] * rhs.m_pData[5])
          + (m_pData[14] * rhs.m_pData[9]) 
          + (m_pData[15] * rhs.m_pData[13]);
  temp[14]= (m_pData[12] * rhs.m_pData[2]) 
          + (m_pData[13] * rhs.m_pData[6])
          + (m_pData[14] * rhs.m_pData[10]) 
          + (m_pData[15] * rhs.m_pData[14]);
  temp[15]= (m_pData[12] * rhs.m_pData[3]) 
          + (m_pData[13] * rhs.m_pData[7])
          + (m_pData[14] * rhs.m_pData[11]) 
          + (m_pData[15] * rhs.m_pData[15]);

  for (int i = 0; i < 16; i++)
  {
    m_pData[i] = temp[i];
  }
  return *this;
}  

// + Operator overload
Matrix Matrix::operator+ (const Matrix& rhs) const
{
  Matrix result = *this;
  result += rhs;
  return result;
}

// - Operator overload
Matrix Matrix::operator- (const Matrix& rhs) const
{
  Matrix result = *this;
  result -= rhs;
  return result;
}

// * Operator overload
Matrix Matrix::operator* (const Matrix& rhs) const
{
  Matrix result = *this;
  result *= rhs;
  return result;
}

Vector Matrix::operator* (const Vector& rhs) const
{
  float x = 
    (m_pData[0] * rhs.x()) + 
    (m_pData[1] * rhs.y()) + 
    (m_pData[2] * rhs.z()) + 
    (m_pData[3] * rhs.w());

  float y = 
    (m_pData[4] * rhs.x()) + 
    (m_pData[5] * rhs.y()) + 
    (m_pData[6] * rhs.z()) + 
    (m_pData[7] * rhs.w());

  float z = 
    (m_pData[8] * rhs.x()) + 
    (m_pData[9] * rhs.y()) + 
    (m_pData[10] * rhs.z()) + 
    (m_pData[11] * rhs.w());

  float w = 
    (m_pData[12] * rhs.x()) + 
    (m_pData[13] * rhs.y()) + 
    (m_pData[14] * rhs.z()) + 
    (m_pData[15] * rhs.w());

  return Vector(x, y, z, w);
}

bool Matrix::operator== (const Matrix& rhs) const
{
  for (int i = 0; i < 16; i++)
  {
    if (m_pData[i] != rhs.m_pData[i])
    {
      return false;
    }
  }
  return true;
}

bool Matrix::operator!= (const Matrix& rhs) const
{
  return !(*this == rhs);
}
  
  
