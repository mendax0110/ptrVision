cmake_minimum_required(VERSION 3.10)
project(ptrVision VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (__LINUX__ OR __APPLE__)
    find_package(GLFW3 REQUIRED)
    set(GLFW3_INCLUDE_DIRS "/opt/homebrew/opt/glfw/include")
    set(GLFW3_LIBRARIES "/opt/homebrew/opt/glfw/lib/libglfw.3.dylib")
endif()

if (WIN32)
    set(GLFW3_INCLUDE_DIRS "C:/glfw/include")
    set(GLFW3_LIBRARIES "C:/glfw/lib/glfw3.dll.a")
endif()

find_package(OpenGL REQUIRED)

include_directories(
        external/dearimgui
        external/imgui_file_dialog
        external/ImGuiColorTextEdit
        external/imgui_club/imgui_multicontext_compositor
        external/imgui_club/imgui_memory_editor
        ${GLFW3_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
)

set(IMGUI_SOURCES
        external/dearimgui/imgui.cpp
        external/dearimgui/imgui_draw.cpp
        external/dearimgui/imgui_tables.cpp
        external/dearimgui/imgui_widgets.cpp
        external/dearimgui/backends/imgui_impl_glfw.cpp
        external/dearimgui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_FILE_DIALOG_SOURCES
        external/ImGuiFileDialog/ImGuiFileDialog.cpp
)

set(IMGUI_COLOR_TEXT_EDIT_SOURCES
        external/ImGuiColorTextEdit/TextEditor.cpp
)

set(IMGUI_CLUB
        external/imgui_club/imgui_multicontext_compositor/imgui_multicontext_compositor.cpp
)

set(SOURCE_CORE
        src/PointerReferenceVisitor.cpp
        src/PointerReferenceConsumer.cpp
        src/PointerReferenceAction.cpp
        src/AsmGen.cpp
        src/FileSystemManager.cpp
        src/CliManager.cpp
        src/LibExporter.cpp
        src/FileHandler.cpp
        src/Logger.cpp
)

set(SOURCE_GUI
        src/GuiManager.cpp
        src/TreeNode.cpp
        src/JsonParser.cpp
        src/ProjectExplorer.cpp
        src/main.cpp
        ${IMGUI_SOURCES}
        ${IMGUI_FILE_DIALOG_SOURCES}
        ${IMGUI_COLOR_TEXT_EDIT_SOURCES}
        ${IMGUI_CLUB}
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

add_executable(${PROJECT_NAME} ${SOURCE_CORE} ${SOURCE_GUI})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE PTRVISION_EXPORTS)

if (WIN32)
    add_library(${PROJECT_NAME}_dll SHARED ${SOURCE_CORE})
    target_include_directories(${PROJECT_NAME}_dll PRIVATE ${INCLUDE_DIRS})
elseif (UNIX)
    if (LINUX)
        add_library(${PROJECT_NAME}_so SHARED ${SOURCE_CORE})
        target_include_directories(${PROJECT_NAME}_so PRIVATE ${INCLUDE_DIRS})
    elseif (APPLE)
        add_library(${PROJECT_NAME}_dylib SHARED ${SOURCE_CORE})
        target_include_directories(${PROJECT_NAME}_dylib PRIVATE ${INCLUDE_DIRS})
    endif ()
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

        clangCodeGen
        clangFrontend
        clangDriver
        clangSerialization
        clangParse
        clangSema
        clangAnalysis
        clangAST
        clangLex
        clangBasic
        LLVMCore
        LLVMSupport

        LLVMAArch64CodeGen
        LLVMAArch64AsmParser
        LLVMAArch64Desc
        LLVMAArch64Disassembler
        LLVMAArch64Info
        LLVMAArch64Utils
)

if (WIN32)
    llvm_map_components_to_libnames(LLVM_LIBS support core irreader)
    set(COMMON_LIBS ${LLVM_LIBS} ${CLANG_LIBS})
else()
    set(COMMON_LIBS LLVM ${CLANG_LIBS})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LIBS} ${GLFW3_LIBRARIES} ${OPENGL_LIBRARIES} /opt/homebrew/opt/glfw/lib/libglfw.3.dylib)

if (__APPLE__)
    target_link_libraries(${PROJECT_NAME} PRIVATE /opt/homebrew/opt/glfw/lib/libglfw.dylib)
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW3_LIBRARIES})
endif()

if (TARGET ${PROJECT_NAME}_dll)
    target_link_libraries(${PROJECT_NAME}_dll PRIVATE ${COMMON_LIBS})
elseif (TARGET ${PROJECT_NAME}_so)
    target_link_libraries(${PROJECT_NAME}_so PRIVATE ${COMMON_LIBS})
elseif (TARGET ${PROJECT_NAME}_dylib)
    target_link_libraries(${PROJECT_NAME}_dylib PRIVATE ${COMMON_LIBS})
endif()
