#include "IndexedMesh/color.h"
#include <cmath>

namespace Mesh3D{
Color::Color(){
    r_ = 0.0;
    g_ = 0.0;
    b_ = 0.0;
    a_ = 1.0;

}

Color::Color(float r, float g, float b, float a){
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = a;
}

Color::Color(float r, float g, float b){
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = 1.0;
}



Color HSVtoRGB(float h, float s, float v){
    float r, g, b;


    double c = v * s;
    double x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
    double m = v - c;

    if (h >= 0.0 && h < 60.0)
    {
        r = c + m;
        g = x + m;
        b =m;
    }
    else if (h >= 60.0 && h < 120.0)
    {
        r = x + m;
        g = c + m;
        b =m;
    }
    else if (h >= 120.0 && h < 180.0)
    {
        r = m;
        g =c + m;
        b = x + m;
    }
    else if (h >= 180.0 && h < 240.0)
    {
        r = m;
        g = x + m;
        b = c + m;
    }
    else if (h >= 240.0 && h < 300.0)
    {
        r = x + m;
        g =  m;
        b = c + m;
    }
    else if (h >= 300.0 && h < 360.0)
    {
        r = c + m;
        g = m;
        b = x + m;
    }
    else
    {
        r = m;
        g = m;
        b = m;
    }


    //std::cout << "Convertion: " << h << " " << s << " " << v << " " << r << " " << g << " " << b << " " << "\n";
    return Color(r,g,b);
}
}
