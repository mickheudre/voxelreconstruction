// -*-C++-*-
/** @file linalg/Matrix.h
 * Classic linear algebra entity.
 * Author: Jean-Sebastien FRANCO
 * Date: 24/01/2001
 */

#ifndef _Matrix_h_
#define _Matrix_h_

#include <assert.h>
#include <iostream>
#include "Vector.h"

/*
template <class T, unsigned int N, unsigned M>
class Regular2DStorage {
public:
  typedef T value_type;
  typedef Vector<M, RegularStorage<T, M> >& row_type;
  typedef const Vector<M, RegularStorage<T, M> >& const_row_type;
  typedef VectorView<N,M,T>& col_type;
  typedef ConstVectorView<N,M,T>& const_col_type;

  inline row_type operator[](unsigned int i)
  {assert(i<N); return m_val[i];}
  inline const const_row_type operator[](unsigned int i) const
  {assert(i<N); return m_val[i];}
  inline col_type column(unsigned int j)
  {assert(j<M); return col_type(&m_val[0][j]);}
  inline const_col_type column(unsigned int j) const
  {assert(j<M); return const_col_type(&m_val[0][j]);}

  inline void set (const T* r) {
    for (int i=0; i<N; ++i)
      m_val[i]=r[i];
  }

  row_type m_val[N];
};


template <class T, int skipwidth, int skipheight>
class Skip2DStorage {
public:
  typedef T value_type;
  inline T& operator[](unsigned int i) {return m_ptr[i*skip];}
  inline const T& operator[](unsigned int i) const {return m_ptr[i*skip];}
  inline void set (T* r) {m_ptr=r;}

  typedef T value_type;
  typedef Vector<M, RegularStorage<T, M> >& row_type;
  typedef const Vector<M, RegularStorage<T, M> >& const_row_type;
  typedef VectorView<N,M,T>& col_type;
  typedef ConstVectorView<N,M,T>& const_col_type;

  inline row_type operator[](unsigned int i)
  {assert(i<N); return m_val[i];}
  inline const const_row_type operator[](unsigned int i) const
  {assert(i<N); return m_val[i];}
  inline col_type column(unsigned int j)
  {assert(j<M); return col_type(&m_val[0][j]);}
  inline const_col_type column(unsigned int j) const
  {assert(j<M); return const_col_type(&m_val[0][j]);}

  inline void set (const T* r) {
    for (int i=0; i<N; ++i)
      m_val[i]=r[i];
  }

  T *m_ptr;
};

template <class T, int skip>
class ConstSkipStorage {
public:
  typedef T value_type;
  inline const T& operator[](unsigned int i) const {return m_ptr[i*skip];}
  inline void set (const T* r) {m_ptr=r;}

  const T *m_ptr;
};
*/

/**
 * Matrix class, template parameters are N and M, row and column size of
 * matrix (N>0 & M>0) and M!=N
 */
template <unsigned int NN, unsigned int MM>
class Matrix //: public IObject
{
public:
  /// make N and M definitions visible to the outside
  enum {N=NN, M=MM};

  typedef Vector<M, RegularStorage<Real, M> >& row_type;
  typedef const Vector<M, RegularStorage<Real, M> >& const_row_type;
  //typedef VectorView<N,M,Real> col_type;
  //typedef ConstVectorView<N,M,Real> const_col_type;
  typedef Vector<N, RegularStorage<Real, N> > col_type;
  typedef const Vector<N, RegularStorage<Real, N> > const_col_type;

  // Constructors

  /// Empty constructor (leaves matrix in undefined state)
  inline Matrix() { assert((N>0)&&(M>0)); }

  /// Constructor from N*M Reals, line major
  inline explicit Matrix(const Real* init) { assert((N>0)&&(M>0)); set(init); }

  /// Constructor from scal*Identity
  inline explicit Matrix(const Real& value) { assert((N>0)&&(M>0)); set(value); }

  /// Constructor from N LineVector<M>
  inline explicit Matrix(const Vector<M>* rows) { assert((N>0)&&(M>0)); set(rows); }

  Real* data() const
  {
    return (Real*)m_row;
  }
  
