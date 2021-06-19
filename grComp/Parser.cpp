#include "Parser.h"

#include "CompileError.h"
#include "Logger.h"

#include <array>

namespace grComp {

	static constexpr const int binop_array_offset = 120;
	static constexpr std::array<char, 500> init_binop_precedence() {
		std::array<char, 500> temp = std::array<char, 500>();
		constexpr int of = binop_array_offset; //offset

		temp['<'+ of] = 10;
		temp['>' + of] = 10;
		temp[tok_grtr_eq + of] = 10;
		temp[tok_lssr_eq + of] = 10;
		temp['+' + of] = 20;
		temp['-' + of] = 20;
		temp['*' + of] = 40;  // highest.
		temp['/' + of] = 40;  // highest.
		//...
		return temp;
	}


	
	Parser::Parser(std::unique_ptr<Lexer> lexer)
		: m_lexer(std::move(lexer))
	{
		m_lexer->get();
	}

	Error::ex<std::unique_ptr<ModuleAST>> Parser::get() {
		return ParseModule();
	}

	//ERROR Logging
	Error::CError Parser::throw_error(std::string msg){
		Error::CError err;
		err.compile_step = Error::CompileStep::parser;
		err.hint_message = "";
		err.err_message = msg;
		err.file = get_filename().string();
		err.line = get_line();
		return err;
	}

	//==============
		


	void Parser::dispatch_next() {
		// const auto elem = *lazy_dispatch.begin();
		// lazy_dispatch.pop_front();
		// if (elem.type == dispatch_item::AstItem) {
		// 	if (!BaseAST::is_lazy(elem.ast)) { // is eager
		// 		if (BaseAST::is_expression(elem.ast)) *reinterpret_cast<std::unique_ptr<ExprAST>*>(elem.ptr) = std::move(ParseExpression().value());
		// 		else if (BaseAST::is_definition(elem.ast)) *reinterpret_cast<std::unique_ptr<DefinitionAST>*>(elem.ptr) = std::move(ParseDefinition().value());
		// 		else {
		// 			GRCOMP_INTERNAL_RUNTIME_ERROR("invalid dispatch item ast-type!!! : " + (int)elem.ast);
		// 		}
		// 	}else{ // is lazy
		// 		// will be implemented eventually...
		// 	}
		// }
		// else if (elem.type == dispatch_item::TokenItem) {
		// 	// process just a string
		// 	grComp::Token* it = reinterpret_cast<grComp::Token*>(elem.ptr);
		// 	while (*it) {
		// 		if (CurTok() != *it) grComp::Log::CompiletimeError("unexpected token! " + (char)CurTok());
		// 		it++;
		// 		GetTok();
		// 	}
		// 	delete[] elem.ptr;
		// }
		// else {
		// 	GRCOMP_INTERNAL_RUNTIME_ERROR("invalid dispatch item type!!! : " + (int)elem.type);
		// }
	}

	/// module ::= definitions
	Error::ex<std::unique_ptr<ModuleAST>> Parser::ParseModule() {
		std::unique_ptr<ModuleAST> ast = std::make_unique<ModuleAST>();
		bool LOOP = true;

		while (LOOP) {
			switch (CurTok()) {
			case tok_eof:
				LOOP = false; break;
			case ';': // ignore top-level semicolons.
				GetTok();
				break;
			case tok_line_break:
				GetTok();
				break;
			/*case tok_extern:
				HandleExtern();
				break;*/
			case tok_identifier:{
				auto X = ParseDefinition();
				if(!X) return Error::unex(X.error()); 
				ast->lines.push_back(std::move(X.value()));
			} break;
			default:
				return Error::unex(throw_error("Unexpected token encountered while parsing module: "+TokToStr(CurTok())));
				break;
			}
		}

		return std::move(ast);
	}

	/// top ::= definition | external | expression | ';'
	//void MainLoop() {
	//	while (1) {
	//		switch (CurTok()) {
	//		case tok_eof:
	//			return;
	//		case ';': // ignore top-level semicolons.
	//			GetTok();
	//			break;
	//		/*case tok_extern:
	//			HandleExtern();
	//			break;*/
	//		default:
	//			HandleDefinition();
	//			break;

	//			//HandleTopLevelExpression(); not allowed in GR
	//			//break;
	//		}
	//	}
	//}

	//void HandleDefinition() {
	//	if (ParseFunctionDefinition()) {
	//		//fprintf(stderr, "Parsed a function definition.\n");
	//		Log::CompiletimeInfo("parsed a function definiton.\n");
	//	}
	//	else {
	//		// Skip token for error recovery.
	//		GetTok();
	//	}
	//}

	/// definition ::= function definition | class definition | ...
	Error::ex<std::unique_ptr<DefinitionAST>> Parser::ParseDefinition() {
		return std::move(ParseFunctionDefinition());
	}

	/// expression
	///   ::= primary binoprhs
	///
	Error::ex<std::unique_ptr<ExprAST>> Parser::ParseExpression() {
		auto LHS = ParsePrimary();
		if(!LHS) return Error::unex(LHS.error());
		// if (!LHS)
		// 	return nullptr;

		return ParseBinOpRHS(0, std::move(*LHS));
	}

	// ========= PRIMARY EXPRESSIONS ==========

	/// primary
	///   ::= identifierexpr
	///   ::= numberexpr
	///   ::= parenexpr
	Error::ex<std::unique_ptr<ExprAST>> Parser::ParsePrimary() {
		switch (CurTok()) {
		default:
			// return LogError<ExprAST>("unknown token ["+ TokToStr((Token)CurTok()) +"] when expecting an expression");
			return Error::unex(throw_error("unknown token ["+ TokToStr(CurTok()) +"] when expecting an expression"));
		case tok_identifier:
			return ParseIdentifierExpr();
		case tok_real_number:
			return ParseNumberExpr();
		case '(':
			return ParseParenExpr();
		}
	}

