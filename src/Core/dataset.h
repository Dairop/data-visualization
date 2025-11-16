#pragma once

#include <unordered_map>
#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <filesystem>

struct Dataset {
    std::filesystem::path filePath;
    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, std::vector<int>> edges; //oriented

    void loadFromFile(std::filesystem::path path);
    void generateDataset(int nNodes, int nEdges);
};