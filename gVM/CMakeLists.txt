
file(GLOB gVM_SRC
    "*.h"
    "*.cpp"
)
add_library(gVM ${gVM_SRC})

target_include_directories(gVM PUBLIC
                           "${PROJECT_BINARY_DIR}"
						   "${CMAKE_CURRENT_SOURCE_DIR}/../"
                           )