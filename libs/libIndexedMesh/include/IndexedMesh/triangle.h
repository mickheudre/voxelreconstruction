#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>

namespace Mesh3D{
/*! \struct Triangle
    \ingroup Mesh3D
    \brief Basic triangle structure
*/
struct Triangle {
    int v0, ///< Indice of the first vertex of the triangle.
    v1, ///< Indice of the second vertex of the triangle.
    v2; ///< Indice of the third vertex of the triangle.

    //! Creates an empty triangle
    /*! \relates Triangle
    */
    Triangle(){
        v0 = -1;
        v1 = -1;
        v2 = -1;
    }
    //! Creates a triangle from 3 vertices
    /*! \relates Triangle
        \param v0 the first vertex of the triangle
        \param v1 the second vertex of the triangle
        \param v2 the third vertex of the triangle
    */
    Triangle(int v0,int v1, int v2):v0(v0),v1(v1),v2(v2){

    }
};


inline Triangle make_triangle(int v0,int v1, int v2){
    Triangle triangle;
    triangle.v0 = v0;
    triangle.v1 = v1;
    triangle.v2 = v2;

    return triangle;
}

//! << operator.
/*! \relates Triangle
*/
inline std::ostream &operator<<(std::ostream &out,const Triangle& a){
    out << a.v0 << " " << a.v1 << " " << a.v2;
    return out;
}

}

#endif // TRIANGLE_H
