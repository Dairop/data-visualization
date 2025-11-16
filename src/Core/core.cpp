#include "core.h"
#include <cmath>
#include <algorithm>


Core::Core(){
    datasetPath = "";
    graph = nullptr;

    connect(this, &Core::finishedApplyingForceDirected, this, &Core::onFinishedApplyingForceDirected);

}

Core::~Core() {
    stopApplyingForceDirected();
}

void Core::setNewDatasetPath(std::filesystem::path path){
    datasetPath = path;

    dataset.loadFromFile(path);
    graph = new Graph(&dataset);

    emit datasetLoaded();
}

void Core::generateNewDataset(int nodes, int edges){
    dataset.generateDataset(nodes, edges);

    graph = new Graph(&dataset);

    emit datasetGenerated();
}



void Core::joinWorkerIfNeeded() {
    if (forceDirectedThread.joinable()) {
        forceDirectedThread.join();
    }
    
    threadForceDirectedRunning.store(false);
}


void Core::startApplyingForceDirected(){
    if (forceDirectedThread.joinable()) {
        if (!threadForceDirectedRunning.load()) {
            forceDirectedThread.join();
        } else {
            return;
        }
    }

    threadsMustStop.store(false);
    forceDirectedThread = std::thread(&Core::forceDirected, this);
    threadForceDirectedRunning.store(true);
}


//I am using c++ 17, lerp wasn't in the standard library until c++20
float lerp(float a, float b, float t){
    return a + t * (b - a);
}


//for 0.0 <= t <= 1.0
//fast growth from t = 0.0 to 0.25, slow from t=0.25 to 0.5,   at t >= 0.5, same as when t = t - 0.45
float twoPullsProgression(float t){
    if (t > 0.5f) t -= 0.45f;
    return  (t - 1.0f)*(t - 1.0f)*-2.0f+2.0f;
}



