#pragma once

#include "grlangConfig.h"
#include "Parser.h"

#include "CompileError.h"

#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"

#include "llvm/IR/LegacyPassManager.h"


namespace grComp {

	using namespace llvm;

	class GeneratorLLVM {
		std::unique_ptr<Parser> m_parser;
		std::map<std::string, Value *> NamedValues;

	public:
		llvm::LLVMContext context;
		llvm::IRBuilder<> builder;
		llvm::Module module;
		llvm::legacy::FunctionPassManager opt_fpm; // function pass manager (optimization passes)

		GeneratorLLVM(std::unique_ptr<Parser> parser);

		void generate();

	private:
		Error::CError throw_error(std::string msg);

		Error::ex<Value*> NumberExprASTtoIR(const NumberExprAST* e);
		Error::ex<Value*> VariableExprASTtoIR(const VariableExprAST* e);
		Error::ex<Value*> BinaryExprASTtoIR(const BinaryExprAST* e);
		Error::ex<Value*> CallExprASTtoIR(const CallExprAST* E);
		Error::ex<Function*> PrototypeASTtoIR(PrototypeAST* e);
		Error::ex<Function*> FunctionASTtoIR(FunctionDefAST* e);
		Error::ex<Value*> ExprASTtoIR(const ExprAST* ast);
	};
}