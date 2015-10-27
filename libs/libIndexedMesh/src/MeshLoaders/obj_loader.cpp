#include "IndexedMesh/MeshLoaders/obj_loader.h"


namespace Mesh3D{

OBJ_Loader::OBJ_Loader(){

}


bool OBJ_Loader::load(const std::string &filename, Mesh *target_mesh){
    std::ifstream file_in(filename.c_str());
    if(!file_in.is_open()){
        if (!file_in.is_open()){
            log(DEBUG) << "[Mesh Loader] : Unable to load " << filename << endLog();
            return false;
        }

    }

    std::vector<float3> vertices;
    std::vector<float2> texture_coordinates;
    std::vector<Color> colors;
    std::vector<Triangle> triangles;
    std::vector<Triangle> texture_indices;
    std::string file_buffer;
    std::string material;


    while(!file_in.eof()) {

        std::getline(file_in,file_buffer);

        float x,y,z,r,g,b,a,xt,yt;
        int v0,v1,v2,vt0,vt1,vt2;
        char materials_file[50];

        if (std::sscanf(file_buffer.c_str(),"v %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b) == 6){
            vertices.push_back(make_float3(x,y,z));
            colors.push_back(Color(r,g,b,1));
        }
        else if (std::sscanf(file_buffer.c_str(),"v %f %f %f",&x, &y,&z) == 3){
            vertices.push_back(make_float3(x,y,z));
        }
        else if (std::sscanf(file_buffer.c_str(),"vt %f %f",&xt, &yt) == 2){
            texture_coordinates.push_back(make_float2(xt,yt));
        }
        else if (std::sscanf(file_buffer.c_str(),"f %d/%d %d/%d %d/%d",&v0,&vt0, &v1,&vt1,&v2,&vt2) == 6) {
            triangles.push_back(Triangle(v0-1,v1-1,v2-1));
            texture_indices.push_back(Triangle(vt0,vt1,vt2));
        }
        else if (std::sscanf(file_buffer.c_str(),"f %d %d %d",&v0, &v1, &v2) == 3) {
            triangles.push_back(Triangle(v0-1,v1-1,v2-1));
        }

        else if (std::sscanf(file_buffer.c_str(),"mtllib %s",&materials_file[0]) == 1){
            material = std::string(materials_file);
        }

        file_buffer.clear();
    }

    target_mesh->set_vertices(vertices);
    target_mesh->set_triangles(triangles);
    target_mesh->set_colors(colors);
    target_mesh->set_texture_indices(texture_indices);
    target_mesh->set_texture_coordinates(texture_coordinates);
    target_mesh->set_material_file(material);
    file_in.close();

    log(DEBUG_EXTRA) <<"[Mesh Loader] : loaded a mesh with "<<target_mesh->num_vertices()<<" vertices and "<<target_mesh->num_triangles()<< " triangles" << endLog();

    return true;
}

bool OBJ_Loader::save(const std::string &filename, Mesh *target_mesh){


    //To do : create the folder if needed!
    std::ofstream file_out(filename.c_str());

    if ( !file_out.is_open()){
        log(ERROR) << "[Mesh Loader] : unable to create " << filename << "." << endLog();
        return false;
    }

    file_out << "o " << filename << std::endl;
    file_out << "# " << target_mesh->num_vertices() << " vertices" << std::endl;
    file_out << "# " << target_mesh->num_triangles() << " triangles" << std::endl;

    if (target_mesh->contains_material()){
        file_out << "mtllib " << target_mesh->material_file() << std::endl;
    }

//    if (target_mesh->is_colored()){
//        for (int i = 0; i < target_mesh->num_vertices() ;i++){
//            Color color = target_mesh->get_color(i);
//            file_out << "v " << target_mesh->get_vertex(i) << " "<< color << std::endl;
//        }
//    }
//    else{
        for (int i = 0; i < target_mesh->num_vertices() ;i++){
            file_out << "v " << target_mesh->get_vertex(i) << std::endl;
        }
//    }

    if (!target_mesh->is_textured() || target_mesh->num_triangles()!=target_mesh->num_texture_indices()){
        if (target_mesh->contains_material()){
            file_out << "usemtl material_0" << std::endl;
        }
        for (int i = 0; i < target_mesh->num_triangles(); i++){
            Triangle t = target_mesh->get_triangle(i);
            file_out << "f " << t.v0 +1 << " "<< t.v1 +1 << " " << t.v2 +1 << std::endl;
        }
    }

    else{

        for (int i = 0; i < target_mesh->num_texture_coordinates(); i++){
            file_out << "vt " << target_mesh->get_texture_coordinate(i) << std::endl;
        }

        if (target_mesh->contains_material()){
            file_out << "usemtl material_0" << std::endl;
        }

        for (int i = 0; i < target_mesh->num_triangles(); i++){
            Triangle t = target_mesh->get_triangle(i);
            Triangle v = target_mesh->get_texture_index(i);
            file_out << "f " << t.v0 +1 <<"/"<<v.v0 << " "<< t.v1 +1 <<"/"<<v.v1 << " " << t.v2 +1 <<"/"<<v.v2 << std::endl;
        }
    }

    file_out.close();
    log(DEBUG_EXTRA) <<"[Mesh Loader] : saved a mesh with "<<target_mesh->num_vertices()<<" vertices and "<<target_mesh->num_triangles()<< " triangles" << endLog();
    return true;

}

}

