#pragma once
#include <fstream>
#include "channel/WAVChannel.h"

class SoundEditor {
    WAVChannel* channel;
public:

    explicit SoundEditor(std::ifstream& file) noexcept(false);

    void mute(unsigned int start, unsigned int stop) noexcept(false);

    void mix(std::ifstream& file, unsigned int start) noexcept(false);

    void clip(unsigned int start, unsigned int stop) noexcept(false);

    void write(std::ofstream& out) noexcept(false);

    ~SoundEditor();

};
