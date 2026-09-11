cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(prototype1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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


set(SRCDIR ./src/)
set(HEADERDIR ./include/)

set(HEADERS
	ILogger.hpp
	ConsoleLogger.hpp
)

set(SOURCES
	ConsoleLogger.cpp
)

list(TRANSFORM HEADERS PREPEND ${HEADERDIR})
list(TRANSFORM SOURCES PREPEND ${SRCDIR})

add_subdirectory(lib/common)
add_subdirectory(lib/networking)
add_subdirectory(lib/decorator)
add_subdirectory(test)

