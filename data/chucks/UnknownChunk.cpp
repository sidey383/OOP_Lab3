#include "../WAVMetaData.h"
#include "cstring"

UnknownChunk::UnknownChunk(ChunkHeader header) {
    this->header = new ChunkHeader(header);
    data = new char[header.size];
}

UnknownChunk::UnknownChunk(UnknownChunk &chunk) {
    this->header = new ChunkHeader(*chunk.header);
    this->data = new char[chunk.header->size];
    memcpy(data, chunk.data, chunk.header->size);
}

char *UnknownChunk::getData() {
    return data;
}

unsigned int UnknownChunk::getDataSize() {
    return header->size;
}

ChunkHeader *UnknownChunk::getHeader() {
    return header;
}

UnknownChunk::~UnknownChunk() noexcept {
    delete[] data;
    delete header;
}

UnknownChunk &UnknownChunk::operator=(const UnknownChunk &obj) {
    if (&obj == this)
        return *this;
    delete header;
    header = new ChunkHeader(*obj.header);
    delete data;
    data = new char[obj.header->size];
    memcpy(data, obj.data, obj.header->size);
    return  *this;
}


