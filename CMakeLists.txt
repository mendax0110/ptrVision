cmake_minimum_required(VERSION 3.10)
project(ptrVision VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES
    src/main.cpp
    src/PointerReferenceVisitor.cpp
    src/PointerReferenceConsumer.cpp
    src/FileSystemManager.cpp
    src/CliManager.cpp
    src/LibExporter.cpp
)

set(INCLUDE_DIRS include)


if (WIN32)
    set(CMAKE_GENERATOR_TOOLSET "v143")
    set(CMAKE_GENERATOR_INSTANCE "C:/Program Files/Microsoft Visual Studio/2022/Community")

    set(CLANG_PATH "C:/clang_llvm/bin")
    set(LLVM_DIR "C:/clang_llvm/lib/cmake/llvm")
    set(CLANG_DIR "C:/clang_llvm/lib/cmake/clang")
    set(CMAKE_CXX_COMPILER "${CLANG_PATH}/clang++.exe")
    set(CMAKE_C_COMPILER "${CLANG_PATH}/clang.exe")

    set(DIA_SDK_PATH "C:/Program Files/Microsoft Visual Studio/2022/Community/DIA SDK")
    list(APPEND INCLUDE_DIRS "${DIA_SDK_PATH}/include")
    link_directories("${DIA_SDK_PATH}/lib/amd64")

    set(LIBXML2_INCLUDE_DIR "C:/libxml/libxml2-2.9.1-win32-x86_64/include/libxml2")
    set(LIBXML2_LIBRARY "C:/libxml/libxml2-2.9.1-win32-x86_64/lib/libxml2.dll.a")

else()
    find_program(LLVM_CONFIG llvm-config REQUIRED)

    execute_process(
        COMMAND ${LLVM_CONFIG} --prefix
        OUTPUT_VARIABLE LLVM_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "LLVM_PREFIX is ${LLVM_PREFIX}")

    set(LLVM_DIR "${LLVM_PREFIX}/lib/cmake/llvm")
    set(CLANG_DIR "${LLVM_PREFIX}/lib/cmake/clang")

    find_package(LibXml2 REQUIRED)
    set(LIBXML2_INCLUDE_DIR ${LIBXML2_INCLUDE_DIRS})
    set(LIBXML2_LIBRARY ${LIBXML2_LIBRARIES})
endif()

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
add_compile_options(-fexceptions)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE PTRVISION_EXPORTS)

if (WIN32)
    add_library(${PROJECT_NAME}_dll SHARED ${SOURCE_FILES})
    target_include_directories(${PROJECT_NAME}_dll PRIVATE ${INCLUDE_DIRS})
elseif (UNIX)
    add_library(${PROJECT_NAME}_so SHARED ${SOURCE_FILES})
    target_include_directories(${PROJECT_NAME}_so PRIVATE ${INCLUDE_DIRS})
endif()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4267)
    if (TARGET ${PROJECT_NAME}_dll)
        target_compile_options(${PROJECT_NAME}_dll PRIVATE /wd4267)
    endif()
endif()

set(CLANG_LIBS
    clangFrontend
    clangTooling
    clangAST
    clangBasic
    clangASTMatchers
)

if (WIN32)
    llvm_map_components_to_libnames(LLVM_LIBS support core irreader)
    set(COMMON_LIBS ${LLVM_LIBS} ${CLANG_LIBS})
else()
    set(COMMON_LIBS LLVM ${CLANG_LIBS})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LIBS})
if (TARGET ${PROJECT_NAME}_dll)
    target_link_libraries(${PROJECT_NAME}_dll PRIVATE ${COMMON_LIBS})
elseif (TARGET ${PROJECT_NAME}_so)
    target_link_libraries(${PROJECT_NAME}_so PRIVATE ${COMMON_LIBS})
endif()
