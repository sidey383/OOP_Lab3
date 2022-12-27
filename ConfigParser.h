#pragma once
#include <fstream>
#include <vector>
#include "SoundEditor.h"

class ConfigParser {
    std::ifstream config;
    std::vector<std::string> inputFiles;
    std::string outputFile;
    SoundEditor* editor = nullptr;

public:

    ConfigParser(std::string configFileName, std::string outputFile, std::vector<std::string> inputFiles);

    void apply();

    static std::string getConfigLore();

    ~ConfigParser();

};