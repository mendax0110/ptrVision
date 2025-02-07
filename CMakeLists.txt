cmake_minimum_required(VERSION 3.10)
project(ptrVision)

if (WIN32)
    set(CMAKE_GENERATOR_TOOLSET "v143")
    set(CMAKE_GENERATOR_INSTANCE "C:/Program Files/Microsoft Visual Studio/2022/Community")
endif()

if (WIN32)
    set(DIA_SDK_PATH "C:/Program Files/Microsoft Visual Studio/2022/Community/DIA SDK")
    include_directories("${DIA_SDK_PATH}/include")
    link_directories("${DIA_SDK_PATH}/lib/amd64")
endif()

if (WIN32)
    set(CLANG_PATH "C:/clang_llvm/bin")
    set(LLVM_DIR "C:/clang_llvm/lib/cmake/llvm")
    set(CLANG_DIR "C:/clang_llvm/lib/cmake/clang")
    set(CMAKE_CXX_COMPILER "${CLANG_PATH}/clang++.exe")
    set(CMAKE_C_COMPILER "${CLANG_PATH}/clang.exe")

    set(LIBXML2_PATH "C:/libxml/libxml2-2.9.1-win32-x86_64/bin")
    set(LIBXML2_INCLUDE_DIR "C:/libxml/libxml2-2.9.1-win32-x86_64/include/libxml2")
    set(LIBXML2_LIBRARY "C:/libxml/libxml2-2.9.1-win32-x86_64/lib/libxml2.dll.a")
endif()


find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}  
    src/main.cpp 
    src/PointerReferenceVisitor.cpp 
    src/PointerReferenceConsumer.cpp
    src/FileSystemManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

if (WIN32)
    llvm_map_components_to_libnames(LLVM_LIBS support core irreader)
    target_link_libraries(${PROJECT_NAME} ${LLVM_LIBS} clangFrontend clangTooling clangAST clangBasic clangASTMatchers)
else()
    target_link_libraries(${PROJECT_NAME} LLVM clangFrontend clangTooling clangAST clangBasic clangASTMatchers)
endif()