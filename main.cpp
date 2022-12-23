#include "WAVReader.h"
#include "WAVWriter.h"

int main() {
    std::ifstream input("input.wav", std::ios::binary);
    if (! input.is_open() ) {
        return 1;
    }
    std::ofstream output("output.wav", std::ios::binary);
    if (!input.is_open() || !output.is_open()) {
        return 1;
    }
    WAVChannel* reader = new WAVReader(input);
    reader = getMuteChannel(reader, 10, 50);
    WAVWriter* writer = new WAVWriter( reader ,output);
    writer->write();
    reader->close();
    writer->close();
    return 0;
}
