#include "ASTs.h"
#include <string>

namespace grComp {

	BaseAST::BaseAST(const AstType& ast_type)
		: ast_type(ast_type)
	{}

	EagerAST::EagerAST(const AstType& ast_type)
		: BaseAST(ast_type)
	{}
	LazyAST::LazyAST(const AstType& ast_type)
		: BaseAST(ast_type)
	{}

	ExprAST::ExprAST(const AstType& ast_type)
		: EagerAST(ast_type)
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

	DefinitionAST::DefinitionAST(const AstType& ast_type)
		: EagerAST(ast_type)
	{}

	PrototypeAST::PrototypeAST(const std::string &name, std::vector<std::string> Args)
		: Name(name), Args(std::move(Args)), EagerAST(AstType::PrototypeAST_t)
	{}

	FunctionDefAST::FunctionDefAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
		: Proto(std::move(Proto)), Body(std::move(Body)), DefinitionAST(AstType::FunctionDefAST_t)
	{}

	ModuleAST::ModuleAST()
		: EagerAST(AstType::ModuleAST_t)
	{}


	// type functions

#define AST_TYPE_MODIFIER(xTYPE) template<> struct BaseAST::AstTypeToClassType<AstType::xTYPE##_t> { typedef grComp::##xTYPE type;  };
	AST_TYPES
#undef AST_TYPE_MODIFIER 


	template<AstType ast>
	inline bool BaseAST::is_lazy() {
		return is_lazy<AstTypeToClassType<ast>::type>();
	}
	template<AstType ast>
	inline bool BaseAST::is_expression() {
		return is_expression<AstTypeToClassType<ast>::type>();
	}
	template<AstType ast>
	inline bool BaseAST::is_definition() {
		return is_definition<AstTypeToClassType<ast>::type>();
	}
#define AST_TYPE_MODIFIER(xTYPE) template bool BaseAST::is_lazy<AstType::xTYPE##_t>();
AST_TYPES
#define AST_TYPE_MODIFIER(xTYPE) template bool BaseAST::is_expression<AstType::xTYPE##_t>();
AST_TYPES
#define AST_TYPE_MODIFIER(xTYPE) template bool BaseAST::is_definition<AstType::xTYPE##_t>();
AST_TYPES

	template<typename ast>
	inline bool BaseAST::is_lazy() {
		return std::is_base_of<LazyAST, ast>::value;
	}
	template<typename ast>
	inline bool BaseAST::is_expression() {
		return std::is_base_of<ExprAST, ast>::value;
	}
	template<typename ast>
	inline bool BaseAST::is_definition() {
		return std::is_base_of<DefinitionAST, ast>::value;
	}
#define AST_TYPE_MODIFIER(xTYPE) template bool BaseAST::is_lazy<xTYPE>();
AST_TYPES
#define AST_TYPE_MODIFIER(xTYPE) template bool BaseAST::is_expression<xTYPE>();
AST_TYPES
#define AST_TYPE_MODIFIER(xTYPE) template bool BaseAST::is_definition<xTYPE>();
AST_TYPES





	// to string conversions

	static std::string BinaryExprASTtoStr(const BinaryExprAST* ast) {
		return ASTtoStr(ast->LHS.get()) + (char)ast->Op + ASTtoStr(ast->RHS.get());
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