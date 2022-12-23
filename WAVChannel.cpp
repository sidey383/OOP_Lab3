#include "WAVChannel.h"
#include "stdexcept"
#include "channel/WAVMute.cpp"
#include "channel/WAVMixer.cpp"

WAVMute* getMuteChannel(WAVChannel* channel, unsigned long timeStart, unsigned long timeStop) {
    WAVMetaData data = channel->getInfo();
    if (data.getNumChannel() == 1 && data.getBlockAlign() == 2) {
        return new WAVMute16_1(channel, timeStart, timeStop);
    }
    throw NotSupportedException();
}
