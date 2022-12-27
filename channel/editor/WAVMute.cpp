#include "WAVMute.h"
#include <cstring>
#include "../../WAVexcepiont.h"

WAVMute16_1::WAVMute16_1(WAVChannel *channel, unsigned int start, unsigned int end) : inputChannel(channel),
                                                                                      info(channel->getInfo()) {
    if (info.getNumChannel() != 1)
        throw WAVInvalidFormatException("invalid channel count for WAVMute16_1");
    if (info.getBlockAlign() != 2)
        throw WAVInvalidFormatException("invalid sample size for WAVMute16_1");
    if (info.getFormat() != 1)
        throw WAVInvalidFormatException("invalid sound format for WAVMute16_1");
    this->start = start * info.getSampleRate();
    this->end = end * info.getSampleRate();
}

unsigned int WAVMute16_1::readSample(void *buff, unsigned int count) {
    unsigned int part_start = inputChannel->getPose();
    unsigned int size = inputChannel->readSample(buff, count);
    unsigned int localStart = std::max(start, part_start);
    unsigned int localEnd = std::min(end, part_start + count);
    short *data = reinterpret_cast<short *>(buff);
    if (localStart <= localEnd) {
        memset(data + (localStart - part_start), 0, (localEnd - localStart) * sizeof(short));
    }
    return size;
}

void WAVMute16_1::skip(unsigned int count) {
    inputChannel->skip(count);
}

WAVMetaData &WAVMute16_1::getInfo() {
    return info;
}

unsigned int WAVMute16_1::getPose() {
    return inputChannel->getPose();
}

bool WAVMute16_1::isEnd() {
    return inputChannel->isEnd();
}

void WAVMute16_1::close() {
    inputChannel->close();
}

WAVMute16_1::~WAVMute16_1() {
    delete inputChannel;
}
