#pragma once

#include <memory>
#include <vector>
#include "Type.h"

namespace grComp {
	enum class ExprAstType : char {
		ExprAST_t,
		NumberExprAST_t,
		VariableExprAST_t,
		BinaryExprAST_t,
		CallExprAST_t
	};


	class ExprAST {
	public:
		std::shared_ptr<grComp::Type> Type;
		//virtual ~ExprAST() {}
		static const ExprAstType expr_type = ExprAstType::ExprAST_t;
	};

	/// NumberExprAST - Expression class for numeric literals like "1.0".
	const ExprAstType NumberExprAST::expr_type = ExprAstType::NumberExprAST_t;
	class NumberExprAST : public ExprAST {
	public:
		union {
			double ValR;
			long long ValZ;
			unsigned long long ValN;
		};

		NumberExprAST(double Val) : ValR(Val) {}
		/*NumberExprAST(long long Val) : ValZ(Val) {}
		NumberExprAST(unsigned long long Val) : ValN(Val) {}*/

	};

	/// VariableExprAST - Expression class for referencing a variable, like "a".
	const ExprAstType VariableExprAST::expr_type = ExprAstType::VariableExprAST_t;
	class VariableExprAST : public ExprAST {
	public:
		std::string Name;

		VariableExprAST(const std::string &Name) : Name(Name) {}
	};

	/// BinaryExprAST - Expression class for a binary operator.
	const ExprAstType BinaryExprAST::expr_type = ExprAstType::BinaryExprAST_t;
	class BinaryExprAST : public ExprAST {
	public:
		signed short Op;
		std::unique_ptr<ExprAST> LHS, RHS;

		BinaryExprAST(signed short op, std::unique_ptr<ExprAST> LHS,
			std::unique_ptr<ExprAST> RHS)
			: Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
		
	};

	/// CallExprAST - Expression class for function calls.
	const ExprAstType CallExprAST::expr_type = ExprAstType::CallExprAST_t;
	class CallExprAST : public ExprAST {
	public:
		std::string Callee;
		std::vector<std::unique_ptr<ExprAST>> Args;

		CallExprAST(const std::string &Callee,
			std::vector<std::unique_ptr<ExprAST>> Args)
			: Callee(Callee), Args(std::move(Args)) {}
	};


	/// PrototypeAST 
	//const ExprAstType PrototypeAST::expr_type = ExprAstType::PrototypeAST_t;
	class PrototypeAST {
	public:
		std::string Name;
		std::vector<std::string> Args;

		PrototypeAST(const std::string &name, std::vector<std::string> Args)
			: Name(name), Args(std::move(Args)) {}

		const std::string &getName() const { return Name; }
	};

	/// FunctionAST - This class represents a function definition itself.
	//const ExprAstType FunctionAST::expr_type = ExprAstType::FunctionAST_t;
	class FunctionAST {
	public:
		std::unique_ptr<PrototypeAST> Proto;
		std::unique_ptr<ExprAST> Body;

		FunctionAST(std::unique_ptr<PrototypeAST> Proto,
			std::unique_ptr<ExprAST> Body)
			: Proto(std::move(Proto)), Body(std::move(Body)) {}
	};
}