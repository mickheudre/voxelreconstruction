cmake_minimum_required(VERSION 2.8)
PROJECT(VoxelReconstuction)
SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )
SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g ")

add_definitions(-DNO_GUI)

add_subdirectory(libs)


#find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
find_package(Boost COMPONENTS program_options format filesystem system)
FIND_PACKAGE ( OpenCV )
MESSAGE ("OpenCV_FOUND="${OpenCV_FOUND} " OpenCV_VERSION="${OpenCV_VERSION} )



#include_directories(${GLEW_INCLUDE_PATH})
#include_directories(${OPENGL_INCLUDE_DIR})
#include_directories(${GLUT_INCLUDE_DIR})

include_directories(${OpenCV_INCLUDE_DIRS} )

include_directories(include
${libIndexedMesh_SOURCE_DIR}/include
#					${GLM_SOURCE_DIR}
#					"${CMAKE_CURRENT_SOURCE_DIR}/libs/CImg"
)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()



set(LIBS
  ${OpenCV_LIBS}
  IndexedMesh
)
#  GLEW
#  GLM
#  pthread
#  ${CMAKE_CURRENT_SOURCE_DIR}/libs/CImg
#  ${OPENGL_GL_LIBRARY}
#  ${OPENGL_LIBRARIES}
#  ${OPENGL_glu_LIBRARY}
#  ${GLUT_glut_LIBRARY}
#  ${Boost_LIBRARIES}



set(INCLUDE
# ${GLM_SOURCE_DIR}
    include/CamParameters.h


)

set(SOURCES 
  src/CamParameters.cpp
  src/main.cpp
)

# Add -g for debugging
add_definitions("-Wall" "-g")

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDE})

# Link the current project with the libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

