/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef MESH3D_H
#define MESH3D_H

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <stdio.h>
#include <stdexcept>


#include "IndexedMesh/color.h"
#include "IndexedMesh/float3.h"
#include "IndexedMesh/float2.h"
#include "IndexedMesh/triangle.h"

/*! \file mesh3D.h
    \brief Contains the Mesh3D class that stores and manages a basic 3D mesh.
*/


namespace Mesh3D{



/*! \class Mesh
    \ingroup Mesh3D
    \brief the basic 3D mesh class : contains and manages a 3D mesh.
*/
class Mesh
{
public:
    Mesh();
    Mesh(const Mesh& mesh);
    //! Build a Mesh3D from a file.
    Mesh(const std::string filename);
    //! Build a Mesh3D from a point cloud
    /*! \param points the points of the cloud
        \param normals the normals of the cloud
    */
    Mesh(std::vector<float3> points,std::vector<float3> normals);

    //! Check if the mesh is empty, i.e. it does not contains vertices.
    /*!
      \return true if the mesh is empty, false if not.
     */
    bool is_empty() const{
        if (num_vertices_ == 0) return true;
        return false;
    }

    //! Check if the mesh is textured, i.e. it contains a uv map.
    /*!
      \return true if the mesh is textured, false if not.
      */
    bool is_textured() const{
        if (texture_coordinates_.size() > 0 && texture_indices_.size() >0) return true;
        return false;
    }

    //! Check if the mesh is colored, i.e. a color is associated to each vertex.
    /*!
      \return true if the mesh is colored, false if not.
      */
    bool is_colored() const{
        if (colors_.size() == vertices_.size()) return true;
        return false;
    }
    //! Check if the mesh is associated to any material file
    /*! \returns true if the mesh is associated to a material file, false if not
    */
    bool contains_material() const{
        if (material_file_.size() >0){
            return true;
        }
        return false;
    }

    //! Get the vertices of the mesh.
    /*!
      \return a reference to the vertices vector.
      */
    const std::vector<float3>& vertices() const{
        return vertices_;
    }

    //! Get a specific vertex of the mesh.
    /*!
      \param index the index of the vertex.
      \return the value of the vertex.
      */
    float3 get_vertex(int index) const{
        return vertices_[index];
    }

    //! Get the normals of the mesh
    /*!
      \return a reference to the normals vector.
      */
    const std::vector<float3>& normals() const{
        return normals_;
    }

    //! Get the triangles of the mesh
    /*!
      \return a reference to the triangles vector.
      */
    const std::vector<Triangle>& triangles() const{
        return triangles_;
    }

    //! Get a specific triangle of the mesh
    /*!
      \param the index of the triangle we want to access.
      \return the value of the triangle.
      */
    Triangle get_triangle(int index) const{
        return triangles_[index];
    }

    //! Get the colors of the mesh
    /*!
      \return a reference to the colors vector.
      */
    const std::vector<Color>& colors() const{
        return colors_;
    }

    //! Get a specific color of the mesh
    /*!
      \param the index of the color we want to access.
      \return the value of the color.
      */
    Color get_color(int index) const{
        return colors_[index];
    }

    //! Get a specific texture coordinate of the mesh
    /*!
      \param the index of the texture coordinate we want to access.
      \return the value of the texture coordinate.
      */
    float2 get_texture_coordinate(int index) const{
        return texture_coordinates_[index];
    }

    //! Get the number of texture coordinates in the mesh.
    /*!
      \return the number of texture coordinates in the mesh.
      */
    int num_texture_coordinates() const {
        return texture_coordinates_.size();
    }

    //! Get a specific texture index of the mesh
    /*!
      \param the index of the texture index we want to access.
      \return the value of the texture index.
      */
    Triangle get_texture_index(int index) const{
        return texture_indices_[index];
    }

    //! Get the number of texture indices in the mesh.
    /*!
      \return the number of texture indices in the mesh.
      */
    int num_texture_indices() const {
        return texture_indices_.size();
    }

    //! Get the number of vertices in the mesh.
    /*!
      \return the number of vertices in the mesh.
      */
    int num_vertices() const{
        return vertices_.size();
    }


    //! Get the number of triangles in the mesh.
    /*!
      \return the number of triangles in the mesh.
      */
    int num_triangles() const {
        return triangles_.size();
    }

    //! Add a triangle to the mesh
    /*! \param triangle the triangle to add
    */
    void add_triangle(Triangle triangle){
        triangles_.push_back(triangle);
        num_triangles_ = triangles_.size();
    }
    //! Add a triangle to the mesh
    /*! \param v0 the first vertex of the triangle
        \param v1 the second vertex of the triangle
        \param v2 the third vertex of the triangle
    */
    void add_triangle(int v0,int v1, int v2){
        triangles_.push_back(Triangle(v0,v1,v2));
        num_triangles_ = triangles_.size();
    }

