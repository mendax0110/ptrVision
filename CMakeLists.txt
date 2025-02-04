cmake_minimum_required(VERSION 3.10)
project(ptrVision)

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
target_link_libraries(${PROJECT_NAME} LLVM clangFrontend clangTooling clangAST clangBasic clangASTMatchers)
