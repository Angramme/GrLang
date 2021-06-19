#include "grlangConfig.h"

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include "grComp/compile.h"


int main(int argc, char** argv) {

	// report version
	std::cout << "GRlang" << " Version " << grlang_VERSION_MAJOR << "."
		<< grlang_VERSION_MINOR << std::endl;


	if (argc >= 2) {
		grComp::CompileLLVM(argv[1]);
	}else{
		std::cout << "no arguments specified! terminating the program..." << std::endl;
	}

	std::cout << "\n>end" << std::endl;

	// std::cin.get();

	return 0;
}