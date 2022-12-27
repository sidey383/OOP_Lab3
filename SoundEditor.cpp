#include "SoundEditor.h"

SoundEditor::SoundEditor(std::ifstream &file) {
    channel = getWAVReader(file);
}

void SoundEditor::mute(unsigned int start, unsigned int stop) {
    if (channel == nullptr)
        throw std::logic_error("Wrong sound editor state state");
    channel = getMuteChannel(channel, start, stop);
}

void SoundEditor::mix(std::ifstream& file, unsigned int start) {
    if (channel == nullptr)
        throw std::logic_error("Wrong sound editor state state");
    WAVChannel* additional = getWAVReader(file);
    channel = getMixerChannel(channel, additional, start);
}

void SoundEditor::clip(unsigned int start, unsigned int stop) {
    if (channel == nullptr)
        throw std::logic_error("Wrong sound editor state state");
    channel = getClipChannel(channel, start, stop);
}

void SoundEditor::write(std::ofstream& out) {
    if (channel == nullptr)
        throw std::logic_error("Wrong sound editor state state");
    WAVWriter* writer = getWAVWriter(channel, out);
    writer->write();
    writer->close();
    channel = nullptr;
}

SoundEditor::~SoundEditor() {
    if (channel != nullptr)
        delete channel;
}
