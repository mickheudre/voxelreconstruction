/** @file linalg/Vector.h
 * Classic linear algebra entity.
 * Author: Jean-Sebastien FRANCO
 * Date: 23/01/2001
 */

#ifndef _Vector_h_
#define _Vector_h_

#include <assert.h>
#include <iostream>
#include <math.h>
#ifndef NO_GUI
#ifndef APPLE
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#endif

//#include "Object.h"
typedef double Real;

template <class T, unsigned int N>
class RegularStorage {
public:
  typedef T value_type;
  inline T& operator[](unsigned int i) {assert(i<N); return m_val[i];}
  inline const T& operator[](unsigned int i) const
  {assert(i<N); return m_val[i];}

  inline void set (const T* r) {
    for (int i=0; i<N; ++i)
      m_val[i]=r[i];
  }

  T m_val[N];
};

/*
template <class Bracketable, class T, int skip, int offset=0>
class SkipStorage {
public:
  typedef T value_type;
  inline T& operator[](unsigned int i) {return (*m_b)[offset+i*skip];}
  inline const T& operator[](unsigned int i) const {return (*m_b)[offset+i*skip];}

  Bracketable* m_b;
};

template <class Bracketable, class T, int skip, int offset=0>
class ConstSkipStorage {
public:
  typedef T value_type;
  inline const T& operator[](unsigned int i) const {return (*m_b)[offset+i*skip];}

  const Bracketable* m_b;
};
*/

/*template <class T, class STORAGE<T>, int skip=1>
  class SkipStorage {
    public:
    typedef T value_type;
    inline T& operator[](unsigned int i) {return m_b[i*skip];}
    inline const T& operator[](unsigned int i) const {return (*m_b)[i*skip];}
    
    T* m_b;
    };*/

template <class T, int skip>
  class SkipStorage {
  public:
  typedef T value_type;
  inline T& operator[](unsigned int i) {return m_b[i*skip];}
  inline const T& operator[](unsigned int i) const {return (*m_b)[i*skip];}
  
  T* m_b;
};

template <class T, int skip>
class ConstSkipStorage {
public:
  typedef T value_type;
  inline const T& operator[](unsigned int i) const {return m_b[i*skip];}

  const T* m_b;
};

/**
 * Generic vector class, template size and storage type
 */
template <unsigned int NN, class StorageType=RegularStorage<Real, NN> >
class Vector {
public:

  /// make N value available to outside users
  enum {N=NN};
  typedef typename StorageType::value_type T;
  typedef typename StorageType::value_type value_type;
  typedef Vector<N, RegularStorage<T, N> > TmpStorage;

  /// empty constructor (Vector left in undefined state)
  inline Vector() {};
  
  /// initialize from array
  inline Vector(const T* init) { assert(N>0); set(init); }

  /// initialize to single constant
  inline Vector(const T& value) { assert(N>0); set(value); }

  /// initialize to constant at position pos, 0 everywhere else
  inline Vector(unsigned int pos, const T& value) { assert(N>0); set(pos,value); }

  template <class Storage>
  inline Vector(const Vector<NN, Storage>& v)
  { assert(NN>0); set(v);}

  /// set to array
  template <class Storage>
  inline void set(const Vector<N, Storage>& v2) 
    {
      (*this)=v2;
    }

  template <class Storage>
  inline void operator=(const Vector<N, Storage>& v2) 
    {
      for (int i=0; i<N; ++i)
	(*this)[i]=v2[i];
    }

  /// set to single constant
  inline void set(const T& value)
    {
      for (int i=0; i<N; ++i)
	(*this)[i]=value;
    }

  /// set to single constant
  inline void set(const T* r)
    {
      for (int i=0; i<N; ++i)
	(*this)[i]=r[i];
    }


  /// set to constant at position i, 0 everywhere else
  inline void set(unsigned int pos, const T& value)
    {
      assert(pos<N);
      unsigned int i=0;
      for (; i<pos; ++i)
	(*this)[i]=0.;

      (*this)[i++]=value;

      for (; i<N; ++i)
	(*this)[i]=0.;

    }

  /// data accessor
  const T *data() const
    {
      return (T*) &m_storage[0];
    }

  inline void normalize()
    {
      T invnorm=1.0f/norm();
      (*this)*=invnorm;
    }

  /// get norm 2
  inline T norm() const
    {
      T res=m_storage[0]*m_storage[0];
      for (int i=1; i<N; ++i)
	res+=m_storage[i]*m_storage[i];
      return sqrt(res);
    }
      
  
  /// individual element selector
  inline const T& operator[](unsigned int i) const
  {assert(i<N); return m_storage[i];}

