/*
@File FvxDecoder.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_DECODER_H
#define FVX_DECODER_H

#include <string>
#include "4dv/FvxMesh.h"
#include "4dv/FvxFrameInfo.h"
#include "4dv/DllExport.h"

/** FvxDecoder class.
 * An FvxDecoder is in charge of decoding a 4DM chunk into a FvxMesh.
 **/
class DllImportExport FvxDecoder {
public:

  FvxDecoder() {}
  virtual ~FvxDecoder() {}

  /** Precise the profile to be used by the decoder.
   **/
  virtual bool setProfile(const std::string &profile) = 0;
  
  /** Retrieve the frame info from a 4DM chunk.
   **/
  virtual FvxFrameInfo* getFrameInfo(const void *data, int bufferSize) = 0;
  /** Method for decoding a 4DM chunk into a FvxMesh.
   * @param data pointer to the binary 4DM data.
   * @param bufferSize size in bytes of the binary 4DM data.
   * @return pointer to a FvxMesh containing the data. The decoding was succesfull if the validity flag is ok on the given mesh.
   **/
  virtual FvxMesh* decode(const void *data, int bufferSize) = 0;

  // Static method for creating a FvxDecoder.
  static FvxDecoder* newFvxDecoder(const std::string &profile = std::string(""));
  // Static method for deleting a FvxDecoder.
  static void deleteFvxDecoder(FvxDecoder *decoder);
};

#endif // FVX_DECODER_H
