#include "WAVReader.h"
#include "stdexcept"


WAVReader::WAVReader(std::ifstream &file) : file(file), pose(0) {
    try {
        FileHeader fileHeader{};
        if (file.read((char *) &fileHeader, sizeof(FileHeader)).gcount() < sizeof(FileHeader)) {
            throw std::invalid_argument("File too small");
        }
        data = WAVMetaData(fileHeader);

        ChunkHeader chunkHeader{};
        bool findData = false;

        do {
            if (file.read((char *) &chunkHeader, sizeof(ChunkHeader)).gcount() < sizeof(ChunkHeader)) {
                throw std::invalid_argument("File too small");
            }
            SomeChunk* chunk = data.createChunk(chunkHeader);
            if (chunk == nullptr) {
                findData = true;
                break;
            }
            char* chunkData = chunk->getData();
            if (file.read(chunkData, chunk->getDataSize()).gcount() < chunk->getDataSize()) {
                throw std::invalid_argument("File too small");
            }
        } while (file.peek() != EOF);

        if (!findData) {
            throw std::invalid_argument("No data in file");
        }
        if (!data.isCorrect()) {
            throw std::invalid_argument("No fmt chunk in file");
        }
    } catch (std::invalid_argument &e) {
        throw e;
    } catch (...) {
        throw std::exception();
    }
}

unsigned int WAVReader::getPose() {
    return pose;
}

unsigned int WAVReader::readSample(void *buff, unsigned int count) {
    unsigned int read = file.read((char*)buff, count * data.getBlockAlign()).gcount();
    pose += read / data.getBlockAlign();
    return read;
}

void WAVReader::skip(unsigned int  count) {
    file.ignore(count * data.getBlockAlign());
}

bool WAVReader::isEnd() {
    return file.peek() == EOF || !file.is_open();
}

WAVMetaData& WAVReader::getInfo() {
    return data;
}

void WAVReader::close() {
    file.close();
}

WAVReader::~WAVReader() {}