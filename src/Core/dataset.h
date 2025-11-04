#pragma once

#include <unordered_map>
#include <iostream>
#include <random>
#include <time.h>
#include <vector>

struct Dataset {
    std::string filePath;
    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, std::vector<int>> edges; //oriented

    void loadFromFile(std::string path);
    void generateDataset(int nNodes, int nEdges);
};