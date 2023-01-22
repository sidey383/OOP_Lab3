#pragma once
#include "../WAVChannel.h"
#include "fstream"

class WAVReader : public WAVChannel {
private:
    WAVMetaData data;
    std::ifstream file;
    std::string fileName;
    unsigned int pose;

public:

    explicit WAVReader(std::string& file);

    ~WAVReader() override;

    unsigned int readSample(void* buff, unsigned int count) override;

    void skip(unsigned int count) override;

    WAVMetaData& getInfo() override;

    unsigned int getPose() override;

    bool isEnd() override;

    void close() override;

    WAVReader& operator= (WAVReader& reader) = delete;

};
