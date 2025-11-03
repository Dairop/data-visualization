#pragma once

#include <QPointF>

#include <unordered_map>
#include "dataset.h"
#include "quadtree.h"



struct Graph {
    Graph() = default;
    Graph(const Dataset* d);

    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, int> edges; //oriented
    std::unordered_map<int, QPointF> nodesPosition;

    Quadtree *quadtree;

    void placePointsInCircle();

private:
    QPointF environmentSize;
};