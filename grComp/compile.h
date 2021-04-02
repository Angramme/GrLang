#pragma once

#include <string>
#include "gVM/ProgramMemory.h"
#include "third-party/tl/expected.hpp"

namespace grComp {
	struct compile_error
	{
		enum { SYNTAX, UNDECLARED_IDENTIFIER } code;
		unsigned long long line_num;
		std::string line;
		std::string file;
		std::string message;
	};
	tl::expected <grComp::ProgramMemory, compile_error> compile_string(const std::string& pstring);
}