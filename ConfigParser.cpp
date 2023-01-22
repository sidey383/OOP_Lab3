#include "ConfigParser.h"
#include <sstream>

ConfigParser::ConfigParser(std::ifstream& config, std::vector<std::string>& inputs) : inputs(inputs), config(config) {}

std::string ConfigParser::getConfigFormat() {
    return "Some config format lore";
}

SoundAction ConfigParser::nextAction() {
    std::vector<std::string> params;
    std::vector<std::string> files;
    std::string name = "";
    std::string string;
    while(std::getline(config, string)) {
        if (string[0] == '#')
            continue;
        std::stringstream ss(string);
        ss >> name;
        std::string param;
        while(!ss.eof()) {
            ss >> param;
            if (param[0] == '$') {
                try {
                    std::string  channelStr = param;
                    channelStr.erase(0, 1);
                    int channel = std::stoi(channelStr);
                    if (inputs.size() >= channel) {
                        files.push_back(inputs[channel - 1]);
                        continue;
                    }
                } catch (...) {}
            }
            params.push_back(param);
        }
        break;
    }
    return (SoundAction) {name, params, files};
}

