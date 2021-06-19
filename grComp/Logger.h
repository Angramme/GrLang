#pragma once

#include <string>

namespace grComp {
	namespace Log {
		enum COLOR {
			DEFAULT=0, red = 1, green = 2, blue, yellow, magenta, cyan, black, white
		};
		void PRINT_COLOR(const char* msg, const COLOR& color, FILE* STDOUT = stdout);
		void PRINT_COLOR(const std::string& str, const COLOR& color, FILE* STDOUT = stdout);

		void InternalRuntimeError(std::string msg, const char* in_file, const int on_line, const char* in_function);
		void CompiletimeError(std::string msg);
		void CompiletimeInfo(std::string msg);

#define GRCOMP_INTERNAL_RUNTIME_ERROR(msg) grComp::Log::InternalRuntimeError((msg), __FILE__, __LINE__, __func__);
	}
}