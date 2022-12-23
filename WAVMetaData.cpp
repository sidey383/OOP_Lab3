#include "WAVMetaData.h"

FMTChunk::FMTChunk(ChunkHeader header) {
    if (WAVMetaData::getType(header) != ChunkType::FMT || header.size != sizeof(FMTChunkData)) {
        throw std::invalid_argument("Wrong chunk header");
    }
    this->header = new ChunkHeader(header);
}
