#include "WAVexcepiont.h"

WAVException::WAVException(std::string reason) : reason(reason) {}

const char *WAVException::what() const noexcept {
    return reason.data();
}

int WAVException::getReturnCode() {
    return 3;
}

int WAVUnsupportedFormatException::getReturnCode() {
    return 4;
}

int WAVFileSizeException::getReturnCode() {
    return 5;
}

int WAVInvalidFormatException::getReturnCode() {
    return 6;
}
