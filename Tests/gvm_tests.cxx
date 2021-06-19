#include "testing.h"


TEST_FILE_START()

TEST_CATEGORY_START(gVM tests)

// gVM::StackMemory gmem = gVM::StackMemory();

// grComp::ProgramMemory program = grComp::ProgramMemory();
// program
//     << grCore::SET << grCore::maddress(1) << (char)80
//     << grCore::PRINTschar
//     << grCore::MOVE << grCore::maddress(1)
//     << grCore::SET << grCore::maddress(1) << (char)8
//     << grCore::PRINTschar
//     << grCore::ADDcharA << grCore::maddress(0)
//     << grCore::PRINTschar
//     << grCore::PRINTascii << (unsigned)1;


// std::cout << "pgm size: " << program.get_size() << std::endl;
// std::cout << "pgm alloc size: " << program.get_heap_size() << std::endl;
// std::cout << "starting execution..." << std::endl;

// gVM::execute_program(program.get_data(), program.get_size(), 0, &gmem);

TEST_CATEGORY_END()

TEST_FILE_END()