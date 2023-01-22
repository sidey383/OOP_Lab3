#include "WAVReader.h"
#include "../../WAVexcepiont.h"


WAVReader::WAVReader(std::string& file_name) : pose(0), fileName(file_name) {
    file = std::ifstream(file_name, std::ios::binary);
    if (!file.is_open()) {
        throw WAVException(file_name + " file not found");
    }
    FileHeader fileHeader{};
    if (file.read((char *) &fileHeader, sizeof(FileHeader)).gcount() < sizeof(FileHeader)) {
        throw WAVFileSizeException("File too small, no file header");
    }
    data = WAVMetaData(fileHeader);

    ChunkHeader chunkHeader{};
    bool findData = false;

    do {
        if (file.read((char *) &chunkHeader, sizeof(ChunkHeader)).gcount() < sizeof(ChunkHeader)) {
            throw WAVFileSizeException("File too small, can't read file chunk");
        }
        FileChunk *chunk = data.createChunk(chunkHeader);
        if (chunk == nullptr) {
            findData = true;
            break;
        }
        char *chunkData = chunk->getData();
        if (file.read(chunkData, chunk->getDataSize()).gcount() < chunk->getDataSize()) {
            throw WAVFileSizeException("File too small, can't read block data");
        }
    } while (file.peek() != EOF);

    if (!findData) {
        throw WAVInvalidFormatException("No data in file");
    }
    if (!data.isCorrect()) {
        throw WAVInvalidFormatException("Corrupted fmt chunk in file");
    }
}

unsigned int WAVReader::getPose() {
    return pose;
}

unsigned int WAVReader::readSample(void *buff, unsigned int count) {
    if (isEnd())
        return 0;
    file.read((char *) buff, count * data.getBlockAlign());
    unsigned int read = file.gcount();
    pose += read / data.getBlockAlign();
    return read / data.getBlockAlign();
}

void WAVReader::skip(unsigned int count) {
    file.seekg(count * data.getBlockAlign(), std::ios::cur);
    pose += count;
}

bool WAVReader::isEnd() {
    if (!file.is_open())
        return true;
    if (file.peek() == EOF)
        return true;
    return false;
}

WAVMetaData &WAVReader::getInfo() {
    return data;
}

void WAVReader::close() {
    file.close();
}

WAVReader::~WAVReader() {}