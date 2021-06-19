#pragma once

#include <iostream>
#include "../core/BaseTypes.h"

namespace gVM {

	using maddress = grCore::maddress;

	class StackMemory {
	public:
		StackMemory();
		~StackMemory();

		inline void* at(maddress i) { return m_buffer+i; }
		inline void setat(maddress i, void* data, maddress len) { memcpy(m_buffer + i, data, (size_t)len); }

	private:
		void m_alloc(size_t size);
		void m_free();

		size_t m_size;
		char* m_buffer;
	};
}