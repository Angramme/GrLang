#include "ASTs.h"
#include <string>

namespace grComp {
	std::string ExprASTtoStr(const ExprAST* ast) {
		switch (ast->expr_type)
		{
		default:
			break;
		case ExprAstType::BinaryExprAST_t: return BinaryExprASTtoStr(static_cast<const BinaryExprAST*>(ast)); break;
		case ExprAstType::NumberExprAST_t: return NumberExprASTtoStr(static_cast<const NumberExprAST*>(ast)); break;
		}
	}

	static std::string BinaryExprASTtoStr(const BinaryExprAST* ast) {
		return ExprASTtoStr(ast->LHS.get()) + (char)ast->Op + ExprASTtoStr(ast->RHS.get());
	}

	static std::string NumberExprASTtoStr(const NumberExprAST* ast) {
		return std::to_string(ast->ValR);
	}
}