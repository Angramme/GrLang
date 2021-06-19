#pragma once

#include <string>
#include <filesystem>
#include "third-party/tl/expected.hpp"

namespace grComp {
	// tl::expected <grComp::ProgramMemory, grComp::Log::compile_error> compile_string(const std::string& pstring);

	void CompileLLVM(const std::filesystem::path& path);
}