float dist(const QPointF& p1, const QPointF& p2){
    return std::sqrtf((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}


void Core::keepNodesInsideQuadtree(){
    float quad_center_x = graph->quadtree->getQuadCenterPos().x();
    float quad_center_y = graph->quadtree->getQuadCenterPos().y();

    float quad_min_x = quad_center_x - graph->quadtree->getQuadSize().x() / 2.0f;
    float quad_min_y = quad_center_y - graph->quadtree->getQuadCenterPos().y() / 2.0f;
    
    float quad_max_x = quad_center_x + graph->quadtree->getQuadSize().x() / 2.0f;
    float quad_max_y = quad_center_y + graph->quadtree->getQuadCenterPos().y() / 2.0f;

    graph->positionMutex.lock();
    for (auto&[id, pos]: graph->nodesPosition){
        if (pos.x() < quad_min_x) pos.setX(quad_min_x);
        else if (pos.x() > quad_max_x) pos.setX(quad_max_x);

        if (pos.y() < quad_min_y) pos.setY(quad_min_y);
        else if (pos.y() > quad_max_y) pos.setY(quad_max_y);
    }
    graph->positionMutex.unlock();

}



void Core::stopApplyingForceDirected() {
    threadsMustStop.store(true);

    if (forceDirectedThread.joinable()) {
        forceDirectedThread.join();
    }
}


bool Core::checkForceDirectedEnd() {
    if (threadsMustStop.load()) {
        emit finishedApplyingForceDirected();
        threadsMustStop.store(false);

        threadForceDirectedRunning.store(false);

        return true;
    }
    return false;
}









void Core::forceDirected(){
    const int NB_ITERATIONS = 3000;
    float iterationsProgression = 0.0f;

    //frames between two collisions checks
    int timeBetweenTwoCollisionChecks = 15;
    if (graph->nodesNames.size() > 1000) timeBetweenTwoCollisionChecks = graph->nodesNames.size() / 25;

    const float velocityDamping = 0.95f;


    //evolving simulation parameters
    float softCollisionsRadius = 400.0f;
    float thisIterationOptimalEdgeLen = 150.0f;
    float this_iteration_force;
     

    //we are using this map for displacement to apply all of them at once
    //also helps with the calculations as the nodes are not moving at eatch edge checked
    std::unordered_map<int, QPointF> displacements;
    std::unordered_map<int, QPointF> velocity;
    for (auto &[id, _] : graph->nodesPosition){
        displacements[id] = QPointF(0, 0);
        velocity[id] = QPointF(0, 0);
    }


    for (unsigned int it = 0; it < NB_ITERATIONS; it++){
        if (checkForceDirectedEnd()) return;

        iterationsProgression = (float) it / (float) NB_ITERATIONS;

        this_iteration_force = 0.1f + (1.0f - twoPullsProgression(iterationsProgression)) / 100.0f;
        softCollisionsRadius = lerp(10.0f, 1000.0f, twoPullsProgression(iterationsProgression));
        
        thisIterationOptimalEdgeLen = 100.0f + 50.0f * twoPullsProgression(iterationsProgression);

        //for every edge, pull toward the startId node
        for (const auto &[startId, edgesFromStart] : graph->edges){
            for (const int endId : edgesFromStart){
                if (startId == endId) continue;

                QPointF start_pos = graph->nodesPosition[startId];
                QPointF end_pos = graph->nodesPosition[endId];

                float d = dist(start_pos, end_pos);

                //move one if the nodes are too close, prevents potential edge cases
                if (d < 0.01f){
                    float randomOffset1 = (float) ((startId * 7057 + it * 3527) % 51 - 24) * 0.1f;
                    float randomOffset2 = (float) ((endId * 4241 - startId * 173) % 51 - 24) * 0.1f;
                    end_pos += QPointF(randomOffset1, randomOffset2);

                    d = dist(start_pos, end_pos);
                    if (d < 0.01f) continue;
                }

                QPointF deltaPos = end_pos - start_pos;
                QPointF normDir = deltaPos / d;

                float thisNodeNorm = edgesFromStart.size();

                float desiredLength = thisIterationOptimalEdgeLen * (1.0f + thisNodeNorm / 2.0f);
                float k = 0.07f;
                float appliedForce = k * (d - desiredLength);

                //following is deprecated, mass now used in displacement instead
                //- we want to reduce the force of a spring depending on the norm of it's nodes
                //- to avoid the hairball effect
                //- see https://www.researchgate.net/figure/The-Hairball-Effect-Ontology-visualization-using-a-2D-Force-Directed-layout-showing_fig3_272489007
                float degNorm = 1.0f;//std::sqrtf((thisNodeNorm + 1) * (graph->edges[endId].size() + 1));
                appliedForce /= degNorm;

                //to be applied later
                displacements[startId] += normDir * appliedForce;
                displacements[endId] -= normDir * appliedForce;
            }
        }



        for (auto &[id, disp] : displacements) {
            //limit max acceleration to keep the simulation stable
            float len = std::sqrt(disp.x()*disp.x() + disp.y()*disp.y());

            if (len > 6.0f)
                disp *= (6.0f / len);

            float mass = graph->nodesMass[id];

            velocity[id] = velocity[id] * velocityDamping + disp / mass;

            float vel = dist(velocity[id], QPointF(0, 0));
            if (vel > 50.0f)
                velocity[id] *= 50.0f / vel;

            disp.setX(0.0f);
            disp.setY(0.0f);

            graph->positionMutex.lock();
            graph->nodesPosition[id] += velocity[id];
            graph->positionMutex.unlock();
        }




        /*
        //move away from center if close
        const QPointF graphCenter = graph->quadtree->getQuadCenterPos();
        const float min_size = std::fmin(graph->quadtree->getQuadSize().x(), graph->quadtree->getQuadSize().y());
        const float middle_displacement_radius = std::fmin(min_size * 0.25f, std::sqrt(graph->nodesNames.size()) * 300.0f);

        for (const auto&[id, _] : dataset.nodesNames){
            //move away from the center
            QPointF start_pos = graph->nodesPosition[id];

            float centerDist = dist(start_pos, graphCenter);
            if (centerDist < middle_displacement_radius){
                float displacementForce = middle_displacement_radius / (centerDist + 1.0f); 
                displacementForce *= 0.03f;
                displacementForce *= displacementForce;
                displacementForce = fmin(0.004f, displacementForce);

                

                float newX = lerp(0.0f, start_pos.x(), 1.0f + displacementForce);
                float newY = lerp(0.0f, start_pos.y(), 1.0f + displacementForce);

                graph->nodesPosition[id] = QPointF(newX, newY);
            }
        }*/





        
        if ((it % timeBetweenTwoCollisionChecks) == 0) {
            for (unsigned int i = 0; i < graph->nodesNames.size() / 50; i++) {
                if (checkForceDirectedEnd()) return;

                collisions(softCollisionsRadius, 0.3f);
            }

            for (unsigned int i = 0; i < graph->nodesNames.size() / 50; i++) {
                if (checkForceDirectedEnd()) return;

                collisions(softCollisionsRadius*0.5f, 0.5f);
            }
        }

        emit positionsUpdated(it, NB_ITERATIONS);
    }

    
    for (unsigned int i = 0; i < 20; i++) {
        if (checkForceDirectedEnd()) return;

        collisions(150.0f, 1.0f);
    }
    
    emit positionsUpdated(NB_ITERATIONS, NB_ITERATIONS);
    emit finishedApplyingForceDirected();
}





void Core::collisionsOneThread(float radius, float force){
    float queryRadiusNorm = radius; // useful when we have rect queries, not used here

    graph->quadtree->del();

    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->insert(pos, id);        
    }


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








void collisionsThread(int threadId, int startIndex, int endIndex, 
                    Graph* graph,
                    float radius, 
                    float force,
                    const std::vector<int>& nodesIds,
                    std::unordered_map<int, QPointF>& displacements){



    std::vector<int> nodesInRange;
    for (unsigned int iteratorNodesIds = startIndex; iteratorNodesIds < endIndex; iteratorNodesIds++){
        if (iteratorNodesIds > nodesIds.size()) return;

        int nodeId = nodesIds[iteratorNodesIds];
        QPointF nodePos = graph->nodesPosition[nodeId];


        graph->quadtree->queryRangeCircle(nodePos, radius, nodesInRange);    
        


        //if too close, move the nodes
        for (unsigned int i = 0; i < nodesInRange.size(); i++){
            if (nodeId >= nodesInRange[i]) continue;

            QPointF start_pos = nodePos;
            QPointF end_pos = graph->nodesPosition[nodesInRange[i]];

            float d = dist(start_pos, end_pos);

            //avoid dividing by 0
            if (d < 0.001f){
                float randomOffset1 = (float) ((i * 7057 + nodeId * 3527) % 51 - 24) * 0.1f;
                float randomOffset2 = (float) ((i * 4241 - nodeId * 173) % 51 - 24) * 0.1f;
                end_pos += QPointF(randomOffset1, randomOffset2);

                d = dist(start_pos, end_pos);
                if (d < 0.001f) continue;
            }


            float dx = start_pos.x() - end_pos.x();
            float dy = start_pos.y() - end_pos.y();
            float normX = dx / d;
            float normY = dy / d;

            float overlapRatio = (radius - d) / radius;

            //applied to each node in opposition
            float displacementX = normX * overlapRatio * force;
            float displacementY = normY * overlapRatio * force;

            displacements[nodeId] += QPointF(displacementX * 0.5f, displacementY * 0.5f);
            displacements[nodesInRange[i]] += QPointF(displacementX * -0.5f, displacementY * -0.5f);
        }

        nodesInRange.clear();
    }
}



void Core::collisions(float radius, float force){
    const int nThreads = std::thread::hardware_concurrency() - 2; //minus this and ui
    if (nThreads <= 0) {
        collisionsOneThread(radius, force);
        return;
    }
    
    //displacement of nodes for every thread, values are added at the end if 
    //they correspond to the same node
    std::vector<std::unordered_map<int, QPointF>> localDisplacements(nThreads);
    
    std::vector<std::thread> collisionsThreads;
    std::vector<int> nodesIds;

    for (const auto &[id, _] : graph->nodesPosition)
        nodesIds.push_back(id);


    graph->quadtree->del();

    for (const auto &[id, pos] : graph->nodesPosition){
        graph->quadtree->insert(pos, id);        
    }


    const int numberOfNodes = nodesIds.size();
    const int nodesPerThread = (numberOfNodes + nThreads - 1) / nThreads;


    for (int t = 0; t < nThreads; ++t) {
        //nodes that the next thread will have to work on
        int start = t * nodesPerThread;
        int end = std::min(start + nodesPerThread, numberOfNodes);

        if (start >= numberOfNodes) break;


        /*  Parameters:
            int threadId, int startIndex, int endIndex, 
            Graph* graph,
            float radius, 
            float force,
            const std::vector<int>& nodesIds,
            std::unordered_map<int, QPointF>& displacements
        */
       //std::ref and std::cref are used because if not std::thread copies the arguments before starting the thread
        collisionsThreads.push_back(std::thread(collisionsThread, t, start, end, graph, radius, force, std::cref(nodesIds), std::ref(localDisplacements[t])));
    }
 
    for (auto &t : collisionsThreads){
        t.join();
    }



    for (unsigned int i = 0; i < localDisplacements.size(); i++){
        for (const auto& [id, delta] : localDisplacements[i]){
            graph->positionMutex.lock();
            graph->nodesPosition[id] += delta;
            graph->positionMutex.unlock();
        }
    }


}




void Core::onFinishedApplyingForceDirected() {
    joinWorkerIfNeeded();
    threadForceDirectedRunning.store(false);
}


void Core::moveNode(int nodeId, QPointF& newPos){
    if (threadForceDirectedRunning.load() || graph == nullptr) return;
    if (!graph->nodesNames.count(nodeId)) return;

    graph->positionMutex.lock();

    if (graph->quadtree->remove(graph->nodesPosition[nodeId], nodeId)){
        graph->nodesPosition[nodeId] = newPos;
        graph->quadtree->insert(newPos, nodeId);
    }

    graph->positionMutex.unlock();
}
