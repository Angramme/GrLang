#include <iostream>


namespace grComp {
	namespace Log {
		void Error(std::string msg) {
			std::cout << msg << std::endl;
		}
	}
}