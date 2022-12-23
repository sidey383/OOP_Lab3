#pragma once
#include "WAVChannel.h"
#include <fstream>

class WAVWriter {
    WAVChannel* channel;
    std::ofstream& stream;
public:
    WAVWriter(WAVChannel* channel, std::ofstream& stream) : channel(channel), stream(stream) {}

    void write();

    void close();

};

