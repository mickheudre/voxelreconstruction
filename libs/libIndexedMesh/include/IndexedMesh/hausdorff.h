/* *************************************************
 * Copyright (C) 2014 INRIA
 * *************************************************/
#include <Logger.h>
#include "float3.h"
#include "mesh3d.h"
#include <climits>

/*!
  \file hausdorff.h
  \author Mickaël HEUDRE
  \brief A set of functions to compute the Hausdorf distance between two point clouds
*/
using Mesh3D::float3;

double DirectedHausdorffDiscance(const std::vector<float3> &p, const std::vector<float3> &q);
//Compute the Hausdorff Distance between two meshes
/*! \relates Mesh3D
    \param mesh1 the first mesh
    \param mesh2 the second mesh
    \returns the Haussdorff distance between mesh1 and mesh2
*/
double HausdorffDiscance(const Mesh3D::Mesh &mesh1, const Mesh3D::Mesh &mesh2);
