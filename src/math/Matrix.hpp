#ifndef __GUARD_MATRIX_HPP__
#define __GUARD_MATRIX_HPP__

#include "math/Vector.hpp"

class Matrix {

public:
  /// Constructs the Identity matrx.
  Matrix();
  /// Constructs a matrix in which all cells contain fill
  /// @param[in] fill The value you wish all the cells to contain.
  Matrix(float fill);
  /// Constructs a matrix and fills it with the float data specified.
  Matrix(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33);
  /// Deconstructor
 ~Matrix();
  /// Copy Constructor
  Matrix(const Matrix& original);
  /// Assignment operator
  Matrix& operator= (const Matrix& rhs);

  



  /// Mutable Accessor
  float& operator() (int row, int col);
  /// Const Accessor
  float  operator() (int row, int col) const;

  // += Operator overload
  Matrix& operator+= (const Matrix& rhs);
  // -= Operator overload
  Matrix& operator-= (const Matrix& rhs);
  // *= Operator overload
  Matrix& operator*= (const Matrix& rhs);
  
  // + Operator overload
  Matrix operator+ (const Matrix& rhs) const;
  // - Operator overload
  Matrix operator- (const Matrix& rhs) const;
  // * Operator overload
  Matrix operator* (const Matrix& rhs) const;
  // * Operator overload
  Vector operator* (const Vector& rhs) const;
  

  bool operator== (const Matrix& rhs) const;
  bool operator!= (const Matrix& rhs) const;


protected:
private:
  float* m_pData;        ///< The storage array

};

#endif
