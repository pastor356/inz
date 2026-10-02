cmake_minimum_required(VERSION 3.10)
project(nodesession)

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
	DatabaseCommunicator.hpp
	Handshaker.hpp
	IConnectionHandler.hpp
	IStateContext.hpp
	IStateFactory.hpp
	MonitoredNode.hpp
	SessionBuilder.hpp
	SessionConfig.hpp
	SessionFactory.hpp
	Session.hpp
	SessionRegistry.hpp
	CacheAccessor.hpp
)

set(SOURCES
	Handshaker.cpp
	MonitoredNode.cpp
	SessionBuilder.cpp
	Session.cpp
	#SessionFactory.cpp
	SessionRegistry.cpp
	CacheAccessor.cpp
)

list(TRANSFORM HEADERS PREPEND ${INCLUDEDIR})
list(TRANSFORM SOURCES PREPEND ${SOURCEDIR})


add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC networking)
target_link_libraries(${PROJECT_NAME} PUBLIC decorator)
target_link_libraries(${PROJECT_NAME} PUBLIC common)


