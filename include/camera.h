#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include <boost/array.hpp>


class Camera {
public:


   Camera(const Mat3x4 &pmat);
   Camera(const Raw3x4 &pmat, bool transpose_raw = false);
   
   Raw4x4 projection_matrix(const unsigned int width, 
							const unsigned int height, 
							const double       znear, 
							const double       zfar) const ;

   Raw4x4 model_matrix() const ;
   Mat3 getK(){
       return mK;
   }
   void setK(Mat3 prj){
       mK = prj;
   }

protected:
   Mat3 mK;
   Mat3 mRw2c;
   Vec3 mPos;
};

#endif // CAMERA_H
