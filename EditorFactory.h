#pragma once
#include "channel/WAVChannel.h"
#include "ConfigParser.h"


class EditorFactory {
public:
    WAVChannel* getChannel(SoundAction& action, WAVChannel* oldChannel);

    WAVWriter* getWriter(WAVChannel* channel, std::string& file);

    static std::string getConfigCommands();

};
