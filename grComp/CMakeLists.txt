
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB grComp_SRC
    "*.h"
    "*.cpp"
)
add_library(grComp ${grComp_SRC})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader passes)
target_link_libraries(grComp PUBLIC "${llvm_libs}")

target_include_directories(grComp PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/third-party"
						   "${CMAKE_CURRENT_SOURCE_DIR}/../"
						   "${LLVM_INCLUDE_DIRS}"
                           )