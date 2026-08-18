cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(prototype1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -O0")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)


set(SRCDIR ./src/)
set(HEADERDIR ./include/)
set(TESTDIR ./test/)

set(NET_SRCFILES 
	TcpConnection.cpp
	TcpSocket.cpp
	TcpCallee.cpp
	TcpCaller.cpp
	ConsoleLogger.cpp
	Resolver.cpp
)

set(NET_HEADERS
	ILogger.hpp
	ConsoleLogger.hpp
	IConnection.hpp
	ISocket.hpp
	TcpCallee.hpp
	TcpCaller.hpp
	TcpConnection.hpp
	TcpSocket.hpp
	Resolver.hpp
)

list(TRANSFORM NET_SRCFILES PREPEND ${SRCDIR})
list(TRANSFORM NET_HEADERS PREPEND ${HEADERDIR})

add_library(network OBJECT ${NET_SRCFILES} ${NET_HEADERS})
target_include_directories(network PRIVATE ${HEADERDIR})


set(TESTSRC
	test_network.cpp
	resolver_test.cpp
)
list(TRANSFORM TESTSRC PREPEND ${TESTDIR})

add_executable(network_test ${TESTDIR}maintest_gtest.cpp ${TESTSRC})
target_link_libraries(network_test network
	gtest_main gtest
)
target_include_directories(network_test PRIVATE ${HEADERDIR})

