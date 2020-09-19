#include "grlangConfig.h"
#include <iostream>

#include "core/BaseTypes.h"
#include "gVM/execute.h"

#include "grComp/ProgramMemory.h"
#include "grComp/compile.h"

//file loading
#include <string>
#include <fstream>
#include <streambuf>


int main(int argc, char** argv) {

	// report version
	std::cout << "GRlang" << " Version " << grlang_VERSION_MAJOR << "."
		<< grlang_VERSION_MINOR << std::endl;
	
	/*{ //TEST 1
		gVM::StackMemory gmem = gVM::StackMemory();
		
		grComp::ProgramMemory program = grComp::ProgramMemory();
		program
			<< grCore::SET << grCore::maddress(1) << (char)80
			<< grCore::PRINTschar
			<< grCore::MOVE << grCore::maddress(1)
			<< grCore::SET << grCore::maddress(1) << (char)8
			<< grCore::PRINTschar
			<< grCore::ADDcharA << grCore::maddress(0)
			<< grCore::PRINTschar
			<< grCore::PRINTascii << (unsigned)1;
			

		std::cout << "pgm size: " << program.get_size() << std::endl;
		std::cout << "pgm alloc size: " << program.get_heap_size() << std::endl;
		std::cout << "starting execution..." << std::endl;

		gVM::execute_program(program.get_data(), program.get_size(), 0, &gmem);
	}*/


	{ //TEST 2
		//load file to string
		std::string str;
		{
			std::ifstream t("fff.txt");

			if (!t.good()) {
				std::cerr << "ERROR, cannot open file!" << std::endl;
				std::cin.get();
				return 0;
			}

			t.seekg(0, std::ios::end);
			str.reserve(t.tellg());
			t.seekg(0, std::ios::beg);

			str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
		}

		//grComp::ProgramMemory prgm = grComp::compile_string(str);
		std::cout << "compiling:\n" << str << std::endl;
		grComp::compile_string(str);
		
		//execute...
	}


	std::cout << "\n\nprogram ended" << std::endl;

	std::cin.get();

	return 0;
}