#pragma once

#include <iostream>

namespace grComp {
	class ProgramMemory {
	public:
		ProgramMemory();

		ProgramMemory(size_t init_size);

		~ProgramMemory();

		template<typename T>
		ProgramMemory& operator<<(const T& v);

		inline char* get_data() { return m_data; }
		inline size_t get_size() { return m_size;  }
		inline size_t get_heap_size() { return m_data_size; }
	private:
		void m_realloc(size_t size);

		void m_free();

	private:
		size_t m_size;
		size_t m_data_size;
		char* m_data;
	};

	//helper functions
	constexpr unsigned long long closest_pow2(unsigned long long v);
	constexpr unsigned long long bigger_pow2(unsigned long long v);

	template<typename T>
	ProgramMemory& ProgramMemory::operator<<(const T& v)
	{
		if (sizeof(T) + m_size > m_data_size) {
			m_realloc(m_data_size == 0 ? bigger_pow2(sizeof(T)) : bigger_pow2(m_size + sizeof(T)));
		}
		memcpy(m_data + m_size, &v, sizeof(T));
		m_size += sizeof(T);
		return *this;
	}
}