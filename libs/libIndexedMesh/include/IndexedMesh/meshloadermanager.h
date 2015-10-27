/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef MESHLOADERMANAGER_H
#define MESHLOADERMANAGER_H

#include <string>

#include "mesh3d.h"
#include "Logger.h"
#include "IndexedMesh/meshloaderinterface.h"
#include "boost/filesystem.hpp"

/*! \file meshloadermanager.h
    \author Mickaël HEUDRE
    \brief Contains a class to easily load and save 3D meshes.
*/


namespace Mesh3D{

using namespace boost::filesystem;

/*! \enum FileFormat
    \relates MeshLoaderManager
    \brief the list of known file formats
*/
enum FileFormat{
    OFF, /*!< .off Object file format */
    OBJ, /*!<.obj Wavefront Technoloty file format*/
    FDM, /*!<  .4dv 4DView Solution file format*/
    UNKNOWN /*!< unknown file format */
};

/*! \class MeshLoaderManager
    \ingroup Mesh3D
    \brief This class is used to load and saves 3D meshes.

    It first scans the target file name to be sure it can open or save it.
*/
class MeshLoaderManager
{
public:
    MeshLoaderManager();
    ~MeshLoaderManager();
    //! Load a file containing a 3D mesh and stores the data in a target mesh.
    /*! \param filename the name of the file we want to load.
        \param target_mesh a pointer to a Mesh3D object to store the data.
        \return true if the mesh is succefully loaded, false if not.
    */
    bool load_file(const std::string &filename, Mesh *target_mesh);

    //! Save a 3D Mesh in a file.
    //! Save a mesh stored in a Mesh3D object into a file.
    /*! \param filename the name of the file where we want to save the mesh.
        \param target_mesh a pointer to a Mesh3D object containing the data.
        \return true if the mesh is succefully saved, false if not.
    */
    bool save_file(const std::string &filename, Mesh *target_mesh);
private:
    //! Check if the file to open is valid.
    bool is_readable(const std::string &filename);
    //! Check if the target file is valid.
    bool is_savable(const std::string &filename);
    //! Get the format of the target file.
    FileFormat get_file_format(const std::string &filename);

private:
    //! A pointer to the MeshLoaderInterface children associated to the file format.
    MeshLoaderInterface *mesh_loader_;
};
}
#endif // MESHLOADERMANAGER_H
