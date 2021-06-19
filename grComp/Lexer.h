#pragma once

#include "Reader.h"
#include <memory>
#include <string>

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
		// std::unique_ptr<std::istream> code;
		// std::istream* code;
		std::unique_ptr<Reader> reader;
		int last_char = ' ';
		
		int cur_tok;
		std::string identifier_str;
		long long relative_num_value;
		double real_num_value;
	public:
		inline int get_cur_tok(){return cur_tok; };
		inline std::string get_identifier_str(){ return identifier_str; };
		inline long long get_relative_num_value(){ return relative_num_value; };
		inline double get_real_num_value(){ return real_num_value; };

		Lexer(std::unique_ptr<Reader> reader);
		int get();

		inline decltype(reader->get_filename()) get_filename() { return reader->get_filename(); }
        inline decltype(reader->get_line()) get_line() { return reader->get_line(); }
	};


	std::string TokToStr(int tok);
}