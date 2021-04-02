
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(grlang_tests tests.cxx)


# target_link_libraries(grlang_tests PUBLIC gVM)
# target_link_libraries(grlang_tests PUBLIC grComp)
target_link_libraries(grlang_tests PRIVATE grComp gVM)

target_include_directories(grlang_tests PUBLIC
                           "${PROJECT_BINARY_DIR}"
						   "${CMAKE_CURRENT_SOURCE_DIR}/../"
						   "${LLVM_INCLUDE}"
						   "${LLVM_BUILD_INCLUDE}"
                           )



add_custom_command(TARGET grlang_tests POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/grlang_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running GrLang tests..."
	)