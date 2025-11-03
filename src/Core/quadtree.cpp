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
    northWest = new Quadtree(quadCenterPosition - quadSize / 4.0f, quadSize/2.0f, maxDepth - 1);
    
    int new_x = quadCenterPosition.x() + quadSize.x() / 4.0f;
    int new_y = quadCenterPosition.y() - quadSize.y() / 4.0f;
    northEast = new Quadtree(QPointF(new_x, new_y), quadSize/2.0f, maxDepth - 1);

    southEast = new Quadtree(quadCenterPosition + quadSize / 4.0f, quadSize/2.0f, maxDepth - 1);

    new_x = quadCenterPosition.x() - quadSize.x() / 4.0f;
    new_y = quadCenterPosition.y() + quadSize.y() / 4.0f;
    southWest = new Quadtree(QPointF(new_x, new_y), quadSize/2.0f, maxDepth - 1);
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


//bool queryRangeCircle(QPointF& center, QPointF& size, std::vector<int>& pointsInRange);






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