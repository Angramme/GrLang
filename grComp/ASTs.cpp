#include "ASTs.h"
#include <string>

namespace grComp {

	BaseAST::BaseAST(const AstType& expr_type)
		: expr_type(expr_type)
	{}

	ExprAST::ExprAST(const AstType& expr_type)
		: BaseAST(expr_type)
	{}

	NumberExprAST::NumberExprAST(double Val)
		: ValR(Val), ExprAST(AstType::NumberExprAST_t) 
	{}

	VariableExprAST::VariableExprAST(const std::string& Name)
		: Name(Name), ExprAST(AstType::VariableExprAST_t)
	{}

	BinaryExprAST::BinaryExprAST(signed short op, std::unique_ptr<ExprAST> LHS,
		std::unique_ptr<ExprAST> RHS)
		: Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)), ExprAST(AstType::BinaryExprAST_t)
	{}

	CallExprAST::CallExprAST(const std::string &Callee,
		std::vector<std::unique_ptr<ExprAST>> Args)
		: Callee(Callee), Args(std::move(Args)), ExprAST(AstType::CallExprAST_t)
	{}

	PrototypeAST::PrototypeAST(const std::string &name, std::vector<std::string> Args)
		: Name(name), Args(std::move(Args)), BaseAST(AstType::PrototypeAST_t)
	{}

	FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
		: Proto(std::move(Proto)), Body(std::move(Body)), BaseAST(AstType::FunctionAST_t)
	{}



	// to string conversions

	static std::string BinaryExprASTtoStr(const BinaryExprAST* ast) {
		return ExprASTtoStr(ast->LHS.get()) + (char)ast->Op + ExprASTtoStr(ast->RHS.get());
	}

	static std::string NumberExprASTtoStr(const NumberExprAST* ast) {
		return std::to_string(ast->ValR);
	}

	std::string ASTtoStr(const BaseAST* ast) {
		switch (ast->get_ast_type())
		{
		default:
			break;
		case AstType::BinaryExprAST_t: return BinaryExprASTtoStr(static_cast<const BinaryExprAST*>(ast)); break;
		case AstType::NumberExprAST_t: return NumberExprASTtoStr(static_cast<const NumberExprAST*>(ast)); break;
		}
	}
}