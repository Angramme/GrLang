
#include <string>
#include <sstream>

#include "ASTs.h"
#include "Logger.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

namespace grComp {



	struct LLVM_IR_GEN_CTX{
		LLVMContext& context;
		IRBuilder<>& builder;
		Module& module;
	};
	//static LLVMContext TheContext;
	//static IRBuilder<> Builder(TheContext);
	//static std::unique_ptr<Module> TheModule;
	static std::map<std::string, Value *> NamedValues;
	

	Value* ASTtoIR(const ExprAST* ast, const LLVM_IR_GEN_CTX& ctx);


	Value* LogErrorV(const std::string& msg) {
		Log::CompiletimeError(msg);
		return nullptr;
	}

	Value* NumberExprASTtoIR(const NumberExprAST* e, const LLVM_IR_GEN_CTX& ctx) {
		return ConstantFP::get(ctx.context, APFloat(e->ValR));
	}

	Value* VariableExprASTtoIR(const VariableExprAST* e, const LLVM_IR_GEN_CTX& ctx) {
		// Look this variable up in the function.
		Value *V = NamedValues[e->Name];
		if (!V)
			LogErrorV("The variable " + e->Name + " is undefined!");
		return V;

	}

	Value* BinaryExprASTtoIR(const BinaryExprAST* e, const LLVM_IR_GEN_CTX& ctx) {
		Value *L = ASTtoIR(e->LHS.get(), ctx);
		Value *R = ASTtoIR(e->RHS.get(), ctx);
		if (!L || !R)
			return nullptr;

		switch (e->Op) {
		case '+':
			return ctx.builder.CreateFAdd(L, R, "addtmp");
		case '-':
			return ctx.builder.CreateFSub(L, R, "subtmp");
		case '*':
			return ctx.builder.CreateFMul(L, R, "multmp");
		case '<':
			L = ctx.builder.CreateFCmpULT(L, R, "cmptmp");
			// Convert bool 0/1 to double 0.0 or 1.0
			return ctx.builder.CreateUIToFP(L, llvm::Type::getDoubleTy(ctx.context),
				"booltmp");
		default:
			return LogErrorV((std::string)"invalid binary operator \"" + (char)e->Op + "\"");
		}
	}

	Value* CallExprASTtoIR(const CallExprAST* E, const LLVM_IR_GEN_CTX& ctx) {
		// Look up the name in the global module table.
		Function *CalleeF = ctx.module.getFunction(E->Callee);
		if (!CalleeF)
			return LogErrorV("Function " + E->Callee + " is undefined.");

		// If argument mismatch error.
		if (CalleeF->arg_size() != E->Args.size())
			return LogErrorV("Incorrect # arguments passed");

		std::vector<Value *> ArgsV;
		for (unsigned i = 0, e = E->Args.size(); i != e; ++i) {
			ArgsV.push_back(ASTtoIR(E->Args[i].get(), ctx));
			if (!ArgsV.back())
				return nullptr;
		}

		return ctx.builder.CreateCall(CalleeF, ArgsV, "calltmp");
	}

	Function *PrototypeASTtoIR(PrototypeAST* e, const LLVM_IR_GEN_CTX& ctx) {
		// Make the function type:  double(double,double) etc.
		std::vector<llvm::Type*> Doubles(e->Args.size(),
			llvm::Type::getDoubleTy(ctx.context));
		FunctionType *FT =
			FunctionType::get(llvm::Type::getDoubleTy(ctx.context), Doubles, false);

		Function *F =
			Function::Create(FT, Function::ExternalLinkage, e->Name, &ctx.module);

		unsigned Idx = 0;
		for (auto &Arg : F->args())
			Arg.setName(e->Args[Idx++]);

		return F;
	}

	Function *FunctionASTtoIR(FunctionDefAST* e, const LLVM_IR_GEN_CTX& ctx) {
		// First, check for an existing function from a previous 'extern' declaration.
		Function *TheFunction = ctx.module.getFunction(e->Proto->getName());

		if (!TheFunction)
			TheFunction = PrototypeASTtoIR(e->Proto.get(), ctx);

		if (!TheFunction)
			return nullptr;

		if (!TheFunction->empty())
			return (Function*)LogErrorV("Function "+e->Proto->getName()+" cannot be redefined!");
		// Create a new basic block to start insertion into.
		BasicBlock *BB = BasicBlock::Create(ctx.context, "entry", TheFunction);
		ctx.builder.SetInsertPoint(BB);

		// Record the function arguments in the NamedValues map.
		NamedValues.clear();
		for (auto &Arg : TheFunction->args())
			NamedValues[Arg.getName()] = &Arg;
		if (Value *RetVal = ASTtoIR(e->Body.get(), ctx)) {
			// Finish off the function.
			ctx.builder.CreateRet(RetVal);

			// Validate the generated code, checking for consistency.
			if (!verifyFunction(*TheFunction)) {
				GRCOMP_INTERNAL_RUNTIME_ERROR("verification for generated IR code of function " + e->Proto->getName() + " failed!");
			}

			return TheFunction;
		}
		// Error reading body, remove function.
		TheFunction->eraseFromParent();
		return nullptr;
	}

	Value* ASTtoIR(const BaseAST* ast, const LLVM_IR_GEN_CTX& ctx) {
		switch (ast->get_ast_type())
		{
		default:
			GRCOMP_INTERNAL_RUNTIME_ERROR("Unknown AST expression passed! ast expression type is an invalid value: " + (int)ast->get_ast_type());
			throw 0;
			break;
		case AstType::NumberExprAST_t:	 return NumberExprASTtoIR(static_cast<const NumberExprAST*>(ast), ctx);
		case AstType::VariableExprAST_t: return VariableExprASTtoIR(static_cast<const VariableExprAST*>(ast), ctx);
		case AstType::BinaryExprAST_t:	 return BinaryExprASTtoIR(static_cast<const BinaryExprAST*>(ast), ctx);
		case AstType::CallExprAST_t:	 return CallExprASTtoIR(static_cast<const CallExprAST*>(ast), ctx);
		}
	}
}


