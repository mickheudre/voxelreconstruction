/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef MESH3D_FLOAT2_H_DEFINED
#define MESH3D_FLOAT2_H_DEFINED

#include <cmath> // for sqrtf
#include <iostream>
/*! \file float2.h
    \brief Basic 3D Point structure and basic algebra operations.
*/

namespace Mesh3D
{
/*! \struct float2
    \brief Basic 3D Point structure.
    \ingroup Mesh3D
    Basic 3D point with a float precision.
*/
struct float2     { float x,y; };

//! float2 constructor.
/*! Turns 2 floats x,y into a float2.
  \relates float2
*/
inline float2 make_float2( const float& x, const float& y) {
    float2 res;
    res.x = x; res.y = y;
    return res;
}

//! << operator.
//! \relates float2
inline std::ostream &operator<<(std::ostream &out,const float2& a){
    out << a.x << " " << a.y;
    return out;
}


//! - operator.
//! \relates float2
inline float2 operator - ( const float2& a ) {
    return make_float2(-a.x, -a.y);
}

//! + operator.
//! \relates float2
inline float2 operator + ( const float2& a, const float2& b ) {
    return make_float2( a.x+b.x, a.y+b.y);
}

//! -operator.
//! \relates float2
inline float2 operator - ( const float2& a, const float2& b ) {
    return make_float2( a.x-b.x, a.y-b.y) ;
}

//! += operator.
//! \relates float2
inline float2& operator += (float2& a, const float2& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

//! -= operator.
//! \relates float2
inline float2& operator -= (float2& a, const float2& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}


// ####################
// VECTOR SPACE
inline float2 operator * ( const float2& a, const float& s ) {
    return make_float2( a.x*s, a.y*s );
}

inline float2 operator * ( const float& s, const float2& a ) {
    return make_float2( a.x*s, a.y*s);
}

inline float2 operator / ( const float2& a, const float& s ) {
    return make_float2( a.x/s, a.y/s);
}

inline float2& operator *= ( float2&a, const float s) {
    a.x *= s;
    a.y *= s;
    return a;
}

inline float2& operator /= ( float2&a, const float s) {
    a.x /= s;
    a.y /= s;
    return a;
}

inline float dot( const float2& a, const float2& b ) {
    return a.x*b.x + a.y*b.y;
}


// ####################
// Normalization
inline float norm2( const float2& a ) {
    return sqrtf( dot(a,a) );
}

inline float norm_squared( const float2& a ) {
    return dot(a,a);
}

inline void normalize( float2& a ) {
    float norm = norm2(a);
    a.x /= norm;
    a.y /= norm;
}


}








#endif
