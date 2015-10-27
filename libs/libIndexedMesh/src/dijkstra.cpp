#include "IndexedMesh/dijkstra.h"
#include <iostream>

namespace Mesh3D{


float Dijkstra(std::vector<int> &adjacent_vertices,
                          std::vector<float> &distance,
                          std::vector<int> &boundaries,
                          std::vector<int> &target_path,
                          int start,
                          int end)
{
    //Initialisation.
    int nb_vertices = boundaries.size() -1; //The number of vertices in the graph.

    std::vector<float> distances(nb_vertices,10000); //The vector that holds the distance between the starting point and each vertex in tyhe graph.
    std::vector<int> previous_vertices(nb_vertices,-1);  //The vector that holds the previous vertex in the shortest path.
    std::set<int> vertices; //The set of all the vertices that haven't been visited.
    
    //    std::cout << "Vertices ";
    for(int i = 0; i < boundaries.size()-1;i++){
        //        std::cout << i << " ";
        vertices.insert(i);
    }
    //    std::cout << std::endl<<std::endl;

    distances[start] = 0;

    //Main Loop.
    while(!vertices.empty()){
        //        std::cout << vertices.size() << " vertices remaining" << std::endl;
        //Get vertex in vertices with the smallest distance in distances.
        float min = 10000;
        int min_id = -1;

        std::set<int>::iterator it = vertices.begin();

        while(it != vertices.end()){

            //Get the vertex in vertices with the smallest distance.
            if (distances[*it] < min){
                min = distances[*it];
                min_id = *it;
            }
            it ++;
        }

        if (min_id == -1) break;
        if (distances[min_id] == 10000) break;
        //        std::cout << "Vertex " << min_id << std::endl;

        vertices.erase(min_id);

        //For each neighbours of min_id
        for (int i = boundaries[min_id]; i < boundaries[min_id+1]; i++ ){

            //            std::cout << "Neighbour " << adjacent_vertices[i] << std::endl;

            if (vertices.find(adjacent_vertices[i]) != vertices.end()){
                float dist = distances[min_id] + distance[i];
                if (adjacent_vertices[i] == end){
                    //                    std::cout << "Finished" << std::endl;
                    previous_vertices[adjacent_vertices[i]] = min_id;
                    std::vector<int> path;

                    path.push_back(end);
                    int id = min_id;

                    while(id != start){
                        path.push_back(id);
                        id = previous_vertices[id];
                    }

                    target_path =  path;
                    return dist;
                }

                //                std::cout << "Dist "  << dist << std::endl;
                if (dist < distances[adjacent_vertices[i]]){
                    distances[adjacent_vertices[i]] = dist;
                    previous_vertices[adjacent_vertices[i]] = min_id;
                }
            }
        }
        //        std::cout << std::endl;
    }

}

}
