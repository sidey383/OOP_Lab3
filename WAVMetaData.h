#pragma once

#include <vector>
#include <stdexcept>

class NotSupportedException : public std::exception {
    const char *what() const noexcept override {
        return "This functionality is not supported";
    }
};

struct FileHeader {
    char chunkId[4]; // "RIFF"
    unsigned int size; // size of file - 8 (byte)
    char format[4]; // "WAVE"
};

struct ChunkHeader {
    char id[4];
    unsigned int size;
};

class SomeChunk {

public:

    virtual char *getData() = 0;

    virtual unsigned int getDataSize() {
        return getHeader()->size;
    }

    virtual ChunkHeader* getHeader() = 0;

    virtual ~SomeChunk() = default;

    SomeChunk& operator= (const SomeChunk& obj) = default;

};

class FMTChunk : public SomeChunk {

    ChunkHeader* header;

public:
    struct FMTChunkData {
        unsigned short audioFormat = 0; // 1 - PCM
        unsigned short numChannels = 0; // number oif channels
        unsigned int sampleRate = 0;
        unsigned int byteRate = 0; // (Sample Rate * BitsPerSample * Channels) / 8.
        unsigned short blockAlign = 0;
        unsigned short bitsPerSample = 0;
    } data;

    explicit FMTChunk(ChunkHeader header);

    FMTChunk(FMTChunk& chunk) {
        this->header = new ChunkHeader(*chunk.header);
        data = chunk.data;
    }

    char *getData() override {
        return (char *) &data;
    }

    unsigned int getDataSize() override {
        return header->size;
    }

    ChunkHeader* getHeader() override {
        return header;
    }

    ~FMTChunk() override {
        delete header;
    }

    FMTChunk& operator= (const FMTChunk& obj) {
        if (&obj == this)
            return *this;
        delete header;
        header = new ChunkHeader(*obj.header);
        data = obj.data;
        return *this;
    }

};

class UnknownChunk : public SomeChunk {
    ChunkHeader* header;
    char *data;
public:

    explicit UnknownChunk(ChunkHeader header) {
        this->header = new ChunkHeader(header);
        data = new char[header.size];
    }

    UnknownChunk(UnknownChunk& chunk) {
        this->header = new ChunkHeader(*chunk.header);
        this->data = new char[chunk.header->size];
        memcpy(data, chunk.data, chunk.header->size);
    }

    char *getData() override {
        return data;
    }

    unsigned int getDataSize() override {
        return header->size;
    }

    ChunkHeader* getHeader() override {
        return header;
    }

    ~UnknownChunk() override {
        delete[] data;
        delete header;
    }

    UnknownChunk& operator= (const UnknownChunk& obj) {
        if (&obj == this)
            return *this;
        delete header;
        header = new ChunkHeader(*obj.header);
        delete data;
        data = new char[obj.header->size];
        memcpy(data, obj.data, obj.header->size);
        return  *this;
    }

};


enum class ChunkType {
    RIFF, WAVE, DATA, FMT, OTHER
};

const char riffStr[4]{'R', 'I', 'F', 'F'};
const char waveStr[4] {'W', 'A', 'V', 'E'};
const char dataStr[4] {'d', 'a', 't', 'a'};
const char fmtStr[4] {'f', 'm', 't', ' '};

class WAVMetaData {
private:

    static bool compare4Str(const char *str1, const char *str2) {
        return str1[0] == str2[0] && str1[1] == str2[1]
               && str1[2] == str2[2] && str1[3] == str2[3];
    }

    FileHeader fileHeader{};
    FMTChunk *fmt = nullptr;
    std::vector<SomeChunk *> chunks;
    ChunkHeader dataHeader{{'d', 'a', 't', 'a'}, 0};

public:

    static ChunkType getType(ChunkHeader &header) {
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

    explicit WAVMetaData(FileHeader &header) : fileHeader(header) {
        if (!compare4Str((char *) header.chunkId, riffStr)) {
            throw std::invalid_argument("Wrong fmt id");
        }
        if (!compare4Str((char *) header.format, waveStr)) {
            throw std::invalid_argument("Wrong file format id");
        }
    }

    WAVMetaData() = default;

    unsigned int getBlockAlign() {
        return fmt->data.blockAlign;
    }

    unsigned int getSampleRate() {
        return fmt->data.sampleRate;
    }

    unsigned int getNumChannel() {
        return fmt->data.numChannels;
    }

    unsigned short getFormat() {
        return fmt->data.audioFormat;
    }

    FileHeader getFileHeader() {
        return fileHeader;
    }

    unsigned int getDataSize() {
        return fmt->getHeader()->size;
    }

    SomeChunk *createChunk(ChunkHeader chunkHeader) {
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

    std::vector<SomeChunk*> getChunks() {
        return chunks;
    }

    bool isCorrect() {
        return fmt != nullptr;
    }

    bool isSimilar(WAVMetaData &metaData) {
        return getNumChannel() == metaData.getNumChannel() &&
               getSampleRate() == metaData.getSampleRate() &&
               getBlockAlign() == metaData.getBlockAlign() &&
               getFormat() == metaData.getFormat();
    }

    ~WAVMetaData() {
        for (SomeChunk *&chunk: chunks) {
            delete chunk;
        }
    }

};
