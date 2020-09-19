cmake_minimum_required(VERSION 3.10)

# set the project name
project(grlang VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(configX32 True)
	set(configX64 False)
else()
	set(configX32 False)
	set(configX64 True)
endif()

configure_file(grlangConfig.h.in grlangConfig.h)


add_subdirectory(gVM)
add_subdirectory(grComp)

# add the executable
add_executable(grlang grlang.cxx)

target_link_libraries(grlang PUBLIC gVM)
target_link_libraries(grlang PUBLIC grComp)


target_include_directories(grlang PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/gVM"
						   "${PROJECT_BINARY_DIR}/core"
						   "${PROJECT_BINARY_DIR}/grComp"
						   "${PROJECT_BINARY_DIR}/third-party"
                           )