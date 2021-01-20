
#include <string>
#include <iostream>

#include "ASTs.h"


namespace grComp {
	static LLVMContext TheContext;
	static IRBuilder<> Builder(TheContext);
	static std::unique_ptr<Module> TheModule;
	static std::map<std::string, Value *> NamedValues;

	Value* ASTtoIR(const ExprAST* ast) {
		switch (ast->expr_type)
		{
		default:
			throw runtime_error("Unknown AST expression passed!"); // internal error thus not suitable for LogError
			break;
		case ExprAstType::NumberExprAST_t: return NumberExprASTtoIR(ast); break;
		case ExprAstType::VariableExprAST_t: return VariableExprASTtoIR(ast); break;
		case ExprAstType::BinaryExprAST_t: return BinaryExprASTtoIR(ast); break;
		case ExprAstType::CallExprAST_t: return CallExprASTtoIR(ast); break;
		}
	}

	Value* LogErrorV(const std::string msg) {
		std::cout << msg << std::endl;
		return nullptr;
	}

	Value* NumberExprASTtoIR(const NumberExprAST* e) {
		return ConstantFP::get(TheContext, APFloat(e->ValR));
	}

	Value* VariableExprASTtoIR(const VariableExprAST* e) {
		// Look this variable up in the function.
		Value *V = NamedValues[Name];
		if (!V)
			LogErrorV("The variable " + e->Name + " is undefined!");
		return V;

	}

	Value* BinaryExprASTtoIR(const BinaryExprAST* e) {
		Value *L = ASTtoIR(e->LHS.get());
		Value *R = ASTtoIR(e->RHS.get());
		if (!L || !R)
			return nullptr;

		switch (e->Op) {
		case '+':
			return Builder.CreateFAdd(L, R, "addtmp");
		case '-':
			return Builder.CreateFSub(L, R, "subtmp");
		case '*':
			return Builder.CreateFMul(L, R, "multmp");
		case '<':
			L = Builder.CreateFCmpULT(L, R, "cmptmp");
			// Convert bool 0/1 to double 0.0 or 1.0
			return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext),
				"booltmp");
		default:
			return LogErrorV("invalid binary operator");
		}
	}

	Value* CallExprASTtoIR(const CallExprAST* e) {
		// Look up the name in the global module table.
		Function *CalleeF = TheModule->getFunction(e->Callee);
		if (!CalleeF)
			return LogErrorV("Function " + e->Callee + " is undefined.");

		// If argument mismatch error.
		if (CalleeF->arg_size() != e->Args.size())
			return LogErrorV("Incorrect # arguments passed");

		std::vector<Value *> ArgsV;
		for (unsigned i = 0, e = e->Args.size(); i != e; ++i) {
			ArgsV.push_back(ASTtoIR(e->Args[i]));
			if (!ArgsV.back())
				return nullptr;
		}

		return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
	}

	Function *PrototypeASTtoIR(PrototypeAST* e) {
		// Make the function type:  double(double,double) etc.
		std::vector<Type*> Doubles(e->Args.size(),
			Type::getDoubleTy(TheContext));
		FunctionType *FT =
			FunctionType::get(Type::getDoubleTy(TheContext), Doubles, false);

		Function *F =
			Function::Create(FT, Function::ExternalLinkage, e->Name, TheModule.get());
}


