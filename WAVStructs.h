#pragma once
#include <vector>

struct Header {
    char chunkId[4]; // "RIFF"
    unsigned int size; // size of file - 8 (byte)
    char format[4]; // "WAVE"
};

struct ChunkHeader {
    char id[4];
    unsigned int size;
};

struct FMTChunk {
    unsigned short audioFormat = 0; // 1 - PCM
    unsigned short numChannels = 0; // number oif channels
    unsigned int sampleRate = 0;
    unsigned int byteRate = 0; // (Sample Rate * BitsPerSample * Channels) / 8.
    unsigned short blockAlign = 0;
    unsigned short bitsPerSample = 0;
};

struct SomeChunk {
    char* data;
    unsigned int size = 0;
    ChunkHeader header;

    SomeChunk(ChunkHeader header,unsigned int size) : size(size), header(header){
        data = new char[size];
    }

};

struct {
    char riff[4]{'R','I','F','F'};
    char wave[4]{'W','A','V','E'};
    char data[4]{'d','a','t','a'};
    char fmt[4]{'f','m','t',' '};
} chunkHeaders;

struct WAVMetaData {
    Header header;
    FMTChunk fmt;
    std::vector<SomeChunk*> otherChunks;
    unsigned int sampleCount = 0;
};

