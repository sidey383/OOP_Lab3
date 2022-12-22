#pragma once
#include <vector>

unsigned int dataToUnsignedInt(char data[4]);

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

enum class ChunkTypes {
    FMT,
    INFO,
    DATA
};

struct SomeChunk {
    char* data;
    unsigned int size = 0;
    ChunkHeader header;

    SomeChunk(ChunkHeader header,unsigned int size) : size(size), header(header){
        data = new char[size];
    }

    ~SomeChunk() {
        delete[] data;
    }

};

struct WAVMetaData {
    FMTChunk fmt;
    std::vector<SomeChunk*> otherChunks;
    unsigned int sampleCount = 0;
    unsigned short riffSize = 0;
};

