/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef FDM_LOADER_H
#define FDM_LOADER_H
#include "IndexedMesh/meshloaderinterface.h"
#include "4dv/FvxDecoder.h"
#include <fstream>
namespace Mesh3D{
/*!
    \file 4dm_loader.h
    \author Mickaël HEUDRE
    \brief the .4dm module for the mesh loader
*/

/*! \class FDM_Loader
    \ingroup Mesh3D
    \brief the .4dm module for the mesh loader
  */
class FDM_Loader : public MeshLoaderInterface
{
public:
    FDM_Loader();
    bool load(const std::string &filename, Mesh *target_mesh);
    bool save(const std::string &filename, Mesh *target_mesh);
};
}
#endif // FDM_LOADER_H
