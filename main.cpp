#include "ConfigParser.h"
#include <cstring>
#include <iostream>

int main(int argc, char** argv) {
    std::string config;
    bool hasConfig;
    std::string output;
    bool hasOutput;
    std::vector<std::string> inputs;
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-h") == 0) {
            std::cout << "Help" << std::endl
            << "sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav …]]" << std::endl
            << ConfigParser::getConfigLore() << std::endl;
        }
        if(strcmp(argv[i], "-c") == 0) {
            ++i;
            if (i < argc) {
                config = std::string(argv[i]);
            } else {
                throw std::exception();
            }
            hasConfig = true;
            continue;
        }
        if (!hasOutput) {
            output = std::string(argv[i]);
            hasOutput = true;
            continue;
        }
        inputs.emplace_back(argv[i]);
    }
    if (!(hasOutput && hasConfig)) {
        throw std::exception();
    }
    try {
        ConfigParser parser(config, output, inputs);
        parser.apply();
    } catch (std::exception& e) {
        std::cerr << e.what();
    } catch (...) {
        std::cerr << "Unknown exception";
    }
    return 0;
}
