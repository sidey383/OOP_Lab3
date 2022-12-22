#pragma once
#include "stdlib.h"
#include "WAVStructs.h"


class WAVChannel {
public:

    WAVChannel() {}

    virtual unsigned int readSample(void* buff, size_t count) = 0;
    virtual void skip(size_t count) = 0;

    virtual WAVMetaData getInfo() = 0;

    virtual unsigned int getPose() = 0;

    virtual bool isEnd() = 0;

    virtual void close() = 0;

    ~WAVChannel() = default;

};

class WAVMute : public WAVChannel {};

//WAVMute getMuteChannel(WAVChannel channel, unsigned long timeStart, unsigned long timeStop);

class WAVMix : public WAVChannel {};

//WAVMute getMuteChannel(WAVChannel first, WAVChannel second, unsigned long timeStart);
