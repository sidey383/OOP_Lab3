#include "WAVChannel.h"
#include "editor/WAVMute.h"
#include "editor/WAVMixer.h"
#include "editor/WAVClip.h"
#include "io/WAVWriter.h"
#include "io/WAVReader.h"
#include "../WAVexcepiont.h"

WAVChannel* getWAVReader(std::ifstream& out) noexcept(false) {
    return new WAVReader(out);
}

WAVWriter* getWAVWriter(WAVChannel* channel, std::ofstream& out) {
    return new WAVWriterDefault(channel, out);
}

WAVMute* getMuteChannel(WAVChannel* channel, unsigned long timeStart, unsigned long timeStop) {
    WAVMetaData data = channel->getInfo();
    if (data.getNumChannel() == 1 && data.getBlockAlign() == 2) {
        return new WAVMute16_1(channel, timeStart, timeStop);
    }
    throw WAVUnsupportedFormatException("Can't found muter for this channel");
}

WAVMixer* getMixerChannel(WAVChannel* first, WAVChannel* second, unsigned long timeStart) {
    WAVMetaData data = first->getInfo();
    WAVMetaData data2 = second->getInfo();
    if (data.getNumChannel() == 1 && data.getBlockAlign() == 2 &&
    data.getNumChannel() == data2.getNumChannel() && data.getBlockAlign() == data2.getBlockAlign()) {
        return new WAVMixer16_1(first, second, timeStart);
    }
    throw WAVUnsupportedFormatException("Can't found mixer for this channel");
}

WAVClip* getClipChannel(WAVChannel* channel, unsigned long timeStart, unsigned long timeStop) {
    WAVMetaData data = channel->getInfo();
    if (data.getNumChannel() == 1 && data.getBlockAlign() == 2) {
        return new WAVClip16_1(channel, timeStart, timeStop);
    }
    throw WAVUnsupportedFormatException("Can't found clipper for this channel");
}
