/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/

#ifndef COLOR_H
#define COLOR_H

#include <iostream>

/*! \file color.h
    \author Mickaël HEUDRE
    \brief a basic color class.
*/

/*! \namespace Mesh3D
    \brief the basic meshes management API.
*/
namespace Mesh3D{

/*!
   \class Color
   \ingroup Mesh3D
   \brief a basic Color class.
*/
class Color
{
public:
    //! Construct a Color object with values set to 0 (black).
    Color();
    //! Construct a Color object from RGBA values.
    Color(float r, float g, float b, float a);
    //! Construct a Color object from RGB values.
    Color(float r, float g, float b);

    //! Set the Red value
    //! \param r the new red value.
    void set_red(float r){
        r_ = r;
    }

    //! Set the Green value
    //! \param g the new green value.
    void set_green(float g){
        g_ = g;
    }

    //! Set the Blue value
    //! \param b the new blue value.
    void set_blue(float b){
        b_ = b;
    }

    //! Set the Alpha value
    //! \param a the new alpha value.
    void set_alpha(float a){
        a_ = a;
    }

    //! Get the Red value
    //! \return the red value.
    inline float r() const{
        return r_;
    }

    //! Get the Green value
    //! \return the green value.
    inline float g() const{
        return g_;
    }

    //! Get the Blue value
    //! \return the blue value.
    inline float b() const{
        return b_;
    }

    //! The output stream operator to easily write a Color.
    inline friend std::ostream &operator<<(std::ostream &out, Color& color)
    {
        out << color.r_ << " " << color.g_ << " " << color.b_ << " " << color.a_;
        return out;
    }

    //! The == operator to compare two colors.
    /*! \param a the first color
        \param b the second color
        \return true if a and b have the same RGBA values, false if not.
     */
    inline  friend bool operator == (Color& a, const Color& b) {
        if ((a.r_ == b.r_) && (a.g_ == b.g_) && (a.b_ == b.b_) && (a.a_ == b.a_)) return true;
        return false;
    }

private:
    //! The Red value.
    float r_;
    //! The Green value.
    float g_;
    //! The Blue value.
    float b_;
    //! The Alpha value.
    float a_;
};

//! Convert a HSV color into a RGB color.
/*! \relates Color
    \param the input Hue value.
    \param the input Saturation value.
    \param the input Value value.
    \return a Color object.
*/
Color HSVtoRGB(float h,float s,float v);

}

#endif // COLOR_H
