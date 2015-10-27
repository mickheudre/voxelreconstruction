/*
@File FvxImage.h
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_IMAGE_H
#define FVX_IMAGE_H

#include "4dv/DllExport.h"
#include "4dv/FvxTimeStamp.h"

typedef enum {
  FVX_IMAGE_FORMAT_INVALID,
  FVX_IMAGE_FORMAT_RGBA,
  FVX_IMAGE_FORMAT_BGRA,
  FVX_IMAGE_FORMAT_RGB,
  FVX_IMAGE_FORMAT_DAC16,
  FVX_IMAGE_FORMAT_DXT1,
  FVX_IMAGE_FORMAT_MONO8,
  FVX_IMAGE_FORMAT_RAW8
} FvxImageFormat;

class DllImportExport FvxImage {
public:
  FvxImage() {}
  virtual ~FvxImage() {}

  // Returns the width of the image
  virtual unsigned int getWidth() const = 0;
  // Returns the height of the image
  virtual unsigned int getHeight() const = 0;

  // Returns the image format of the image
  virtual FvxImageFormat getFormat() const = 0;

  // Returns the frameid of the image
  virtual int getFrameId() const = 0;

  // Returns the timestamp info of the image
  virtual FvxTimeStamp getTimeStamp() const = 0;

  // Returns a pointer to the image data. Pixels are stored according to the FvxImageFormat
  virtual const unsigned char *getData() const = 0;

  // Returns true if the image is valid, false otherwise.
  virtual bool isValid() const = 0;
};

#endif // FVX_IMAGE_H
