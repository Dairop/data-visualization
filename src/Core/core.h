#pragma once
#include <QString>
#include "dataset.h"
#include "graph.h"

#include <filesystem>
#include <QObject>
#include <thread>


class Core : public QObject {
Q_OBJECT

private:
    std::filesystem::path datasetPath;
    Dataset dataset;

    Graph* graph;

public:
    Core(){
        datasetPath = "";
        graph = nullptr;
    }

    void setNewDatasetPath(std::filesystem::path path);
    void generateNewDataset(int nodes, int edges);

    Dataset* getDataset() { return &dataset; }
    Graph* getGraph() { return graph; }

    void startApplyingForceDirected();
    
signals:
    void datasetLoaded();
    void datasetGenerated();


private:
    void forceDirected();

};
