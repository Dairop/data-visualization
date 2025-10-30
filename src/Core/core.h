#pragma once
#include <QString>
#include <dataset.h>

#include <filesystem>
#include <QObject>

class Core : public QObject {
Q_OBJECT

public:
    Core(){
        datasetPath = "";
    }

    void setNewDatasetPath(std::filesystem::path path);
    void generateNewDataset(int nodes, int edges);

private:
    std::filesystem::path datasetPath;
    Dataset dataset;
    
signals:
    void datasetLoaded();
    void datasetGenerated();

};
