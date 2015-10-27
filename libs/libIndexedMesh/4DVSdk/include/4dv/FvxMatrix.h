/*
@File FvxMatrix.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_MATRIX_H
#define FVX_MATRIX_H

#include "4dv/FvxVector.h"

/** Class describing a matrix.
 **/
template <int M, int N, typename T=float>
class FvxMatrix : protected FvxVector<M,FvxVector<N,T> >
{
public:
  typedef T&               reference;
  typedef const T&         const_reference;
  typedef std::size_t      size_type;

  FvxMatrix() {}

  void fill(const_reference r) { assign(r); }

  void assign(const_reference r) {
    for(int i = 0; i < M; i++)
      this->vals[i].fill(r);
  }

  const FvxVector<N, T> &operator[](int i) const { return (*(FvxVector<M,FvxVector<N,T> >*)this)[i]; }
  FvxVector<N, T> &operator[](int i) { return (*(FvxVector<M,FvxVector<N,T> >*)this)[i]; }

  const_reference operator()(int i, int j) const {
    return this->vals[i][j];
  }
  reference operator()(int i, int j) {
    return this->vals[i][j];
  }

  FvxVector<M, T> operator*(const FvxVector<N, T> &v) const {
    FvxVector<M, T> r;
    for(int i = 0; i < M; i++) {
      r[i] = this->vals[i][0]*v[0];
      for(int j = 1; j < N; j++) {
        r[i] += this->vals[i][j]*v[j];
      }
    }
    return r;
  }

  FvxMatrix<M, N, T> transpose() {
    FvxMatrix<M, N, T> mat;
    for(int i = 0; i < M; i++)
      for(int j = 0; j < N; j++)
        mat[j][i] = this->vals[i][j];
    return mat;
  }

  FvxMatrix<M, N, T> operator-() {
    FvxMatrix<M, N, T> m;
    for(int i = 0; i < M; i++)
      for(int j = 0; j < N; j++)
        m[i][j] = -this->vals[i][j];
    return m;
  }

  template<int N2>
  FvxMatrix<M, N2> operator*(const FvxMatrix<N, N2, T> &m) {
    FvxMatrix<M, N2> r;
    for(int i = 0; i < M; i++)
      for(int j = 0; j < N2; j++) {
        r[i][j] = this->vals[i][0]*m[0][j];
        for(int z = 1; z < N; z++)
          r[i][j] += this->vals[i][z]*m[z][j];
      }
    return r;
  }

};

template <int M, int N, typename T>
std::istream& operator>>(std::istream &in, FvxMatrix<M, N, T> &m) {
  for(int i = 0; i < M; i++)
    in >> m[i];
  return in;
}

template <int M, int N, typename T>
std::ostream& operator<<(std::ostream &out, const FvxMatrix<M, N, T> &m) {
  out << m[0];
  for(int i = 1; i < M; i++)
    out << " " << m[i];
  return out;
}

typedef FvxMatrix<3,3,float>            FvxMatrix3x3f;
typedef FvxMatrix<3,4,float>            FvxMatrix3x4f;
typedef FvxMatrix<4,4,float>            FvxMatrix4x4f;


#endif // FVX_MATRIX_H1
