/*
@File FvxFrameInfo.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/

#ifndef FVX_FRAME_INFO_H
#define FVX_FRAME_INFO_H

#include "4dv/FvxTimeStamp.h"
#include "4dv/DllExport.h"

/** Class describing the frame information.
 **/
class DllImportExport FvxFrameInfo {
public:
  FvxFrameInfo() {}
  virtual ~FvxFrameInfo() {}

  // Not useful for now
  virtual bool isFrameImportant() = 0;
  // Returns the time stamp 
  virtual FvxTimeStamp getTimeStamp() = 0;
  /** Returns the number of vertices (note that this is only
      an indication as it may depend on your profile).
  **/
  virtual int getVerticesCount() = 0;
  /** Returns the number of faces (note that this is only
      an indication as it may depend on your profile).
  **/
  virtual int getFacesCount() = 0;
};

#endif // FVX_FRAME_INFO_H
