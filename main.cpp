#include "ConfigParser.h"
#include "WAVexcepiont.h"
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
            return 0;
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
        std::cerr<<"No output file or config";
        return 1;
    }
    try {
        ConfigParser parser(config, output, inputs);
        parser.apply();
    } catch (WAVException& e) {
        std::cerr << e.what();
        return e.getReturnCode();
    } catch (std::exception& e){
        std::cerr << e.what();
        return 2;
    } catch (...) {
        std::cerr << "Unknown exception";
        return 1;
    }
    return 0;
}
