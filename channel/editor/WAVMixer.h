#pragma once
#include "../WAVChannel.h"

class WAVMixer16_1 : public WAVMixer {
    WAVMetaData info;
    unsigned int start;
    WAVChannel* baseInput;
    WAVChannel* additionalInput;
public:

    WAVMixer16_1(WAVChannel* base, WAVChannel* additional,  unsigned int start);

    unsigned int readSample(void* buff, unsigned int count) override;

    void skip(unsigned int count) override;

    WAVMetaData& getInfo() override;

    unsigned int getPose() override;

    bool isEnd() override;

    void close() override;

    ~WAVMixer16_1() override;

};
