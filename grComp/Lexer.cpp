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
				return gettok();
			}
		}

		if (LastChar == '$') {
			// Comment until end of line or to the next '$'
			do
				LastChar = code->get();
			while (LastChar != EOF && LastChar != '\n' && LastChar != '\r' && LastChar != '$');

			if (LastChar != EOF) {
				LastChar = code->get();
				return gettok();
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

	void TokToStr(Token tok) {
		switch (tok) {
			case tok_func_arrow: std::cout << " func_arrow "; break;
			case tok_extern: std::cout << " extern "; break;
			case tok_identifier: std::cout << " ID "; break;
			case tok_real_number: std::cout << " R-number "; break;
			case tok_relative_number: std::cout << " Z-number "; break;
			case tok_assign: std::cout << "assign "; break;
			case tok_not_eq: std::cout << " not_eq "; break;
			case tok_grtr_eq: std::cout << " gr_eq "; break;
			case tok_lssr_eq: std::cout << " ls_eq "; break;
			case tok_line_break: std::cout << " (;) "; break;
			default: std::cout << (char)tok; break;
		}
	}
}