	/// numberexpr ::= number
	Error::ex<std::unique_ptr<ExprAST>> Parser::ParseNumberExpr() {
		auto Result = std::make_unique<NumberExprAST>(m_lexer->get_real_num_value());
		GetTok(); // consume the number
		return std::move(Result);
	}

	/// parenexpr ::= '(' expression ')'
	Error::ex<std::unique_ptr<ExprAST>> Parser::ParseParenExpr() {
		GetTok(); // eat (.
		auto V = ParseExpression();
		if (!V) return V;

		if (CurTok() != ')')
			return Error::unex(throw_error("expected ')'"));
			// return LogError<ExprAST>("expected ')'");
		GetTok(); // eat ).
		return V;
	}

	/// identifierexpr
	///   ::= identifier	
	///   ::= identifier '(' expression* ')'
	Error::ex<std::unique_ptr<ExprAST>> Parser::ParseIdentifierExpr() {
		std::string IdName = m_lexer->get_identifier_str();

		GetTok();  // eat identifier.

		if (CurTok() != '(') // Simple variable ref.
			return std::make_unique<VariableExprAST>(IdName);

		// Call.
		GetTok();  // eat (
		std::vector<std::unique_ptr<ExprAST>> Args;
		if (CurTok() != ')') {
			while (1) {
				if (auto Arg = ParseExpression())
					Args.push_back(std::move(*Arg));
				else
					return Arg;

				if (CurTok() == ')')
					break;

				if (CurTok() != ',')
					// return LogError<ExprAST>("Expected ')' or ',' in argument list");
					return Error::unex(throw_error("Expected ')' or ',' in argument list"));
				GetTok();
			}
		}

		// Eat the ')'.
		GetTok();

		return std::make_unique<CallExprAST>(IdName, std::move(Args));
	}


	// =========== BINARY EXPRESSION PARSER =============


	static constexpr std::array<char, 500> binop_precedence = init_binop_precedence();

	static constexpr int GetTokPrecedence(const int tok) {
		/*if (!isascii(CurTok))
			return -1;*/

		#ifndef NDEBUG
			if(tok+binop_array_offset > (int)binop_precedence.size()-1)
				GRCOMP_INTERNAL_RUNTIME_ERROR("invalid token \""+TokToStr(tok)+"\" is out of binop range...");
		#endif

		// Make sure it's inside the defined range
		if( tok+binop_array_offset < 0 ) return -1;
		// Make sure it's a declared binop.
		int TokPrec = binop_precedence[tok + binop_array_offset];
		if (TokPrec <= 0) return -1;
		return TokPrec;
	}

	/// binoprhs
	///   ::= ('+' primary)*
	Error::ex<std::unique_ptr<ExprAST>> Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
		// If this is a binop, find its precedence.
		while (1) {
			int TokPrec = GetTokPrecedence(CurTok());

			// If this is a binop that binds at least as tightly as the current binop,
			// consume it, otherwise we are done.
			if (TokPrec < ExprPrec)
				return LHS;
				
			// Okay, we know this is a binop.
			int BinOp = CurTok();
			GetTok();  // eat binop

			// Parse the primary expression after the binary operator.
			auto RHS = ParsePrimary();
			if (!RHS)
				return RHS;

			// If BinOp binds less tightly with RHS than the operator after RHS, let
			// the pending operator take RHS as its LHS.
			int NextPrec = GetTokPrecedence(CurTok());
			if (TokPrec < NextPrec) {
				RHS = ParseBinOpRHS(TokPrec + 1, std::move(*RHS));
				if (!RHS)
					return RHS;
			}

			// Merge LHS/RHS.
			LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
				std::move(*RHS));
		}  // loop around to the top of the while loop.
	}


	// =========== Function Prototype and Definition Parser ==========
	
	/// prototype
	///   ::= id '(' id* ')'
	Error::ex<std::unique_ptr<PrototypeAST>> Parser::ParsePrototype() {
		if (CurTok() != tok_identifier)
			return Error::unex(throw_error("Expected function name in prototype, got token: "+TokToStr(CurTok())));

		std::string FnName = m_lexer->get_identifier_str();
	
		// Read the list of argument names.
		std::vector<std::string> ArgNames;
		while (GetTok() == tok_identifier) {
			ArgNames.push_back(m_lexer->get_identifier_str());
			if (GetTok() != ':')
				return Error::unex(throw_error("Expected a type delimiter \":\" for the function argument"));
			if (GetTok() != tok_identifier) //eat type  (TODO: add actual type to the language)
				return Error::unex(throw_error("Expected a type for the function argument"));
		}
		if (CurTok() != tok_func_arrow)
			return Error::unex(throw_error("Expected a function arrow \"=>\" followed by return value/s. Got token "+TokToStr(CurTok())));
		GetTok();  // eat '=>'.

		return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
	}
		
	/// definition ::= 'def' prototype expression
	Error::ex<std::unique_ptr<DefinitionAST>> Parser::ParseFunctionDefinition() {
		auto Proto = ParsePrototype();
		if (!Proto) return Error::unex(Proto.error());

		auto E = ParseExpression();
		if (!E) return Error::unex(E.error());

		return std::make_unique<FunctionDefAST>(std::move(*Proto), std::move(*E));
	}
		

	// =========== Command Parser =============

}