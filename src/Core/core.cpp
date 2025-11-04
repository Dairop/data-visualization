#include "core.h"
#include <cmath>
#include <algorithm>


void Core::setNewDatasetPath(std::filesystem::path path){
    datasetPath = path;

    //load
    //...

    emit datasetLoaded();
}

void Core::generateNewDataset(int nodes, int edges){
    dataset.generateDataset(nodes, edges);

    graph = new Graph(&dataset);

    emit datasetGenerated();
}



void Core::startApplyingForceDirected(){
    std::thread t(&Core::forceDirected, this);
    t.detach();
    //forceDirected();
}


//I am using c++ 17, lerp wasn't in the standard library until c++20
float lerp(float a, float b, float t){
    return a + t * (b - a);
}


float cube(const float t){
    return  (t -1)*(t -1)*-1+1;
}



float dist(const QPointF& p1, const QPointF& p2){
    return std::sqrtf((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}



void Core::forceDirected(){
    const int NB_ITERATIONS = 5000;
    float iterationsProgression = 0.0f;

    const float velocityDamping = 0.95f;


    //evolving simulation parameters
    float softCollisionsRadius = 150.0f;
    float thisIterationOptimalEdgeLen = 150.0f;
    float this_iteration_force;
     

    //we are using this map for displacement to apply all of them at once
    //also helps with the calculations as the nodes are not moving at eatch edge checked
    std::unordered_map<int, QPointF> displacements;
    std::unordered_map<int, QPointF> velocity;
    for (auto &[id, _] : graph->nodesPosition){
        displacements[id] = QPointF(0, 0);
        velocity[id] = QPointF(0, 0);
    }


    for (unsigned int it = 0; it < NB_ITERATIONS; it++){
        iterationsProgression = (float) it / (float) NB_ITERATIONS;

        this_iteration_force = 0.1f + (1.0f - cube(iterationsProgression)) / 100.0f;
        softCollisionsRadius = lerp(10.0f, 400.0f, cube(iterationsProgression));
        
        thisIterationOptimalEdgeLen = 10.0f + softCollisionsRadius / 2.5f;

        //for every edge, pull toward the startId node
        for (const auto &[startId, edgesFromStart] : graph->edges){
            for (const int endId : edgesFromStart){
                if (startId == endId) continue;

                QPointF start_pos = graph->nodesPosition[startId];
                QPointF end_pos = graph->nodesPosition[endId];

                float d = dist(start_pos, end_pos);

                //move one if the nodes are too close, prevents potential edge cases
                if (d < 0.01f){
                    float randomOffset1 = (float) ((startId * 7057 + it * 3527) % 51 - 24) * 0.1f;
                    float randomOffset2 = (float) ((endId * 4241 - startId * 173) % 51 - 24) * 0.1f;
                    end_pos += QPointF(randomOffset1, randomOffset2);

                    d = dist(start_pos, end_pos);
                    if (d < 0.01f) continue;
                }

                QPointF deltaPos = end_pos - start_pos;
                QPointF normDir = deltaPos / d;

                float thisNodeNorm = edgesFromStart.size();

                float desiredLength = thisIterationOptimalEdgeLen * (1.0f + thisNodeNorm / 2.0f);
                float k = 0.08f;
                float appliedForce = k * (d - desiredLength);

                //we want to reduce the force of a spring depending on the norm of it's nodes
                //to avoid the hairball effect
                //see https://www.researchgate.net/figure/The-Hairball-Effect-Ontology-visualization-using-a-2D-Force-Directed-layout-showing_fig3_272489007
                float degNorm = 1.0f;//std::sqrtf((thisNodeNorm + 1) * (graph->edges[endId].size() + 1));
                appliedForce /= degNorm;

                //to be applied later
                displacements[startId] += normDir * appliedForce;
                displacements[endId] -= normDir * appliedForce;
            }
        }



        for (auto &[id, disp] : displacements) {
            //limit max acceleration to keep the simulation stable
            float len = std::sqrt(disp.x()*disp.x() + disp.y()*disp.y());

            if (len > 6.0f)
                disp *= (6.0f / len);

            float mass = graph->nodesMass[id];

            velocity[id] = velocity[id] * velocityDamping + disp / mass / mass;
            graph->nodesPosition[id] += velocity[id];

            disp.setX(0.0f);
            disp.setY(0.0f);
        }




        /*
        //move away from center if close
        const QPointF graphCenter = graph->quadtree->getQuadCenterPos();
        const float min_size = std::fmin(graph->quadtree->getQuadSize().x(), graph->quadtree->getQuadSize().y());
        const float middle_displacement_radius = std::fmin(min_size * 0.25f, std::sqrt(graph->nodesNames.size()) * 300.0f);

        for (const auto&[id, _] : dataset.nodesNames){
            //move away from the center
            QPointF start_pos = graph->nodesPosition[id];

            float centerDist = dist(start_pos, graphCenter);
            if (centerDist < middle_displacement_radius){
                float displacementForce = middle_displacement_radius / (centerDist + 1.0f); 
                displacementForce *= 0.03f;
                displacementForce *= displacementForce;
                displacementForce = fmin(0.004f, displacementForce);

                

                float newX = lerp(0.0f, start_pos.x(), 1.0f + displacementForce);
                float newY = lerp(0.0f, start_pos.y(), 1.0f + displacementForce);

                graph->nodesPosition[id] = QPointF(newX, newY);
            }
        }*/






        if (it % 25) for (unsigned int i = 0; i < graph->nodesNames.size() / 100; i++) collisions(softCollisionsRadius, 0.5f);
    }

    
    for (unsigned int i = 0; i < 20; i++) collisions(150.0f, 1.0f);

}




void Core::collisions(float radius, float force){
    float queryRadiusNorm = radius; // used when we have rect queries

    graph->quadtree->del();

    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->insert(pos, id);        
    }


    std::vector<int> pointsInRange;
    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->queryRangeCircle(pos, radius, pointsInRange);    
        

        //eloigner les points si trop proches
        for (unsigned int i = 0; i < pointsInRange.size(); i++){
            if (id == pointsInRange[i]) continue;

            QPointF start_pos = graph->nodesPosition[id];
            QPointF end_pos = graph->nodesPosition[pointsInRange[i]];

            float d = dist(start_pos, end_pos);

            //permet d'eviter les divisions par 0
            if (d < 0.001f){
                float randomOffset1 = (float) ((i * 7057 + id * 3527) % 51 - 24) * 0.1f;
                float randomOffset2 = (float) ((i * 4241 - id * 173) % 51 - 24) * 0.1f;
                end_pos += QPointF(randomOffset1, randomOffset2);

                d = dist(start_pos, end_pos);
                if (d < 0.001f) continue;
            }

            float dx = start_pos.x() - end_pos.x();
            float dy = start_pos.y() - end_pos.y();
            float normX = dx / d;
            float normY = dy / d;

            float overlapRatio = (queryRadiusNorm - d) / queryRadiusNorm;

            //applied to each node in opposition
            float displacementX = normX * overlapRatio * force;
            float displacementY = normY * overlapRatio * force;

            QPointF new_start_pos = QPointF(start_pos.x() + displacementX * 0.5f, start_pos.y() + displacementY * 0.5f);
            QPointF new_end_pos = QPointF(end_pos.x() - displacementX * 0.5f, end_pos.y() - displacementY * 0.5f);

            graph->nodesPosition[id] = new_start_pos;
            graph->nodesPosition[pointsInRange[i]] = new_end_pos;
        }


        
        pointsInRange.clear();
    }
}