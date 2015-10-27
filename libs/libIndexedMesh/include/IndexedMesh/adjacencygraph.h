/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef ADJACENCYGRAPH_H
#define ADJACENCYGRAPH_H

#include <vector>
#include "IndexedMesh/mesh3d.h"
#include "Logger.h"


/*! \file adjacencygraph.h
    \brief an adjacency graph class.
*/

using Mesh3D::Triangle;


/*! \class AdjacencyGraph
    \brief a class to manage vertex adjacency graphs.
    \ingroup Mesh3D
    The structure is simple :\n
    The vertex i is adjacent to adjacent_vertices[boundaries[i]]...adjacent_vertices[boundaries[i+1]-1]

*/
class AdjacencyGraph
{
public:
    //! Construct an empty adjacency graph.
    AdjacencyGraph();
    //! Construct an adjacency from an existing structure.
    /*! \param adjacent_vertices a vector of adjacent vertices
        \param boundaries a vector containing the boundaries to extract the adjacent vertices of a given vertex in the adjacent_vertices vector.
    */
    AdjacencyGraph(std::vector<int> adjacent_vertices, std::vector<int> boundaries);

        //typedef std::vector<int>::const_iterator AdjacencyIterator;

    //! Get an iterator to the begining of the adjacent vertices of a given vertex.
    /*! \param id the index of the vertex
        \return an iterator to the begining of the adjacent vertices of the vertex id.
    */
    inline std::vector<int>::iterator get_neighbours_vertices_iterator_begin(int id){
            return adjacent_vertices_.begin() + boundaries_[id];
    }

    //! Get an iterator to the end of the adjacent vertices of a given vertex.
    /*! \param id the index of the vertex
        \return an iterator to the end of the adjacent vertices of the vertex id.
    */
    inline std::vector<int>::iterator get_neighbours_vertices_iterator_end(int id){
        return adjacent_vertices_.begin() + boundaries_[id+1];
    }


    inline const std::vector<int>& adjacent_vertices() const{
        return adjacent_vertices_;
    }

    inline const std::vector<int>& boundaries() const{
        return boundaries_;
    }

    //! Get the adjacent vertices of a given vertex.
    std::vector<int> adjacent_vertices_of(int node_id) const;

    //! Get the number of adjacent vertices to a given vertex.
    int num_adjacent_vertices_of(int node_id) const;

    //! Reindex the adjacency graph following a new order.
    void reindex_adjacency_graph(std::vector<int> old_indices);

protected:
    void set_boundaries(const std::vector<int>& boundaries){
        boundaries_ = boundaries;
    }
    void set_ajacent_vertices(const std::vector<int>& adjacent_vertices){
        adjacent_vertices_ = adjacent_vertices;
    }

protected:
    //! The list of the adjacent vertices filled following the vertices index.
    std::vector<int> adjacent_vertices_;
    //! The boundaries that delimits the adjacent vertices of every vertex.
    std::vector<int> boundaries_;
};

#endif // ADJACENCYGRAPH_H
