#pragma once

namespace grCore {
	enum INSTRUCTION : unsigned char {
		NO_OP = 0,
		END = 1,

		SET_AOF, //set address offset 
		FLG_AOF, //enable/disable address offset
		FLG_CAOF, //enable/disable address offset for cursor

		JUMP, // jump to arg1 const value address
		JUMPP, // jump to arg1 pointers value address
		JUMPIF, // jump to arg1 const value address if arg2 value is not zero
		JUMPPIF, // jump to arg1 pointers value address if arg2 value is not zero

		MOVE, //move cursor to arg1 const pointer
		MOVEP, //move cursor to arg1 pointers value
		SET, //set value at current cursor position to arg2 const value of arg1 length in bytes
		COPY, //copy arg2 pointers value to value at current cursor position of length arg1 in bytes

#define MATH_OPERATOR(NAME, TYPE) \
		Cur##NAME##TYPE, /*add value to current cursor*/ \
		Cur##NAME##TYPE##P, /*add value at pointer to current cursor*/ \
		Cur##NAME##TYPE##S, /*add value from stack to current cursor*/ \
		Stack##NAME##TYPE, /*add value to stack position at arg1*/ \
		Stack##NAME##TYPE##P, /*add value at pointer to stack position at arg2*/ \
		Stack##NAME##TYPE##S, /*add value from stack arg1 to stack position at arg1*/

#define MATH_OPERATOR_TYPES(NAME) \
		MATH_OPERATOR(NAME, float) \
		MATH_OPERATOR(NAME, int) \
		MATH_OPERATOR(NAME, char) \

		MATH_OPERATOR_TYPES(ADD)
		MATH_OPERATOR_TYPES(SUB)
		MATH_OPERATOR_TYPES(MUL)
		MATH_OPERATOR_TYPES(DIV)
		MATH_OPERATOR_TYPES(MOD) 

#undef MATH_OPERATOR_TYPES
#undef MATH_OPERATOR

		//ADDfloat,  ADDfloatA, //add value at pointer to current cursor
		//ADDint,    ADDintA,
		//ADDchar,   ADDcharA,

		//SUBfloat,  SUBfloatA,
		//SUBint,    SUBintA,
		//SUBchar,   SUBcharA,

		//MULfloat,  MULfloatA,
		//MULint,    MULintA,
		//MULchar,   MULcharA,

		//DIVfloat,  DIVfloatA,
		//DIVint,    DIVintA,
		//DIVchar,   DIVcharA,

		//MODfloat,  MODfloatA,
		//MODint,	   MODintA,
		//MODchar,   MODcharA,

		SINfloat,
		//...
		COSfloat,
		//...

		PRINTascii, //print cursor pointers value of arg2 value length
		PRINTschar,
	};
}