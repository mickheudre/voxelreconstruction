/*
@File FvxMesh.h
@Author Clement Menier
@Copyright (c) 4D View Solutions 2007, 2008, 2009, 2010. All Rights Reserved.
*/


#ifndef FVX_MESH_H
#define FVX_MESH_H

#include "4dv/FvxVector.h"
#include "4dv/FvxMatrix.h"
#include "4dv/FvxImage.h"
#include "4dv/FvxTimeStamp.h"
#include "4dv/FvxCameraCalib.h"
#include "4dv/DllExport.h"

/** A mesh is composed:
 * A list of vertices
 *   Each vertex has a position VxVector3f
 *   Potentially each vertex has a color VxVector3b (RGB)
 * Potentially a list of normals
 *   -> Normals can be associated either with:
 *       + each vertices
 *       + each faces through indices
 * Potentially a list of texture coordinates VxVector2f (UV)
 *   -> Texture coordinates are always associated to faces through indices
 * Potentially faces (if no faces then the mesh consists in a cloud of points)
 *   -> 3 indices information can be stored per face
 *       + vertex indices
 *       + normal indices (if normals are available and normals not associated with vertices)
 *       + texture coordinate indices (if uv coordinates are available)
 * For color 3 modes are available:
 *   -> Color through vertices color if available
 *   -> Color through single texture + texcoords
 *   -> Color through multiple texture + camera calibration
 **/

/** Mode for mesh coloring.
 **/
typedef enum {
  /// No coloring
  FVX_MESH_NO_COLORING,
  /// Coloring by vertices colors
  FVX_MESH_COLORING_BY_VERTICES_COLORS,
  /// Coloring with a single texture and uv coordinates
  FVX_MESH_COLORING_BY_SINGLE_TEXTURE,
  /// Coloring by using the textures from the cameras
  FVX_MESH_COLORING_BY_MULTIPLE_TEXTURE,
} FvxMeshColoringMode;

/** Mesh class.
 **/
class DllImportExport FvxMesh {
public:

  FvxMesh() {}
  virtual ~FvxMesh() {}
  
  /// Returns true if mesh is valid.
  virtual bool isValid() const = 0;
  
  /***** Vertices Information *****/

  /// Returns the number of vertices.
  virtual int getVerticesCount() const = 0;
  /** Get Vertex Positions ( NULL if mesh is invalid ) **/
  virtual const FvxVector3f *getVerticesPositionsPtr() const = 0;
  virtual const FvxVector3f& getVertexPosition(unsigned int i) const = 0;

  virtual bool hasVerticesColors() const = 0;
  virtual const FvxVector3b *getVerticesColorsPtr() const = 0;
  virtual const FvxVector3b& getVertexColor(unsigned int i) const = 0;

  /***** Normals Information *****/

  virtual int getNormalsCount() const = 0;
  virtual bool areNormalsAssociatedToVertices() const = 0;
  /** Get Vertex Normals ( NULL if mesh is invalid or no
   *                       vertex normals have been computed ) **/
  virtual const FvxVector3f *getNormalsPtr() const = 0;
  virtual const FvxVector3f& getNormal(unsigned int i) const = 0;
  virtual void computeNormalsAssociatedToVertices() = 0;

  /***** Texture Coordinates Information *****/

  virtual int getTexCoordsCount() const = 0;
  virtual bool hasTexCoords() const = 0;
  virtual const FvxVector2f *getTexCoordsPtr() const = 0;
  virtual const FvxVector2f& getTexCoord(unsigned int i) const = 0;

  /***** Faces Information *****/

  virtual int getFacesCount() const = 0;
  virtual const FvxVector3i *getFaceVertexIndicesPtr() const = 0;
  virtual const FvxVector3i& getFaceVertexIndices(unsigned int i) const = 0;
  virtual bool hasFaceNormalIndices() const = 0;
  virtual const FvxVector3i *getFaceNormalIndicesPtr() const = 0;
  virtual const FvxVector3i& getFaceNormalIndices(unsigned int i) const = 0;
  virtual bool hasFaceTexCoordIndices() const = 0;
  virtual const FvxVector3i *getFaceTexCoordIndicesPtr() const = 0;
  virtual const FvxVector3i& getFaceTexCoordIndices(unsigned int i) const = 0;

  /***** Texture Information *****/
  virtual bool hasSingleTexture() const = 0;
  virtual const FvxImage* getSingleTexture() const = 0;

  virtual bool hasMultipleTexture() const = 0;
  virtual int getMultipleTextureCameraCount() const = 0;
  virtual const FvxImage* getTextureFromCamera(unsigned int i) const = 0;
  virtual const FvxCameraCalib& getCameraCalibration(unsigned int i) const = 0;
  
  virtual FvxMeshColoringMode getColoringMode() const = 0;

  virtual bool hasNormalMap() const = 0;
  virtual const FvxImage* getNormalMap() const = 0;

  virtual const FvxTimeStamp& getTimeStamp() const = 0;
};

#endif // FVX_MESH_H
