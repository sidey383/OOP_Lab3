#include "WAVReader.h"
#include "WAVWriter.h"

int main() {
    std::ifstream input;
    input.open("input.wav");
    if (! input.is_open() ) {
        return 1;
    }
    std::ofstream output;
    output.open("output.wav");
    WAVReader reader(input);
    WAVWriter writer( reader ,output);
    writer.write();
    reader.close();
    writer.close();
    return 0;
}
