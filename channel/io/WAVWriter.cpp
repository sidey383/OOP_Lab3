#include "WAVWriter.h"
#define BUFFER_SIZE 262144

WAVWriterDefault::WAVWriterDefault(WAVChannel *channel, std::ofstream &stream) : channel(channel), stream(stream) {}

void WAVWriterDefault::write() {
    WAVMetaData metaData = channel->getInfo();
    FileHeader fileHeader = metaData.getFileHeader();
    stream.write((char *) &fileHeader, sizeof(FileHeader));
    for (FileChunk *&chunk: metaData.getChunks()) {
        char *header = (char *) chunk->getHeader();
        stream.write(header, sizeof(ChunkHeader));
        stream.write(chunk->getData(), chunk->getDataSize());
    }
    ChunkHeader dataHeader{{'d', 'a', 't', 'a'}, metaData.getDataSize()};
    stream.write((char *) &dataHeader, sizeof(ChunkHeader));
    short buffer[BUFFER_SIZE];
    unsigned int toRead = BUFFER_SIZE;
    unsigned int read;
    while ((read = channel->readSample(buffer, toRead)) > 0) {
        stream.write((char*)buffer, read * metaData.getBlockAlign());
    }
}

void WAVWriterDefault::close() {
    stream.close();
}

WAVWriterDefault::~WAVWriterDefault() {
    channel->close();
    delete channel;
}

