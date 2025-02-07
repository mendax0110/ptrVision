#include "FileSystemManager.h"
#include "SyntaxHighlighter.h"
#include "PointerReferenceAction.h"
#include <regex>
#include <fstream>

namespace fs = std::filesystem;
using namespace clang;
using namespace clang::tooling;
using namespace llvm;

void FileSystemManager::processFile(const std::string& filePath, std::vector<std::string>& compileArgs)
{
    llvm::outs() << "Processing file: " << filePath << "\n";
    std::vector<std::string> sourcePaths = { filePath };
#if defined(__APPLE__) || defined(__linux__)
    FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
#elif defined(_WIN32)
    FixedCompilationDatabase EmptyDB("C:\\Temp", compileArgs);
#endif
    ClangTool Tool(EmptyDB, sourcePaths);
    Tool.run(newFrontendActionFactory<PointerReferenceAction>().get());
}

void FileSystemManager::processDirectory(const std::string& directoryPath, std::vector<std::string>& compileArgs)
{
    llvm::outs() << "Processing directory: " << directoryPath << "\n";
    std::vector<std::string> sourcePaths;
    
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp")
        {
            sourcePaths.push_back(entry.path().string());
        }
    }

    if (sourcePaths.empty())
    {
        printWarning("No .cpp files found in directory: " + directoryPath);
        return;
    }

#if defined(__APPLE__) || defined(__linux__)
    FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
#elif defined(_WIN32)
    FixedCompilationDatabase EmptyDB("C:\\Temp", compileArgs);
#endif
    ClangTool Tool(EmptyDB, sourcePaths);
    Tool.run(newFrontendActionFactory<PointerReferenceAction>().get());
}

void FileSystemManager::processSnippetFromSpecifiedFile(const std::string& filePath, std::vector<std::string>& compileArgs, const std::string& marker)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        printError("Error opening file: " + filePath);
        return;
    }

    std::string line;
    std::string codeSnippet;
    bool foundMarker = false;

    while (std::getline(file, line))
    {
        if (!foundMarker)
        {
            if (line.find(marker) != std::string::npos)
            {
                foundMarker = true;
            }
        }
        else
        {
            codeSnippet += line + "\n";
        }
    }

    if (codeSnippet.empty())
    {
        printWarning("No snippet found after marker '" + marker + "' in file: " + filePath);
        return;
    }

    printInfo("Extracted Code Snippet:\n" + codeSnippet);

    std::vector<std::string> sourcePaths = { filePath };

#if defined(__APPLE__) || defined(__linux__)
    FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
#elif defined(_WIN32)
    FixedCompilationDatabase EmptyDB("C:\\Temp", compileArgs);
#endif

    ClangTool Tool(EmptyDB, sourcePaths);
    Tool.run(newFrontendActionFactory<PointerReferenceAction>().get());
}

