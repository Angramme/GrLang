#include "Parser.h"

#include "Lexer.h"
#include "ASTs.h"

#include <memory>
#include <array>

namespace grComp {

	static constexpr int binop_array_offset = 120;
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

	class Parser {
	public:


	private:
		std::unique_ptr<grComp::Lexer> m_lexer;

	public:
		Parser(std::unique_ptr<Lexer> lexer)
			: m_lexer(std::move(lexer))
		{}

	private:

		//ERROR Logging
		std::unique_ptr<ExprAST> LogError(const char *Str) {
			fprintf(stderr, "LogError: %s\n", Str);
			return nullptr;
		}
		std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
			LogError(Str);
			return nullptr;
		}


		//==============

#define CurTok m_lexer->cur_tok
#define GetTok m_lexer->get_tok


		/// expression
		///   ::= primary binoprhs
		///
		std::unique_ptr<ExprAST> ParseExpression() {
			auto LHS = ParsePrimary();
			if (!LHS)
				return nullptr;

			return ParseBinOpRHS(0, std::move(LHS));
		}

		// ========= PRIMARY EXPRESSIONS ==========

		/// primary
		///   ::= identifierexpr
		///   ::= numberexpr
		///   ::= parenexpr
		std::unique_ptr<ExprAST> ParsePrimary() {
			switch (CurTok) {
			default:
				return LogError("unknown token when expecting an expression");
			case tok_identifier:
				return ParseIdentifierExpr();
			case tok_real_number:
				return ParseNumberExpr();
			case '(':
				return ParseParenExpr();
			}
		}

		/// numberexpr ::= number
		std::unique_ptr<ExprAST> ParseNumberExpr() {
			auto Result = std::make_unique<NumberExprAST>(m_lexer->RealNumValue);
			GetTok(); // consume the number
			return std::move(Result);
		}

		/// parenexpr ::= '(' expression ')'
		std::unique_ptr<ExprAST> ParseParenExpr() {
			GetTok(); // eat (.
			auto V = ParseExpression();
			if (!V)
				return nullptr;

			if (CurTok != ')')
				return LogError("expected ')'");
			GetTok(); // eat ).
			return V;
		}

		/// identifierexpr
		///   ::= identifier	
		///   ::= identifier '(' expression* ')'
		std::unique_ptr<ExprAST> ParseIdentifierExpr() {
			std::string IdName = m_lexer->IdentifierStr;

			GetTok();  // eat identifier.

			if (CurTok != '(') // Simple variable ref.
				return std::make_unique<VariableExprAST>(IdName);

			// Call.
			GetTok();  // eat (
			std::vector<std::unique_ptr<ExprAST>> Args;
			if (CurTok != ')') {
				while (1) {
					if (auto Arg = ParseExpression())
						Args.push_back(std::move(Arg));
					else
						return nullptr;

					if (CurTok == ')')
						break;

					if (CurTok != ',')
						return LogError("Expected ')' or ',' in argument list");
					GetTok();
				}
			}

			// Eat the ')'.
			GetTok();

			return std::make_unique<CallExprAST>(IdName, std::move(Args));
		}


		// =========== BINARY EXPRESSION PARSER =============


		static constexpr std::array<char, 500> binop_precendence = init_binop_precedence();

		static int GetTokPrecedence(int tok) {
			/*if (!isascii(CurTok))
				return -1;*/

			// Make sure it's a declared binop.
			int TokPrec = binop_precendence[tok + binop_array_offset];
			if (TokPrec <= 0) return -1;
			return TokPrec;
		}

		/// binoprhs
		///   ::= ('+' primary)*
		std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
			// If this is a binop, find its precedence.
			while (1) {
				int TokPrec = GetTokPrecedence(CurTok);

				// If this is a binop that binds at least as tightly as the current binop,
				// consume it, otherwise we are done.
				if (TokPrec < ExprPrec)
					return LHS;
				
				// Okay, we know this is a binop.
				int BinOp = CurTok;
				GetTok();  // eat binop

				// Parse the primary expression after the binary operator.
				auto RHS = ParsePrimary();
				if (!RHS)
					return nullptr;

				// If BinOp binds less tightly with RHS than the operator after RHS, let
				// the pending operator take RHS as its LHS.
				int NextPrec = GetTokPrecedence(CurTok);
				if (TokPrec < NextPrec) {
					RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
					if (!RHS)
						return nullptr;
				}

				// Merge LHS/RHS.
				LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
					std::move(RHS));
			}  // loop around to the top of the while loop.
		}

#undef CurTok
#undef GetTok
	};
}