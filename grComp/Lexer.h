#pragma once

#include <istream>

namespace grComp {
	enum Token : int {
		tok_eof = -1,

		//binary operators [!] must be less than 127 [!]
		tok_assign = -11,  //:=
		tok_not_eq = -12,  //=/
		tok_grtr_eq = -13, //>=
		tok_lssr_eq = -14, //<=
		
		//operators
		tok_func_arrow = -101,

		// commands
		tok_extern = -201,
		tok_module = -202,
		tok_import = -203,
		tok_run = -204,

		// primary
		tok_line_break = -301,
		tok_identifier = -302,
		tok_relative_number = -303,
		tok_real_number = -304,
	};

	class Lexer {
	private:
		std::istream* code;
		int LastChar = ' ';
	public:
		int cur_tok;
		std::string IdentifierStr;
		long long RelativeNumValue;
		double RealNumValue;

		Lexer(std::istream* stream);
		int get_tok();
	};
}