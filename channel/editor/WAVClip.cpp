#include "WAVClip.h"
#include <cstring>
#include "../../WAVexcepiont.h"

WAVClip16_1::WAVClip16_1(WAVChannel *channel, unsigned int start, unsigned int stop) : input(channel){
    WAVMetaData metaData = channel->getInfo();
    if (metaData.getNumChannel() != 1)
        throw WAVInvalidFormatException("invalid channel count for WAVMute16_1");
    if (metaData.getBlockAlign() != 2)
        throw WAVInvalidFormatException("invalid sample size for WAVMute16_1");
    if (metaData.getFormat() != 1)
        throw WAVInvalidFormatException("invalid sound format for WAVMute16_1");
    start = start * metaData.getSampleRate();
    stop = stop * metaData.getSampleRate();
    start = std::max(start, (unsigned int)0);
    stop = std::min(stop, metaData.getDataSize() / metaData.getBlockAlign());
    if(stop < start) {
        stop = 0;
        start = 0;
    }
    FileHeader header = metaData.getFileHeader();
    header.size = header.size - metaData.getDataSize() + (stop - start) * metaData.getSampleRate();
    info = WAVMetaData(header);
    for (FileChunk *chunk: metaData.getChunks()) {
        FileChunk* newChunk = info.createChunk(*chunk->getHeader());
        memcpy(newChunk->getData(), chunk->getData(), chunk->getDataSize());
    }
    ChunkHeader dataHeader = metaData.getDataHeader();
    dataHeader.size = (stop - start) * metaData.getBlockAlign();
    info.createChunk(dataHeader);
    if (!info.isCorrect()) {
        throw WAVException("WAVMetaData create exception");
    }
    channel->skip(start);
    this->sampleCount = stop - start;
}

unsigned int WAVClip16_1::readSample(void *buff, unsigned int count) {
    auto *data = reinterpret_cast<short *>(buff);
    unsigned int size = input->readSample(data, count);
    if (pose + size >= sampleCount) {
        if (pose >= sampleCount) {
            return 0;
        } else {
            size = sampleCount - pose;
        }
    }
    pose += size;
    return size;
}

void WAVClip16_1::skip(unsigned int count) {
    input->skip(count);
    pose += count;
}

WAVMetaData &WAVClip16_1::getInfo() {
    return info;
}

unsigned int WAVClip16_1::getPose() {
    return pose;
}

bool WAVClip16_1::isEnd() {
    return input->isEnd() || pose >= sampleCount;
}

void WAVClip16_1::close() {
    input->close();
}

WAVClip16_1::~WAVClip16_1() {
    delete input;
}

