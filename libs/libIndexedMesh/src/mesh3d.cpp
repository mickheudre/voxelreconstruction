#include "IndexedMesh/mesh3d.h"
#include "Logger.h"
#include "IndexedMesh/meshloadermanager.h"
#include <limits>

namespace Mesh3D{
Mesh::Mesh()
{
    vertices_.resize(0);
    normals_.resize(0);
    colors_.resize(0);
    triangles_.resize(0);

    texture_coordinates_.resize(0);
    texture_indices_.resize(0);

    num_vertices_ = 0;
    num_triangles_ = 0;

}

Mesh::Mesh(const std::string filename){

    vertices_.resize(0);
    normals_.resize(0);
    colors_.resize(0);
    triangles_.resize(0);

    texture_coordinates_.resize(0);
    texture_indices_.resize(0);

    num_vertices_ = 0;
    num_triangles_ = 0;

    load(filename);

    if (num_triangles_ != 0){
        average_normals();
    }
}

Mesh::Mesh(const Mesh &mesh){
#pragma omp critical
    {
        vertices_ = mesh.vertices();
        triangles_ = mesh.triangles();
        normals_ = mesh.normals();

        num_vertices_ = vertices_.size();
        num_triangles_ = triangles_.size();

        if (mesh.is_colored()){
            colors_ = mesh.colors();
        }
        if (mesh.is_textured()){
            texture_coordinates_ = mesh.texture_coordinates_;
            texture_indices_ = mesh.texture_indices_;
        }
    }
}

Mesh::Mesh(std::vector<float3> points, std::vector<float3> normals){

    if (points.size() != normals.size()){
        log(ERROR) << "[Mesh 3D] : invalid mesh. One normal should be associated to each points." << endLog();
        throw( std::runtime_error("[Fatal Error] : invalid mesh." ) );
    }

    vertices_ = points;
    num_vertices_ = points.size();
    normals_ = normals;
    triangles_.resize(0);
    colors_.resize(0);

    texture_coordinates_.resize(0);
    texture_indices_.resize(0);
}

void Mesh::average_normals(){
    if(num_triangles_ == 0){
        log(ERROR) << "[Mesh3D] : Unable to average normals. The number of triangles should be greater than 0..." << endLog();
    }
    else{
        normals_.resize(num_vertices_,make_float3(0,0,0));
        for(std::vector<Triangle>::const_iterator t_itr = triangles_.begin(); t_itr != triangles_.end(); ++t_itr)
        {
            const float3& v0 = vertices_[t_itr->v0];
            const float3& v1 = vertices_[t_itr->v1];
            const float3& v2 = vertices_[t_itr->v2];

            float3 localNorm( cross(v1-v0, v2-v0) );
            normals_[t_itr->v0] += localNorm;
            normals_[t_itr->v1] += localNorm;
            normals_[t_itr->v2] += localNorm;
        }

        for( std::vector<float3>::iterator n_itr = normals_.begin(); n_itr != normals_.end(); ++n_itr){
            double l = norm2(*n_itr);
            if( l != 0.0) *n_itr /= l;
        }
    }
}

void Mesh::load(const std::string filename){
    MeshLoaderManager mesh_loader;
    if(!mesh_loader.load_file(filename,this)){
        log(ERROR) << "[Mesh 3D] : unable to read " + filename << endLog();
        throw( std::runtime_error("[Fatal Error] : invalid file. "));
    };

    path_ = filename;
    log(ALWAYS) << "[Mesh3D] : successfully loaded " + filename << endLog();
}

void Mesh::save(const std::string filename){
    MeshLoaderManager mesh_loader;
    if(!mesh_loader.save_file(filename,this)){
        log(ERROR) << "[Mesh 3D] : unable to save " + filename << endLog();
        throw( std::runtime_error("[Fatal Error] : invalid file."));
    };

    log(ALWAYS) << "[Mesh3D] : successfully saved " + filename << endLog();
}

double Mesh::get_mean_edge_length() const{
    if( triangles_.size() == 0 ){
        log(ERROR) << "[Mesh 3D] : the mesh does not contain triangles." << endLog();
        throw( std::runtime_error("[Fatal Error] : invalid mesh." ) );
    }

    double meanEdge = 0.0;
    for(int i =0; i < num_triangles_ ; i++ )
    {
        double t1 = norm2( vertices_[ triangles_[i].v0] - vertices_[ triangles_[i].v1] );
        double t2 = norm2( vertices_[ triangles_[i].v1] - vertices_[ triangles_[i].v2] );
        double t3 = norm2( vertices_[ triangles_[i].v2] - vertices_[ triangles_[i].v0] );
        meanEdge += t1+t2+t3;
    }
    return meanEdge / double( 3*num_triangles_);
}

void Mesh::get_bounding_box(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const{
    xmin = ymin = zmin = std::numeric_limits<double>::max();
    xmax = ymax = zmax = std::numeric_limits<double>::min();

    for(std::vector<float3>::const_iterator v_itr = vertices_.begin(); v_itr != vertices_.end(); ++ v_itr)
    {
        const float3& v = *v_itr;
        if( xmin > v.x ) xmin = v.x;
        if( xmax < v.x ) xmax = v.x;
        if( ymin > v.y ) ymin = v.y;
        if( ymax < v.y ) ymax = v.y;
        if( zmin > v.z ) zmin = v.z;
        if( zmax < v.z ) zmax = v.z;
    }
}

float3 Mesh::get_center_of_mass() const{
    const double one_div_6 = (double)(1.0/6.0);
    const double one_div_24 = (double)(1.0/24.0);

    // 1, x, y, z
    double integral[4] = { 0.0, 0.0, 0.0, 0.0 };


    for (int i = 0; i < triangles_.size(); i++)
    {
        // Get vertices_ of triangle i.
        float3 v0 = vertices_[triangles_.at(i).v0];
        float3 v1 = vertices_[triangles_.at(i).v1];
        float3 v2 = vertices_[triangles_.at(i).v2];

        // Get normal vector
        float3 V1mV0 = v1 - v0;
        float3 V2mV0 = v2 - v0;
        float3 N = cross(V1mV0,V2mV0);

        // Compute integral terms.
        double tmp0, tmp1, tmp2;
        double f1x, f2x;
        tmp0 = v0.x + v1.x;
        f1x = tmp0 + v2.x;
        tmp1 = v0.x*v0.x;
        tmp2 = tmp1 + v1.x*tmp0;
        f2x = tmp2 + v2.x*f1x;

        double f1y, f2y;
        tmp0 = v0.y + v1.y;
        f1y = tmp0 + v2.y;
        tmp1 = v0.y*v0.y;
        tmp2 = tmp1 + v1.y*tmp0;
        f2y = tmp2 + v2.y*f1y;

        double f1z, f2z;
        tmp0 = v0.z + v1.z;
        f1z = tmp0 + v2.z;
        tmp1 = v0.z*v0.z;
        tmp2 = tmp1 + v1.z*tmp0;
        f2z = tmp2 + v2.z*f1z;

        // Update integrals.
        integral[0] += N.x*f1x;
        integral[1] += N.x*f2x;
        integral[2] += N.y*f2y;
        integral[3] += N.z*f2z;

    }

    integral[0] *= one_div_6;
    integral[1] *= one_div_24;
    integral[2] *= one_div_24;
    integral[3] *= one_div_24;


    return  make_float3(integral[1], integral[2], integral[3])/integral[0];
}

bool Mesh::align_with(const Mesh &target_mesh){
    if(is_empty() || target_mesh.is_empty()) return false;
    float3 mesh_cog = get_center_of_mass();
    float3 target_cog = target_mesh.get_center_of_mass();

    float3 delta = target_cog - mesh_cog;

    for (int i = 0; i < num_vertices_; i++){
        vertices_[i] += delta;
    }
    return true;
}

void Mesh::translate(float3 translation){
    for (int i = 0; i < num_vertices_; i++){
        vertices_[i] -= translation;
    }
}

void Mesh::scale(float scale_factor){
    for (int i = 0; i < num_vertices_; i++){
        vertices_[i] *= scale_factor;
    }
}

void Mesh::switchYZAxis(){
    for (int i = 0; i < num_vertices(); i++){
        float3 vert = vertices_[i];
        vertices_[i] = make_float3(vert.x,vert.z,vert.y);
    }
}
}
