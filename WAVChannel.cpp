#include "WAVChannel.h"
#include "stdexcept"
#include "string.h"

class WAVMute16_1 : public WAVMute {
    WAVMetaData info;
    unsigned int start;
    unsigned int end;
    WAVChannel& inputChannel;
public:

    WAVMute16_1(WAVChannel& channel, unsigned int start, unsigned int stop) : inputChannel(channel), start(start), end(stop),
                                                                              info(channel.getInfo()){
        if (info.fmt.numChannels != 1)
            throw new std::invalid_argument("invalid channel count for WAVMute16_1");
        if (info.fmt.blockAlign != 2)
            throw new std::invalid_argument("invalid sample size for WAVMute16_1");
    }

    unsigned int readSample(void* buff, unsigned int count) {
        short* data = reinterpret_cast<short *>(buff);
        unsigned int part_start = inputChannel.getPose();
        unsigned int size = inputChannel.readSample(data, count);
        unsigned int localStart = std::max(start * info.fmt.blockAlign, part_start);
        unsigned int localEnd = std::min(end * info.fmt.blockAlign, part_start + count);
        if(localStart <= localEnd)
            memset( data + (localStart - part_start), 0, (localEnd - localStart) * sizeof(short));
        return size;
    }

    void skip(size_t count) {
        inputChannel.skip(count);
    }

    unsigned int getPose() {
        return inputChannel.getPose();
    }

    WAVMetaData getInfo() {
        return info;
    }

    virtual bool isEnd() {
        return inputChannel.isEnd();
    }

};