  /// set from N*M Reals, line major
  inline void set(const Real* init)
    {
      for (int i=0; i<N; i++, init+=M )
	m_row[i].set(init);
    }

  /// set to value*Identity
  inline void set(const Real& value)
    {
      for (unsigned int i=0; i<N; i++)
	m_row[i].set(i,value);
    }

  /// set from N rows (Vector<M>)
  inline void set(const Vector<M>* rows)
    {
      for (int i=0; i<N; i++)
	m_row[i]=rows[i];
    }

  /// extract sub-matrix
  template <unsigned int R, unsigned int C>
  inline Matrix<R,C> extract(unsigned int row=0, unsigned int col=0) const
    {
      assert(row+R<=N); assert(col+C<=M);
      Matrix<R,C> res;
      for (unsigned int i=0; i<R; ++i)
	for (unsigned int j=0; j<C; ++j)
	  res[i][j]=m_row[i+row][j+col];
      return res;
    }
  
  // selectors

  /// const row selector
  inline const_row_type operator[](int i) const
    {
      assert((i>=0)&&(i<N));
      return m_row[i];
    }

  /// row mutator
  inline row_type operator[](int i)
    {
      assert((i>=0)&&(i<N));
      return m_row[i];
    }

  /// column selector (constructs vector in O(N))
  /*inline const_col_type column(int j) const
    {
      //return const_col_type(&m_row[0][j]);
      const_col_type col;
      for (int i=0; i<N; ++i)
	col[i]=m_row[i][j];
      return col;
      }*/

  /// column mutator (constructs vector in O(N))
  inline col_type column(int j) const
    {
      //return col_type(&m_row[0][j]);
      col_type col;
      for (int i=0; i<N; ++i)
	col[i]=m_row[i][j];
      return col;
    }

  /// column mutator (operates in O(N))
  inline void setColumn(int j, const Vector<N>& v)
    {
      assert((j>=0)&&(j<M));
      for (int i=0; i<N; ++i)
	m_row[i][j]=v[i];
    }
  
  inline const Matrix<M,N> getTranspose() const
    {
      Matrix<M,N> res;
      for (int i=0; i<N; ++i)
	for (int j=0; j<M; ++j)
	  res.m_row[j][i]=m_row[i][j];
      
      return res;
    }

  inline Real norm() const {
    assert(N>=1);
    Real res=m_row[0]*m_row[0];
    for (int i=1; i<N; ++i)
      res+=m_row[i]*m_row[i];
    return sqrt(res);
  }

  // accumulators

  /// addition accumulator
  inline void operator+=(const Matrix<N,M>& m)
    {
      for (int i=0; i<N; ++i)
	m_row[i]+=m.m_row[i];
    }

  /// subtraction accumulator
  inline void operator-=(const Matrix<N,M>& m)
    {
      for (int i=0; i<N; ++i)
	m_row[i]-=m.m_row[i];
    }

  /// scalar multiplication accumulator
  inline void operator*=(const Real& scal)
    {
      for (int i=0; i<N; ++i)
	m_row[i]*=scal;
    }

  /// vector multiplication
  inline Vector<N> operator*(const Vector<M>& v) const
    {
      Vector<N> res;
      for (int i=0; i<N; ++i)
	res[i]=m_row[i]*v;
      return res;
    }

  // testers
  inline bool operator==(const Matrix<N,M>& m)
    {
      int i;
      for (i=0; i<N; ++i)
	if (m_row[i]!=m.m_row[i])
	  return false;
      
      return true;
    }
  
protected:

  // the dum compiler won't accept "Vector<M> m_row[N];"
  // "arrays of objects containing zero-size arrays are illegal"
  // but it accepts m[N][M], should pose the same array size problem !!!
  // fool the compiler: see enum added in Vector definition...

  /// matrix is implemented as line-major
  Vector<M> m_row[N];

};

template <unsigned int NN>
class SquareMatrix : public Matrix<NN,NN>
{
public:

  /// make N and M definitions visible to the outside
  enum {N=NN, M=NN};
  // Constructors

  /// Empty constructor (leaves matrix in undefined state)
  inline SquareMatrix() { assert(N>0); }
  
