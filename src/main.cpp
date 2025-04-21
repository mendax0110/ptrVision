#include <FileSystemManager.h>
#include <CliManager.h>
#include <GuiManager.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/ADT/StringRef.h>
#include <filesystem>
#include <vector>
#include <cstring>
#include <algorithm>

namespace fs = std::filesystem;

using namespace clang::tooling;
using namespace clang;
using namespace llvm;

static cl::OptionCategory Tool("ptrVision");

bool isGuiMode(int argc, const char** argv)
{
    // Look for --gui in the argument list
    return std::any_of(argv + 1, argv + argc, [](const char* arg) {
        return std::strcmp(arg, "--gui") == 0;
    });
}

int main(int argc, const char** argv)
{
#if LLVM_VERSION_MAJOR >= 14
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, Tool);
#elif LLVM_VERSION_MAJOR <= 12
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, Tool, llvm::cl::OneOrMore, nullptr);
#endif

    if (isGuiMode(argc, argv))
    {
        GuiManager guiManager;
        guiManager.Run();
        return 0;
    }

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

    const std::string inputPath = argv[1];

    if (argc == 3)
    {
        const std::string marker = argv[2];
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
        CliManager::showHelp();
        return 1;
    }

    return 0;
}
