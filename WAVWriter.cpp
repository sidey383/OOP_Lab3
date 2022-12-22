#include "WAVWriter.h"

void WAVWriter::write() {
    WAVMetaData data = channel.getInfo();
    ChunkHeader riffHeader{{'R', 'I','F','F'}, data.riffSize};
    stream.write((char*)&riffHeader, sizeof(ChunkHeader));
    stream.write("WAVE", 4);
    ChunkHeader fmtHeader{{'f', 'm','t',' '}, 16};
    stream.write((char*)&fmtHeader, sizeof(ChunkHeader));
    stream.write((char *) &data.fmt, sizeof(FMTChunk));
    for (SomeChunk* chunk : data.otherChunks) {
        stream.write((char*)&chunk->header, sizeof(ChunkHeader));
        stream.write(chunk->data, chunk->size);
    }
    ChunkHeader dataHeader{{'d', 'a','t','a'}, data.sampleCount * data.fmt.sampleRate};
    stream.write((char*)&dataHeader, sizeof(ChunkHeader));
    char buffer[2048];
    unsigned int toRead = 2048 / data.fmt.blockAlign;
    unsigned int read;
    while( (read = channel.readSample(buffer, toRead)) > 0)
        stream.write(buffer, read);
}

void WAVWriter::close() {
    stream.close();
}
