cmake_minimum_required(VERSION 3.10)
project(decorator)

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
	ConnectionDecorator.hpp
	IQualityStrategy.hpp
	LiveMonitor.hpp
	Qos1.hpp
)

set(SOURCES
	ConnectionDecorator.cpp
	LiveMonitor.cpp
	Qos1.cpp
)


list(TRANSFORM HEADERS PREPEND ${INCLUDEDIR})
list(TRANSFORM SOURCES PREPEND ${SOURCEDIR})

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PUBLIC networking)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEDIR})
