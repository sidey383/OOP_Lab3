#pragma once
#include "../WAVChannel.h"

class WAVMute16_1 : public WAVMute {
    WAVMetaData info;
    unsigned int start;
    unsigned int end;
    WAVChannel* inputChannel;
public:

    WAVMute16_1(WAVChannel* channel, unsigned int start, unsigned int end);

    unsigned int readSample(void* buff, unsigned int count) override;

    void skip(unsigned int count) override;

    WAVMetaData& getInfo() override;

    unsigned int getPose() override;

    bool isEnd() override;

    void close() override;

    ~WAVMute16_1() override;

};
