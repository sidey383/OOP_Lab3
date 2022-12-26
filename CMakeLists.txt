cmake_minimum_required(VERSION 3.22)
project(OOP_Lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(OOP_Lab3 main.cpp channel/io/WAVReader.cpp channel/WAVChannel.cpp channel/io/WAVWriter.cpp data/WAVMetaData.cpp channel/editor/WAVMute.h data/chucks/FMTChunk.cpp data/chucks/UnknownChunk.cpp channel/editor/WAVClip.h channel/editor/WAVClip.cpp)
