cmake_minimum_required(VERSION 3.10)
project(common)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "\
        -O0\
        -Wall\
        -Wextra\
        -Wunused\
        -Wno-error=maybe-uninitialized\
        -Wcast-align\
        -Winvalid-pch\
        -Wmissing-include-dirs\
        -g\
")


set(INCLUDEDIR include/)
set(SOURCEDIR src/)

set(HEADERS
	ILogger.hpp
	ConsoleLogger.hpp
	Packet.hpp
)

set(SOURCES
	ConsoleLogger.cpp
)

list(TRANSFORM HEADERS PREPEND ${INCLUDEDIR})
list(TRANSFORM SOURCES PREPEND ${SOURCEDIR})

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEDIR})
