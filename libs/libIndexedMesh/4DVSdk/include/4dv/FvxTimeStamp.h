/*
@File FvxTimeStamp.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_TIME_STAMP_H
#define FVX_TIME_STAMP_H

#include "4dv/DllExport.h"


class DllImportExport FvxTimeStamp {
public:
  FvxTimeStamp() : time_ms(0), time_s(0), frame_id(-1) {}
  FvxTimeStamp(unsigned int s, unsigned int ms, int id) : time_ms(ms), time_s(s), frame_id(id) {}

  // Milliseconds part of the time stamp.
  unsigned int time_ms;
  // Seconds part of the time stamp (since EPOCH).
  unsigned int time_s;
  // Frame id.
  int frame_id;
};

#endif // FVX_TIME_STAMP_H
