#pragma once

#include "grComp/Lexer.h"
#include "grComp/ASTs.h"
#include "grComp/CompileError.h"

#include <string>
#include <list>
#include <memory>

namespace grComp {
	class Parser {
	private:
		std::unique_ptr<grComp::Lexer> m_lexer;

		struct dispatch_item {
			void* ptr; // void pointer to the unique pointer of type AstType (yes I know, I'm a terrible person)
			enum : char {
				AstItem = 1,
				TokenItem,
			} type;
			AstType ast;
		};
		std::list<dispatch_item> lazy_dispatch;

	public:
		Parser(std::unique_ptr<Lexer> lexer);


		Error::ex<std::unique_ptr<ModuleAST>> get();

		inline decltype(m_lexer->get_line()) get_line() { return m_lexer->get_line(); }
		inline decltype(m_lexer->get_filename()) get_filename() { return m_lexer->get_filename(); }

	private:

		//ERROR Logging
		Error::CError throw_error(std::string msg);


		//==============


	private:

		inline decltype(m_lexer->get_cur_tok()) CurTok() const { return m_lexer->get_cur_tok(); }
		inline decltype(m_lexer->get()) GetTok() const { return m_lexer->get(); }


		void dispatch_next();

		/// module ::= definitions
		Error::ex<std::unique_ptr<ModuleAST>> ParseModule();

		/// definition ::= function definition | class definition | ...
		Error::ex<std::unique_ptr<DefinitionAST>> ParseDefinition();

		/// expression
		///   ::= primary binoprhs
		///
		Error::ex<std::unique_ptr<ExprAST>> ParseExpression();

		// ========= PRIMARY EXPRESSIONS ==========

		/// primary
		///   ::= identifierexpr
		///   ::= numberexpr
		///   ::= parenexpr
		Error::ex<std::unique_ptr<ExprAST>> ParsePrimary();

		/// numberexpr ::= number
		Error::ex<std::unique_ptr<ExprAST>> ParseNumberExpr();

		/// parenexpr ::= '(' expression ')'
		Error::ex<std::unique_ptr<ExprAST>> ParseParenExpr();

		/// identifierexpr
		///   ::= identifier	
		///   ::= identifier '(' expression* ')'
		Error::ex<std::unique_ptr<ExprAST>> ParseIdentifierExpr();


		// =========== BINARY EXPRESSION PARSER =============

		/// binoprhs
		///   ::= ('+' primary)*
		Error::ex<std::unique_ptr<ExprAST>> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);


		// =========== Function Prototype and Definition Parser ==========

		/// prototype
		///   ::= id '(' id* ')'
		Error::ex<std::unique_ptr<PrototypeAST>> ParsePrototype();

		/// definition ::= 'def' prototype expression
		Error::ex<std::unique_ptr<DefinitionAST>> ParseFunctionDefinition();
	};
}