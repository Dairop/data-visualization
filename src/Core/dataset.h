#pragma once

#include <unordered_map>
#include <iostream>
#include <random>
#include <time.h>

struct Dataset {
    std::string filePath;
    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, int> edges; //oriented

    //we should use a map of linked lists to have multiple edges starting from a node

    void loadFromFile(std::string path);
    void generateDataset(int nNodes, int nEdges);
};