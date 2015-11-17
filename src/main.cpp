#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include <cfloat>
#include <bitset>

#include <boost/format.hpp>
#include <boost/dynamic_bitset.hpp>
#include "CamParameters.h"

#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "IndexedMesh/mesh3d.h"

using namespace cv;

Mesh3D::Mesh point_cloud_to_voxels(std::vector<Mesh3D::float3> point_cloud, float block_size, float shrink = 1.0){
    std::vector<Mesh3D::float3> cube_points;
    cube_points.push_back(Mesh3D::make_float3(.5,-.5,-.5));
    cube_points.push_back(Mesh3D::make_float3(.5,-.5,.5));
    cube_points.push_back(Mesh3D::make_float3(-.5,-.5,.5));
    cube_points.push_back(Mesh3D::make_float3(-.5,-.5,-.5));
    cube_points.push_back(Mesh3D::make_float3(.5,.5,-.5));
    cube_points.push_back(Mesh3D::make_float3(.5,.5,.5));
    cube_points.push_back(Mesh3D::make_float3(-.5,.5,.5));
    cube_points.push_back(Mesh3D::make_float3(-.5,.5,-.5));

    std::vector<Mesh3D::Triangle> cube_triangles;
    cube_triangles.push_back(Mesh3D::make_triangle(0,1,2));
    cube_triangles.push_back(Mesh3D::make_triangle(7,6,5));
    cube_triangles.push_back(Mesh3D::make_triangle(4,5,1));
    cube_triangles.push_back(Mesh3D::make_triangle(5,6,2));
    cube_triangles.push_back(Mesh3D::make_triangle(2,6,7));
    cube_triangles.push_back(Mesh3D::make_triangle(4,0,3));
    cube_triangles.push_back(Mesh3D::make_triangle(3,0,2));
    cube_triangles.push_back(Mesh3D::make_triangle(4,7,5));
    cube_triangles.push_back(Mesh3D::make_triangle(0,4,1));
    cube_triangles.push_back(Mesh3D::make_triangle(1,5,2));
    cube_triangles.push_back(Mesh3D::make_triangle(3,2,7));
    cube_triangles.push_back(Mesh3D::make_triangle(7,4,3));

    Mesh3D::Mesh cube;
    cube.set_vertices(cube_points);
    cube.set_triangles(cube_triangles);

    cube.scale(block_size);
    cube.scale(shrink);
    std::vector<Mesh3D::float3> voxel_points;
    std::vector<Mesh3D::Triangle> voxel_triangles;

    for (int i = 0; i < point_cloud.size();i++){
        int current_vertex_count = voxel_points.size();
        Mesh3D::Mesh current_voxel(cube);
        current_voxel.translate(point_cloud[i]);
        for (int j = 0; j < current_voxel.num_vertices();j++){
            voxel_points.push_back(current_voxel.get_vertex(j));
        }
        for (int j=0; j< current_voxel.num_triangles();j++){
            Mesh3D::Triangle current_triangle = current_voxel.get_triangle(j);
            voxel_triangles.push_back(Mesh3D::make_triangle(current_triangle.v0+current_vertex_count,current_triangle.v1+current_vertex_count,current_triangle.v2+current_vertex_count));
        }
    }
    Mesh3D::Mesh voxel_cloud;
    voxel_cloud.set_vertices(voxel_points);
    voxel_cloud.set_triangles(voxel_triangles);

    return voxel_cloud;
}

class VoxelGrid{
public:
    VoxelGrid();
    VoxelGrid(size_t x, size_t y, size_t z):grid_size_x_(x),grid_size_y_(y),grid_size_z_(z){
        data_ = boost::dynamic_bitset<>(x*y*z);
        int d = 0;
        data_[0] = d;

    }
    void set_bit(int val, size_t x_pos, size_t y_pos, size_t z_pos){
        if (x_pos < grid_size_x_ && y_pos < grid_size_y_ && z_pos < grid_size_z_){
            //            std::cout << grid_size_x_ << " " << grid_size_y_ << " " << grid_size_z_ << std::endl;
            int pos = x_pos * grid_size_y_ * grid_size_z_ + y_pos*grid_size_z_ + z_pos;
            data_[pos] = val;
            //            for (size_t i =0; i < data_.size();i++){

            //                std::cout << data_[i] << std::endl;
            //            }
        }
        else std::cout << "Error, index out of boundaries" << std::endl;
    }