  /// copy
  inline SquareMatrix(const Matrix<N,N>& m) { assert(N>0); set(m.data()); }

  /// Constructor from N*M Reals, line major
  inline explicit SquareMatrix(const Real* init) { assert(N>0); this->set(init); }

  /// Constructor from scal*Identity
  inline explicit SquareMatrix(const Real& value) { assert(N>0); this->set(value); }

  /// Constructor from N LineVector<M>
  inline explicit SquareMatrix(const Vector<N>* rows) { assert(N>0); this->set(rows); }

  /// in-place transpose
  inline void transpose()
    {
      Real tmp;
      for (int i=0; i<N; ++i)
	for (int j=0; j<N-1; ++j)
	{
	  tmp=this->m_row[i][j]; this->m_row[i][j]=this->m_row[j][i]; this->m_row[j][i]=tmp;
	}
    }

  /// determinant
  Real det()
    {
      // not yet implemented
      assert(false);
      return 0;
    }

  /// get inverse. sets inv to inverse and returns true iff inverse exists
  bool getInverse(Matrix<N,N>* inv) const
    {
      SquareMatrix<N> tmp=(*this); bool res=tmp.inverse();
      (*inv)=tmp;
      return res;
    }

  /// in-place inverse. true if exists
  bool inverse()
    {
      if (N == 1) return false;  // must be of dimension >= 2
      for (int i=1; i < N; i++) this->m_row[0][i] /= this->m_row[0][0]; // normalize row 0
      for (int i=1; i < N; i++)  {
	for (int j=i; j < N; j++)  { // do a column of L
	  Real sum = 0.0;
	  for (int k = 0; k < i; k++)
            sum += this->m_row[j][k] * this->m_row[k][i];
	  this->m_row[j][i] -= sum;
        }
	if (i == N-1) continue;
	for (int j=i+1; j < N; j++)  {  // do a row of U
	  Real sum = 0.0;
	  for (int k = 0; k < i; k++)
            sum += this->m_row[i][k]*this->m_row[k][j];
	  this->m_row[i][j] =
	    (this->m_row[i][j]-sum) / this->m_row[i][i];
        }
      }
      for ( int i = 0; i < N; i++ )  // invert L
	for ( int j = i; j < N; j++ )  {
	  Real x = 1.0;
	  if ( i != j ) {
	    x = 0.0;
	    for ( int k = i; k < j; k++ )
	      x -= this->m_row[j][k]*this->m_row[k][i];
	  }
	  this->m_row[j][i] = x / this->m_row[j][j];
	}
      for ( int i = 0; i < N; i++ )   // invert U
	for ( int j = i; j < N; j++ )  {
	  if ( i == j ) continue;
	  Real sum = 0.0;
	  for ( int k = i; k < j; k++ )
	    sum += this->m_row[k][j]*( (i==k) ? 1.0 : this->m_row[i][k] );
	  this->m_row[i][j] = -sum;
        }
      for ( int i = 0; i < N; i++ )   // final inversion
	for ( int j = 0; j < N; j++ )  {
	  Real sum = 0.0;
	  for ( int k = ((i>j)?i:j); k < N; k++ )
            sum += ((j==k)?1.0:this->m_row[j][k])*this->m_row[k][i];
	  this->m_row[j][i] = sum;
        }

      return true;
      
    }
};

/**
 * Matrix3 class, specialized Matrix 3x3
 */
class Matrix3 : public SquareMatrix<3>
{
public:
  /// make N and M definitions visible to the outside
  enum {N=3, M=3};

  inline Matrix3(const Matrix<3,3>& m) { set(m.data());}

   /// Empty constructor (leaves matrix in undefined state)
  inline Matrix3() {}

  /// Constructor from N*M Reals, line major
  inline explicit Matrix3(const Real* init) {set(init); }

  /// Constructor from scal*Identity
  inline explicit Matrix3(const Real& value) { set(value); }

  /// Constructor from N LineVector<M>
  inline explicit Matrix3(const Vector<3>* rows) { set(rows); }
  
