cmake_minimum_required(VERSION 3.10)
project(cons_app)

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
	MethodRequest.hpp
	ActiveQueue.hpp
	Scheduler.hpp
	DatabaseAccessor.hpp
	AppendRequest.hpp
)

set(SOURCES
	ActiveQueue.cpp
	Scheduler.cpp
	AppendRequest.cpp
	DatabaseAccessor.cpp
)

list(TRANSFORM HEADERS PREPEND ${INCLUDEDIR})
list(TRANSFORM SOURCES PREPEND ${SOURCEDIR})

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
add_subdirectory(influxdb-cxx)


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDEDIR})
target_include_directories(${PROJECT_NAME} PUBLIC influxdb-cxx/include)

target_link_libraries(${PROJECT_NAME} PUBLIC networking)
target_link_libraries(${PROJECT_NAME} PUBLIC decorator)
target_link_libraries(${PROJECT_NAME} PUBLIC nodesession)
target_link_libraries(${PROJECT_NAME} PUBLIC common)
target_link_libraries(${PROJECT_NAME} PUBLIC reactor)


add_executable(consApp src/main.cpp ${SOURCES} ${HEADERS})
target_include_directories(consApp PUBLIC ${INCLUDEDIR})
target_include_directories(consApp PUBLIC influxdb-cxx/include)
target_compile_features(consApp PUBLIC cxx_std_20)

target_link_libraries(consApp PUBLIC networking)
target_link_libraries(consApp PUBLIC decorator)
target_link_libraries(consApp PUBLIC nodesession)
target_link_libraries(consApp PUBLIC common)
target_link_libraries(consApp PUBLIC reactor)
target_link_libraries(consApp PUBLIC InfluxDB)
