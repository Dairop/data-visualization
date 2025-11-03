#pragma once

#include <unordered_map>
#include <QPointF>


class Quadtree {
    const int capacity = 15;

    int maxDepth;

    QPointF quadCenterPosition;
    QPointF quadSize;

    std::unordered_map<int, QPointF> data;

    Quadtree* northWest;
    Quadtree* southWest;
    Quadtree* northEast;
    Quadtree* southEast;

public:
    Quadtree(const QPointF& position, const QPointF& size, const int _maxDepth = 10) : 
        maxDepth(_maxDepth), quadCenterPosition(position), quadSize(size) {
        northWest = nullptr;
        southWest = nullptr;
        northEast = nullptr;
        southEast = nullptr;

        data.clear();
    };


    bool insert(QPointF p, int id);
    bool queryRangeRect(const QPointF& center, const QPointF& size, std::vector<int>& pointsInRange);
    //bool queryRangeCircle(const QPointF& center, const QPointF& size, std::vector<int>& pointsInRange);

    void del();

private:
    void subdivide();
    
};