  /// individual element mutator
  inline T& operator[](unsigned int i)
  {assert(i<N); return m_storage[i];}

  /// dot product operator
  template <class Storage>
  inline T operator *(const Vector<N, Storage>& v) const
    {
      T res=m_storage[0]*v[0];

      for (int i=1; i<N; ++i)
	res+=m_storage[i]*v[i];

      return res;
    }
       
  /// addition
  template <class St1>
  inline TmpStorage operator +(const Vector<N, St1>& v) const
    {
      TmpStorage res;
      for (int i=0; i<N; ++i)
	res[i]=m_storage[i]+v[i];
      return res;
    }

  /// substraction
  template <class St1>
  inline TmpStorage operator -(const Vector<N, St1>& v) const
    {
      TmpStorage res;
      for (int i=0; i<N; ++i)
	res[i]=m_storage[i]-v[i];
      return res;
    }
  
  // return -v
  inline TmpStorage operator -() const
    {
      TmpStorage res;
      for (int i=0; i<N; ++i)
	res[i]=-m_storage[i];
      return res;
    }

  // accumulators

  /// scalar multiply accumulator
  inline void operator *=(const T& f)
    {
      for (int i=0; i<N; ++i)
	m_storage[i]*=f;
    }

  /// scalar divide accumulator
  inline void operator /=(const T& f)
    {
      for (int i=0; i<N; ++i)
	m_storage[i]/=f;
    }

  /// component to component multiplication accumulator
  template <class Storage>
  inline void operator *=(const Vector<N, Storage>& v)
    {
      for (int i=0; i<N; ++i)
	m_storage[i]*=v[i];
    }

  /// component to component multiplication accumulator
  template <class Storage>
  inline void operator /=(const Vector<N, Storage>& v)
    {
      for (int i=0; i<N; ++i)
	m_storage[i]/=v[i];
    }

  /// addition accumulator
  template <class Storage>
  inline void operator +=(const Vector<N, Storage>& v)
    {
      for (int i=0; i<N; ++i)
	m_storage[i]+=v[i];
    }

  /// substraction accumulator
  template <class Storage>
  inline void operator -=(const Vector<N, Storage>& v)
    {
      for (int i=0; i<N; ++i)
	m_storage[i]-=v[i];
    }

  /// negate vector
  inline void neg()
    {
      for (int i=0; i<N; ++i)
	m_storage[i]-=m_storage[i];
    }

  /// cross product
  template <class St1, class St2>
  inline void setToCross(const Vector<N, St1>& v1,
			 const Vector<N, St2>& v2)
    {
      // does not make sense otherwise
      assert(N>2);
      int i;
      for (i=0; i<N-2; ++i)
	m_storage[i]=v1[i+1]*v2[i+2]-v1[i+2]*v2[i+1];

      m_storage[i]=v1[i+1]*v2[0]-v1[0]*v2[i+1];
      m_storage[i+1]=v1[0]*v2[1]-v1[1]*v2[0];
    }

  // tester
  template <class Storage>
  inline bool operator==(const Vector<N, Storage>& v) const
    {
      int i=0;
      for (; (i<N) && (m_storage[i]==v[i]); ++i) ;
      return i==N;
    }

  // tester
  template <class Storage>
  inline bool operator!=(const Vector<N, Storage>& v) const
    {
      return !(*this==v);
    }

protected:
  StorageType m_storage;
};

template <unsigned int N, int Skip=1, class T=Real>
class VectorView : public Vector<N, SkipStorage<T, Skip> > {

  typedef typename VectorView::T ST;
public:

  /// empty constructor (Vector left in undefined state)
  inline VectorView() {};
  
  /// initialize from array
  inline VectorView(ST* init) { assert(N>0); set(init); }

  /// initialize to single constant
  inline VectorView(const ST& value) { assert(N>0); set(value); }

  /// initialize to constant at position pos, 0 everywhere else
  inline VectorView(unsigned int pos, const ST& value) { assert(N>0); set(pos,value); }

  template <class Storage>
  inline VectorView(const Vector<N, Storage>& v)
  { assert(N>0); set(v);}

  void set(ST* r) {this->m_storage.m_b=r;}
};

template <unsigned int N, int Skip=1, class T=Real>
class ConstVectorView : public Vector<N, ConstSkipStorage<T, Skip> > {

  typedef typename ConstVectorView::T ST;

public:

  /// empty constructor (Vector left in undefined state)
  inline ConstVectorView() {};
  
