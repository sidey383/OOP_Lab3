#include "WAVReader.h"
#include "stdexcept"

bool compare4Str(char* str1, char* str2) {
    return str1[0] == str2[0] && str1[1] == str2[1]
    && str1[2] == str2[2] && str1[3] == str2[3];
}


WAVReader::WAVReader(std::ifstream &file) : file(file), pose(0) {
    try {
        if ( file.read((char *) &data.header, sizeof(Header)).gcount() < sizeof(Header)) {
            throw std::invalid_argument("File too small");
        }
        if (!compare4Str((char *) data.header.chunkId, chunkHeaders.riff)) {
            throw std::invalid_argument("Wrong fmt id");
        }
        if (!compare4Str((char *) data.header.format, chunkHeaders.wave)) {
            throw std::invalid_argument("Wrong file format id");
        }
        ChunkHeader chunkHeader;
        do {
            if( file.read((char *) &chunkHeader, sizeof(ChunkHeader)).gcount() < sizeof(ChunkHeader))
                throw new std::invalid_argument("File too small");

            if (compare4Str(chunkHeader.id, chunkHeaders.data))
                break;
            if (readFMT(chunkHeader, file))
                continue;

            readOther(chunkHeader, file);
        } while (file.peek() != EOF);

        if (!compare4Str(chunkHeader.id, chunkHeaders.data)) {
            throw std::invalid_argument("No sound data in file");
        }
        if (data.fmt.audioFormat == 0) {
            throw std::invalid_argument("No fmt chunk in file");
        }

        data.sampleCount = chunkHeader.size / data.fmt.blockAlign;
    } catch (std::invalid_argument &e) {
        throw e;
    } catch (...) {
        throw std::exception();
    }
}

bool WAVReader::readFMT(ChunkHeader chunkHeader, std::ifstream &file) {
    if (compare4Str(chunkHeader.id, chunkHeaders.fmt)) {
        if (chunkHeader.size != 16) {
            throw new std::invalid_argument("wrong fmt fmt size");
        }
        file.read((char*)&data.fmt, chunkHeader.size);
        return true;
    }
    return false;
}

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
    unsigned int read = file.read((char*)buff, count * data.fmt.blockAlign).gcount();
    pose += read / data.fmt.blockAlign;
    return read;
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