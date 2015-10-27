/*
@File FvxCameraSetup.h
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
 */

#ifndef FVX_CAMERA_SETUP_H
#define FVX_CAMERA_SETUP_H

#include "4dv/FvxCamera.h"
#include <string>
#include <vector>

class FvxCameraSetup {
public:
  FvxCameraSetup() {}
  virtual ~FvxCameraSetup() {}

  virtual int getCameraNumber() const = 0;

  virtual const std::string &getCameraHost(int camera) const = 0;

  virtual FvxCamera *connectToCamera(int camera) = 0;
  virtual void disconnect(FvxCamera *c) = 0;

  virtual std::vector<const FvxImage *> getImages(const std::vector<FvxCamera *> &cameras, bool decodeRaw = true) = 0;

  static FvxCameraSetup *loadCameraSetup();
  static void closeCameraSetup(FvxCameraSetup *);
};

#endif // FVX_CAMERA_SETUP_H
