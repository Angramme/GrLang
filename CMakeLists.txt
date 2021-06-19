cmake_minimum_required(VERSION 3.10)

# set the project name
project(grlang VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# ARCHITECTURE
if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(configX32 True)
	set(configX64 False)
else()
	set(configX32 False)
	set(configX64 True)
endif()

set(PLATFORM_UNIX ${UNIX})
set(PLATFORM_WIN32 ${WIN32})


# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})

# DEFINITION HEADER FILE
include(CheckTypeSize)
check_type_size("void*" SIZEOF_VOID_P BUILTIN_TYPES_ONLY)
configure_file(grlangConfig.h.in grlangConfig.h)

add_subdirectory(gVM)
add_subdirectory(grComp)
add_subdirectory(Tests)

### EXECUTABLES
# add the compiler executable
add_executable(grlang grlang.cxx)


### LINKING
target_link_libraries(grlang PUBLIC gVM)
target_link_libraries(grlang PUBLIC grComp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
# llvm_map_components_to_libnames(llvm_libs support core irreader)
# target_link_libraries(grlang PUBLIC "${llvm_libs}")


### INCLUDE DIRECTORIES
target_include_directories(grlang PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/gVM"
						   "${PROJECT_BINARY_DIR}/core"
						   "${PROJECT_BINARY_DIR}/grComp"
						   "${PROJECT_BINARY_DIR}/third-party"
						   "${LLVM_INCLUDE_DIRS}"
                           )

