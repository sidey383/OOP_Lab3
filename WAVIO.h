#pragma once
#include "WAVEditor.h"
#include <fstream>
#include <iostream>

namespace wave {

    class FileReader : public  wave::MonoSound {
        std::ifstream& file;
    public:

        FileReader(std::ifstream& file) : file(file) {}

        virtual unsigned int getSampleRate() {

        }

        virtual unsigned int getSampleSize();

        virtual unsigned int getPose();

        virtual unsigned int getData();

        virtual bool hasNext();

        virtual bool skipData(unsigned int count);
    };

}
