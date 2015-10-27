#include "IndexedMesh/colorgradient.h"

namespace Mesh3D{

ColorGradient::ColorGradient()
{
    colors_ = std::vector<Color>(0);
    values_ = std::vector<float>(0);
}

void ColorGradient::addColor(Color color, float value){


    if (colors_.size() == 0){
        colors_.push_back(color);
        values_.push_back(value);
        return;
    }

    for (int i = 0; i < colors_.size(); i++){
        if (value < values_[i]){
            colors_.insert(colors_.begin()+i,color);
            values_.insert(values_.begin()+i,value);
            return;
        }
    }
    colors_.push_back(color);
    values_.push_back(value);
}

Color ColorGradient::getColorAtValue(float value) const{


    if (colors_.size() != values_.size()) return Color(0,0,0);
    if (colors_.size() == 0) return Color(0,0,0);

    for (int i = 0 ; i < colors_.size(); i++){
        if (value < values_[i]){
            Color previousColor = colors_[std::max(0,i-1)];
            Color indexColor = colors_[i];
            float deltaValue = values_[i] - values_[std::max(0,i-1)];
            float fracValue = (deltaValue ==0) ? 0 : (value - values_[std::max(0,i-1)] ) / deltaValue;
            float r = (indexColor.r() - previousColor.r())*fracValue + previousColor.r();
            float g = (indexColor.g() - previousColor.g())*fracValue + previousColor.g();
            float b = (indexColor.b() - previousColor.b())*fracValue + previousColor.b();

            return Color(r,g,b);
        }
    }

    return colors_.back();

}

}
