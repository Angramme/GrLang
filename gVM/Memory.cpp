#include "Memory.h"

namespace gVM {
	StackMemory::StackMemory() 
		: m_size(0)
	{
		m_alloc(2048);
	}

	StackMemory::~StackMemory() {
		m_free();
	}

	void StackMemory::m_alloc(size_t size) {
		m_free();
		//m_buffer = reinterpret_cast<char*>(alloca(size));
		m_buffer = new char[size];
		m_size = size;
	}
	void StackMemory::m_free() {
		if (m_size != 0) {
			//_freea(m_buffer);
			delete[] m_buffer;
			m_size = 0;
		}
	}
}