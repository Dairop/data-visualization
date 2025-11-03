#include "graph.h"

Graph::Graph(const Dataset* d){
    this->nodesNames = d->nodesNames;
    this->edges = d->edges;

    placePointsInCircle();
}



void Graph::placePointsInCircle() {
    const int n_nodes = this->nodesNames.size();

    nodesPosition.clear();

    float TWO_PI = 3.141592f * 2.f;
    int i = 0;
    float angle = 0.0f;
    float radius = std::sqrt(n_nodes) * 200.f;
    for (auto & [start, _] : nodesNames){
        angle = (float) i / (float) n_nodes * TWO_PI;
        QPointF pos(cosf(angle) * radius, sinf(angle) * radius);
        nodesPosition.insert(std::pair<int, QPointF>(start, pos));
        
        i++;
    }
}