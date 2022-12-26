#include "WAVClip.h"
#include <stdexcept>
#include <cstring>

WAVClip16_1::WAVClip16_1(WAVChannel *channel, unsigned int start, unsigned int stop) : input(channel),
                                                                                       start(start), stop(stop){
    WAVMetaData metaData = channel->getInfo();
    if (info.getNumChannel() != 1)
        throw std::invalid_argument("invalid channel count for WAVMute16_1");
    if (info.getBlockAlign() != 2)
        throw std::invalid_argument("invalid sample size for WAVMute16_1");
}

unsigned int WAVClip16_1::readSample(void *buff, unsigned int count) {
    auto *data = reinterpret_cast<short *>(buff);
    unsigned int part_start = input->getPose();
    unsigned int size = input->readSample(data, count);
    unsigned int localStart = std::max(start * info.getSampleRate(), part_start);
    unsigned int localEnd = std::min(stop * info.getSampleRate(), part_start + count);
    if (localStart <= localEnd) {
        memset(data + (localStart - part_start), 0, (localEnd - localStart) * sizeof(short));
    }
    return size;
}

void WAVClip16_1::skip(unsigned int count) {
    input->skip(count);
}

WAVMetaData &WAVClip16_1::getInfo() {
    return info;
}

unsigned int WAVClip16_1::getPose() {
    return input->getPose();
}

bool WAVClip16_1::isEnd() {
    return input->isEnd();
}

void WAVClip16_1::close() {
    input->close();
}

WAVClip16_1::~WAVClip16_1() {
    delete input;
}

