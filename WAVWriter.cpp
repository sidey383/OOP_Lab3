#include "WAVWriter.h"

void WAVWriter::write() {
    WAVMetaData data = channel->getInfo();
    FileHeader header = data.getFileHeader();
    stream.write((char*)&header, sizeof(FileHeader));
    for (SomeChunk*& chunk : data.getChunks()) {
        char* header = (char*)chunk->getHeader();
        stream.write(header, sizeof(ChunkHeader));
        stream.write(chunk->getData(), chunk->getDataSize());
    }
    ChunkHeader dataHeader{{'d', 'a', 't', 'a'}, data.getDataSize()};
    stream.write((char*)&dataHeader, sizeof(ChunkHeader));
    char buffer[2048];
    unsigned int toRead = 2048 / data.getBlockAlign();
    unsigned int read;
    while( (read = channel->readSample(buffer, toRead)) > 0) {
        stream.write(buffer, read);
    }
}

void WAVWriter::close() {
    stream.close();
}
