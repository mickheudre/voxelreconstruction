/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/

#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H

#include <IndexedMesh/color.h>
#include <vector>

/*! \file colorgradient.h
    \author Mickaël HEUDRE
    \brief a class to generate Color gradients.
*/


namespace Mesh3D{

/*! \class ColorGradient
    \ingroup Mesh3D
    \brief a class to generate a color gradient
    The user set a list of colors associated to a float value. \n
    The class generates the corresponding color for any value.
*/
class ColorGradient
{
public:
    //! Construct an empty Color Gradient.
    ColorGradient();
    //! add a Color at a specified value.
    /*! \param color the new color
        \param value the value associated to the color.
    */
    void addColor(Color color, float value);
    //! Get the color at the specified value.
    /*! \param value the value of the color we want to get.
        \return a Color object.
      */
    Color getColorAtValue(float value) const;
private:
    //! A vector of colors.
    std::vector<Color>  colors_;
    //! A vector of values.
    std::vector<float>  values_;
};
}
#endif // COLORGRADIENT_H
