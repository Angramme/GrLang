#include "testing.h"

#include "grComp/ASTs.h"

#include "core/BaseTypes.h"
#include "gVM/Memory.h"
#include "gVM/execute.h"
#include "gVM/ProgramMemory.h"


TEST_FILE_START()

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

TEST_CATEGORY_END()

TEST_CATEGORY_START(AST methods)

grComp::VariableExprAST test("yessss");
TEST_FUNCTION(test.get_ast_type(), grComp::AstType::VariableExprAST_t);
TEST_FUNCTION(test.is_lazy(), false)
TEST_FUNCTION(test.is_definition(), false)
TEST_FUNCTION(test.is_expression(), true)


TEST_CATEGORY_END()

TEST_FILE_END()