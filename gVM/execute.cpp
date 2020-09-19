#include "execute.h"
#include <iostream>

namespace gVM {

	using namespace grCore;
	
	void execute_program(
		char* instructions, size_t length, size_t start_at, 
		StackMemory* smem) {

		maddress curr_instr = start_at;
		INSTRUCTION instr = NO_OP;
		bool loop = true;

#define INSTRUCTION_OFFSET curr_instr += sizeof(INSTRUCTION)
#define GET_INSTR_ARG(TYPE, VAR) VAR = *reinterpret_cast<TYPE*>(instructions + curr_instr); curr_instr += sizeof(TYPE);
#define GET_CONST_INSTR_ARG(TYPE, VAR) \
	const TYPE& VAR = *reinterpret_cast<TYPE*>(instructions + curr_instr); curr_instr += sizeof(TYPE);

		maddress cursor = 0;

		while (loop && curr_instr < length) {
			instr = *reinterpret_cast<INSTRUCTION*>(instructions + curr_instr);

			switch (instr) {
			case NO_OP:
				INSTRUCTION_OFFSET;
				break;
			case END:
				INSTRUCTION_OFFSET;
				loop = false;
				break;


			case JUMP: {
				INSTRUCTION_OFFSET;
				GET_CONST_INSTR_ARG(maddress, newadd);
				curr_instr = newadd;
			}
			break;


			case MOVE: {
				INSTRUCTION_OFFSET;
				GET_INSTR_ARG(maddress, cursor);
			}
			break;
			case SET: {
				INSTRUCTION_OFFSET;
				GET_CONST_INSTR_ARG(maddress, len);
				smem->setat(cursor, instructions + curr_instr, len);
				curr_instr += len;
			}
			break;

			
			//case ADDchar: {
			//	INSTRUCTION_OFFSET;
			//	GET_CONST_INSTR_ARG(char, num);
			//	*reinterpret_cast<char*>(smem->at(cursor)) += num;
			//}break;
			//case ADDcharA: {
			//	INSTRUCTION_OFFSET;
			//	GET_CONST_INSTR_ARG(maddress, add);
			//	*reinterpret_cast<char*>(smem->at(cursor)) += *reinterpret_cast<char*>(smem->at(add));
			//}break;


			//macro for math operations
#define MATH_OPERATOR(NAME, TYPE, OP)                                                                  \
			case Cur##NAME##TYPE: {                                                                         \
				INSTRUCTION_OFFSET;                                                                    \
				GET_CONST_INSTR_ARG(TYPE, num);                                                        \
				*reinterpret_cast<TYPE*>(smem->at(cursor)) OP num;                                     \
			}break;                                                                                    \
			case Cur##NAME##TYPE##P: {                                                                      \
				INSTRUCTION_OFFSET;                                                                    \
				GET_CONST_INSTR_ARG(maddress, add);                                                    \
				/*change this -->*/*reinterpret_cast<TYPE*>(smem->at(cursor)) OP *reinterpret_cast<TYPE*>(smem->at(add)); \
			}break;

			//second macro for all used types
#define MATH_OPERATOR_TYPES(NAME, OP)      \
			MATH_OPERATOR(NAME, int,   OP) \
			MATH_OPERATOR(NAME, float, OP) \
			MATH_OPERATOR(NAME, char,  OP) \

			//use the macro
			MATH_OPERATOR_TYPES(ADD, +=)
			MATH_OPERATOR_TYPES(SUB, -=)
			MATH_OPERATOR_TYPES(MUL, *=)
			MATH_OPERATOR_TYPES(DIV, /=)
			
			//special cases
			MATH_OPERATOR(MOD, int, %=) 
			//MATH_OPERATOR(MOD, float, %=) 
			MATH_OPERATOR(MOD, char, %=)

			
#undef MATH_OPERATOR_TYPES
#undef MATH_OPERATOR


			case PRINTascii: {
				INSTRUCTION_OFFSET;
				GET_CONST_INSTR_ARG(unsigned int, len);
				printf("%.*s", len, smem->at(cursor));
			}
			break;
			case PRINTschar: {
				INSTRUCTION_OFFSET;
				// + promotes to print in number form instead of ascii
				std::cout << +*reinterpret_cast<char*>(smem->at(cursor)) << std::endl;
			}
			break;

			}

		}
	}
}

#undef INSTRUCTION_OFFSET 
#undef GET_INSTR_ARG
#undef GET_CONST_INSTR_ARG