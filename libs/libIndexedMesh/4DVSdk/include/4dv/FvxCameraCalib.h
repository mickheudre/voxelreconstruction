/*
@File FvxCameraCalib.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/

#ifndef FVX_CAMERA_CALIB_H
#define FVX_CAMERA_CALIB_H

#include "4dv/FvxVector.h"
#include "4dv/FvxMatrix.h"

class FvxCameraCalib {
public:
  FvxCameraCalib() : _width(-1), _height(-1) {}

  void setCameraCalib(const FvxMatrix3x4f &proj, const FvxVector3f &center,
                      int width = -1, int height = -1) {
    _proj = proj;
    _center = center;
    _width = width;
    _height = height;
  }

  const FvxMatrix3x4f &getProjectionMatrix() const { return _proj; }
  const FvxVector3f &getCenter() const { return _center; }
  int getWidth() const { return _width; }
  int getHeigh() const { return _height; }
  
private:
  FvxMatrix3x4f _proj;
  FvxVector3f _center;
  int _width, _height;
};

#endif // FVX_CAMERA_CALIB_H
