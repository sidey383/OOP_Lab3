#include "WAVMute.h"
#include <stdexcept>
#include <cstring>

WAVMute16_1::WAVMute16_1(WAVChannel *channel, unsigned int start, unsigned int stop) : inputChannel(channel),
                                                                                       start(start), end(stop),
                                                                                       info(channel->getInfo()) {
    if (info.getNumChannel() != 1)
        throw std::invalid_argument("invalid channel count for WAVMute16_1");
    if (info.getBlockAlign() != 2)
        throw std::invalid_argument("invalid sample size for WAVMute16_1");
}

unsigned int WAVMute16_1::readSample(void *buff, unsigned int count) {
    auto *data = reinterpret_cast<short *>(buff);
    unsigned int part_start = inputChannel->getPose();
    unsigned int size = inputChannel->readSample(data, count);
    unsigned int localStart = std::max(start * info.getSampleRate(), part_start);
    unsigned int localEnd = std::min(end * info.getSampleRate(), part_start + count);
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
