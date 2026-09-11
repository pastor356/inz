cmake_minimum_required(VERSION 3.10)
project(networking)

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
	IConnection.hpp
	ISocket.hpp
	Resolver.hpp
	TcpCallee.hpp
	TcpCaller.hpp
	TcpConnection.hpp
	TcpSocket.hpp
)

set(SOURCES
	Resolver.cpp
	TcpCallee.cpp
	TcpCaller.cpp
	TcpConnection.cpp
	TcpSocket.cpp
)

list(TRANSFORM HEADERS PREPEND ${INCLUDEDIR})
list(TRANSFORM SOURCES PREPEND ${SOURCEDIR})


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC common)
