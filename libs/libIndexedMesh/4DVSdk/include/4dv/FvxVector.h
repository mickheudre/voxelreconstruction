/*
@File FvxVector.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_VECTOR_H
#define FVX_VECTOR_H

#include <math.h>
#include <iostream>
#include <stdexcept>

/** Class describing a fixed size vector.
 * Mathematical operations can be applied.
 **/
template <int N, typename T=float>
class FvxVector {
public:
  typedef T                value_type;
  typedef T*               iterator;
  typedef const T*         const_iterator;
  typedef T&               reference;
  typedef const T&         const_reference;
  typedef std::size_t      size_type;
  typedef std::ptrdiff_t   difference_type;

  FvxVector() {}
  FvxVector(const_reference f) { assign(f); }
  FvxVector(const_reference v1, const_reference v2) { vals[0] = v1; vals[1] = v2; }
  FvxVector(const_reference v1, const_reference v2, const_reference v3) 
  { vals[0] = v1; vals[1] = v2; vals[2] = v3; }
  FvxVector(const_reference v1, const_reference v2, const_reference v3, const_reference v4) 
  { vals[0] = v1; vals[1] = v2; vals[2] = v3; vals[3] = v4; }
  FvxVector(const FvxVector<N-1,T> &v, const_reference a)
  {
    std::copy(v.begin(), v.end, begin());
    vals[N-1] = a;
  }

  template<typename T2>
  explicit FvxVector(const T2* p) { std::copy(p, p+N, begin()); }

  reference x() { return vals[0]; }
  const_reference x() const { return vals[0]; }
  reference y() { return vals[1]; }
  const_reference y() const { return vals[1]; }
  reference z() { return vals[2]; }
  const_reference z() const { return vals[2]; }
  reference w() { return vals[3]; }
  const_reference w() const { return vals[3]; }

  reference operator[](size_type i) { return vals[i]; }
  const_reference operator[](size_type i) const { return vals[i]; }

  reference at(size_type i) { checkRange(i); return vals[i]; }
  const_reference at(size_type i) const { checkRange(i); return vals[i]; }

  static size_type size() { return N; }
  static bool empty() { return false; }
  static size_type max_size() { return N; }
  
  // Iterator
  iterator begin() { return vals; }
  const_iterator begin() const { return vals; }
  iterator end() { return vals+N; }
  const_iterator end() const { return vals+N; }

  reference front() { return vals[0]; }
  const_reference front() const { return vals[0]; }
  reference back() { return vals[N-1]; }
  const_reference back() const { return vals[N-1]; }
  
  void swap(FvxVector<N,T> &v2) { std::swap_ranges(begin(), end(), v2.begin()); }

  void assign(const_reference v) { 
    std::fill_n(begin(), N, v);
//    std::fill(begin(), end(), v);
  }

  const T* data() const { return vals; }
  T* data() { return vals; }

  template <typename T2>
  FvxVector<N,T>& operator= (const FvxVector<N,T2>&v) {
    std::copy(v.begin(), v.end(), begin());
    return *this;
  }

  template <typename T2>
  FvxVector<N,T>& operator=(const T2* p) {
    std::copy(p, p+N, begin());
    return *this;
  }

  template <int M, typename T2>
  void operator=(const FvxVector<M,T2> &v) {
    std::copy(v.begin(), v.begin()+(N>M?M:N), begin());
  }

  void fill(const_reference a) { assign(a); }

  /** Linear Algebra **/

  FvxVector<N,T> operator*(const_reference f) const {
    FvxVector<N,T> r;
    for(int i = 0; i < N; i++) r[i] = f*vals[i];
    return r;
  }

  void operator *=(const_reference f) { for(int i = 0; i < N; i++) vals[i] *= f; }

  FvxVector<N,T> operator/(const_reference f) {
    FvxVector<N,T> r;
    for(int i = 0; i < N; i++) r[i] = vals[i]/f;
    return r;
  }

  void operator/=(const_reference f) { for(int i = 0; i < N; i++) vals[i] /= f; }

  T operator*(const FvxVector<N,T> &v) const {
    T r = vals[0]*v[0];
    for(int i = 1; i < N; i++) r += vals[i]*v[i];
    return r;
  }

  FvxVector<N,T> operator+(const FvxVector<N,T> &v) const {
    FvxVector<N,T> r;
    for(int i = 0; i < N; i++) r[i] = vals[i]+v[i];
    return r;
  }

  void operator+=(const FvxVector<N,T> &v) {
    for(int i = 0; i < N; i++) vals[i] += v[i];
  }

  FvxVector<N,T> operator-(const FvxVector<N,T> &v) const {
    FvxVector<N,T> r;
    for(int i = 0; i < N; i++) r[i] = vals[i]-v[i];
    return r;
  }

  void operator-=(const FvxVector<N,T> &v) {
    for(int i = 0; i < N; i++) vals[i] -= v[i];
  }

  FvxVector<N,T> operator-() const {
    FvxVector<N,T> r;
    for(int i = 0; i < N; i++) r[i] = -vals[i];
    return r;
  }
  
