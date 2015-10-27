#############################################################
#               Copyright (C) 2014 INRIA                    #
#############################################################
project (libIndexedMesh)

cmake_minimum_required(VERSION 2.8)


find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set( INDEXEDMESH_SRC
                     src/mesh3d.cpp
                     src/color.cpp
					 src/triangle.cpp
                     src/meshloadermanager.cpp
                     src/MeshLoaders/off_loader.cpp
                     src/MeshLoaders/obj_loader.cpp
                     src/MeshLoaders/4dm_loader.cpp
                     src/adjacencygraph.cpp
                     src/edgeadjacencygraph.cpp
                     src/dijkstra.cpp
                     src/colorgradient.cpp
                     src/hausdorff.cpp
)

set(INDEXEDMESH_INCLUDE
    include/IndexedMesh/float3.h
    include/IndexedMesh/float2.h
    include/IndexedMesh/color.h
	include/IndexedMesh/triangle.h
    include/IndexedMesh/mesh3d.h
    include/IndexedMesh/meshloaderinterface.h
    include/IndexedMesh/meshloadermanager.h


    include/IndexedMesh/MeshLoaders/off_loader.h
    include/IndexedMesh/MeshLoaders/obj_loader.h
    include/IndexedMesh/MeshLoaders/4dm_loader.h

    include/IndexedMesh/adjacencygraph.h
    include/IndexedMesh/edgeadjacencygraph.h
    include/IndexedMesh/dijkstra.h
    include/IndexedMesh/colorgradient.h
    include/IndexedMesh/hausdorff.h
)

INCLUDE_DIRECTORIES( include
                     ${Logger_SOURCE_DIR}/include
                     4DVSdk/include
                     4DVSdk/lib64
                     4DVSdk/win32
                     4DVSdk/win64
                     )

ADD_DEFINITIONS(-DINDEXEDMESH_EXPORTS -D_CRT_NOFORCE_MANIFEST)
ADD_LIBRARY(IndexedMesh SHARED ${INDEXEDMESH_SRC} ${INDEXEDMESH_INCLUDE})
MARK_AS_ADVANCED(LIB_4DVSDK)

# ##############################
# Platform Specific
IF(WIN32)
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
FIND_LIBRARY(LIB_4DVSDK NAME 4DVSdk PATHS 4DVSdk/win32)
ENDIF(WIN32)

IF(WIN64)
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
FIND_LIBRARY(LIB_4DVSDK NAME 4DVSdk PATHS 4DVSdk/win64)
ENDIF(WIN64)

if( UNIX )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}  -O3 -fopenmp")
SET_TARGET_PROPERTIES( IndexedMesh PROPERTIES COMPILE_FLAGS -fPIC)
FIND_LIBRARY(LIB_4DVSDK NAME 4DVSdk PATHS 4DVSdk/lib64)
endif( UNIX )

TARGET_LINK_LIBRARIES(IndexedMesh Logger ${Boost_LIBRARIES} ${LIB_4DVSDK})


