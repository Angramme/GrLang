#include "Lexer.h"
#include <string>
#include <sstream>


#include <iostream>

namespace grComp {
	Lexer::Lexer(std::istream* stream)
		: code(stream)
	{}

	int Lexer::get_tok() {
		while (isspace(LastChar) && LastChar != '\n') LastChar = code->get();

#define RETURN(val) { cur_tok = val; return val; }

		if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
			IdentifierStr = LastChar;
			while (isalnum(LastChar = code->get()))
				IdentifierStr += LastChar;

			if (IdentifierStr == "extern")
				RETURN(tok_extern);
			RETURN(tok_identifier);
		}
			
		if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9]+\.?[0-9]*
			std::string NumStr;
			bool hadDot = LastChar == '.';

			do {
				NumStr += LastChar;
				LastChar = code->get();
			} while (isdigit(LastChar) || (LastChar == '.' && !hadDot && (hadDot=true)));

			if (hadDot) {
				RealNumValue = strtod(NumStr.c_str(), 0);
				RETURN(tok_real_number);
			}
			else {
				RelativeNumValue = strtod(NumStr.c_str(), 0);
				RETURN(tok_relative_number);
			}
		}

		if (LastChar == '>') {
			LastChar = code->get();
			if (LastChar == '=') {
				LastChar = code->get();
				RETURN(tok_grtr_eq);
			}
			RETURN('>');
		}

		if (LastChar == '<') {
			LastChar = code->get();
			if (LastChar == '=') {
				LastChar = code->get();
				RETURN(tok_lssr_eq);
			}
			RETURN('<');
		}

		if (LastChar == ':') {
			LastChar = code->get();
			if (LastChar == '=') {
				LastChar = code->get();
				RETURN(tok_assign);
			}
			RETURN(':');
		}

		if (LastChar == '=') {
			LastChar = code->get();
			if (LastChar == '>') {
				LastChar = code->get();
				RETURN(tok_func_arrow);
			}
			if (LastChar == '/') {
				LastChar = code->get();
				RETURN(tok_not_eq);
			}
			RETURN('=');
		}

		if (LastChar == ';' || LastChar == '\n') {
			LastChar = code->get();
			if (cur_tok != tok_line_break) {
				RETURN(tok_line_break);
			}
			else {
				return get_tok();
			}
		}

		if (LastChar == '$') {
			// Comment until end of line or to the next '$'
			do
				LastChar = code->get();
			while (LastChar != EOF && LastChar != '\n' && LastChar != '\r' && LastChar != '$');

			if (LastChar != EOF) {
				LastChar = code->get();
				return get_tok();
			}
		}

		if (LastChar == EOF)
			RETURN(tok_eof);

		// Otherwise, just return the character as its ascii value.
		const auto ThisChar = LastChar;
		LastChar = code->get();
		RETURN(ThisChar);
	
#undef RETURN

	}

	std::string TokToStr(const Token& tok) {
		switch (tok) {
			case tok_func_arrow: return " func_arrow "; 
			case tok_extern: return " extern ";
			case tok_identifier: return " ID ";
			case tok_real_number: return " R-number ";
			case tok_relative_number: return " Z-number ";
			case tok_assign: return "assign ";
			case tok_not_eq: return " not_eq ";
			case tok_grtr_eq: return " gr_eq ";
			case tok_lssr_eq: return " ls_eq ";
			case tok_line_break: return " (;) ";
			default: return " "+(char)tok;
		}
	}
}