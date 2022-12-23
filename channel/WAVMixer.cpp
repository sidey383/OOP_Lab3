#include <stdexcept>

class WAVMixer16_1 : public WAVMute {
    WAVMetaData info;
    unsigned int start;
    WAVChannel* baseInput;
    WAVChannel* additionalInput;
public:

    WAVMixer16_1(WAVChannel* base, WAVChannel* additional,  unsigned int start) : baseInput(base), additionalInput(additional), start(start),
                                                                              info(base->getInfo()){
        if (info.getNumChannel() != 1)
            throw std::invalid_argument("invalid channel count for WAVMixer16_1");
        if (info.getBlockAlign() != 2)
            throw std::invalid_argument("invalid sample size for WAVMixer16_1");
        if (info.getFormat() != 1)
            throw std::invalid_argument("invalid sound format for WAVMixer16_1");
        WAVMetaData additionalMetadata = additional->getInfo();
        if(!info.isSimilar(additionalMetadata)) {
            throw std::invalid_argument("Different sound formats");
        }
    }

    unsigned int readSample(void* buff, unsigned int count) override {
        auto* data = reinterpret_cast<short *>(buff);
        unsigned int part_start = baseInput->getPose();
        unsigned int size = baseInput->readSample(data, count);
        if (part_start + size / info.getBlockAlign() > start * info.getSampleRate()) {
            unsigned int additional_part_start = std::max((unsigned int)0, start * info.getSampleRate() - part_start);
            unsigned int additionalSize = size - additional_part_start;
            short additionalData[additionalSize];
            if ( (additionalSize = additionalInput->readSample(additionalData, additionalSize * info.getSampleRate())) > 0 ) {
                for (unsigned int i = 0; i < additionalSize; i++) {
                    data[part_start + i] = data[part_start + i] / 2 + additionalData[i] / 2;
                }
            }
        }
        return size;
    }

    void skip(unsigned int count) override {
        baseInput->skip(count);
        if (baseInput->getPose() > start * info.getSampleRate()) {
            additionalInput->skip(count);
        }
    }

    WAVMetaData getInfo() override {
        return info;
    }

    unsigned int getPose() override {
        return baseInput->getPose();
    }

    bool isEnd() override {
        return baseInput->isEnd();
    }

    void close() override {
        baseInput->close();
        additionalInput->close();
    }

    ~WAVMixer16_1() override {
        delete baseInput;
        delete additionalInput;
    }

};
