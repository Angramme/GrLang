#pragma once

#include "grComp/Logger.h"
#include <iostream>


#define TEST_FILE_START() \
int main() {\
	grComp::Log::PRINT_COLOR("\n====\nRunning tests...", grComp::Log::COLOR::DEFAULT); \
	std::string CATEGORY_NAME = ""; \
	unsigned long long TESTS_EXECUTED = 0; \
	unsigned long long TESTS_FAILED = 0; 


#define TEST_FILE_END() \
        if (TESTS_FAILED > 0) { \
            grComp::Log::PRINT_COLOR("\nFailed " +std::to_string(TESTS_FAILED) + " of " + std::to_string(TESTS_EXECUTED) + " tests!!!\n", grComp::Log::COLOR::red); \
            return 10; \
        } \
        \
        grComp::Log::PRINT_COLOR("\nPassed all " + std::to_string(TESTS_EXECUTED) + " tests with no errors...\n", grComp::Log::COLOR::green); \
        return 0; \
    }

#define TEST_CATEGORY_START(name) {\
	CATEGORY_NAME = #name; \
	grComp::Log::PRINT_COLOR("\nlaunching " #name " tests...\n", grComp::Log::COLOR::blue); \
	unsigned long long test_num = 1; \
	unsigned long long tests_failed = 0; \

#define TEST_CATEGORY_END() \
	if(tests_failed) grComp::Log::PRINT_COLOR(std::to_string(tests_failed) + " of " + std::to_string(test_num-1) + " " + CATEGORY_NAME + " tests failed...\n", grComp::Log::COLOR::red); \
	else grComp::Log::PRINT_COLOR("all " + CATEGORY_NAME + " tests passed...\n", grComp::Log::COLOR::green); }
	//if(tests_failed) std::cout << tests_failed << " of " << test_num-1 << " " << CATEGORY_NAME << " tests failed..." << std::endl; \
	//else std::cout << "all " << CATEGORY_NAME << " tests passed..." << std::endl; }

#define TEST_FUNCTION(func_call, result) {\
		auto val = func_call; \
		TESTS_EXECUTED += 1; \
		if (val != (result)) { \
			grComp::Log::PRINT_COLOR((\
			"\n====!!!{\ntest number " + std::to_string(test_num) + " failed! (line:" + std::to_string(__LINE__) + ")\n" \
			"\t\"" #func_call "\"\n" \
			"\texpected " #result "\n}====!!!\n"), grComp::Log::COLOR::red, stderr); \
			TESTS_FAILED += 1; \
			tests_failed += 1; \
		} \
		test_num++; \
	}
	//		std::cout << "\n====!!!{\ntest number " << test_num << " failed! (line:" << __LINE__ << ")" << std::endl; \
	//		std::cout << "\t\"" #func_call "\"" << std::endl; \
	//		std::cout << "\texpected " #result "\n}====!!!" << std::endl; \
	//		TESTS_FAILED += 1; \
	//		tests_failed += 1; \
	//	} \
	//	test_num++; \
	//}