#include "dataset.h"
#include <string>
#include <fstream>
#include <sstream>

void Dataset::loadFromFile(std::filesystem::path path){
    nodesNames.clear();
    edges.clear();

    filePath = path;

    std::ifstream file(path);
    if (!file.is_open()){
        std::cerr << "Failed to open " << path << std::endl;
        return;
    }

    std::string line;

    //skip first line
    if (!std::getline(file, line)) return;


    int nNodes = 0;
    if (!std::getline(file, line)) return;
    try {
        nNodes = std::stoi(line);
    } catch (...) {
        std::cerr << "Invalid node count" << std::endl;
        return;
    }


    //create nodes
    for (int i = 0; i < nNodes; i++){
        nodesNames[i] = "Node_" + std::to_string(i);
    }

    //skip 3rd line
    if (!std::getline(file, line)) return;

    //edges
    while (std::getline(file, line)){
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string fromStr, toStr;

        if (!std::getline(ss, fromStr, ',')) continue;
        if (!std::getline(ss, toStr, ',')) continue;

        int from = std::stoi(fromStr);
        int to = std::stoi(toStr);

        if (from >= 0 && from < nNodes && to >= 0 && to < nNodes) {
            edges[from].push_back(to);
        }
    }
}


int randInt32b() {
    //useful if RAND_MAX == 0x7fff (windows)
    //linux based systems are already 32b
    int bin1 = rand();
    return ((bin1 << 16) | rand())& 0x7fffffff; //in case first bit becomes 1 (skip negative values)
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
            /*
            int start = randInt32b() % nNodes;
            int end = (double) start / (double) (1.0 + (randInt32b()%100) / 10.0);
            */

            int start = j % nNodes;
            int end = randInt32b() % nNodes;
            if (start == end) start = (start + 1) % nNodes;
            edges[start].push_back(end);
        }
    
    } else {
        for (int j = 0; j < nEdges; j++){
            int start = j % nNodes;
            int end = randInt32b() % nNodes;
            if (start == end) start = (start + 1) % nNodes;
            edges[start].push_back(end);
        }
    }
}