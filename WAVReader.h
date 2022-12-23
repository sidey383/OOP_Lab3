#pragma once
#include "WAVChannel.h"
#include "fstream"

class WAVReader : public WAVChannel {
private:
    WAVMetaData data;
    std::ifstream& file;
    unsigned int pose;

    bool readFMT(ChunkHeader header, std::ifstream &file);

    bool readOther(ChunkHeader header, std::ifstream &file);

public:

    explicit WAVReader(std::ifstream& file);

    ~WAVReader();

    unsigned int readSample(void* buff, unsigned int count) override;

    void skip(unsigned int count) override;

    WAVMetaData getInfo() override;

    unsigned int getPose() override;

    bool isEnd() override;

    void close() override;

    WAVReader& operator= (WAVReader& reader) = delete;

};
