#include "IndexedMesh/MeshLoaders/off_loader.h"

namespace Mesh3D{

OFF_Loader::OFF_Loader(){
}
bool OFF_Loader::load(const std::string &filename, Mesh *target_mesh){

    OFFFormat off_format;
    int num_vertices,num_faces,num_edges;
    std::vector<float3> vertices;
    std::vector<Triangle> triangles;
    std::vector<Color> colors;


    std::ifstream file(filename.c_str());

    if (!file.is_open()){
        log(ERROR) << "[Mesh Loader] : Unable to load " << filename << endLog();
        return false;
    }

    std::string buffer;
    std::getline(file,buffer);

    if (buffer.find("OFF") == 0){
        off_format = STANDARDOFF;
    }
    else if (buffer.find("COFF") == 0){
        off_format = COFF;
    }

    else if (buffer.find("NOFF") == 0){
        off_format = NOFF;
    }

    else{
        log(ERROR) << "[Mesh Loader] : Unable to load " << filename << " : unknown File Format" << endLog();
        off_format = UNKNOWNOFF;
        return false;
    }

    std::string file_buffer;
    std::getline(file,file_buffer);
    std::sscanf(file_buffer.c_str(),"%d %d %d", &num_vertices, &num_faces, &num_edges);


    float x,y,z,r,g,b,a;
    int v0,v1,v2;


    for(size_t i = 0; i < num_vertices;i++){
        std::getline(file,file_buffer);

        if (off_format == STANDARDOFF){
            std::sscanf(file_buffer.c_str(),"%f %f %f",&x, &y,&z);
            vertices.push_back(make_float3(x,y,z));
        }
        else if (off_format == COFF){
            std::sscanf(file_buffer.c_str(),"%f %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b, &a);
            vertices.push_back(make_float3(x,y,z));
            if (r > 1 || b > 1 || g > 1 || a > 1){
                r /= 255; g /=255; b/=255; a/=255;
            }
            colors.push_back(Color(r,g,b,a));
        }

    }
    target_mesh->set_vertices(vertices);
    target_mesh->set_colors(colors);
    for (size_t i = 0; i < num_faces; i++){
        std::getline(file,file_buffer);

        std::sscanf(file_buffer.c_str(),"3 %d %d %d",&v0, &v1, &v2);
        triangles.push_back(Triangle(v0,v1,v2));
    }



//    if (std::sscanf(file_buffer.c_str(),"# %s", &trash) == 1){
//        log(ALWAYS) << "Deleted comment" << endLog();
//    }


//    if (std::sscanf(file_buffer.c_str(),"%f %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b, &a) == 6){
//        vertices.push_back(make_float3(x,y,z));

//        if (r > 1 || b > 1 || g > 1 || a > 1){
//            r /= 255; g /=255; b/=255; a/=255;
//        }
//        colors.push_back(Color(r,g,b,a));
//    }

//    else if (std::sscanf(file_buffer.c_str(),"%f %f %f",&x, &y,&z) == 3){
//        vertices.push_back(make_float3(x,y,z));
//    }

//    else if (std::sscanf(file_buffer.c_str(),"3 %d %d %d",&v0, &v1, &v2) == 3) {
//        triangles.push_back(make_triangle(v0,v1,v2));
//    }

//    else if (std::sscanf(file_buffer.c_str(),"3 %d %d %d %s",&v0,&v1,&v2,&trash) == 4) {
//        triangles.push_back(make_triangle(v0,v1,v2));
//    }


//    file_buffer.clear();
//}
//    file >> num_vertices >> num_faces >> num_edges;
//    std::vector<float3> vertices;
//    if (off_format == STANDARDOFF){
//        float x,y,z;
//        for (size_t i = 0; i < num_vertices; i++){
//            file >> x >> y >> z;
//            vertices.push_back(make_float3(x,y,z));
//        }
//        target_mesh->set_vertices(vertices);
//    }

//    else if (off_format == COFF){
//        std::vector<Color> colors;
//        float x,y,z,r,g,b,a;
//        for (size_t i = 0; i < num_vertices; i++){
//            file >> x >> y >>z >> r >> g >> b >> a;
//            vertices.push_back(make_float3(x,y,z));

//            if (r > 1 || b > 1 || g > 1 || a > 1){
//                r /= 255; g /=255; b/=255; a/=255;
//            }
//            colors.push_back(Color(r,g,b,a));
//        }
//        target_mesh->set_vertices(vertices);
//        target_mesh->set_colors(colors);
//    }

//    else if (off_format == NOFF){
//        std::vector<float3> normals;
//        float x,y,z,nx,ny,nz;
//        for (size_t i = 0; i < num_vertices; i++){
//            file >> x >> y >>z >> nx >> ny >> nz;
//            vertices.push_back(make_float3(x,y,z));
//            normals.push_back(make_float3(nx,ny,nz));
//        }
//        target_mesh->set_vertices(vertices);
//        target_mesh->set_normals(normals);
//    }

//    int num_vertices_face = 0;
//    float v1,v2,v3;
//    std::string file_buffer;
//    std::vector<Triangle> triangles;

//    for (size_t i = 0; i < num_faces; i ++){

//        file >> num_vertices_face >> v1 >> v2 >> v3;// >> trash >> trash >> trash;
//        triangles.push_back(make_triangle(v1,v2,v3));
//        if (num_vertices_face != 3){
//            log(DEBUG) << "[Mesh Loader] : Faces are not triangles..." << endLog();
//            return false;
//        }
//    }
//    target_mesh->set_triangles(triangles);


if (triangles.size() == num_faces) target_mesh->set_triangles(triangles);


log(DEBUG_EXTRA) <<"[Mesh Loader] : loaded a mesh with "<<target_mesh->num_vertices()<<" vertices and "<<target_mesh->num_triangles()<< " triangles" << endLog();
file.close();
return true;
}

bool OFF_Loader::save(const std::string &filename, Mesh *target_mesh){

    int numVertices = target_mesh->num_vertices();
    int numTriangles = target_mesh->num_triangles();


    assert(int(target_mesh->colors().size()) == numVertices || !target_mesh->is_colored());

    std::ofstream f_out(filename.c_str());
    if(!f_out.is_open()){
        log(ERROR) << "[Mesh Loader] : unable to create " << filename << "." << endLog();
        return false;
    }

    if (target_mesh->is_colored()){
        f_out<<"COFF\n"<<numVertices<<" "<<numTriangles<<" 0"<< std::endl;
       // f_out<<"OFF\n"<<numVertices<<" "<<numTriangles<<" 0"<< std::endl;

        // write down vertices
        for (int vi=0;vi<numVertices;++vi)
        {
            float3 p = target_mesh->get_vertex(vi);
            Color c = target_mesh->get_color(vi);

            f_out<<p.x<<" "<<p.y<<" "<<p.z <<" "<<c<<"\n";
        }
    }
    else{
        f_out<<"OFF\n"<<numVertices<<" "<<numTriangles<<" 0"<< std::endl;

        // write down vertices
        for (int vi=0;vi<numVertices;++vi)
        {
            float3 p = target_mesh->get_vertex(vi);

            f_out<<p.x<<" "<<p.y<<" "<<p.z<< "\n";
        }
    }
    // write down triangles
    for(int  ti=0;ti<numTriangles;++ti)
    {
        const Triangle& t = target_mesh->triangles()[ti];
        f_out<<"3 "<<t.v0<<" "<<t.v1<<" "<<t.v2<<"\n";
    }

    f_out.close();

    log(DEBUG_EXTRA) <<"[Mesh Loader] : saved a mesh with "<<numVertices<<" vertices and "<<numTriangles<< " triangles" << endLog();
    return true;
}






}
