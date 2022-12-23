#include <stdexcept>
#include <cstring>

class WAVMute16_1 : public WAVMute {
    WAVMetaData info;
    unsigned int start;
    unsigned int end;
    WAVChannel* inputChannel;
public:

    WAVMute16_1(WAVChannel* channel, unsigned int start, unsigned int stop) : inputChannel(channel), start(start), end(stop),
                                                                              info(channel->getInfo()){
        if (info.getNumChannel() != 1)
            throw std::invalid_argument("invalid channel count for WAVMute16_1");
        if (info.getBlockAlign() != 2)
            throw std::invalid_argument("invalid sample size for WAVMute16_1");
    }

    unsigned int readSample(void* buff, unsigned int count) override {
        auto* data = reinterpret_cast<short *>(buff);
        unsigned int part_start = inputChannel->getPose();
        unsigned int size = inputChannel->readSample(data, count);
        unsigned int localStart = std::max(start * info.getSampleRate(), part_start);
        unsigned int localEnd = std::min(end * info.getSampleRate(), part_start + count);
        if(localStart <= localEnd) {
            memset(data + (localStart - part_start), 0, (localEnd - localStart) * sizeof(short));
        }
        return size;
    }

    void skip(unsigned int count) override {
        inputChannel->skip(count);
    }

    WAVMetaData getInfo() override {
        return info;
    }

    unsigned int getPose() override {
        return inputChannel->getPose();
    }

    bool isEnd() override {
        return inputChannel->isEnd();
    }

    void close() override {
        inputChannel->close();
    }

    ~WAVMute16_1() override {
        delete inputChannel;
    }

};
