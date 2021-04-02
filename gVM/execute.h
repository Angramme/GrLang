#pragma once

#include "Memory.h"
#include "core/Instructions.h"

namespace gVM {
	void execute_program(char* instructions, size_t length, size_t start_at, StackMemory* smem);
}