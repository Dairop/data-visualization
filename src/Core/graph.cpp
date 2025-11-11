#include "graph.h"

Graph::Graph(const Dataset* d){
    this->nodesNames = d->nodesNames;
    this->edges = d->edges;

    float r = std::sqrt(d->nodesNames.size()) * 200.f;
    environmentSize = QPointF(r * 2.5f, r * 2.5f);

    quadtree = new Quadtree(QPointF(0.0f, 0.0f), environmentSize);

    placePointsInCircle();

    for (const auto& [idStart, vectEnds]: edges) {
        nodesMass[idStart] += edges[idStart].size(); //node

        /*
        for (unsigned int i = 0; i < vectEnds.size(); i++){
            nodesMass[vectEnds[i]] += 1;    //neighbors
        }*/
    }

    for (auto& [id, _] : nodesNames)
        nodesMass[id] = 1.0f + std::sqrtf(nodesMass[id]);
}



void Graph::placePointsInCircle() {
    const int n_nodes = this->nodesNames.size();

    nodesPosition.clear();

    float TWO_PI = 3.141592f * 2.f;
    int i = 0;
    float angle = 0.0f;
    float radius = std::sqrt(n_nodes) * 200.f; // need to be careful with environmentSize

    for (auto & [start, _] : nodesNames){
        angle = (float) i / (float) n_nodes * TWO_PI;
        QPointF pos(cosf(angle) * radius, sinf(angle) * radius);
        nodesPosition.insert(std::pair<int, QPointF>(start, pos));
        
        i++;
    }

    if (quadtree == nullptr) quadtree = new Quadtree(QPointF(0.0f, 0.0f), environmentSize);
    quadtree->del();

    for (const auto &[id, pos] : nodesPosition){
        quadtree->insert(pos, id);        
    }
}