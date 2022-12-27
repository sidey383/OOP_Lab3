#pragma once
#include "exception"
#include "string"

class WAVException : public std::exception {
protected:
    std::string reason;
public:

    explicit WAVException(std::string reason);

    const char * what() const noexcept override;

    virtual int getReturnCode();

};

class WAVFileSizeException : public WAVException {
public:
    explicit WAVFileSizeException(std::string reason) : WAVException(reason) {};

    int getReturnCode() override;

};

class WAVUnsupportedFormatException : public WAVException {
public:
    explicit WAVUnsupportedFormatException(std::string reason) : WAVException(reason) {};

    int getReturnCode() override;

};

class WAVInvalidFormatException : public WAVException {
public:
    explicit WAVInvalidFormatException(std::string reason) : WAVException(reason) {};

    int getReturnCode() override;

};
