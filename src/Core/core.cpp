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
    const int NB_ITERATIONS = 10;

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
}