#pragma once
#include "../WAVChannel.h"

class WAVClip16_1 : public WAVClip {
    WAVMetaData info;
    unsigned int sampleCount = 0;
    WAVChannel* input;
    unsigned int pose = 0;
public:

    WAVClip16_1(WAVChannel *channel, unsigned int start, unsigned int sto);

    unsigned int readSample(void* buff, unsigned int count) override;

    void skip(unsigned int count) override;

    WAVMetaData& getInfo() override;

    unsigned int getPose() override;

    bool isEnd() override;

    void close() override;

    ~WAVClip16_1() override;
};