  T norm2() const {
    T r = vals[0]*vals[0];
    for(int i = 1; i < N; i++) r += vals[i]*vals[i];
    return r;
  }

  T norm() const { return sqrt(norm2()); }

  void normalize() {
    T r = norm();
    for(int i = 0; i < N; i++) vals[i] /= r;
  }

private:
  static void checkRange(size_type i) {
    if (i >= N) { throw std::range_error("FvxVector"); }
  }

protected:
  T vals[N];
};

template <int N, typename T>
std::ostream& operator<<(std::ostream &out, const FvxVector<N,T> &v) {
  out << v[0];
  for(int i = 1; i < N; i++) out << " " << v[i];
  return out;
}

template <int N, typename T>
std::istream& operator>>(std::istream &in, FvxVector<N,T> &v) {
  int c = in.peek();
  while (c==' ' || c=='\n' || c=='\t' || c=='<' || c=='[') { in.get(); c = in.peek(); }
  in >> v[0];
  for (int i = 1; i < N; i++) {
    c = in.peek(); while (c==' ' || c==',') { in.get(); c = in.peek(); }
    in >> v[i];
  }
  c = in.peek();
  while (c=='>' || c==']') { in.get(); c = in.peek(); }
  return in;
}

template <int N, typename T>
FvxVector<N,T> operator*(const T& f, const FvxVector<N,T> &v)
{
  return v*f;
}

template <typename T>
inline FvxVector<3,T> FvxVectorCross(const FvxVector<3,T>& a, const FvxVector<3,T> &b) {
  return FvxVector<3,T>(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]);
}

template <int N, typename T>
inline T dot(const FvxVector<N,T> &a, const FvxVector<N,T> &b) { return a*b; }

template <int N, typename T1, typename T2>
bool operator==(const FvxVector<N,T1> &v1, const FvxVector<N, T2> &v2) {
  return std::equal(v1.begin(), v1.end(), v2.begin());
}

template <typename T1, typename T2>
inline bool operator==(const FvxVector<2,T1> &v1, const FvxVector<2,T2> &v2) {
  return (v1[0] == v2[0]) && (v1[1] == v2[1]);
}
template <typename T1, typename T2>
inline bool operator==(const FvxVector<3,T1> &v1, const FvxVector<3,T2> &v2) {
  return (v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]);
}
template <typename T1, typename T2>
inline bool operator==(const FvxVector<4,T1> &v1, const FvxVector<5,T2> &v2) {
  return (v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]) && (v1[3] == v2[3]);
}

template <int N, typename T1, typename T2>
bool operator<(const FvxVector<N,T1> &v1, const FvxVector<N,T2> &v2) {
  return std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
}

template <int N, typename T1, typename T2>
inline bool operator!=(const FvxVector<N,T1> &v1, const FvxVector<N,T2> &v2) {
  return !(v1 == v2);
}

template <int N, typename T1, typename T2>
inline bool operator>(const FvxVector<N,T1> &v1, const FvxVector<N,T2> &v2) {
  return v2 < v1;
}

template <int N, typename T1, typename T2>
inline bool operator<=(const FvxVector<N,T1> &v1, const FvxVector<N,T2> &v2) {
  return !(v2 > v1);
}

template <int N, typename T1, typename T2>
inline bool operator>=(const FvxVector<N,T1> &v1, const FvxVector<N,T2> &v2) {
  return !(v1 < v2);
}

namespace std {

template <int N, typename T1, typename T2>
inline void swap(const FvxVector<N,T1> &v1, const FvxVector<N,T2> &v2) {
  v1.swap(v2);
}

} // namespace std

typedef FvxVector<2,char>           FvxVector2c;
typedef FvxVector<2,unsigned char>  FvxVector2b;
typedef FvxVector<2,short>          FvxVector2s;
typedef FvxVector<2,unsigned short> FvxVector2us;
typedef FvxVector<2,int>            FvxVector2i;
typedef FvxVector<2,unsigned int>   FvxVector2ui;
typedef FvxVector<2,float>          FvxVector2f;
typedef FvxVector<2,double>         FvxVector2d;

typedef FvxVector<3,char>           FvxVector3c;
typedef FvxVector<3,unsigned char>  FvxVector3b;
typedef FvxVector<3,short>          FvxVector3s;
typedef FvxVector<3,unsigned short> FvxVector3us;
typedef FvxVector<3,int>            FvxVector3i;
typedef FvxVector<3,unsigned int>   FvxVector3ui;
typedef FvxVector<3,float>          FvxVector3f;
typedef FvxVector<3,double>         FvxVector3d;

typedef FvxVector<4,char>           FvxVector4c;
typedef FvxVector<4,unsigned char>  FvxVector4b;
typedef FvxVector<4,short>          FvxVector4s;
typedef FvxVector<4,unsigned short> FvxVector4us;
typedef FvxVector<4,int>            FvxVector4i;
typedef FvxVector<4,unsigned int>   FvxVector4ui;
typedef FvxVector<4,float>          FvxVector4f;
typedef FvxVector<4,double>         FvxVector4d;


#endif // FVX_VECTOR_H
