#pragma once
#include <vector>

struct FileHeader {
    char chunkId[4]; // "RIFF"
    unsigned int size; // size of file - 8 (byte)
    char format[4]; // "WAVE"
};

struct ChunkHeader {
    char id[4];
    unsigned int size;
};

class FileChunk {

public:

    virtual char *getData() = 0;

    virtual unsigned int getDataSize() {
        return getHeader()->size;
    }

    virtual ChunkHeader* getHeader() = 0;

    virtual ~FileChunk() = default;

    FileChunk& operator= (const FileChunk& obj) = default;

};

enum class ChunkType {
    RIFF, WAVE, DATA, FMT, OTHER
};

class FMTChunk : public FileChunk {
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

    FMTChunk(FMTChunk& chunk);

    char *getData() override;

    unsigned int getDataSize() override;

    ChunkHeader* getHeader() override;

    ~FMTChunk() override;

    FMTChunk& operator= (const FMTChunk& obj);

};

class UnknownChunk : public FileChunk {
    ChunkHeader* header;
    char *data;
public:

    explicit UnknownChunk(ChunkHeader header);

    UnknownChunk(UnknownChunk& chunk);

    char *getData() override;

    unsigned int getDataSize() override;

    ChunkHeader* getHeader() override;

    ~UnknownChunk() override;

    UnknownChunk& operator= (const UnknownChunk& obj);

};


const char riffStr[4]{'R', 'I', 'F', 'F'};
const char waveStr[4] {'W', 'A', 'V', 'E'};
const char dataStr[4] {'d', 'a', 't', 'a'};
const char fmtStr[4] {'f', 'm', 't', ' '};

class WAVMetaData {
private:

    static bool compare4Str(const char *str1, const char *str2);

    FileHeader fileHeader{};
    FMTChunk *fmt = nullptr;
    std::vector<FileChunk *> chunks;
    ChunkHeader dataHeader{{'d', 'a', 't', 'a'}, 0};

public:

    static ChunkType getType(ChunkHeader &header);

    explicit WAVMetaData(FileHeader &header);

    WAVMetaData(const WAVMetaData& origin);

    WAVMetaData& operator= (const WAVMetaData& origin);

    WAVMetaData() = default;

    unsigned int getBlockAlign();

    unsigned int getSampleRate();

    unsigned int getNumChannel();

    unsigned short getFormat();

    FileHeader getFileHeader();

    unsigned int getDataSize();

    ChunkHeader getDataHeader();

    FileChunk *createChunk(ChunkHeader chunkHeader);

    std::vector<FileChunk*> getChunks();

    bool isCorrect();

    bool isSimilar(WAVMetaData &metaData);

    ~WAVMetaData();

};
