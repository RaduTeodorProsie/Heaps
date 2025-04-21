cmake_minimum_required(VERSION 3.30)
project(Heaps)

set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories(include)

set(SOURCES
        main.cpp

        src/quaternary.cpp
        src/pairing.cpp
        src/Rheap.cpp
        src/priority_queue.cpp
)


# Executable target
add_executable(Heaps ${SOURCES})
