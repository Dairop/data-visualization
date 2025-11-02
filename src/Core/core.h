#pragma once
#include <QString>
#include <dataset.h>

#include <filesystem>
#include <QObject>


class Core : public QObject {
Q_OBJECT

private:
    std::filesystem::path datasetPath;
    Dataset dataset;

public:
    Core(){
        datasetPath = "";
    }

    void setNewDatasetPath(std::filesystem::path path);
    void generateNewDataset(int nodes, int edges);

    Dataset* getDataset() { return &dataset; }
    
signals:
    void datasetLoaded();
    void datasetGenerated();

};
