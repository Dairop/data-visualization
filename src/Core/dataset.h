#include <unordered_map>
#include <iostream>
#include <random>
#include <time.h>

struct Dataset {
    std::string filePath;
    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, int> edges; //oriented

    void loadFromFile(std::string path);
    void generateDataset(int nNodes, int nEdges);
};