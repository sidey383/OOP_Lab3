#include "channel/WAVChannel.h"
#include <fstream>

int main() {
    std::ifstream input("input.wav", std::ios::binary);
    if (! input.is_open() ) {
        return 1;
    }
    std::ifstream input2("input2.wav", std::ios::binary);
    if (! input2.is_open() ) {
        return 1;
    }
    std::ofstream output("output.wav", std::ios::binary);
    if (!input.is_open() || !output.is_open()) {
        return 1;
    }
    WAVChannel* reader = getWAVReader(input);
    WAVChannel* reader2 = getWAVReader(input2);
    reader = getMixerChannel(reader, reader2, 20);
    WAVWriter* writer = getWAVWriter(reader, output);
    writer->write();
    reader->close();
    writer->close();
    return 0;
}
