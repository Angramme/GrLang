
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(grlang_ast_tests ast_tests.cxx)
add_executable(grlang_gvm_tests gvm_tests.cxx)

set(LIB_FILES 
	grComp
	gVM
	)

target_link_libraries(grlang_ast_tests PRIVATE "${LIB_FILES}")
target_link_libraries(grlang_gvm_tests PRIVATE "${LIB_FILES}")

set(INCLUDE_DIRS
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/../"
	"${LLVM_INCLUDE_DIRS}"
	)

target_include_directories(grlang_ast_tests PUBLIC "${INCLUDE_DIRS}")
target_include_directories(grlang_gvm_tests PUBLIC "${INCLUDE_DIRS}")


enable_testing()
add_test(basic_ast_tests grlang_ast_tests)
add_test(basic_gvm_tests grlang_gvm_tests)

