#pragma once

namespace wave {

    unsigned int dataToUnsignedInt(char data[4]);

    struct Header {
        unsigned char chunkId[4]; // "RIFF"
        unsigned char fileSize[4]; // size of file - 8 (byte)
        unsigned char format[4]; // "WAVE"
    };

    struct ChunkData {
        unsigned char id[4];
        unsigned char size[4];
    };

    struct FMTChunk {
        unsigned char audioFormat[2]; // 1 - PCM
        unsigned char numChannels[2]; // number oif channels
        unsigned char sampleRate[4];
        unsigned char byteRate[4]; // (Sample Rate * BitsPerSample * Channels) / 8.
        unsigned char blockAlign[2];
        unsigned char bitsPerSample[2];
    };

    enum class ChunkTypes {
        FMT,
        INFO,
        DATA
    };

    struct SoundMetaData {
        unsigned int sampleRate;
        unsigned int byteRate; // byte per seconds
        unsigned int size; // count of samples
        unsigned int blockSize; // size of one sample
    };

    class MonoSound {
    public:
        virtual unsigned int getSampleRate();

        virtual unsigned int getSampleSize();

        virtual unsigned int getPose();

        virtual unsigned int getData();

        virtual bool hasNext();

        virtual bool skipData(unsigned int count);

    };
}
