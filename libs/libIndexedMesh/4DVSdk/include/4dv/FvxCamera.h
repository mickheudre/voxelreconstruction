/*
@File FvxCamera.h
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/

#ifndef FVX_CAMERA_H
#define FVX_CAMERA_H

#include "4dv/FvxImage.h"

class FvxCamera {
public:
  FvxCamera() {}
  virtual ~FvxCamera() {}

  virtual const FvxImage *getImage(bool decodeRaw = true) = 0;

  virtual bool isConnected() const = 0;

  virtual void disconnect() = 0;
};

#endif // FVX_CAMERA_H
