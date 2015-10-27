/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#ifndef EDGEADJACENCYGRAPH_H
#define EDGEADJACENCYGRAPH_H

#include "adjacencygraph.h"

namespace Mesh3D{
/*! \struct Edge
    \ingroup Mesh3D
    \brief Basic edge structure.
*/
struct Edge {
    int            v; ///< Indice of the first vertex of the edge.
    unsigned char  hasNext; ///< A boolean value : if set to 0, the edge is not complete and contains only one vertex v, vnext is set to -1.
    int            vnext; ///< Indice of the second vertex of the edge.
};

class EdgeAdjacencyGraph : public AdjacencyGraph
{
public:
    EdgeAdjacencyGraph();
    EdgeAdjacencyGraph(std::vector<Edge> adjacent_edges, std::vector<int> boundaries);



    const std::vector<Edge>& adjacent_edges() const{
        return adjacent_edges_;
    }

    int num_triangles();
    void reindex_edge_adjacency_graph(const std::vector<int>& old_indices);
    std::vector<Triangle> convert_adjacency_graph_to_triangles();
    std::vector<int> convert_edges_to_vertices();

private:
    void set_adjacent_edges(const std::vector<Edge>& adjacent_edges){
        adjacent_edges_ = adjacent_edges;
        adjacent_vertices_ = convert_edges_to_vertices();
    }

private:
    std::vector<Edge> adjacent_edges_;
};

}

#endif // EDGEADJACENCYGRAPH_H
