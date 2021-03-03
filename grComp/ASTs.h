#pragma once

#include <memory>
#include <vector>
#include "Type.h"

namespace grComp {
	enum class AstType : char {
		ErrorAST_t = 0,

		ExprAST_t,
		NumberExprAST_t,
		VariableExprAST_t,
		BinaryExprAST_t,
		CallExprAST_t,

		PrototypeAST_t,
		FunctionAST_t,
	};

	//base class for all ASTs
	class BaseAST {
	private:
		const AstType expr_type = AstType::ErrorAST_t;
	protected:
		BaseAST(const AstType& expr_type);
	public:
		AstType get_ast_type() inline const { return expr_type;  }
	};


	class ExprAST : public BaseAST {
	public:
		std::shared_ptr<grComp::Type> Type;
		//virtual ~ExprAST() {}
	protected:
		ExprAST(const AstType& expr_type);
	};

	/// NumberExprAST - Expression class for numeric literals like "1.0".
	class NumberExprAST : public ExprAST {
	public:
		union {
			double ValR;
			long long ValZ;
			unsigned long long ValN;
		};

		NumberExprAST(double Val);
		/*NumberExprAST(long long Val) : ValZ(Val) {}
		NumberExprAST(unsigned long long Val) : ValN(Val) {}*/

	};

	/// VariableExprAST - Expression class for referencing a variable, like "a".
	class VariableExprAST : public ExprAST {
	public:
		std::string Name;

		VariableExprAST(const std::string &Name);
	};

	/// BinaryExprAST - Expression class for a binary operator.
	class BinaryExprAST : public ExprAST {
	public:
		signed short Op;
		std::unique_ptr<ExprAST> LHS, RHS;

		BinaryExprAST(signed short op, std::unique_ptr<ExprAST> LHS,
			std::unique_ptr<ExprAST> RHS);
		
	};

	/// CallExprAST - Expression class for function calls.
	class CallExprAST : public ExprAST {
	public:
		std::string Callee;
		std::vector<std::unique_ptr<ExprAST>> Args;

		CallExprAST(const std::string &Callee,
			std::vector<std::unique_ptr<ExprAST>> Args);
	};


	/// PrototypeAST 
	class PrototypeAST : BaseAST {
	public:
		std::string Name;
		std::vector<std::string> Args;

		PrototypeAST(const std::string &name, std::vector<std::string> Args);

		const std::string &getName() const { return Name; }
	};

	/// FunctionAST - This class represents a function definition itself.
	class FunctionAST : BaseAST {
	public:
		std::unique_ptr<PrototypeAST> Proto;
		std::unique_ptr<ExprAST> Body;

		FunctionAST(std::unique_ptr<PrototypeAST> Proto,
			std::unique_ptr<ExprAST> Body);
	};


	std::string ExprASTtoStr(const ExprAST* ast);
}