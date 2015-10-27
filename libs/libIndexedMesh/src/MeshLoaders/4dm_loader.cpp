#include "IndexedMesh/MeshLoaders/4dm_loader.h"

namespace Mesh3D{

FDM_Loader::FDM_Loader(){

}

bool FDM_Loader::load(const std::string &filename, Mesh *target_mesh){

    FvxDecoder *decoder = FvxDecoder::newFvxDecoder();
    char *fileNameFormat = NULL;
    int firstIt;
    int lastIt;

    std::ifstream fin(filename.c_str(), std::ios::binary);
    if (!fin) {
        log(ERROR) << "[Mesh Loader] : Error while loading file " << filename << endLog();
        return false;
    }
    // 4DM File contains first a 4 bytes header
    char header[4];
    fin.read(header, 4);
    // 4DM File contains a 4 bytes integer value containing the size of the file
    unsigned int chunkSize;
    fin.read((char *) &chunkSize, 4);
    chunkSize -= 8; // Remove the (header + chunkSize) size!
    // chunkSize now contains the real size of the 4DM Chunk
    char *data = new char[chunkSize];
    fin.read(data, chunkSize); // Read from file the chunk
    FvxMesh *mesh = decoder->decode(data, chunkSize); // Decode the 4DM chunk.
    delete [] data;
    if (!mesh->isValid()) {
        log(ERROR) << "[4DVSdk] : Error while decoding file " << filename << endLog();
        return false;
    }
//    log(DEBUG) << "Nb vertices = " << mesh->getVerticesCount() << "\n";
//    log(DEBUG) << "Nb faces = " << mesh->getFacesCount() << "\n";

    if (mesh->getVerticesCount() == 0) {
        log(ERROR) << "[4DVSdk] : Empty mesh : nothing to draw." << endLog();
        return false;
    }

    const FvxVector3f *vertices4dv = mesh->getVerticesPositionsPtr();
    const FvxVector3i *faces4dv = mesh->getFaceVertexIndicesPtr();
    std::vector<float3> vertices(mesh->getVerticesCount());
    std::vector<Triangle> triangles(mesh->getFacesCount());
    for (int i = 0; i < mesh->getVerticesCount(); i++){
        vertices[i] = make_float3(vertices4dv[i].x(),vertices4dv[i].y(),vertices4dv[i].z());
    }
    for (int i = 0; i < mesh->getFacesCount();i++){
        triangles[i] = Triangle(faces4dv[i].x(),faces4dv[i].y(),faces4dv[i].z());
    }
    target_mesh->set_vertices(vertices);
    target_mesh->set_triangles(triangles);
    //    if (mesh->getColoringMode() == FVX_MESH_COLORING_BY_SINGLE_TEXTURE) {
    //        //// Mesh is colored with a single baked texture

    //        //// Read each vertex position
    //        //for(int i = 0; i < mesh->getVerticesCount(); i++) {
    //        //  std::cout << "vertex " << i << " position : " << mesh->getVertexPosition(i) << "\n";
    //        //}
    //        //
    //        //// Read each uv coordinates
    //        //for(int i = 0; i < mesh->getTexCoordsCount(); i++) {
    //        //  std::cout << "uv coord " << i << " : " << mesh->getTexCoord(i) << "\n";
    //        //}
    //        //
    //        //// Read each facet containing vertex position indices + UV coordinates indices
    //        //for(int i = 0; i < mesh->getFacesCount(); i++) {
    //        //  const FvxVector3i &fi = mesh->getFaceVertexIndices(i);
    //        //  const FvxVector3i &ft = mesh->getFaceTexCoordIndices(i);
    //        //  std::cout << "Facet " << i << ":\n";
    //        //  std::cout << "  vertices indices : " << fi[0] << " " << fi[1] << " " << fi[2] << "\n";
    //        //  std::cout << "  uv coord indices : " << ft[0] << " " << ft[1] << " " << ft[2] << "\n";
    //        //}

    //        {
    //            // Retrieve the texture associated with this mesh
    //            const FvxImage *teximage = mesh->getSingleTexture();
    //            std::cout << "Texture image : " << teximage->getWidth() << "x" << teximage->getHeight();
    //            switch(teximage->getFormat())
    //            {
    //            case FVX_IMAGE_FORMAT_INVALID:
    //                std::cout << "  - invalid image\n";
    //                break;
    //            case FVX_IMAGE_FORMAT_RGBA:
    //                std::cout << "  - RGBA\n";
    //                break;
    //            case FVX_IMAGE_FORMAT_BGRA:
    //                std::cout << "  - BGRA\n";
    //                break;
    //            case FVX_IMAGE_FORMAT_RGB:
    //                std::cout << "  - RGB\n";
    //                break;
    //            case FVX_IMAGE_FORMAT_DXT1:
    //                std::cout << "  - DXT1\n";
    //                break;
    //            default: break;
    //            }
    //        }


    // Destroy the decoder
    FvxDecoder::deleteFvxDecoder(decoder);
    return true;
}
bool FDM_Loader::save(const std::string &filename, Mesh *target_mesh){
    return false;
}
}
