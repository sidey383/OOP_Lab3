#pragma once

#include <string>
#include "stdlib.h"
#include "WAVStructs.h"


class WAVChannel {
public:

    WAVChannel(WAVChannel&) = delete;

    WAVChannel() = default;

    virtual unsigned int readSample(void* buff, unsigned int count) = 0;

    virtual void skip(unsigned int count) = 0;

    virtual WAVMetaData getInfo() = 0;

    virtual unsigned int getPose() = 0;

    virtual bool isEnd() = 0;

    virtual void close() = 0;

    ~WAVChannel() = default;

    WAVChannel& operator= (WAVChannel& reader) = delete;

};

class WAVMute : public WAVChannel {};

void getMuteChannel(WAVChannel& result ,WAVChannel& channel, unsigned long timeStart, unsigned long timeStop);

class WAVMix : public WAVChannel {};

WAVMute& getMixerChannel(WAVChannel& first, WAVChannel& second, unsigned long timeStart);

class NotSupportedException : public std::exception {

    const char * what() const noexcept override {
        return "This functionality is not supported";
    }

};
