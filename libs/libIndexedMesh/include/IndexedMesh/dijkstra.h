/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <set>
#include <utility>

/*! \file diskstra.h
    \author Mickaël HEUDRE
    \brief an implementation of the diskstra nearest neighbours search algorith
*/

namespace Mesh3D{
//! Computes the shortest path between to points
/*!
  \relates Mesh3D
  \param adjacent_vertices the adjacency graph of the mesh
  \param distance the distance between a vertex and its neighbours, sorted following adjacent_vertices
  \param boundaries the boundaries of the adjacency graph. The neighbours of the vertex i are between adjacent_vertices[boundaries[i]] and adjacent_vertices[boundaries[i+1]]
  \param target_path the shortest path computed by the algorith
  \param start the index of the start vertex
  \param stop the index of the stop vertex.
*/

float Dijkstra(std::vector<int>&   adjacent_vertices,
               std::vector<float>& distance,
               std::vector<int>&   boundaries,
               std::vector<int> &  target_path,
               int                 start,
               int                 end);

}
#endif // DIJKSTRA_H
