#include "../WAVMetaData.h"

FMTChunk::FMTChunk(ChunkHeader header) {
    this->header = new ChunkHeader(header);
}

FMTChunk::FMTChunk(FMTChunk &chunk) {
    this->header = new ChunkHeader(*chunk.header);
    data = chunk.data;
}

char *FMTChunk::getData() {
    return (char *) &data;
}

unsigned int FMTChunk::getDataSize() {
    return header->size;
}

ChunkHeader *FMTChunk::getHeader() {
    return header;
}

FMTChunk::~FMTChunk() noexcept {
    delete header;
}

FMTChunk &FMTChunk::operator=(const FMTChunk &obj) {
    if (&obj == this)
        return *this;
    delete header;
    header = new ChunkHeader(*obj.header);
    data = obj.data;
    return *this;
}
