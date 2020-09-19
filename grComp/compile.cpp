#include "compile.h"

#include <unordered_map>
//#include <sstream>
//#include <regex>
//#include <cctype> //isspace
//#include <array>
#include <string>
#include <sstream>

//#include "Instructions.h"
//#include "BaseTypes.h"
#include "ProgramMemory.h"

#include "Lexer.h"

namespace grComp {
	tl::expected <grComp::ProgramMemory, grComp::compile_error> compile_string(const std::string& pstring) {
		std::istringstream sstream(pstring);
		grComp::Lexer lexer = grComp::Lexer(&sstream);

		


		return ProgramMemory();

		//USE BISON and FLEX  /  ANTLR

		//grComp::ProgramMemory program;
		//std::unordered_map<std::string, maddress> symbols;

		//size_t i = 0;
		//size_t n = pstring.size();
		//while (i < n) {
		//	auto word = nextWord(pstring.data(), n, i);
		//	if (word[1] == 0)break;
		//	
		//	//function


		//	i = word[0] + word[1];
		//}

		//return program;
	}

	/*
	grComp::ProgramMemory compile_string(const std::string& pstring) {
		grComp::ProgramMemory program;
		std::unordered_map<std::string, maddress> symbols;

		std::string s = pstring;
		
		{ //FIND ALL FUNCTIONS
			//function signature
			std::regex reg;
			try {
				reg = "([a-zA-Z]+[a-zA-Z0-9]*) " //function name
					//arguments
					"(([a-zA-Z]+[a-zA-Z0-9]*:[a-zA-Z]+[a-zA-Z0-9]* )+|_ )" 
					"=>" //syntax
					//return values
					"(( [a-zA-Z]+[a-zA-Z0-9]*:[a-zA-Z]+[a-zA-Z0-9]*)+| _)"
					"\\s*"
					//function body
					"(\\{(.|\n)*\\})"; 
			}catch (const std::regex_error& e) {
				std::cout << "regex_error caught: " << e.what() << '\n';
			}

			//empty
			const std::regex_token_iterator<std::string::iterator> rend;
			//find
			const int submatches[] = { 1, 2, 3, 4 };
			std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), reg, submatches);
			//repeat until empty
			while (a != rend) {
				std::string fname = *a++;
				std::string args = *a++;
				std::string returns = *a++;
				std::string body = *a++;

				std::cout << "function: " << fname << std::endl
					<< "arguments: \n" << args << std::endl
					<< "return values: \n" << returns << std::endl
					<< "body: \n" << body << std::endl;
			}
		}

		return program;
	}

	grComp::ProgramMemory compile_function(const std::string& pstring){
		//(...)

		std::istringstream pstream(pstring);
		std::string line;

		while (std::getline(pstream, line)) {
			std::cout << "line: " << line << std::endl;
		}

		//(...)

		return {};
	}
	*/
}