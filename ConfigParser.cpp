#include <sstream>
#include "ConfigParser.h"

ConfigParser::ConfigParser(std::string configFileName, std::string outputFile, std::vector<std::string> inputFiles) :
        config(configFileName) , inputFiles(inputFiles), outputFile(outputFile) {
    if (!this->config.is_open())
        throw std::exception();
    if (inputFiles.size() == 0)
        throw std::exception();
    std::ifstream inputFile(inputFiles[0], std::ios::binary);
    if (!inputFile.is_open())
        throw std::exception();
    editor = new SoundEditor(inputFile);
}

void ConfigParser::apply() {
    std::string string;
    while(std::getline(config, string)) {
        if (string[0] == '#')
            continue;
        std::stringstream ss(string);
        std::string param;
        ss >> param;
        if(param == "mute") {
            std::string startS;
            std::string stopS;
            ss >> startS >> stopS;
            int start = std::stoi(startS);
            int stop = std::stoi(stopS);
            editor->mute(start, stop);
            continue;
        }
        if(param == "mix") {
            std::string channelS;
            std::string startS;
            ss >> channelS >> startS;
            if (channelS[0] != '$') {
                throw std::exception();
            }
            channelS.erase(0, 1);
            int channelNumber = std::stoi(channelS);
            int start = std::stoi(startS);
            if (channelNumber < inputFiles.size())
                throw std::exception();
            std::ifstream file(inputFiles[channelNumber], std::ios::binary);
            editor->mix(file, start);
            continue;
        }
        if(param == "clip") {
            std::string startS;
            std::string stopS;
            ss >> startS >> stopS;
            int start = std::stoi(startS);
            int stop = std::stoi(stopS);
            editor->clip(start, stop);
            continue;
        }
        throw std::exception();
    }
    std::ofstream output(outputFile, std::ios::binary);
    if (!output.is_open())
        throw std::exception();
    editor->write(output);
}

std::string ConfigParser::getConfigLore() {
    return "Some config format lore";
}

ConfigParser::~ConfigParser() {
    delete editor;
}
