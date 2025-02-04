#include "FileSystemManager.h"
#include "SyntaxHighlighter.h"
#include "PointerReferenceAction.h"

namespace fs = std::filesystem;
using namespace clang;
using namespace clang::tooling;
using namespace llvm;

void FileSystemManager::processFile(const std::string& filePath, std::vector<std::string>& compileArgs)
{
    llvm::outs() << "Processing file: " << filePath << "\n";
    std::vector<std::string> sourcePaths = { filePath };
    FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
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

    FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
    ClangTool Tool(EmptyDB, sourcePaths);
    Tool.run(newFrontendActionFactory<PointerReferenceAction>().get());
}
