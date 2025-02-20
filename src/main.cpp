#include "FileSystemManager.h"
#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/ADT/StringRef.h>
#include <filesystem>
#include <vector>
#include <cstring>

namespace fs = std::filesystem;

using namespace clang::tooling;
using namespace clang;
using namespace llvm;

static cl::OptionCategory Tool("ptrVision");

static void showHelp()
{
    errs() << "Usage: ptrVision <file|directory> [marker] [options]\n";
    errs() << "Options:\n";
    errs() << "  -help\t\tShow this help message\n";
}

int main(int argc, const char** argv)
{
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, Tool);
    if (!ExpectedParser)
    {
        errs() << "Error: " << toString(ExpectedParser.takeError()) << "\n";
        return 1;
    }

    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    std::vector<std::string> compileArgs;

    if (argc < 2)
    {
        errs() << "Please provide a file or directory to analyze.\n";
        return 1;
    }

    std::string inputPath = argv[1];

    if (argc == 3)
    {
        std::string marker = argv[2];
        FileSystemManager::processSnippetFromSpecifiedFile(inputPath, marker);
    }
    else if (fs::is_directory(inputPath))
    {
        FileSystemManager::processDirectory(inputPath, compileArgs);
    }
    else if (fs::is_regular_file(inputPath))
    {
        FileSystemManager::processFile(inputPath, compileArgs);
    }
    else
    {
        errs() << "Invalid input path: " << inputPath << "\n";
        return 1;
    }

    return 0;
}