#include "WAVChannel.h"
#include "stdexcept"
#include "string.h"

class WAVMute16_1 : public WAVMute {
    WAVMetaData info;
    unsigned int start;
    unsigned int end;
    WAVChannel& inputChannel;
public:

    WAVMute16_1(WAVMute16_1&& other)  noexcept : info(std::move(other.info)), start(std::move(other.start)), end(std::move(other.end)), inputChannel(other.inputChannel) {}

    WAVMute16_1(WAVChannel& channel, unsigned int start, unsigned int stop) : inputChannel(channel), start(start), end(stop),
                                                                              info(channel.getInfo()){
        if (info.fmt.numChannels != 1)
            throw std::invalid_argument("invalid channel count for WAVMute16_1");
        if (info.fmt.blockAlign != 2)
            throw std::invalid_argument("invalid sample size for WAVMute16_1");
    }

    unsigned int readSample(void* buff, unsigned int count) override {
        auto* data = reinterpret_cast<short *>(buff);
        unsigned int part_start = inputChannel.getPose();
        unsigned int size = inputChannel.readSample(data, count);
        unsigned int localStart = std::max(start * info.fmt.blockAlign, part_start);
        unsigned int localEnd = std::min(end * info.fmt.blockAlign, part_start + count);
        if(localStart <= localEnd)
            memset( data + (localStart - part_start), 0, (localEnd - localStart) * sizeof(short));
        return size;
    }

    void skip(unsigned int count) override {
        inputChannel.skip(count);
    }

    WAVMetaData getInfo() override {
        return info;
    }

    unsigned int getPose() override {
        return inputChannel.getPose();
    }

    bool isEnd() override {
        return inputChannel.isEnd();
    }

    void close() override {
        inputChannel.close();
    }

};

WAVMute& _getMuteChannel(WAVChannel& channel, WAVMute*& mute, unsigned long timeStart, unsigned long timeStop) {
    WAVMetaData data = channel.getInfo();
    if (data.fmt.numChannels == 1 && data.fmt.blockAlign == 2) {
        (*mute) = WAVMute16_1(channel, timeStart, timeStop);
    }
    throw NotSupportedException();
}
