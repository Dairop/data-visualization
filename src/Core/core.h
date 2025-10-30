#pragma once
#include <QString>
#include <dataset.h>

class Core {
public:
    Core(){
        datasetPath = "";
    }


    void setNewDatasetPath(std::string path);
    void generateNewDataset(int nodes, int edges);

private:
    std::string datasetPath;
    Dataset dataset;
    

};
