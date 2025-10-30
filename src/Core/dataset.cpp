#include "dataset.h"
#include <string>

void Dataset::loadFromFile(std::string path){

}


void Dataset::generateDataset(int nNodes, int nEdges){
    if (nNodes == 0) return;

    srand(time(nullptr));

    for (int i = 0; i < nNodes; i++){
        nodesNames.insert(std::pair<int, std::string>(i, "Node "+std::to_string(i)));
    }

    for (int j = 0; j < nEdges; j++){
        int start = rand() % nNodes;
        int end = (double) start / (double) (1.0 + (rand()%100) / 10.0);
        edges.insert(std::pair<int, int>());

        std::cout << start << " " << end << "\n";
    }
}