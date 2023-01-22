#pragma once
#include "../WAVChannel.h"
#include <fstream>

class WAVWriterDefault : public WAVWriter {
    WAVChannel* channel;
    std::ofstream stream;
public:
    WAVWriterDefault(WAVChannel* channel, std::string& stream);

    ~WAVWriterDefault();

    void write() override;

    void close() override;

};

