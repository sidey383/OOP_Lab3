#include "WAVMetaData.h"

bool WAVMetaData::compare4Str(const char *str1, const char *str2) {
    return str1[0] == str2[0] && str1[1] == str2[1]
           && str1[2] == str2[2] && str1[3] == str2[3];
}

ChunkType WAVMetaData::getType(ChunkHeader &header) {
    if (compare4Str(header.id, riffStr)) {
        return ChunkType::RIFF;
    }
    if (compare4Str(header.id, waveStr)) {
        return ChunkType::WAVE;
    }
    if (compare4Str(header.id, dataStr)) {
        return ChunkType::DATA;
    }
    if (compare4Str(header.id, fmtStr)) {
        return ChunkType::FMT;
    }
    return ChunkType::OTHER;
}

WAVMetaData::WAVMetaData(FileHeader &header) : fileHeader(header) {
    if (!compare4Str((char *) header.chunkId, riffStr)) {
        throw std::invalid_argument("Wrong fmt id");
    }
    if (!compare4Str((char *) header.format, waveStr)) {
        throw std::invalid_argument("Wrong file format id");
    }
}

WAVMetaData::WAVMetaData(const WAVMetaData &origin) : fileHeader(origin.fileHeader), dataHeader(origin.dataHeader) {
    for (SomeChunk* chunk : origin.chunks) {
        ChunkType type = getType(*chunk->getHeader());
        if (type == ChunkType::FMT) {
            fmt = new FMTChunk(*(FMTChunk*)chunk);
            chunks.push_back(fmt);
        } else {
            chunks.push_back(new UnknownChunk(*(UnknownChunk*)chunk));
        }
    }
}

WAVMetaData &WAVMetaData::operator=(const WAVMetaData &origin) {
    if (&origin == this)
        return *this;
    for (SomeChunk *&chunk: chunks) {
        delete chunk;
    }
    chunks.clear();
    fmt = nullptr;
    fileHeader = origin.fileHeader;
    dataHeader = origin.dataHeader;
    for (SomeChunk*& chunk : chunks) {
        ChunkType type = getType(*chunk->getHeader());
        if (type == ChunkType::FMT) {
            fmt = new FMTChunk(*(FMTChunk*)chunk);
            chunks.push_back(fmt);
        } else {
            chunks.push_back(new UnknownChunk(*(UnknownChunk*)chunk));
        }
    }
}

SomeChunk *WAVMetaData::createChunk(ChunkHeader chunkHeader) {
    ChunkType type = getType(chunkHeader);
    SomeChunk *chunk;
    switch (type) {
        case ChunkType::FMT:
            fmt = new FMTChunk(chunkHeader);
            chunk = fmt;
            break;
        case ChunkType::DATA:
            dataHeader= chunkHeader;
            return nullptr;
        default:
            chunk = new UnknownChunk(chunkHeader);
    }
    chunks.push_back(chunk);
    return chunk;
}

std::vector<SomeChunk *> WAVMetaData::getChunks() {
    return chunks;
}

bool WAVMetaData::isCorrect() {
    return fmt != nullptr;
}

bool WAVMetaData::isSimilar(WAVMetaData &metaData)  {
    return getNumChannel() == metaData.getNumChannel() &&
           getSampleRate() == metaData.getSampleRate() &&
           getBlockAlign() == metaData.getBlockAlign() &&
           getFormat() == metaData.getFormat();
}

WAVMetaData::~WAVMetaData() {
    for (SomeChunk *&chunk: chunks) {
        delete chunk;
    }
}


unsigned int WAVMetaData::getBlockAlign() {
    if (fmt == nullptr)
        return 0;
    return fmt->data.blockAlign;
}

unsigned int WAVMetaData::getSampleRate() {
    if (fmt == nullptr)
        return 0;
    return fmt->data.sampleRate;
}

unsigned int WAVMetaData::getNumChannel() {
    if (fmt == nullptr)
        return 0;
    return fmt->data.numChannels;
}

unsigned short WAVMetaData::getFormat() {
    if (fmt == nullptr)
        return 0;
    return fmt->data.audioFormat;
}

FileHeader WAVMetaData::getFileHeader() {
    return fileHeader;
}

unsigned int WAVMetaData::getDataSize() {
    return dataHeader.size;
}