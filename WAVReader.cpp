#include "WAVReader.h"
#include "stdexcept"

bool compare4Str(char* str1, char* str2) {
    return str1[0] == str2[0] && str1[1] == str2[1]
    && str1[2] == str2[2] && str1[3] == str2[3];
}

static char riffStr[4]{'R','I','F','F'};
static char waveStr[4]{'W','A','V','E'};
static char dataStr[4]{'d','a','t','a'};

WAVReader::WAVReader(std::ifstream &file) : file(file), pose(0) {
    try {
        Header header;
        if ( file.read((char *) &header, sizeof(Header)).gcount() < sizeof(Header)) {
            throw new std::invalid_argument("File too small");
        }
        if (!compare4Str((char *) header.chunkId, riffStr)) {
            throw new std::invalid_argument("Wrong fmt id");
        }
        if (!compare4Str((char *) header.format, waveStr)) {
            throw new std::invalid_argument("Wrong file format id");
        }
        data.riffSize = header.size;
        ChunkHeader chunkHeader;
        do {
            if( file.read((char *) &chunkHeader, sizeof(ChunkHeader)).gcount() < sizeof(ChunkHeader)) {
                throw new std::invalid_argument("File too small");
            }
            if (compare4Str(chunkHeader.id, dataStr))
                break;
            if (readFMT(chunkHeader, file))
                continue;
            readOther(chunkHeader, file);
        } while (file.peek() != EOF);

        if (!compare4Str(chunkHeader.id, dataStr)) {
            throw new std::invalid_argument("No sound data in file");
        }
        if (data.fmt.audioFormat == 0) {
            throw new std::invalid_argument("No fmt chunk in file");
        }

        data.sampleCount = chunkHeader.size / data.fmt.blockAlign;
    } catch (std::invalid_argument &e) {
        throw e;
    } catch (...) {
        throw new std::exception();
    }
}

static char fmtStr[4]{'f','m','t',' '};

bool WAVReader::readFMT(ChunkHeader chunkHeader, std::ifstream &file) {
    if (compare4Str(chunkHeader.id, fmtStr)) {
        if (chunkHeader.size != 16) {
            throw new std::invalid_argument("wrong fmt fmt size");
        }
        file.read((char*)&data.fmt, chunkHeader.size);
        return true;
    }
    return false;
}

static char listStr[4]{'L','I','S','T'};

bool WAVReader::readOther(ChunkHeader chunkHeader, std::ifstream &file) {
    SomeChunk* chunk = new SomeChunk(chunkHeader ,chunkHeader.size);
    file.read(chunk->data, chunkHeader.size);
    data.otherChunks.push_back(chunk);
    return false;
}

unsigned int WAVReader::getPose() {
    return pose;
}

unsigned int WAVReader::readSample(void *buff, size_t count) {
    return file.read((char*)buff, count * data.fmt.blockAlign).gcount();
}

void WAVReader::skip(size_t count) {
    file.ignore(count * data.fmt.blockAlign);
}

bool WAVReader::isEnd() {
    return file.peek() == EOF || !file.is_open();
}

WAVMetaData WAVReader::getInfo() {
    return data;
}

void WAVReader::close() {
    file.close();
}

WAVReader::~WAVReader() {
    for (SomeChunk* chunk : data.otherChunks) {
        delete[] chunk;
    }
    data.otherChunks.clear();
}