#include "dataset.h"
#include <string>

void Dataset::loadFromFile(std::string path){

}


int randInt32b() {
    //useful if RAND_MAX == 0x7fff (windows)
    //linux based systems are already 32b
    int bin1 = rand();
    return (bin1 << 16) | rand();
}


void Dataset::generateDataset(int nNodes, int nEdges){
    nodesNames.clear();
    edges.clear();
    if (nNodes == 0) return;

    srand(time(nullptr));

    for (int i = 0; i < nNodes; i++){
        nodesNames.insert(std::pair<int, std::string>(i, "Node "+std::to_string(i)));
    }

    if (RAND_MAX < nNodes && RAND_MAX == 0x7fff) {
        //randmax trop faible pour grands graphes
        for (int j = 0; j < nEdges; j++){
            int start = randInt32b() % nNodes;
            int end = (double) start / (double) (1.0 + (randInt32b()%100) / 10.0);
            edges.insert(std::pair<int, int>(start, end));
        }
    
    } else {

        for (int j = 0; j < nEdges; j++){
            int start = rand() % nNodes;
            int end = (double) start / (double) (1.0 + (rand()%100) / 10.0);
            edges.insert(std::pair<int, int>(start, end));
        }

    } 
}