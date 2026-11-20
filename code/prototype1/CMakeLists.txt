cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(prototype1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "\
    -O0\
    -Wall\
    -Wextra\
    -Wno-error=maybe-uninitialized\
    -Wcast-align\
    -Winvalid-pch\
    -Wmissing-include-dirs\
	-g\
")


set(SRCDIR ./src/)
set(HEADERDIR ./include/)

set(HEADERS
)

set(SOURCES
	main.cpp
)

list(TRANSFORM HEADERS PREPEND ${HEADERDIR})
list(TRANSFORM SOURCES PREPEND ${SRCDIR})

add_subdirectory(lib/common)
add_subdirectory(lib/networking)
add_subdirectory(lib/decorator)
add_subdirectory(lib/nodesession)
add_subdirectory(lib/reactor)
add_subdirectory(lib/cons_app)
add_subdirectory(lib/SQLiteCpp)
add_subdirectory(test)

add_executable(${PROJECT_NAME}  ${SOURCES} ${HEADERS})

add_executable(client src/main_client.cpp)

target_link_libraries(${PROJECT_NAME}
        common
        networking
        decorator
        nodesession
        reactor
        GTest::gtest_main
        GTest::gmock
)

target_link_libraries(client
        common
        networking
        decorator
        nodesession
        reactor
        GTest::gtest_main
        GTest::gmock
)
