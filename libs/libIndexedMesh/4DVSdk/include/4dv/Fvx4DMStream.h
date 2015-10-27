/*
@File Fvx4DMStream.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_4DM_STREAM_H
#define FVX_4DM_STREAM_H

#include <string>
#include "4dv/DllExport.h"

/** Fvx4DMStream class.
 * A Fvx4DMStream is in charge of connecting and retrieving 4DM chunks from a
 * server.
 **/
class DllImportExport Fvx4DMStream {
public:

  /** Chunk class.
   * A chunk stores a 4DM chunk data. This class is an interface.
   * You can implement your own Chunk class in order to use your own memory allocator.
   **/
  class Chunk {
  public:
    Chunk() {}
    virtual ~Chunk() {}

    /** Return the size of the chunk data.
     * A value of 0 means that no data is available.
     **/
    virtual int size() const = 0;
    /** Return a pointer to the chunk data.
     **/
    virtual char *data() = 0;

    /** Realloc data to accomodate for size s.
     **/
    virtual void dataRealloc(int s) = 0;
    /** Free data
     **/
    virtual void dataFree() = 0;
  };
  
public:
  Fvx4DMStream() {}
  virtual ~Fvx4DMStream() {}

  /** Connects to a server.
   * @param server Server name or IP.
   * @param port Server port number.
   * @return true if succesfull
   **/
  virtual bool connectToServer(const std::string &server, int port) = 0;
  /** Disconnects from server.
   **/
  virtual void disconnect() = 0;

  /** Retrieve a new chunk.
   * @param chunk Use this chunk for allocating data. If NULL, a new chunk
                  will be allocated.
   * @param waiting If true, this function blocks until new data is available. 
                    Otherwise function will return empty data if no new data
                    is available. 
   * @return the chunk containing the data.
   **/
  virtual Chunk *getNewChunk(Chunk *chunk = NULL, bool waiting = false) = 0;

  // Static method for creating a Fvx4DMStream.
  static void deleteFvx4DMStream(Fvx4DMStream *s);
  // Static method for deleting a Fvx4DMStream.
  static Fvx4DMStream* newFvx4DMStream();
};

#endif // FVX_4DM_STREAM_H
