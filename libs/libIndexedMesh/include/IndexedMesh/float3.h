/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef MESH3D_FLOAT3_H_DEFINED
#define MESH3D_FLOAT3_H_DEFINED

#include <cmath> // for sqrtf
#include <iostream>
/*! \file float3.h
    \brief Basic 3D Point structure and basic algebra operations.
*/

namespace Mesh3D
{
/*! \struct float3
    \brief Basic 3D Point structure.
    \ingroup Mesh3D
    Basic 3D point with a float precision.
*/
struct float3     { float x,y,z; };

//! float3 constructor.
/*! Turns 3 floats x,y,z into a float3.
  \relates float3
*/
inline float3 make_float3( const float& x, const float& y, const float& z ) {
    float3 res;
    res.x = x; res.y = y; res.z = z;
    return res;
}

//! << operator.
//! \relates float3
inline std::ostream &operator<<(std::ostream &out,const float3& a){
    out << a.x << " " << a.y << " " << a.z;
    return out;
}


//! - operator.
//! \relates float3
inline float3 operator - ( const float3& a ) {
    return make_float3(-a.x, -a.y, -a.z);
}

//! + operator.
//! \relates float3
inline float3 operator + ( const float3& a, const float3& b ) {
    return make_float3( a.x+b.x, a.y+b.y, a.z+b.z );
}

//! -operator.
//! \relates float3
inline float3 operator - ( const float3& a, const float3& b ) {
    return make_float3( a.x-b.x, a.y-b.y, a.z-b.z) ;
}

//! += operator.
//! \relates float3
inline float3& operator += (float3& a, const float3& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

//! -= operator.
//! \relates float3
inline float3& operator -= (float3& a, const float3& b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}



// ####################
// MATRIX ALGEBRA
inline float3 prod3( const float* A, const float3& b )
{
    return make_float3( A[0*3+0] * b.x +  A[0*3+1] * b.y + A[0*3+2] * b.z,
                        A[1*3+0] * b.x +  A[1*3+1] * b.y + A[1*3+2] * b.z,
                        A[2*3+0] * b.x +  A[2*3+1] * b.y + A[2*3+2] * b.z);
}


// ####################
// VECTOR SPACE
inline float3 operator * ( const float3& a, const float& s ) {
    return make_float3( a.x*s, a.y*s, a.z*s );
}

inline float3 operator * ( const float& s, const float3& a ) {
    return make_float3( a.x*s, a.y*s, a.z*s );
}

inline float3 operator / ( const float3& a, const float& s ) {
    return make_float3( a.x/s, a.y/s, a.z/s );
}

inline float3& operator *= ( float3&a, const float s) {
    a.x *= s;
    a.y *= s;
    a.z *= s;
    return a;
}

inline float3& operator /= ( float3&a, const float s) {
    a.x /= s;
    a.y /= s;
    a.z /= s;
    return a;
}

inline float dot( const float3& a, const float3& b ) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// ####################
// VECTOR ALGEBRA
inline float3 cross ( const float3& a, const float3& b ) {
    return make_float3( a.y*b.z - a.z*b.y,
                        a.z*b.x - a.x*b.z,
                        a.x*b.y - a.y*b.x );
}




// ####################
// Normalization
inline float norm2( const float3& a ) {
    return sqrtf( dot(a,a) );
}

inline float norm_squared( const float3& a ) {
    return dot(a,a);
}

inline void normalize( float3& a ) {
    float norm = norm2(a);
    a.x /= norm;
    a.y /= norm;
    a.z /= norm;
}

inline double distance(const float3 &a, const float3 &b){
    return sqrt( pow(a.x-b.x,2) + pow(a.y-b.y,2) + pow(a.z-b.z,2));
}

}








#endif
