/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef OFF_LOADER_H
#define OFF_LOADER_H

#include "IndexedMesh/meshloaderinterface.h"

/*!
    \file off_loader.h
    \author Mickaël HEUDRE
    \brief the .off module for the mesh loader
*/
namespace Mesh3D{

/*! \enum OFFFormat
    \brief The different types of .off files
    \relates OFF_loader
*/
enum OFFFormat{
    STANDARDOFF, /*!< Standart OFF file, contains a list of points and triangles */
    COFF, /*!< Colored OFF file, contains a list of points associated to a color and triangles */
    NOFF, /*!< Normal OFF file, contains a list of points associated to a normal and triangles */
    UNKNOWNOFF
};
/*! \class OFF_Loader
    \ingroup Mesh3D
    \brief the .off module for the mesh loader
  */
class OFF_Loader : public MeshLoaderInterface
{
public:
    OFF_Loader();
    bool load(const std::string &filename, Mesh *target_mesh);
    bool save(const std::string &filename, Mesh *target_mesh);

};
}
#endif // OFF_LOADER_H