  /// 
  Real det() const
    {
      Real det; Real *mat=(Real *)m_row;
  
      det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
	- mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
	+ mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );
  
      return det;
    }
	
  /// invert matrix
  bool getInverse(Matrix<3,3>* resmat) const;
  /// in-place inverse. true if exists
  bool inverse()
    {
      Matrix3 inv;
      if (getInverse(&inv))
      {
	(*this)=inv;
	return true;
      }
      else
      {
	return false;
      }
    }
};

/**
 * Matrix4 class, specialized Matrix 4x4, homogeneous transforms
 */
class Matrix4 : public SquareMatrix<4>
{
public:
  /// make N and M definitions visible to the outside
  enum {N=4, M=4};

  inline Matrix4(const Matrix<4,4>& m) {set(m.data());}

   /// Empty constructor (leaves matrix in undefined state)
  inline Matrix4() {}

  /// Constructor from N*M Reals, line major
  inline explicit Matrix4(const Real* init) {set(init); }

  /// Constructor from scal*Identity
  inline explicit Matrix4(const Real& value) { set(value); }

  /// Constructor from N LineVector<M>
  inline explicit Matrix4(const Vector<4>* rows) { set(rows); }

  
};

/**
 * General transform 3D class, specialized Matrix 4x4 (R | t)
 */
class Transform3D : public Matrix<3,4>
{
public:
  /// make N and M definitions visible to the outside
  enum {N=3, M=4};

  inline Transform3D(const Matrix<3,4>& m) {set(m.data());}

   /// Empty constructor (leaves matrix in undefined state)
  inline Transform3D() {}

  /// Constructor from N*M Reals, line major
  inline Transform3D(const Real* init) {set(init); }

  /// Constructor from scal*Identity
  inline Transform3D(const Real& value) { set(value); }

  /// Constructor from N LineVector<M>
  inline Transform3D(const Vector<4>* rows) { set(rows); }

  /// in-place inverse, computed as (transpose(R) | -transpose(R)*t)
  bool inverse()
    {
      Transform3D inv;
      if (getInverse(&inv))
      {
	(*this)=inv;
	return true;
      }
      else
      {
	return false;
      }
    }
  
  /// invert matrix
  bool getInverse(Transform3D* res) const
    {
      Transform3D& out = *res;
      for (int i=0; i<3; ++i)
      {
	for (int j=0; j<3; ++j)
	{
	  out[i][j]=(*this)[j][i];
	}
      }

      for (int row=0; row<3; ++row)
      {
	out[row][3] = -(out[row][0]*(*this)[0][3]+out[row][1]*(*this)[1][3]+
			out[row][2]*(*this)[2][3]);
      }
      
      return true;
    }

};

typedef Matrix<3,4> Matrix3x4;

/// matrix multiplication template
/// m1 and res have to be different objects, or else result is undefined!!
/// @param res result of multiplication of m1 and m2
/*
  template <class M1, class M2, class M3>
void mul(const M1& m1, const M2& m2, M3& res)
{
  // dimension check
  assert((M1::N==M3::N)&&(M2::M==M3::M)&&(M1::M==M2::N));

  // precaution: test if objects are different
  assert((void *)&m1!=(void *)&res);

  for (int j=0; j<M3::M; ++j)
  {
    Vector<M1::M> m2jcol(m2.column(j));
    for (int i=0; i<M1::N; ++i)
      res[i][j]=m1[i]*m2jcol;
  }
}
*/

/// matrix multiplication template
template <unsigned int N, unsigned int M, unsigned int L>
inline Matrix<N,M> operator *(const Matrix<N,L>& m1, const Matrix<L,M>& m2)
{

  Matrix <N,M> res;
  
  for (unsigned int j=0; j<M; ++j)
  {
    //Matrix<L,M>::const_col_type m2jcol=m2.column(j);
    typename Matrix<L,M>::col_type m2jcol=m2.column(j);
    for (unsigned int i=0; i<N; ++i)
      res[i][j]=m1[i]*m2jcol;
  }
  return res;
}

