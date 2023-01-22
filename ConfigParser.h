#pragma once
#include <fstream>
#include <vector>

struct SoundAction {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::string> files;
};

class ConfigParser {
    std::vector<std::string> inputs;
    std::ifstream& config;
public:
    ConfigParser(std::ifstream& config, std::vector<std::string>& inputs);

    SoundAction nextAction(void);

    static std::string getConfigFormat(void);
};
