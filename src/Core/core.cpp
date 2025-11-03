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
    return std::sqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}



void Core::forceDirected(){
    const int NB_ITERATIONS = 100;

    float this_iteration_force = 0.5f;

    for (unsigned int it = 0; it < NB_ITERATIONS; it++){
        this_iteration_force = (1.0f - (float) it / (float) NB_ITERATIONS) / 20.0f;

        //for every edge, pull toward the start node
        for (const auto &[start, end] : graph->edges){
            QPointF start_pos = graph->nodesPosition[start];
            QPointF end_pos = graph->nodesPosition[end];

            //positif si les noeuds sont eloignes, negatif si les noeuds sont trop proches
            float direction = (dist(start_pos, end_pos) > 150.0f ? 1.0f : -1.0f);

            float newSX = lerp(start_pos.x(), end_pos.x(), this_iteration_force * direction);
            float newSY = lerp(start_pos.y(), end_pos.y(), this_iteration_force * direction);
            float newEX = lerp(start_pos.x(), end_pos.x(), 1.0f - this_iteration_force * direction);
            float newEY = lerp(start_pos.y(), end_pos.y(), 1.0f - this_iteration_force * direction);

            QPointF new_start_pos = QPointF(newSX, newSY);
            QPointF new_end_pos = QPointF(newEX, newEY);

            graph->nodesPosition[start] = new_start_pos;
            graph->nodesPosition[end] = new_end_pos;
        }
    }


    collisions();
    collisions();
    collisions();
}




void Core::collisions(){
    QPointF radius(100.0f, 100.0f);
    float queryRadiusNorm = dist(QPointF(0, 0), radius);

    graph->quadtree->del();

    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->insert(pos, id);        
    }

    float force = 1.0f;

    std::vector<int> pointsInRange;
    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->queryRangeRect(pos, radius, pointsInRange);    
        

        //eloigner les points si trop proches
        for (unsigned int i = 0; i < pointsInRange.size(); i++){
            if (id == pointsInRange[i]) continue;

            QPointF start_pos = graph->nodesPosition[id];
            QPointF end_pos = graph->nodesPosition[pointsInRange[i]];

            float d = dist(start_pos, end_pos);

            //permet d'eviter les divisions par 0
            if (d < 0.001f){
                int randomOffset1 = (i + id) % 50 - 24;
                int randomOffset2 = (i - id) % 50 - 24;
                end_pos += QPointF(randomOffset1, randomOffset2);

                d = dist(start_pos, end_pos);
            }

            float dx = start_pos.x() - end_pos.x();
            float dy = start_pos.y() - end_pos.y();
            float normX = dx / d;
            float normY = dx / d;

            float overlapRatio = (queryRadiusNorm - d) / queryRadiusNorm;

            //applied to each node in opposition
            float displacementX = normX * overlapRatio * force * d;
            float displacementY = normY * overlapRatio * force * d;

            QPointF new_start_pos = QPointF(start_pos.x() + displacementX * 0.5f, start_pos.y() + displacementY * 0.5f);
            QPointF new_end_pos = QPointF(end_pos.x() - displacementX * 0.5f, end_pos.y() - displacementY * 0.5f);

            graph->nodesPosition[id] = new_start_pos;
            graph->nodesPosition[pointsInRange[i]] = new_end_pos;
        }


        
        pointsInRange.clear();
    }
}