  /// initialize from array
  inline ConstVectorView(const ST* init) { assert(N>0); set(init); }

  /// initialize to single constant
  inline ConstVectorView(const ST& value) { assert(N>0); set(value); }

  /// initialize to constant at position pos, 0 everywhere else
  inline ConstVectorView(unsigned int pos, const ST& value) { assert(N>0); set(pos,value); }

  template <class Storage>
  inline ConstVectorView(const Vector<N, Storage>& v)
  { assert(N>0); set(v);}

  void set(const ST* r) {this->m_storage.m_b=r;}
};

class Vector2 : public Vector<2>
{
public:
  inline Vector2(){}

  template <class St>
  inline Vector2(const Vector<2, St>& v)
    {(*this)[0]=v[0]; (*this)[1]=v[1];}
  /// initialize to single constant
  inline Vector2(const T& value) { assert(N>0); set(value); }

  inline Vector2(T a, T b)
    {
      (*this)[0]=a; (*this)[1]=b;
    }

#ifndef NO_GUI // CM
  void glVertex() const
    {
      glVertex2d((*this)[0], (*this)[1]);
    }

  void glGet(GLenum e) {
    double tmp[2]; glGetDoublev(e, tmp);
    (*this)[0]=tmp[0]; (*this)[1]=tmp[1];
  }

#endif // NO_GUI
};

class Vector3 : public Vector<3>
{
public:
  inline Vector3(){}

  template <class St>
  inline Vector3(const Vector<3, St>& v)
    {(*this)[0]=v[0]; (*this)[1]=v[1]; (*this)[2]=v[2];}
  
  inline Vector3(T a, T b, T c)
    {
      (*this)[0]=a; (*this)[1]=b; (*this)[2]=c;
    }

#ifndef NO_GUI // CM
  void glVertex() const
    {
      glVertex3d((*this)[0], (*this)[1], (*this)[2]);
    }

  void glNormal() const
    {
      glNormal3d((*this)[0], (*this)[1], (*this)[2]);
    }

  void glGet(GLenum e) {
    double tmp[3]; glGetDoublev(e, tmp);
    (*this)[0]=tmp[0]; (*this)[1]=tmp[1]; (*this)[2]=tmp[2]; 
  }

#endif // NO_GUI
};

class Vector4 : public Vector<4>
{
public:
  inline Vector4(){}

  template <class St>
  inline Vector4(const Vector<4, St>& v)
    {(*this)[0]=v[0]; (*this)[1]=v[1]; (*this)[2]=v[2]; (*this)[3]=v[3];}
  inline Vector4(T a, T b, T c, T d)
    {
      (*this)[0]=a; (*this)[1]=b; (*this)[2]=c; (*this)[3]=d; 
    }

#ifndef NO_GUI // CM
  void glVertex() const
    {
      glVertex4d((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
    }

  void glGet(GLenum e) {
    double tmp[4]; glGetDoublev(e, tmp);
    (*this)[0]=tmp[0]; (*this)[1]=tmp[1]; (*this)[2]=tmp[2]; (*this)[3]=tmp[3];
  }

#endif // NO_GUI
};

// implementation
template<unsigned int N, class St>
inline std::ostream& operator<<(std::ostream& os, const Vector<N, St>& v)
{
  os << v[0];
  for (unsigned int i=1; i<N; ++i)
  {
    os << " " << v[i];
  }
  return os;
}

// implementation
template<unsigned int N, class St>
inline std::istream& operator>>(std::istream& is, Vector<N, St>& v)
{
  is >> v[0];
  for (unsigned int i=1; i<N; ++i)
  {
    is >> v[i];
  }
  return is;
}

template<unsigned int N, class St1, class St2>
inline Vector<N, RegularStorage<typename St2::value_type, N> > operator^(const Vector<N, St1>& v1, const Vector<N, St2>& v2)
{
  Vector<N, RegularStorage<typename St2::value_type, N> > res;
  res.setToCross(v1,v2);
  return res;
}

/// scalar multiplication
template<unsigned int N, class St1>
inline Vector<N, RegularStorage<typename St1::value_type, N> > operator *(typename St1::value_type s, const Vector<N, St1>& v)
{
  Vector<N, RegularStorage<typename St1::value_type, N> > res;
  for (unsigned int i=0; i<N; ++i)
    res[i]=s*v[i];
  return res;
}

/// scalar multiplication
template<unsigned int N, class St1>
inline Vector<N, RegularStorage<typename St1::value_type, N> > operator *(const Vector<N, St1>& v, typename St1::value_type s)
{
  return s*v;
}

#endif // _Vector_h_
