cmake_minimum_required(VERSION 3.22)
project(OOP_Lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(OOP_Lab3
        channel/WAVChannel.cpp
        data/WAVMetaData.cpp
        data/chucks/FMTChunk.cpp
        data/chucks/UnknownChunk.cpp
        channel/io/WAVReader.cpp
        channel/io/WAVWriter.cpp
        channel/editor/WAVClip.cpp
        channel/editor/WAVMute.cpp
        channel/editor/WAVMixer.cpp
        main.cpp WAVexcepiont.cpp ConfigParser.cpp EditorFactory.cpp)
