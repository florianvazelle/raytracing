# ┌──────────────────────────────────────────────────────────────────┐
# │  Projects Settings                                               │
# └──────────────────────────────────────────────────────────────────┘

project("rtx")
cmake_minimum_required(VERSION 3.1)

option(RTX_WITH_TESTS "Compile and run rtx test executables" ON)
option(RTX_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -g")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Doing this makes testing easier, NanoGUI example* will all go to ./bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ┌──────────────────────────────────────────────────────────────────┐
# │  Build executable                                                │
# └──────────────────────────────────────────────────────────────────┘

# Find source files
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Include header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})

# ┌──────────────────────────────────────────────────────────────────┐
# │  Tests                                                           │
# └──────────────────────────────────────────────────────────────────┘

if(RTX_WITH_TESTS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/ext/googletest)

    enable_testing()

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_subdirectory(test)
endif()
