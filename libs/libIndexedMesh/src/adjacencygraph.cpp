#include "IndexedMesh/adjacencygraph.h"

AdjacencyGraph::AdjacencyGraph()
{
    boundaries_.resize(0);
    adjacent_vertices_.resize(0);
}

AdjacencyGraph::AdjacencyGraph(std::vector<int> adjacent_vertices, std::vector<int> boundaries)
{
    adjacent_vertices_ = adjacent_vertices;
    boundaries_ = boundaries;

}


std::vector<int> AdjacencyGraph::adjacent_vertices_of(int node_id) const{

    std::vector<int> adjacent_vertices;
    if(node_id >= boundaries().size()){
        log(ERROR) << "[Adjacency Graph] : vertex index out of limits." << endLog();
        return adjacent_vertices;
    }

    for (int i = boundaries_[node_id]; i < boundaries_[node_id+1]; i++){
        adjacent_vertices.push_back(adjacent_vertices_[i]);

    }
    return adjacent_vertices;
}

int AdjacencyGraph::num_adjacent_vertices_of(int node_id) const{
    return boundaries_[node_id+1] - boundaries_[node_id];
}

void AdjacencyGraph::reindex_adjacency_graph(std::vector<int> old_indices){
//    std::vector<int> new_adjacency_graph(adjacent_vertices_.size());
//    std::vector<int> new_boundaries;

//    // Size verifications
//    int numVertices = old_indices.size();
//    assert( int(boundaries_.size() - 1) == numVertices );

//    std::vector<int> newIndices(numVertices);
//    for(int vi=0;vi<numVertices;++vi) {
//        newIndices[old_indices[vi]] = vi;
//    }


//    // Allocation
//    new_boundaries.resize(numVertices + 1);
//    new_adjacency_graph.clear();
//    new_adjacency_graph.reserve( adjacent_edges_.size() );


//    // And lets write the thing
//    int offset = 0;
//    for(int vi=0;vi<numVertices;++vi)
//    {
//        new_boundaries[vi] = offset;
//        const EdgeIterator eBegin_old = adjacent_edges_.begin() + boundaries_[old_indices[vi]];
//        const EdgeIterator eEnd_old   = adjacent_edges_.begin() + boundaries_[old_indices[vi]+1];
//        for(EdgeIterator e_itr = eBegin_old; e_itr != eEnd_old; ++e_itr) {
//            Edge e = *e_itr;
//            e.v = newIndices[e.v];
//            if( e.hasNext ) e.vnext = newIndices[e.vnext];
//            new_adjacency_graph.push_back(e);
//            offset++;
//        }
//    }
//    new_boundaries[numVertices] = offset;

//    set_adjacent_edges(new_adjacency_graph);
//    set_boundaries(new_boundaries);
//    adjacent_vertices_ = convert_edges_to_vertices();

}

