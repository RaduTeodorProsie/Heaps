cmake_minimum_required(VERSION 3.20) # Bumped slightly for target_link_options, though 3.13+ is sufficient
project(Heaps CXX) # Added CXX language specifier

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Good practice
set(CMAKE_CXX_EXTENSIONS OFF)       # Good practice, if you don't rely on extensions

# Include directories
# Using target_include_directories is generally preferred over include_directories
# include_directories(include) # Keep this if you prefer, but see below

set(SOURCES
        main.cpp

        src/quaternary.cpp
        src/pairing.cpp
        src/Rheap.cpp
        src/priority_queue.cpp
)

# Executable target
add_executable(Heaps ${SOURCES})

# --- Preferred way to handle include directories ---
target_include_directories(Heaps PRIVATE include)

# --- Set Stack Size (512 MB = 536,870,912 bytes) ---
set(DESIRED_STACK_SIZE_BYTES 536870912)

message(STATUS "Attempting to set stack size for target 'Heaps' to ${DESIRED_STACK_SIZE_BYTES} bytes.")

# Check which compiler/linker is being used
if(MSVC)
    # MSVC Linker Flag: /STACK:<reserve>
    message(STATUS "Using MSVC linker flag: /STACK:${DESIRED_STACK_SIZE_BYTES}")
    target_link_options(Heaps PRIVATE "/STACK:${DESIRED_STACK_SIZE_BYTES}")
else()
    # GCC/Clang Linker Flag: -Wl,--stack,<size>
    message(STATUS "Using GCC/Clang linker flag: -Wl,--stack,${DESIRED_STACK_SIZE_BYTES}")
    target_link_options(Heaps PRIVATE "-Wl,--stack,${DESIRED_STACK_SIZE_BYTES}")
endif()
