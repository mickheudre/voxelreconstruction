/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/

#ifndef MESHLOADERINTERFACE_H
#define MESHLOADERINTERFACE_H

#include <string>
#include <iostream>
#include <fstream>
#include "Logger.h"

#include "IndexedMesh/mesh3d.h"

/*! \file meshloaderinterface.h
    \author Mickaël Heudre
    \brief Interface for the mesh loaders
*/

namespace Mesh3D{



/*! \class MeshLoaderInterface
    \ingroup Mesh3D
    \brief An interface for the mesh loaders

    This class is an interface for the mesh loaders. It is purely virtual.
    To add a new file format, create a class that inherits this class and
    implememnts the following methods.
    The register this new class in the meshloadermanager class.

*/
class MeshLoaderInterface
{
public:

    //! A method that loads a file containing a mesh and stores it in a target mesh.
    /*!
        \param filename the path to the mesh file.
        \param target_mesh the mesh that will store the mesh information
        \return true if the mesh is loaded. false if not.
    */
    virtual bool load(const std::string &filename, Mesh *target_mesh) = 0;

    //! A method that saves a mesh into a file.
    /*!
        \param filename the path to the target file.
        \param target_mesh containing the mesh we want to store.
        \return true if the mesh is saved. false if not.
    */
    virtual bool save(const std::string &filename, Mesh *target_mesh) = 0;

};
}
#endif // MESHLOADERINTERFACE_H
