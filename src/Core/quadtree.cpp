#include "quadtree.h"



float distSqrd(const QPointF& pos1, const QPointF& pos2) {
    return (pos2.x() - pos1.x()) * (pos2.x() - pos1.x()) + (pos2.y() - pos1.y()) * (pos2.y() - pos1.y());
}


//border included
bool isPointInsideRect(const QPointF& point, const QPointF& rectCenter, const QPointF& rectSize){
    return std::abs(point.x() - rectCenter.x()) <= rectSize.x()/2.0f
        && std::abs(point.y() - rectCenter.y()) <= rectSize.y()/2.0f;
}


//rects defined by their center
bool collideRectAndRect(const QPointF& pos1, const QPointF& size1, const QPointF& pos2, const QPointF& size2){
    bool colX = std::abs((pos2.x() - pos1.x())) <= std::abs(size2.x()/2.0f + size1.x()/2.0f);
    bool colY = std::abs((pos2.y() - pos1.y())) <= std::abs(size2.y()/2.0f + size1.y()/2.0f);

    return colX && colY;
}



bool Quadtree::insert(QPointF p, int id){
    if (!isPointInsideRect(p, this->quadCenterPosition, this->quadSize)){
        return false;
    }
        
    if (data.size() < capacity){
        data.insert(std::pair<int, QPointF>(id, p));

        return true;
    }

    if (northWest == nullptr){
        subdivide();
    }

    if (northWest->insert(p, id)) return true;
    if (northEast->insert(p, id)) return true;
    if (southEast->insert(p, id)) return true;
    if (southWest->insert(p, id)) return true;


    return false;
}



void Quadtree::subdivide(){
    const QPointF childSize = quadSize / 2.0f;    
    const QPointF quarterSize = quadSize / 4.0f;        

    QPointF nwCenter = quadCenterPosition + QPointF(-quarterSize.x(), -quarterSize.y());
    northWest = new Quadtree(nwCenter, childSize, maxDepth - 1);

    QPointF neCenter = quadCenterPosition + QPointF(+quarterSize.x(), -quarterSize.y());
    northEast = new Quadtree(neCenter, childSize, maxDepth - 1);

    QPointF seCenter = quadCenterPosition + QPointF(+quarterSize.x(), +quarterSize.y());
    southEast = new Quadtree(seCenter, childSize, maxDepth - 1);

    QPointF swCenter = quadCenterPosition + QPointF(-quarterSize.x(), +quarterSize.y());
    southWest = new Quadtree(swCenter, childSize, maxDepth - 1);
}




bool Quadtree::queryRangeRect(const QPointF& center, const  QPointF& size, std::vector<int>& pointsInRange){
    if (collideRectAndRect(quadCenterPosition, quadSize, center, size)){
        for (const auto&[id, dataPos]: data){
            if (isPointInsideRect(dataPos, center, size)){
                pointsInRange.push_back(id);
            }
        }

        if (northWest != nullptr){
            northWest->queryRangeRect(center, size, pointsInRange);
            northEast->queryRangeRect(center, size, pointsInRange);
            southEast->queryRangeRect(center, size, pointsInRange);
            southWest->queryRangeRect(center, size, pointsInRange);
        }
    }

    return true;
}


bool Quadtree::queryRangeCircle(const QPointF& center, const float radius, std::vector<int>& pointsInRange){
    QPointF circleQuadSize = QPointF(radius * 2.0f, radius * 2.0f);

    if (collideRectAndRect(quadCenterPosition, quadSize, center, circleQuadSize)){
        for (const auto&[id, dataPos]: data){
            if (distSqrd(dataPos, center) <= radius*radius){
                pointsInRange.push_back(id);
            }
        }

        if (northWest != nullptr){
            northWest->queryRangeCircle(center, radius, pointsInRange);
            northEast->queryRangeCircle(center, radius, pointsInRange);
            southEast->queryRangeCircle(center, radius, pointsInRange);
            southWest->queryRangeCircle(center, radius, pointsInRange);
        }
    }

    return true;
}






void Quadtree::del() {
    data.clear();

    if (northWest != nullptr) {
        northWest->del();
        northEast->del();
        southEast->del();
        southWest->del();
    }

    delete northWest;
    delete northEast;
    delete southEast;
    delete southWest;

    northWest = nullptr;
    northEast = nullptr;
    southEast = nullptr;
    southWest = nullptr;

    return;
}