// stream output template (Mat is a matrix type)
template <unsigned int N, unsigned int M>
inline std::ostream& operator<<(std::ostream& os, const Matrix<N,M>& m)
{
  for (unsigned int i=0; i<N; ++i)
    os << m[i] << "\n";
  return os;
}

// stream output template (Mat is a matrix type)
template <unsigned int N, unsigned int M>
inline std::istream& operator>>(std::istream& is, Matrix<N,M>& m)
{
  for (unsigned int i=0; i<N; ++i)
    is >> m[i];

  return is;
}

template <unsigned int N>
SquareMatrix<N> Cholesky(const Matrix<N,N>& a)
{
  SquareMatrix<N> l;
  for (int j=0; j<N; ++j) {
    {
      Real sum=a[j][j];
      for (int k=0; k<j; ++k)
	sum -= l[j][k]*l[j][k];

      l[j][j]= sqrt(sum);
    }

    for (int i=j+1; i<N; ++i) {
      Real sum=a[i][j];
      for (int k=0; k<j; ++k)
	sum -= l[i][k]*l[j][k];

      l[i][j]=sum/l[j][j];
    }
  }
  return l;
}

template <unsigned int N>
inline SquareMatrix<N> outerProduct(const Vector<N>& v1, const Vector<N>& v2)
{
  SquareMatrix<N> res;
  for (int i=0; i<N; ++i) {
    for (int j=0; j<N; ++j) {
      res[i][j]=v1[i]*v2[j];
    }
  }
  return res;
}

/// scalar multiplication
template<unsigned int N, unsigned int M>
inline Matrix<N, M> operator *(Real s, const Matrix<N, M>& m)
{
  Matrix<N,M> res=m; res*=s;
  return res;
}

/// minus operator
template<unsigned int N, unsigned int M>
inline Matrix<N, M> operator -(const Matrix<N, M>& m1, const Matrix<N, M>& m2)
{
  Matrix<N,M> res=m1; res-=m2;
  return res;
}

/*template <unsigned int N>
SquareMatrix<N> Cholesky(const Matrix<N,N>& S)
{
  SquareMatrix<N> T(0.);
  Real* s = S.data(); Real* t = T.data(); Real* ti = t;
  for (int i=0; i<N ; i++)
   {
      ti=t+N*i;
      Real* tj = t; Real sum; int k;
      for (int j=0; j<i; j++)
      {
         Real* tk = ti; sum = 0.0; k = j;
         while (k--) { sum += *tj++ * *tk++; }
         *tk = (*s++ - sum) / *tj++;
      }
      sum = 0.0; k = i;
      while (k--) { sum += (*ti)*(*ti); ++ti; }
      Real d = *s++ - sum;
      //assert(d>0.0);
      *ti++ = sqrt(d);
   }
  return T;
}*/

inline bool Matrix3::getInverse(Matrix<3,3>* resmat) const
{
  Real *mat=(Real*)m_row;
  Real determinant = det();

  Matrix<3,3>& res=*resmat;
  if(determinant < 0.0005 && determinant> -0.0005)
    {
      // not invertible
      std::cerr << "\nWARNING: Singular matrix (determinant: " << determinant << ")\n" << *this << "\n";
      //return false;
      assert(false);
    }
      
  res[0][0] =  (mat[4]*mat[8] - mat[5]*mat[7]) / determinant;
  res[0][1] = -(mat[1]*mat[8] - mat[7]*mat[2]) / determinant;
  res[0][2] =  (mat[1]*mat[5] - mat[4]*mat[2]) / determinant;
 
  res[1][0] = -(mat[3]*mat[8] - mat[5]*mat[6]) / determinant;
  res[1][1] =  (mat[0]*mat[8] - mat[6]*mat[2]) / determinant;
  res[1][2] = -(mat[0]*mat[5] - mat[3]*mat[2]) / determinant;
      
  res[2][0] =  (mat[3]*mat[7] - mat[6]*mat[4]) / determinant;
  res[2][1] = -(mat[0]*mat[7] - mat[6]*mat[1]) / determinant;
  res[2][2] =  (mat[0]*mat[4] - mat[1]*mat[3]) / determinant;
  // not yet implemented
      
  return true;
}

#endif // _Matrix_h_





