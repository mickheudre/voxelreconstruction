#include "IndexedMesh/edgeadjacencygraph.h"

namespace Mesh3D{
EdgeAdjacencyGraph::EdgeAdjacencyGraph()
{
    boundaries_.resize(0);
    adjacent_vertices_.resize(0);

}

EdgeAdjacencyGraph::EdgeAdjacencyGraph(std::vector<Edge> adjacent_edges, std::vector<int> boundaries)
{
    adjacent_edges_ = adjacent_edges;
    adjacent_vertices_ = convert_edges_to_vertices();
    boundaries_ = boundaries;

}

std::vector<int> EdgeAdjacencyGraph::convert_edges_to_vertices(){
    std::vector<int> adjacent_vertices(adjacent_edges_.size());
    for (int i = 0; i < adjacent_edges_.size(); i++){
        adjacent_vertices[i] = adjacent_edges_[i].v;
    }
    return adjacent_vertices;
}

int EdgeAdjacencyGraph::num_triangles(){
    int numVertices = boundaries_.size() - 1;
    int numTriangles = 0;
    for(int vi = 0; vi< numVertices; vi++){
        std::vector<Edge>::const_iterator eBegin = adjacent_edges_.begin() + boundaries_[vi];
        std::vector<Edge>::const_iterator eEnd = adjacent_edges_.begin() + boundaries_[vi+1];

        for(std::vector<Edge>::const_iterator e_itr = eBegin; e_itr != eEnd; e_itr++) {
             if( e_itr->hasNext && e_itr->v < vi && e_itr->vnext < vi) numTriangles++;
        }
    }

    return numTriangles;

}

std::vector<Triangle> EdgeAdjacencyGraph::convert_adjacency_graph_to_triangles()
{
    std::vector<Triangle> triangles;

    int numVertices = boundaries_.size() - 1;
    int numTriangles = num_triangles();

    triangles.clear();
    triangles.reserve(numTriangles);

    for(int vi=0;vi<numVertices;++vi)
    {
        std::vector<Edge>::const_iterator eBegin = adjacent_edges_.begin() + boundaries_[vi];
        std::vector<Edge>::const_iterator eEnd =  adjacent_edges_.begin() + boundaries_[vi+1];

        for(std::vector<Edge>::const_iterator e_itr = eBegin; e_itr != eEnd; e_itr++) {
            if( e_itr->hasNext && e_itr->v < vi && e_itr->vnext < vi) {
                Triangle t;
                t.v0 = vi;
                t.v1 = e_itr->v;
                t.v2 = e_itr->vnext;
                triangles.push_back(t);
            }
        }
    }

    assert( int(triangles.size()) == numTriangles );
}


void EdgeAdjacencyGraph::reindex_edge_adjacency_graph(const std::vector<int> &old_indices){
    std::vector<Edge> new_adjacency_graph(adjacent_edges_.size());
    std::vector<int> new_boundaries;
    typedef std::vector<Edge>::const_iterator EdgeIterator;

    // Size verifications
    int numVertices = old_indices.size();
    assert( int(boundaries_.size() - 1) == numVertices );

    std::vector<int> newIndices(numVertices);
    for(int vi=0;vi<numVertices;++vi) {
        newIndices[old_indices[vi]] = vi;
    }


    // Allocation
    new_boundaries.resize(numVertices + 1);
    new_adjacency_graph.clear();
    new_adjacency_graph.reserve( adjacent_edges_.size() );


    // And lets write the thing
    int offset = 0;
    for(int vi=0;vi<numVertices;++vi)
    {
        new_boundaries[vi] = offset;
        const EdgeIterator eBegin_old = adjacent_edges_.begin() + boundaries_[old_indices[vi]];
        const EdgeIterator eEnd_old   = adjacent_edges_.begin() + boundaries_[old_indices[vi]+1];
        for(EdgeIterator e_itr = eBegin_old; e_itr != eEnd_old; ++e_itr) {
            Edge e = *e_itr;
            e.v = newIndices[e.v];
            if( e.hasNext ) e.vnext = newIndices[e.vnext];
            new_adjacency_graph.push_back(e);
            offset++;
        }
    }
    new_boundaries[numVertices] = offset;

    set_adjacent_edges(new_adjacency_graph);
    set_boundaries(new_boundaries);
    adjacent_vertices_ = convert_edges_to_vertices();

}
}
