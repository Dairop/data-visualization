#include "core.h"


void Core::setNewDatasetPath(std::filesystem::path path){
    datasetPath = path;

    //load
    //...

    emit datasetLoaded();
}

void Core::generateNewDataset(int nodes, int edges){
    dataset.generateDataset(nodes, edges);

    graph = new Graph(&dataset);

    emit datasetGenerated();
}
