cmake_minimum_required(VERSION 3.10)
project(tests)

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


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

set(UTESTDIR utest/)
set(MOCKSDIR mocks/)

set(UTESTSOURCE 
	decorator_test.cpp
	maintest_gtest.cpp
	resolver_test.cpp
	test_network.cpp
)

set(MOCKHEADERS
	ConnectorMock.hpp
)

list(TRANSFORM UTESTSOURCE PREPEND ${UTESTDIR})
list(TRANSFORM MOCKHEADERS PREPEND ${MOCKSDIR})

add_executable(${PROJECT_NAME} ${UTESTDIR}maintest_gtest.cpp ${UTESTSOURCE} ${MOCKHEADERS})
target_link_libraries(${PROJECT_NAME}
	common
	networking
	decorator
	GTest::gtest_main
)
