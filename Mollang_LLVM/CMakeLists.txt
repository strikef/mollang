cmake_minimum_required(VERSION 3.13.0)
project(mollang-llvm VERSION 1.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
if (MSVC AND BUILD_SHARED_LIBS)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif() # MSVC

option(FORCE_UTF8 "Force using UTF-8 on presumably unsupported platform" OFF)
if (FORCE_UTF8)
add_compile_definitions(FORCE_UTF8)
endif() # FORCE_UTF8

# Specify LLVM directory to use
# set(LLVM_ROOT )
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_library(PortableString ${SRC_DIR}/portable_string.cpp)
add_library(FS ${SRC_DIR}/fs.cpp)

add_library(MollangToken OBJECT ${SRC_DIR}/token.cpp)
target_compile_options(MollangToken PRIVATE -fPIC)
target_link_libraries(MollangToken PortableString)

# add_library(MollangParser ${SRC_DIR}/parser.cpp)
add_executable(tokenizer ${SRC_DIR}/tokenizer_test.cpp)
target_link_libraries(tokenizer PRIVATE MollangToken PortableString FS)


