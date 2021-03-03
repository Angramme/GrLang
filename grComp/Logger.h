#include <string>

namespace grComp {
	namespace Log {
		void InternalRuntimeError(std::string msg);
		void CompiletimeError(std::string msg);
		void CompiletimeInfo(std::string msg);
	}
}