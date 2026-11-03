cmake_minimum_required(VERSION 3.10)
project(reactor)

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
	EventHandler.hpp
	IPoll.hpp
	IInvoker.hpp
	#Poll.hpp
	NodeInitializer.hpp
	MessageRouter.hpp
	AcceptorSocket.hpp
	Reactor.hpp
	Invoker.hpp
	Demultiplexer.hpp
	AcceptorNode.hpp
	EchoRouter.hpp
	IWaiter.hpp
	EPollWaiter.hpp
)

set(SOURCES
	MessageRouter.cpp
	AcceptorSocket.cpp
	NodeInitializer.cpp
	#Poll.cpp
	Reactor.cpp
	Invoker.cpp
	Demultiplexer.cpp
	AcceptorNode.cpp
	EchoRouter.cpp
	EPollWaiter.cpp
)

list(TRANSFORM HEADERS PREPEND ${INCLUDEDIR})
list(TRANSFORM SOURCES PREPEND ${SOURCEDIR})


add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC networking)
target_link_libraries(${PROJECT_NAME} PUBLIC decorator)
target_link_libraries(${PROJECT_NAME} PUBLIC nodesession)
target_link_libraries(${PROJECT_NAME} PUBLIC common)


