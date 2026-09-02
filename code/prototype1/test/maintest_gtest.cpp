#include <gtest/gtest.h>
#include "ConsoleLogger.hpp"

int main(int argc, char* argv[]) {
	enableLogger();
	configureLogger("Test");

	testing::InitGoogleTest(&argc, argv);
	return(RUN_ALL_TESTS());
}

	


