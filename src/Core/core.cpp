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



float dist(const QPointF& p1, const QPointF& p2){
    return std::sqrtf((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}



void Core::forceDirected(){
    const int NB_ITERATIONS = 1000;

    float this_iteration_force;

    for (unsigned int it = 0; it < NB_ITERATIONS; it++){
        this_iteration_force = 0.1f + (1.0f - (float) it / (float) NB_ITERATIONS) / 10.0f;

        //for every edge, pull toward the start node
        for (const auto &[start, end] : graph->edges){
            if (start == end) continue;

            QPointF start_pos = graph->nodesPosition[start];
            QPointF end_pos = graph->nodesPosition[end];

            float d = dist(start_pos, end_pos);

            //move one if the nodes are too close, prevents potential edge cases
            if (d < 0.01f){
                float randomOffset1 = (float) ((start * 7057 + it * 3527) % 51 - 24) * 0.1f;
                float randomOffset2 = (float) ((end * 4241 - start * 173) % 51 - 24) * 0.1f;
                end_pos += QPointF(randomOffset1, randomOffset2);

                d = dist(start_pos, end_pos);
                if (d < 0.01f) continue;
            }


            //positive if nodes are far from each other, negative if too close
            float direction = (d > 200.0f ? 1.0f : -1.0f);

            //reduce pulling force if already close, helps to have longer branches when needed
            float distance_force = (d > 400.0f) ? 1.0f : lerp(1.0f, 0.2f, d/400.0f);
            float this_node_force = this_iteration_force;
            if (direction > 0.0f) {
                this_node_force *= distance_force;
            }

            //calculate and apply new position
            float newSX = lerp(start_pos.x(), end_pos.x(), this_iteration_force * direction);
            float newSY = lerp(start_pos.y(), end_pos.y(), this_iteration_force * direction);
            float newEX = lerp(start_pos.x(), end_pos.x(), 1.0f - this_iteration_force * direction);
            float newEY = lerp(start_pos.y(), end_pos.y(), 1.0f - this_iteration_force * direction);

            QPointF new_start_pos = QPointF(newSX, newSY);
            QPointF new_end_pos = QPointF(newEX, newEY);

            graph->nodesPosition[start] = new_start_pos;
            graph->nodesPosition[end] = new_end_pos;
        }



        //move away from center if close
        const QPointF graphCenter = graph->quadtree->getQuadCenterPos();
        const float min_size = std::fmin(graph->quadtree->getQuadSize().x(), graph->quadtree->getQuadSize().y());
        const float middle_displacement_radius = std::fmin(min_size * 0.5f, std::sqrt(graph->nodesNames.size()) * 300.0f);

        for (const auto&[id, _] : dataset.nodesNames){
            //move away from the center
            QPointF start_pos = graph->nodesPosition[id];

            float centerDist = dist(start_pos, graphCenter);
            if (centerDist < middle_displacement_radius){
                float displacementForce = middle_displacement_radius / (centerDist + 1.0f); 
                displacementForce *= 0.01f;
                displacementForce *= displacementForce;
                displacementForce = fmin(0.004f, displacementForce);

                

                float newX = lerp(0.0f, start_pos.x(), 1.0f + displacementForce);
                float newY = lerp(0.0f, start_pos.y(), 1.0f + displacementForce);

                graph->nodesPosition[id] = QPointF(newX, newY);
            }
        }






        if (it % 5) for (unsigned int i = 0; i < 10; i++) collisions();
    }

    
    for (unsigned int i = 0; i < 20; i++) collisions();

}




void Core::collisions(){
    float radius = 150.0f;
    float queryRadiusNorm = radius; // used when we have rect queries

    graph->quadtree->del();

    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->insert(pos, id);        
    }

    float force = 1.0f;

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