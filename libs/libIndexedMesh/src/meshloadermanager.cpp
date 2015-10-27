#include "IndexedMesh/meshloadermanager.h"

#include "IndexedMesh/MeshLoaders/off_loader.h"
#include "IndexedMesh/MeshLoaders/obj_loader.h"
#include "IndexedMesh/MeshLoaders/4dm_loader.h"


namespace Mesh3D{

MeshLoaderManager::MeshLoaderManager(){
    mesh_loader_ = NULL;
}

MeshLoaderManager::~MeshLoaderManager(){
    delete mesh_loader_;
}

FileFormat MeshLoaderManager::get_file_format(const std::string &filename){

    if (filename.rfind(".off") != std::string::npos){
        return OFF;
    }

    else if (filename.rfind(".obj") != std::string::npos){
        return OBJ;
    }

    else if (filename.rfind(".4dm") != std::string::npos){
        return FDM;
    }

    return UNKNOWN;
}

bool MeshLoaderManager::is_readable(const std::string &filename){

    if (exists(filename)){
        if (get_file_format(filename) == UNKNOWN){
            log(WARN) << "[Mesh Loader] : " << filename << " unknown file format." << endLog();
            return false;
        }
        return true;
    }

    return false;

}

bool MeshLoaderManager::is_savable(const std::string &filename){
    //#pragma omp critical
    {
        //extract the target directory
        std::string target_dir = filename.substr(0,filename.find_last_of("/"));

        if (exists(target_dir)){
            if (is_directory(target_dir)){
                return true;
            }
        }
        else if (create_directories(target_dir)){
            log(ALWAYS) << "[Mesh Loader] : creating directory " << target_dir << endLog();
            return true;
        }
        return false;
    }
}

bool MeshLoaderManager::load_file(const std::string &filename, Mesh *target_mesh){

    if (is_readable(filename)){
        FileFormat file_format = get_file_format(filename);
        switch(file_format){
        case OFF:
            mesh_loader_ = new OFF_Loader();
            break;
        case OBJ:
            mesh_loader_ = new OBJ_Loader();
            break;
        case FDM:
            mesh_loader_ = new FDM_Loader();
            break;
        }
        return mesh_loader_->load(filename,target_mesh);
    }

    else{
        log(ERROR) << "[Mesh Loader] : unable to read " + filename << endLog();
        return false;
    }
}

bool MeshLoaderManager::save_file(const std::string &filename, Mesh *target_mesh){
    if(is_savable(filename)){
        FileFormat file_format = get_file_format(filename);
        if(file_format != UNKNOWN){
            switch(file_format){
            case OFF:
                mesh_loader_ = new OFF_Loader();
                log(DEBUG_EXTRA) << "[Mesh Loader] : Saving a .off file." << endLog();

                return mesh_loader_->save(filename,target_mesh);
                break;
            case OBJ:
                mesh_loader_ = new OBJ_Loader();
                log(DEBUG_EXTRA) << "[Mesh Loader] : Saving a .obj file." << endLog();
                return mesh_loader_->save(filename,target_mesh);
                break;
            default:
                return false;
            }
        }

        else{
            log(ERROR) << "[Mesh Loader] : unknow file format" << endLog();
            return false;
        }
    }

    log(ERROR) << "[Mesh Loader] : the target directory does not exist." << endLog();
    return false;

}
}
