cmake_minimum_required(VERSION 3.10)
project(ptrVision VERSION 2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(SOURCE_LIB_CORE
    src/PointerReferenceCollector.cpp
    src/CollectorConsumer.cpp
    src/CollectorAction.cpp
    src/CollectorActionWithStorage.cpp
    src/PtrVisionLib.cpp
)

set(INCLUDE_DIRS include)


if (WIN32)
    set(CLANG_PATH "C:/clang_llvm/bin")
    set(LLVM_DIR "C:/clang_llvm/lib/cmake/llvm")
    set(CLANG_DIR "C:/clang_llvm/lib/cmake/clang")
elseif (APPLE)
    find_program(LLVM_CONFIG llvm-config REQUIRED)
    execute_process(
        COMMAND ${LLVM_CONFIG} --prefix
        OUTPUT_VARIABLE LLVM_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "LLVM_PREFIX is ${LLVM_PREFIX}")
    set(LLVM_DIR "${LLVM_PREFIX}/lib/cmake/llvm")
    set(CLANG_DIR "${LLVM_PREFIX}/lib/cmake/clang")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_program(LLVM_CONFIG NAMES llvm-config llvm-config-19 llvm-config-18 llvm-config-17 REQUIRED)
    execute_process(
        COMMAND ${LLVM_CONFIG} --prefix
        OUTPUT_VARIABLE LLVM_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "LLVM_PREFIX is ${LLVM_PREFIX}")
    execute_process(
        COMMAND ${LLVM_CONFIG} --cmakedir
        OUTPUT_VARIABLE LLVM_CMAKE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(LLVM_DIR "${LLVM_CMAKE_DIR}")
    
    execute_process(
        COMMAND ${LLVM_CONFIG} --prefix
        OUTPUT_VARIABLE CLANG_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CLANG_DIR "${CLANG_PREFIX}/lib/cmake/clang")
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

execute_process(
    COMMAND ${LLVM_CONFIG} --includedir
    OUTPUT_VARIABLE LLVM_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
include_directories(${LLVM_INCLUDE_DIR})

add_compile_options(-fexceptions)

add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB_CORE})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE PTRVISION_EXPORTS)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4267)
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    message(STATUS "Building for ARM architecture")
    set(CLANG_LIBS
        clangFrontend
        clangTooling
        clangAST
        clangBasic
        clangASTMatchers
        clangLex
        clangSerialization
        clangParse
        clangSema
        clangAnalysis
    )
else()
    message(STATUS "Building for x86_64 architecture")
    set(CLANG_LIBS
        clangFrontend
        clangTooling
        clangAST
        clangBasic
        clangASTMatchers
        clangLex
        clangSerialization
        clangParse
        clangSema
        clangAnalysis
    )
endif()

if (APPLE OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_library(CLANG_CPP_LIB NAMES clang-cpp libclang-cpp.so.18 libclang-cpp.so.17 libclang-cpp.so.16
                 PATHS 
                 ${LLVM_LIBRARY_DIRS} 
                 /usr/lib/llvm-18/lib 
                 /usr/lib/llvm-17/lib 
                 /usr/lib/llvm-16/lib
                 /usr/lib/x86_64-linux-gnu
                 /usr/lib/aarch64-linux-gnu)
    if(CLANG_CPP_LIB)
        message(STATUS "Found clang-cpp library: ${CLANG_CPP_LIB}")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${CLANG_CPP_LIB})
    else()
        message(FATAL_ERROR "Could not find clang-cpp library")
    endif()

    execute_process(
        COMMAND ${LLVM_CONFIG} --ldflags
        OUTPUT_VARIABLE LLVM_LDFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${LLVM_CONFIG} --system-libs
        OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(STRIP "${LLVM_LDFLAGS}" LLVM_LDFLAGS)
    string(STRIP "${LLVM_SYSTEM_LIBS}" LLVM_SYSTEM_LIBS)

    if(LLVM_LDFLAGS)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LDFLAGS})
    endif()

    if(LLVM_SYSTEM_LIBS)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_SYSTEM_LIBS})
    endif()
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CLANG_LIBS})
endif()

add_executable(${PROJECT_NAME}_example examples/simple_example.cpp)
target_link_libraries(${PROJECT_NAME}_example PRIVATE ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_example PRIVATE ${INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES 
    include/PtrVisionLib.h
    include/PointerReferenceCollector.h
    include/CollectorConsumer.h
    include/CollectorAction.h
    include/CollectorActionWithStorage.h
    DESTINATION include
)


option(BUILD_TESTS "Build the test suite" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
