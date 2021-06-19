#include "Generator_llvm.h"

#include <string>
#include <sstream>

#include "ASTs.h"
#include "Logger.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"


namespace grComp {

	using namespace llvm;

	GeneratorLLVM::GeneratorLLVM(std::unique_ptr<Parser> parser)
		: 
		m_parser(std::move(parser)), 
		context(), 
		builder(context), 
		module("GrLang " + std::to_string(grlang_VERSION_MAJOR), context),
		opt_fpm(&module)
	{
		// Do simple "peephole" optimizations and bit-twiddling optzns.
		opt_fpm.add(llvm::createInstructionCombiningPass());
		// Reassociate expressions.
		opt_fpm.add(llvm::createReassociatePass());
		// Eliminate Common SubExpressions.
		opt_fpm.add(llvm::createGVNPass());
		// Simplify the control flow graph (deleting unreachable blocks, etc).
		opt_fpm.add(llvm::createCFGSimplificationPass());

		opt_fpm.doInitialization();
	}

	void GeneratorLLVM::generate() {	
		const auto mod = m_parser->get();
		if(!mod) Log::CompiletimeError(mod.error().to_string());

		for (const auto& e : (*mod)->lines) {
			switch (e->get_ast_type()) {
			case AstType::FunctionDefAST_t: {
				auto X = FunctionASTtoIR(reinterpret_cast<grComp::FunctionDefAST*>(e.get()));
				if(!X) Log::CompiletimeError(X.error().to_string());
				(*X)->print(llvm::errs());
			} break;
			default:
				GRCOMP_INTERNAL_RUNTIME_ERROR("unexpected ast of value " + (int)e->get_ast_type());
				break;
			}
		}
	}

	Error::CError GeneratorLLVM::throw_error(std::string msg){
		Error::CError err;
		err.compile_step = Error::CompileStep::generator;
		err.err_message = msg;
		err.hint_message = "";
		err.file = m_parser->get_filename().string();
		err.line = m_parser->get_line();
		return err;
	}

	Error::ex<Value*> GeneratorLLVM::NumberExprASTtoIR(const NumberExprAST* e) {
		return ConstantFP::get(context, APFloat(e->ValR));
	}

	Error::ex<Value*> GeneratorLLVM::VariableExprASTtoIR(const VariableExprAST* e) {
		// Look this variable up in the function.
		Value *V = NamedValues[e->Name];
		if (!V) return Error::unex(throw_error("The variable " + e->Name + " is undefined!"));
		return V;
	}

	Error::ex<Value*> GeneratorLLVM::BinaryExprASTtoIR(const BinaryExprAST* e) {
		auto L = ExprASTtoIR(e->LHS.get());
		auto R = ExprASTtoIR(e->RHS.get());
		if (!L) return L;
		if (!R) return R;

		switch (e->Op) {
		case '+':
			return builder.CreateFAdd(*L, *R, "addtmp");
		case '-':
			return builder.CreateFSub(*L, *R, "subtmp");
		case '*':
			return builder.CreateFMul(*L, *R, "multmp");
		case '<':
			L = builder.CreateFCmpULT(*L, *R, "cmptmp");
			// Convert bool 0/1 to double 0.0 or 1.0
			return builder.CreateUIToFP(*L, llvm::Type::getDoubleTy(context),
				"booltmp");
		default:
			return Error::unex(throw_error((std::string)"invalid binary operator \"" + (char)e->Op + "\""));
			// return LogErrorV((std::string)"invalid binary operator \"" + (char)e->Op + "\"");
		}
	}

	Error::ex<Value*> GeneratorLLVM::CallExprASTtoIR(const CallExprAST* E) {
		// Look up the name in the global module table.
		Function *CalleeF = module.getFunction(E->Callee);
		if (!CalleeF)
			return Error::unex(throw_error("Function " + E->Callee + " is undefined."));

		// If argument mismatch error.
		if (CalleeF->arg_size() != E->Args.size())
			return Error::unex(throw_error("Incorrect # arguments passed"));

		std::vector<Value *> ArgsV;
		for (unsigned i = 0, e = E->Args.size(); i != e; ++i) {
			auto X = ExprASTtoIR(E->Args[i].get());
			if(!X) return X;
			ArgsV.push_back(*X);
		}

		return builder.CreateCall(CalleeF, ArgsV, "calltmp");
	}

	Error::ex<Function*> GeneratorLLVM::PrototypeASTtoIR(PrototypeAST* e) {
		// Make the function type:  double(double,double) etc.
		std::vector<llvm::Type*> Doubles(e->Args.size(),
			llvm::Type::getDoubleTy(context));
		FunctionType *FT =
			FunctionType::get(llvm::Type::getDoubleTy(context), Doubles, false);

		Function *F =
			Function::Create(FT, Function::ExternalLinkage, e->Name, &module);

		unsigned Idx = 0;
		for (auto &Arg : F->args())
			Arg.setName(e->Args[Idx++]);

		return F;
	}

	Error::ex<Function*> GeneratorLLVM::FunctionASTtoIR(FunctionDefAST* e) {
		// First, check for an existing function from a previous 'extern' declaration.
		Function *TheFunction = module.getFunction(e->Proto->getName());

		if (!TheFunction){
			auto X = PrototypeASTtoIR(e->Proto.get());
			if(!X) return X;
			TheFunction = *X;
		}

		if (!TheFunction->empty())
			return Error::unex(throw_error("Function "+e->Proto->getName()+" cannot be redefined!"));
			// return (Function*)LogErrorV("Function "+e->Proto->getName()+" cannot be redefined!");
		// Create a new basic block to start insertion into.
		BasicBlock *BB = BasicBlock::Create(context, "entry", TheFunction);
		builder.SetInsertPoint(BB);

		// Record the function arguments in the NamedValues map.
		NamedValues.clear();
		for (auto &Arg : TheFunction->args())
			NamedValues[Arg.getName()] = &Arg;

		auto ReturnValue = ExprASTtoIR(e->Body.get());
		if (ReturnValue) {
			// Finish off the function.
			builder.CreateRet(*ReturnValue);

			{ // Validate the generated code, checking for consistency.
				std::string msg;
				llvm::raw_string_ostream msg_strm(msg);
				if (verifyFunction(*TheFunction, &msg_strm)) {
					GRCOMP_INTERNAL_RUNTIME_ERROR(
						"verification for generated IR code of function "
						+ e->Proto->getName() + " failed! \nllvm error message: \n"
						+ msg);
				}
			}

			// optimize IR
			opt_fpm.run(*TheFunction);

			return TheFunction;
		}else{
			// Error reading body, remove function.
			TheFunction->eraseFromParent();
			return Error::unex(ReturnValue.error());
		}
	}

	Error::ex<Value*> GeneratorLLVM::ExprASTtoIR(const ExprAST* ast) {
		switch (ast->get_ast_type())
		{
		default:
			GRCOMP_INTERNAL_RUNTIME_ERROR("Unknown AST expression passed or AST is not derived from ExprAST! ast expression type is an invalid value: " + (int)ast->get_ast_type());
			throw 0;
			break;
		case AstType::NumberExprAST_t:	 return NumberExprASTtoIR(static_cast<const NumberExprAST*>(ast));
		case AstType::VariableExprAST_t: return VariableExprASTtoIR(static_cast<const VariableExprAST*>(ast));
		case AstType::BinaryExprAST_t:	 return BinaryExprASTtoIR(static_cast<const BinaryExprAST*>(ast));
		case AstType::CallExprAST_t:	 return CallExprASTtoIR(static_cast<const CallExprAST*>(ast));
		}
	}
}


