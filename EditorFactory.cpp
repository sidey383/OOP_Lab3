#include "EditorFactory.h"
#include "WAVexcepiont.h"

static WAVChannel* read(SoundAction &action, WAVChannel *oldChannel) {
    if (action.files.size() == 0) {
        throw WAVInvalidFormatException("No input file");
    }
    delete oldChannel;
    return getWAVReader(action.files[0]);
}


static WAVChannel* mute(SoundAction &action, WAVChannel *oldChannel) {
    if(action.params.size() >= 2) {
        int start, stop = 0;
        try {
            start = std::stoi(action.params[0]);
            stop = std::stoi(action.params[1]);
        } catch (...) {
            throw WAVUnsupportedFormatException("Mute accepts two numbers. Can't convert to numbers");
        }
        return getMuteChannel(oldChannel, start, stop);
    } else {
        throw WAVUnsupportedFormatException("Mute accepts two numbers. Not enough parameters");
    }
}


static WAVChannel* mix(SoundAction &action, WAVChannel *oldChannel) {
    if (action.files.size() == 0) {
        throw WAVInvalidFormatException("A file is required for mix");
    }
    if (action.params.size() == 0) {
        throw WAVInvalidFormatException("A number is required for mix");
    }
    unsigned int start = 0;
    try {
        start = std::stoi(action.params[0]);
    } catch (...) {
        throw WAVInvalidFormatException("A number is required for mix. Can't convert string to number");
    }
    WAVChannel* channel = getWAVReader(action.files[0]);
    getMixerChannel(oldChannel, channel, start);
}

static WAVChannel* clip(SoundAction &action, WAVChannel *oldChannel) {
    if(action.params.size() >= 2) {
        int start, stop = 0;
        try {
            start = std::stoi(action.params[0]);
            stop = std::stoi(action.params[1]);
        } catch (...) {
            throw WAVUnsupportedFormatException("Clip accepts two numbers. Can't convert strings to numbers");
        }
        return getClipChannel(oldChannel, start, stop);
    } else {
        throw WAVUnsupportedFormatException("Clip accepts two numbers. Not enough parameters");
    }
}

WAVChannel *EditorFactory::getChannel(SoundAction &action, WAVChannel *oldChannel) {
    if(action.name == "read") {
        return read(action, oldChannel);
    }
    if(action.name == "mute") {
        return mute(action, oldChannel);
    }
    if(action.name == "mix") {
        return mix(action, oldChannel);
    }
    if(action.name == "clip") {
        return clip(action, oldChannel);
    }
    return nullptr;
}

std::string EditorFactory::getConfigCommands() {
    return "Sound Actions:\nmute <start> <stop>\nclip <start> <stop>\nmix $<fileNumber> <start>";
}

WAVWriter *EditorFactory::getWriter(WAVChannel *channel, std::string& file) {
    return getWAVWriter(channel, file);
}
