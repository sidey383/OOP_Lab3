#include "WAVWriter.h"

WAVWriterDefault::WAVWriterDefault(WAVChannel *channel, std::ofstream &stream) : channel(channel), stream(stream) {}

void WAVWriterDefault::write() {
    WAVMetaData metaData = channel->getInfo();
    FileHeader fileHeader = metaData.getFileHeader();
    stream.write((char *) &fileHeader, sizeof(FileHeader));
    for (SomeChunk *&chunk: metaData.getChunks()) {
        char *header = (char *) chunk->getHeader();
        stream.write(header, sizeof(ChunkHeader));
        stream.write(chunk->getData(), chunk->getDataSize());
    }
    ChunkHeader dataHeader{{'d', 'a', 't', 'a'}, metaData.getDataSize()};
    stream.write((char *) &dataHeader, sizeof(ChunkHeader));
    char buffer[262144];
    unsigned int toRead = 2048 / metaData.getBlockAlign();
    unsigned int read;
    while ((read = channel->readSample(buffer, toRead)) > 0) {
        stream.write(buffer, read);
    }
}

void WAVWriterDefault::close() {
    stream.close();
}

WAVWriterDefault::~WAVWriterDefault() {
    channel->close();
}

