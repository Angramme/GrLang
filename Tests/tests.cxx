#include <iostream>
#include "grComp/ASTs.h"


#define TEST_CATEGORY_START(name) {\
	CATEGORY_NAME = #name; \
	std::cout << ("\nlaunching " #name " tests...") << std::endl; \
	unsigned long long test_num = 1; \
	unsigned long long tests_failed = 0; \

#define TEST_CATEGORY_END \
	if(tests_failed) std::cout << tests_failed << " of " << test_num-1 << " " << CATEGORY_NAME << " tests failed..." << std::endl; \
	else std::cout << "all " << CATEGORY_NAME << " tests passed..." << std::endl; }

#define TEST_FUNCTION(func_call, result) {\
		auto val = func_call; \
		TESTS_EXECUTED += 1; \
		if (val != (result)) { \
			std::cout << "\n====!!!{\ntest number " << test_num << " failed! (line:" << __LINE__ << ")" << std::endl; \
			std::cout << "\t\"" #func_call "\"" << std::endl; \
			std::cout << "\texpected " #result "\n}====!!!" << std::endl; \
			TESTS_FAILED += 1; \
			tests_failed += 1; \
		} \
		test_num++; \
	}

int main() {
	std::cout << "\n====\nRunning tests..." << std::endl;

	std::string CATEGORY_NAME = "";
	unsigned long long TESTS_EXECUTED = 0;
	unsigned long long TESTS_FAILED = 0;


	
	TEST_CATEGORY_START(AST static functions)

	TEST_FUNCTION(grComp::BaseAST::is_lazy<grComp::AstType::BinaryExprAST_t>(), false);
	TEST_FUNCTION(grComp::BaseAST::is_expression<grComp::BinaryExprAST>(), true);
	TEST_FUNCTION(grComp::BaseAST::is_expression<grComp::CallExprAST>(), true);
	TEST_FUNCTION(grComp::BaseAST::is_expression<grComp::PrototypeAST>(), false);

	TEST_FUNCTION(grComp::BaseAST::is_definition<grComp::BinaryExprAST>(), false);
	TEST_FUNCTION(grComp::BaseAST::is_definition<grComp::CallExprAST>(), false);
	TEST_FUNCTION(grComp::BaseAST::is_definition<grComp::FunctionDefAST>(), true);
	TEST_FUNCTION(grComp::BaseAST::is_definition<grComp::PrototypeAST>(), false);

	TEST_FUNCTION(grComp::BaseAST::is_lazy(grComp::AstType::BinaryExprAST_t), false);
	TEST_FUNCTION(grComp::BaseAST::is_definition(grComp::AstType::FunctionDefAST_t), true);
	TEST_FUNCTION(grComp::BaseAST::is_expression(grComp::AstType::BinaryExprAST_t), true);

	TEST_CATEGORY_END



	TEST_CATEGORY_START(AST methods)

	grComp::VariableExprAST test("yessss");
	TEST_FUNCTION(test.get_ast_type(), grComp::AstType::VariableExprAST_t);
	TEST_FUNCTION(test.is_lazy(), false)
	TEST_FUNCTION(test.is_definition(), false)
	TEST_FUNCTION(test.is_expression(), true)


	TEST_CATEGORY_END



	if (TESTS_FAILED > 0) {
		std::cout << "\nFailed " << TESTS_FAILED  << " of " << TESTS_EXECUTED << " tests!!!" << std::endl;
		return 10;
	}

	std::cout << "\nPassed all " << TESTS_EXECUTED << " tests with no errors..." << std::endl;
	return 0;
}