
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB grComp_SRC
    "*.h"
    "*.cpp"
)
add_library(grComp ${grComp_SRC})

target_include_directories(grComp PUBLIC
                           "${PROJECT_BINARY_DIR}"
						   "${CMAKE_CURRENT_SOURCE_DIR}/../core"
						   "${LLVM_INCLUDE}"
                           )