#include "Lexer.h"
#include <string>
#include <sstream>


#include <iostream>

namespace grComp {
	// Lexer::Lexer(std::unique_ptr<std::istream> stream)
		// : reader(std::move(stream))
	Lexer::Lexer(std::unique_ptr<Reader> reader)
		: reader(std::move(reader))
	{}

	int Lexer::get() {
		while (isspace(last_char) && last_char != '\n') last_char = reader->get();

#define RETURN(val) { cur_tok = val; return val; }

		if (isalpha(last_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
			identifier_str = last_char;
			while (isalnum(last_char = reader->get()))
				identifier_str += last_char;

			if (identifier_str == "extern")
				RETURN(tok_extern);
			RETURN(tok_identifier);
		}
			
		if (isdigit(last_char) || last_char == '.') {   // Number: [0-9]+\.?[0-9]*
			std::string NumStr;
			bool hadDot = last_char == '.';

			do {
				NumStr += last_char;
				last_char = reader->get();
			} while (isdigit(last_char) || (last_char == '.' && !hadDot && (hadDot=true)));

			if (hadDot) {
				real_num_value = strtod(NumStr.c_str(), 0);
				RETURN(tok_real_number);
			}
			else {
				relative_num_value = strtoll(NumStr.c_str(), NULL, 0);
				RETURN(tok_relative_number);
			}
		}

		if (last_char == '>') {
			last_char = reader->get();
			if (last_char == '=') {
				last_char = reader->get();
				RETURN(tok_grtr_eq);
			}
			RETURN('>');
		}

		if (last_char == '<') {
			last_char = reader->get();
			if (last_char == '=') {
				last_char = reader->get();
				RETURN(tok_lssr_eq);
			}
			RETURN('<');
		}

		if (last_char == ':') {
			last_char = reader->get();
			if (last_char == '=') {
				last_char = reader->get();
				RETURN(tok_assign);
			}
			RETURN(':');
		}

		if (last_char == '=') {
			last_char = reader->get();
			if (last_char == '>') {
				last_char = reader->get();
				RETURN(tok_func_arrow);
			}
			if (last_char == '/') {
				last_char = reader->get();
				RETURN(tok_not_eq);
			}
			RETURN('=');
		}

		if (last_char == ';' || last_char == '\n') {
			last_char = reader->get();
			if (cur_tok != tok_line_break) {
				RETURN(tok_line_break);
			}
			else {
				return get();
			}
		}

		if (last_char == '$') {
			// Comment until end of line or to the next '$'
			do
				last_char = reader->get();
			while (last_char != EOF && last_char != '\n' && last_char != '\r' && last_char != '$');

			if (last_char != EOF) {
				last_char = reader->get();
				return get();
			}
		}

		if (last_char == EOF)
			RETURN(tok_eof);

		// Otherwise, just return the character as its ascii value.
		const auto ThisChar = last_char;
		last_char = reader->get();
		RETURN(ThisChar);
	
#undef RETURN

	}

	std::string TokToStr(int tok) {
		switch (tok) {
			case tok_func_arrow: return " func_arrow "; 
			case tok_extern: return " extern ";
			case tok_identifier: return " identifier ";
			case tok_real_number: return " R-number ";
			case tok_relative_number: return " Z-number ";
			case tok_assign: return " assign ";
			case tok_not_eq: return " not_eq ";
			case tok_grtr_eq: return " gr_eq ";
			case tok_lssr_eq: return " ls_eq ";
			case tok_line_break: return " line_break ";
			default: return (std::string)" "+static_cast<char>(tok);
		}
	}
}