    ulong get_data(size_t x_pos, size_t y_pos, size_t z_pos){
        int pos = x_pos * grid_size_y_ * grid_size_z_ + y_pos*grid_size_z_ + z_pos;
        return data_[pos];
    }

private:
    size_t grid_size_x_;
    size_t grid_size_y_;
    size_t grid_size_z_;

    boost::dynamic_bitset<> data_;
};

int main(int argc, char **argv){

    std::string camera_path = "/morpheo-nas/React/TOPROCESS/Calibration/%02d.txt";
    std::string silhouettes_path = "/morpheo-nas/React/TOPROCESS/IWONDER/Silhouettes/%02d.png";
    std::string images_path = "/morpheo-nas/React/TOPROCESS/IWONDER/Images/%02d.png";
    int current_camera = 5;
    int nb_cameras = 12;
    int image_width = 1920;
    int image_heigh = 1080;
    float block_size = 0.01;


    Mesh3D::Mesh cube = point_cloud_to_voxels(std::vector<Mesh3D::float3>(),0.1);


    Vector3 area_min, area_max;
    area_min[0] = 10000; area_min[1] = 10000; area_min[2] = 10000;
    area_max[0] = -10000; area_max[1] = -10000; area_max[2] = -10000;


    std::vector<CamParameters> cameras;

    for (int i =0; i < nb_cameras; i++){
        std::fstream file((boost::format(camera_path)%i).str().c_str(), std::ios::in);
        CamParameters cam(image_width,image_heigh);
        file >> cam;
        cameras.push_back(cam);

        Vector3 cam_center = cam.getCenter();

        if (cam_center[0] > area_max[0]) area_max[0] = cam_center[0];
        if (cam_center[0] < area_min[0]) area_min[0] = cam_center[0];

        if (cam_center[1] > area_max[1]) area_max[1] = cam_center[1];
        if (cam_center[1] < area_min[1]) area_min[1] = cam_center[1];

        if (cam_center[2] > area_max[2]) area_max[2] = cam_center[2];
        if (cam_center[2] < area_min[2]) area_min[2] = cam_center[2];
    }

    std::cout << area_min << " " << area_max << std::endl;



    int z_sampling = int(abs((area_max[2] - area_min[2])/block_size));
    int y_sampling = int(abs((area_max[1] - 0)/block_size));
    int x_sampling = int(abs((area_max[0] - area_min[0])/block_size));
    std::cout << "Number of Voxels " << x_sampling*y_sampling*z_sampling << std::endl;
    VoxelGrid vg(x_sampling,y_sampling,z_sampling);
    std::vector<Mesh3D::float3> visible_points;
    std::vector<Mat> silhouettes;
    std::vector<Mat> images;

    for (int i = 0; i < nb_cameras; i++){
        Mat image = imread((boost::format(silhouettes_path)%i).str(),CV_LOAD_IMAGE_GRAYSCALE);
        silhouettes.push_back(image);
        Mat col_image = imread((boost::format(images_path)%i).str());
        images.push_back(col_image);
    }
    for (int x = 0; x < x_sampling; x++){
        for (int y = 0; y < y_sampling; y++){
            for (int z = 0; z < z_sampling; z++){

                Vector3 current_voxel= Vector3(area_min[0],0,area_min[2]) + Vector3(x*block_size,y*block_size,z*block_size);

                int count = 0;
                for (int i = 0 ; i< nb_cameras;i++){
                    //                    std::cout << "Loading Camera " << i << std::endl;
                    //                    Mat image = imread((boost::format(silhouettes_path)%i).str(),CV_LOAD_IMAGE_GRAYSCALE);

                    Matrix3x4 proj_matrix = cameras[i].getProjection();
                    Matrix<4,1> space_vector;
                    space_vector[0][0] = current_voxel[0];
                    space_vector[1][0] = current_voxel[1];
                    space_vector[2][0] = current_voxel[2];
                    space_vector[3][0] = 1;

                    Matrix<3,1> proj = proj_matrix * space_vector;

                    int u = proj[0][0]/proj[2][0];
                    int v = proj[1][0]/proj[2][0];

                    if (u < image_width && u > 0 && v > 0 && v < image_heigh){

                        if (silhouettes[i].at<uchar>(v,u) == 255){
                            count ++;
                        }
                        //                        }
                        //                        if (vg.get_data(x,y,z)==1 && image.at<uchar>(v,u) == 0){
                        //                            vg.set_bit(0,x,y,z);
                        //                            //                            visible_points.push_back(Mesh3D::make_float3(float(space_vector[0][0]), float(space_vector[1][0]),float(space_vector[2][0])));

                        //                        }

                    }
                }
                if (count > 8){
                    //                    std::cout << 'Visible' << std::endl;
                    vg.set_bit(1,x,y,z);
                    //                    visible_points.push_back(Mesh3D::make_float3(current_voxel[0],current_voxel[1],current_voxel[2]));
                }
            }
        }

    }



    std::cout << "Filtering Mesh : all voxels inside of the shape will be deleted" << std::endl;
    std::vector<Mesh3D::Color> voxel_colors;
    for (int x = 0; x < x_sampling; x++){
        for (int y = 0; y < y_sampling; y++){
            for (int z = 0; z < z_sampling; z++){

                if (vg.get_data(x,y,z) == 1 && (!vg.get_data(x+1,y,z) || !vg.get_data(x-1,y,z) || !vg.get_data(x,y+1,z) || !vg.get_data(x,y-1,z) || !vg.get_data(x,y,z-1) || !vg.get_data(x,y,z+1))){
                    Vector3 current_voxel= Vector3(area_min[0],0,area_min[2]) + Vector3(x*block_size,y*block_size,z*block_size);
                    visible_points.push_back(Mesh3D::make_float3(current_voxel[0],current_voxel[1],current_voxel[2]));



                    Matrix<4,1> space_vector;
                    space_vector[0][0] = current_voxel[0];
                    space_vector[1][0] = current_voxel[1];
                    space_vector[2][0] = current_voxel[2];
                    space_vector[3][0] = 1;

                    int r = 0;
                    int g = 0;
                    int b = 0;

                    for (int i=0; i < nb_cameras; i++){
                        std::cout << i << std::endl;
                        Matrix3x4 proj_matrix = cameras[i].getProjection();

                        Matrix<3,1> proj = proj_matrix * space_vector;

                        int u = proj[0][0]/proj[2][0];
                        int v = proj[1][0]/proj[2][0];
                        std::cout << u << " " << v << std::endl;

                        Vec3b color = images[i].at<Vec3b>(v,u);
//                        circle(images[i],Point(u,v),2,0);
//                        images[i].at<uchar>(v,u)= 255;
                        r += int(color.val[2]);
                        g +=  int(color.val[1]);
                        b += int(color.val[0]);
                        std::cout << color << std::endl;
                    }

                    r /= nb_cameras;
                    g /= nb_cameras;
                    b /= nb_cameras;

                    if (b > (g+r)/2) b = (r+g)/2;
                    for (int j = 0; j < 8; j++){
                        voxel_colors.push_back(Mesh3D::Color(r,g,b));
                    }
                }
            }
        }
    }

    //Get the color information for every voxel



    std::cout << "Saving Mesh" << std::endl;
    Mesh3D::Mesh mesh = point_cloud_to_voxels(visible_points,block_size,0.9);
    mesh.set_colors(voxel_colors);
    //    mesh.set_vertices(visible_points);
    mesh.save("/tmp/test.off");
//        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
//        imshow( "Display window", images[0] );                   // Show our image inside it.

//        waitKey(0);                                          // Wait for a keystroke in the window
//        return 0;
}

