#include <string>

namespace grComp {
	namespace Log {
		void InternalRuntimeError(std::string msg, const char* in_file, const int on_line, const char* in_function);
		void CompiletimeError(std::string msg);
		void CompiletimeInfo(std::string msg);

#define GRCOMP_INTERNAL_RUNTIME_ERROR(msg) grComp::Log::InternalRuntimeError((msg), __FILE__, __LINE__, __func__);
	}
}