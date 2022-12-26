#include <stdexcept>
#include "WAVMixer.h"


WAVMixer16_1::WAVMixer16_1(WAVChannel* base,WAVChannel *additional,unsigned int start) :
baseInput (base), additionalInput(additional), start(start), info(base->getInfo()) {
    if (info.getNumChannel() != 1)
        throw std::invalid_argument("invalid channel count for WAVMixer16_1");
    if (info.getBlockAlign() != 2)
        throw std::invalid_argument("invalid sample size for WAVMixer16_1");
    if (info.getFormat() != 1)
        throw std::invalid_argument("invalid sound format for WAVMixer16_1");
    WAVMetaData additionalMetadata = additional->getInfo();
    if (!info.isSimilar(additionalMetadata)) {
        throw std::invalid_argument("Different sound formats");
    }
}

unsigned int WAVMixer16_1::readSample(void *buff, unsigned int count) {
    auto *data = reinterpret_cast<short *>(buff);
    unsigned int part_start = baseInput->getPose();
    unsigned int size = baseInput->readSample(data, count) / info.getBlockAlign();
    if (part_start + size > start * info.getSampleRate()) {
        unsigned int additional_part_start =
                (start * info.getSampleRate()) < part_start ? 0 : start * info.getSampleRate() - part_start;
        unsigned int additionalSize = size - additional_part_start;
        short additionalData[additionalSize];
        if ((additionalSize = additionalInput->readSample(additionalData, additionalSize * info.getSampleRate())) > 0) {
            for (unsigned int i = 0; i < additionalSize; i++) {
                data[part_start + i] = data[part_start + i] / 2 + additionalData[i] / 2;
            }
        }
    }
    return size * info.getBlockAlign();
}

void WAVMixer16_1::skip(unsigned int count) {
    baseInput->skip(count);
    if (baseInput->getPose() > start * info.getSampleRate()) {
        additionalInput->skip(count);
    }
}

WAVMetaData &WAVMixer16_1::getInfo() {
    return info;
}

unsigned int WAVMixer16_1::getPose() {
    return baseInput->getPose();
}

bool WAVMixer16_1::isEnd() {
    return baseInput->isEnd();
}

void WAVMixer16_1::close() {
    baseInput->close();
    additionalInput->close();
}

WAVMixer16_1::~WAVMixer16_1() {
    delete baseInput;
    delete additionalInput;
}

