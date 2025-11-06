#pragma once

#include <QPointF>
#include <mutex>

#include <unordered_map>
#include "dataset.h"
#include "quadtree.h"



struct Graph {
    Graph() = default;
    Graph(const Dataset* d);

    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, std::vector<int>> edges; //oriented
    std::unordered_map<int, QPointF> nodesPosition;
    
    std::unordered_map<int, float> nodesMass; //degree of the node and it's neighbors

    Quadtree *quadtree;

    void placePointsInCircle();

    std::mutex positionMutex;
private:
    QPointF environmentSize;
};