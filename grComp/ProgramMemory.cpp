#include "ProgramMemory.h"
#include <memory>

namespace grComp {
	ProgramMemory::ProgramMemory()
		: m_size(0), m_data_size(0), m_data(nullptr)
	{}

	ProgramMemory::ProgramMemory(size_t init_size)
		: m_size(0), m_data_size(0)
	{
		m_size = init_size;
		//SECURITY WARNING: adapt this for long long!!!
		m_data_size = bigger_pow2(init_size);
		m_data = new char[m_data_size];
	}

	ProgramMemory::~ProgramMemory() {
		m_free();
	}
	
	void ProgramMemory::m_realloc(size_t size) {
		if (m_data_size > size)throw 0;
		if (m_data_size != 0) {
			void* m_prev_data = m_data;
			m_data = new char[size];

			memcpy(m_data, m_prev_data, m_size);
			delete[] m_prev_data;
			m_data_size = size;
		}
		else {
			m_data = new char[size];
			m_data_size = size;
		}
	}

	void ProgramMemory::m_free() {
		if (m_data_size == 0)return;
		delete[] m_data;
		m_data_size = 0;
		m_size = 0;
	}

	//helper functions
	constexpr unsigned long long closest_pow2(unsigned long long v) {
		//credit: https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2

		// compute the next highest power of 2 of 32-bit v
		//unsigned int v; 

		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;

		v |= v >> 32; //<-- extension to 64-bit

		v++;

		return v;
	}

	constexpr unsigned long long bigger_pow2(unsigned long long v) {
		const unsigned long long v2 = closest_pow2(v);
		return v > v2 ? v2 * 2 : v2;
	}
}