    //! Set the triangles of the mesh.
    /*!
      \param triangles the vector containing the triangles
    */
    void set_triangles(std::vector<Triangle> triangles){
        triangles_=triangles;
        num_triangles_ = triangles_.size();
    }

    //! Add a vertex to the mesh
    /*! \param vertex the new vertex
    */
    void add_vertex(float3 vertex){
        vertices_.push_back(vertex);
        num_vertices_ = vertices_.size();
    }

    //! Add a vertex to the mesh
    /*! \param x the x coordinate of the new vertex
        \param y the y coordinate of the new vertex
        \param z the z coordinate of the new vertex
    */
    void add_vertex(float x, float y, float z){
        vertices_.push_back(make_float3(x,y,z));
        num_vertices_ = vertices_.size();
    }

    //! Set the vertices of the mesh.
    /*!
      \param vertices the vector containing the vertices.
    */
    void set_vertices(std::vector<float3> vertices){
        vertices_ = vertices;
        num_vertices_ = vertices_.size();
    }

    //! Set the normals of the mesh.
    /*!
      \param normals the vector containing the normals.
    */
    void set_normals(std::vector<float3> normals){
        normals_ = normals;
    }

    //! Set the colors of the mesh.
    /*!
      \param colors the vector containing the colors.
    */
    void set_colors(std::vector<Color> colors){
        if (colors.size() == num_vertices_){
            colors_ = colors;
        }
    }
    //! Set the color of a given vertex
    /*!
      \param color the color
      \index the index of the vertex
    */
    void set_color(Color color, int index){
        colors_[index] = color;
    }

    //! Associate texture coordinates to the mesh.
    void set_texture_coordinates(std::vector<float2> texture_coordinates){
        texture_coordinates_ = texture_coordinates;
    }

    //! Set the texture indices associated to each face of the mesh
    /*! \param texture_indices the indices of the texels associated to each  vertices of the triangle
    */
    void set_texture_indices(std::vector<Triangle> texture_indices){
        texture_indices_ = texture_indices;
    }

    //! Set the path to the material file associated to the mesh
    /*! \param material_file the path to the material file
    */
    void set_material_file(std::string material_file){
        material_file_ = material_file;

    }

    //! Get the path to the material file associated to the mesh
    /*! \return the path to the material file
    */
    std::string material_file() const{
        return material_file_;
    }

    //! Compute the center of mass of the mesh.
    /*!
      \return the center of mass.
    */
    float3 get_center_of_mass() const;

    //! Average the normals from the triangles and the vertices.
    void average_normals();

    //! Compute the mean edge length.
    /*!
      \return the mean edge length.
    */
    double get_mean_edge_length() const;

    //! Compute the barycenter of the mesh.
    /*! \param xmin a reference to the minimum x coordinate
        \param xmax a reference to the maximum x coordinate
        \param ymin a reference to the minimum y coordinate
        \param ymax a reference to the maximum y coordinat
        \param zmin a reference to the minimum z coordinate
        \param zmax a reference to the maximum z coordinat
    */
    void get_bounding_box(float &xmin,
                          float &xmax,
                          float &ymin,
                          float &ymax,
                          float &zmin,
                          float &zmax) const;

    //! Load the mesh from a file.
    /*! \param filename the path to the file containing the mesh.
    */
    void load(const std::string filename);

    //! Save the mesh to a file.
    /*! \param filename the path to the file containing the mesh.
    */
    void save(const std::string filename);

    //! Align the mesh's barycenter with a target mesh's barycenter.
    /*! \param target_mesh a reference to the target mesh.
        \return true if success, false if an error occured.
    */
    bool align_with(const Mesh &target_mesh);
    void scale(float scale_factor);
    void switchYZAxis();

    void translate(float3 translation);

    //! the output stream operator to display the basic mesh information.
    friend std::ostream &operator<<(std::ostream &out, Mesh& mesh)
    {
        if (mesh.num_vertices_ == 0 && mesh.num_triangles_ == 0){
            out<< "[Mesh3D] : empty mesh.";
        }
        else {
            out<< "[Mesh3D] : " << mesh.num_vertices() << " vertices, " << mesh.num_triangles() << " triangles. ";
        }
        return out;
    }

protected:
    //! The vertices of the mesh.
    std::vector<float3> vertices_;
    //! The normals of the mesh.
    std::vector<float3> normals_;
    //! The triangles of the mesh.
    std::vector<Triangle> triangles_;
    //! A color associated to each vertex of the mesh.
    std::vector<Color> colors_;

    //! The texture coordinates of the mesh if it is textured.
    std::vector<float2> texture_coordinates_;
    //! The texture indices associated to each triangles.
    std::vector<Triangle> texture_indices_;

    //! The material file associated to the mesh.
    std::string material_file_;

    //! The number of vertices.
    unsigned int num_vertices_;
    //! The number of triangles.
    unsigned int num_triangles_;
    //! The path to the file containing the mesh.
    std::string path_;
};


}

#endif // MESH3D_H

