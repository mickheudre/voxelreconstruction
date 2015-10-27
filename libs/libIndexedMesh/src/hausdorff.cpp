#include "IndexedMesh/hausdorff.h"

double DirectedHausdorffDiscance(const std::vector<float3> &p, const std::vector<float3> &q){
    log(DEBUG) << "Computing Haussdorf Distance" << endLog();
    double hDistance = 0.0;
    std::vector<float3>::const_iterator mesh1_iterator;
    std::vector<float3>::const_iterator mesh2_iterator;
    #pragma omp parallel for shared(hDistance)
    for (int i = 0; i < p.size();i++){
        double min = INT_MAX;

        for (int j = 0; j < q.size(); j++){
        //for (mesh2_iterator = q.begin(); mesh2_iterator != q.end(); mesh2_iterator ++){
            double dist = distance(p[i],q[j]);

            if( dist < min){
                min = dist;
            }
        }

        if (min > hDistance){
            hDistance = min;
        }
    }

    return hDistance;

}

double HausdorffDiscance(const Mesh3D::Mesh &mesh1, const Mesh3D::Mesh &mesh2){

    double d12 = DirectedHausdorffDiscance(mesh1.vertices(),mesh2.vertices());
    double d21 = DirectedHausdorffDiscance(mesh2.vertices(),mesh1.vertices());

    return std::max(d12,d21);
}
