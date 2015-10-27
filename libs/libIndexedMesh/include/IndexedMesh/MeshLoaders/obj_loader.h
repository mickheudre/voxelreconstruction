/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "IndexedMesh/meshloaderinterface.h"

namespace Mesh3D{

/*!
    \file obj_loader.h
    \author Mickaël HEUDRE
    \brief the .obj module for the mesh loader
*/

/*! \class OBJ_Loader
    \ingroup Mesh3D
    \brief the .obj module for the mesh loader
  */
class OBJ_Loader : public MeshLoaderInterface
{
public:
    OBJ_Loader();
    bool load(const std::string &filename, Mesh *target_mesh);
    bool save(const std::string &filename, Mesh *target_mesh);
};
}
#endif // OBJ_LOADER_H
