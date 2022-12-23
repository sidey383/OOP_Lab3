#pragma once
#include <string>
#include "stdlib.h"
#include "WAVMetaData.h"


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

    virtual ~WAVChannel() = default;

    WAVChannel& operator= (WAVChannel& reader) = delete;

};

class WAVMute : public WAVChannel {};

WAVMute* getMuteChannel(WAVChannel* channel, unsigned long timeStart, unsigned long timeStop);

class WAVMixer : public WAVChannel {};

WAVMixer& getMixerChannel(WAVChannel& first, WAVChannel& second, unsigned long timeStart);
