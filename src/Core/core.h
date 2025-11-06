#pragma once
#include <QString>
#include "dataset.h"
#include "graph.h"

#include <filesystem>
#include <QObject>
#include <thread>


class Core : public QObject {
Q_OBJECT

public:
    Core();
    ~Core();

    void setNewDatasetPath(std::filesystem::path path);
    void generateNewDataset(int nodes, int edges);

    Dataset* getDataset() { return &dataset; }
    Graph* getGraph() { return graph; }

    void startApplyingForceDirected();
    void stopApplyingForceDirected();

    bool isThreadRunning() { return threadForceDirectedRunning; }
    
signals:
    void datasetLoaded();
    void datasetGenerated();

    //iteration that just finished and the total number of expected iterations
    //we need those to be able to draw a progress bar
    void positionsUpdated(int iteration, int totalIterations);

    void finishedApplyingForceDirected();

private slots:
    void onFinishedApplyingForceDirected(); //we want to join() the workers in the main thread

private:
    void forceDirected();
    void collisions(float radius = 150.0f, float force = 1.0f /*1.0f = solve the collision*/);
    void keepNodesInsideQuadtree();
    bool checkForceDirectedEnd();
    void joinWorkerIfNeeded();

    std::filesystem::path datasetPath;
    Dataset dataset;

    Graph* graph;

    std::thread forceDirectedThread;

    std::atomic_bool threadsMustStop;
    std::atomic_bool threadForceDirectedRunning;
};
