#include "compile.h"

#include <unordered_map>
//#include <sstream>
//#include <regex>
//#include <cctype> //isspace
//#include <array>
#include <string>
#include <fstream>

//#include "Instructions.h"
//#include "BaseTypes.h"

//#include "Lexer.h"
//#include "Parser.h"
#include "Generator_llvm.h"
//#include "Pipe.h"
#include "Logger.h"


// debugging mess
#include <iostream>
#include <sstream>

namespace grComp {

	namespace fs = std::filesystem;

	/*tl::expected <grComp::ProgramMemory, grComp::Log::compile_error> compile_string() {
		std::istringstream sstream(pstring);
		grComp::Lexer lexer = grComp::Lexer(&sstream);




		return ProgramMemory();

	}*/

	void CompileLLVM(const fs::path& path) {
		if (!fs::exists(path) || !fs::is_regular_file(path)) {
			Log::CompiletimeError("Error: file \"" + path.generic_string() + "\" not found! \
				( It either does not exist or is not a valid file. )");
			return;
		}

		// auto my_str_stream = std::make_unique<std::istringstream>("MAIN a:R b=R => a * (a + b)");

		grComp::GeneratorLLVM pipe(
			std::make_unique<grComp::Parser>(
			std::make_unique<grComp::Lexer>(
			std::make_unique<grComp::Reader>(path))));
			// std::make_unique<grComp::Reader>(std::move(my_str_stream)))));

		pipe.generate();
	}
}