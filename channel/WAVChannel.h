#pragma once
#include <string>
#include "../data/WAVMetaData.h"


class WAVChannel {
public:

    WAVChannel(WAVChannel&) = delete;

    WAVChannel() = default;

    virtual unsigned int readSample(void* buff, unsigned int count) = 0;

    virtual void skip(unsigned int count) = 0;

    virtual WAVMetaData& getInfo() = 0;

    virtual unsigned int getPose() = 0;

    virtual bool isEnd() = 0;

    virtual void close() = 0;

    virtual ~WAVChannel() = default;

    WAVChannel& operator= (WAVChannel& reader) = delete;

};

WAVChannel* getWAVReader(std::ifstream& out) noexcept(false);

class WAVWriter {
public:

    virtual void write() = 0;

    virtual void close() = 0;

};

WAVWriter* getWAVWriter(WAVChannel* channel, std::ofstream& out) noexcept(false);


class WAVMute : public WAVChannel {};

WAVMute* getMuteChannel(WAVChannel* channel, unsigned long timeStart, unsigned long timeStop) noexcept(false);


class WAVMixer : public WAVChannel {};

WAVMixer* getMixerChannel(WAVChannel* first, WAVChannel* second, unsigned long timeStart) noexcept(false);

class WAVClip : public WAVChannel {};

WAVClip* getClipChannel(WAVChannel* channel, unsigned long timeStart, unsigned long timeStop) noexcept(false);
