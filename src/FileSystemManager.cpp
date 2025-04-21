#include <FileSystemManager.h>
#include <SyntaxHighlighter.h>
#include <PointerReferenceAction.h>
#include <regex>
#include <fstream>
#include <optional>
#include <sstream>
#include "PointerReferenceFactory.h"

namespace fs = std::filesystem;
using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// Initialize static members
std::stringstream FileSystemManager::stdOutBuffer;
std::stringstream FileSystemManager::stdErrBuffer;
std::string FileSystemManager::stdOutContent;
std::string FileSystemManager::stdErrContent;
std::streambuf* FileSystemManager::originalStdOut = nullptr;
std::streambuf* FileSystemManager::originalStdErr = nullptr;

void FileSystemManager::processFile(const std::string& filePath, std::vector<std::string>& compileArgs)
{
    try
    {
        printCanonicalFilePath(filePath);

        const std::vector<std::string> sourcePaths = { filePath };

#if defined(_WIN32)
        FixedCompilationDatabase EmptyDB("C:\\Temp", compileArgs);
#else
        FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
#endif
        ClangTool Tool(EmptyDB, sourcePaths);
        const auto factory = std::make_unique<PointerReferenceFactory>();
        Tool.run(factory.get());

        if (const auto action = factory->getLastAction())
        {
            stdOutContent += action->getOutput();
        }
    }
    catch (const std::exception& e)
    {
        printError("Exception: " + std::string(e.what()));
        throw std::runtime_error("Exception: " + std::string(e.what()));
    }
}

void FileSystemManager::processDirectory(const std::string& directoryPath, std::vector<std::string>& compileArgs)
{
    try
    {
        printCanonicalDirectoryPath(directoryPath);

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

#if defined(_WIN32)
        FixedCompilationDatabase EmptyDB("C:\\Temp", compileArgs);
#else
        FixedCompilationDatabase EmptyDB("/tmp", compileArgs);
#endif
        ClangTool Tool(EmptyDB, sourcePaths);
        auto factory = std::make_unique<PointerReferenceFactory>();
        Tool.run(factory.get());

        if (auto action = factory->getLastAction())
        {
            stdOutContent += action->getOutput();
        }
    }
    catch (const std::exception& e)
    {
        printError("Exception: " + std::string(e.what()));
        throw std::runtime_error("Exception: " + std::string(e.what()));
    }
}

void FileSystemManager::processSnippetFromSpecifiedFile(const std::string& filePath, const std::string& marker)
{
    try
    {
        std::vector<std::string> compileArgs;

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            printError("Error opening file: " + filePath);
            return;
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // find function call after the marker
        std::regex markerRegex(R"(\s*)" + marker + R"(\s+([a-zA-Z_][a-zA-Z0-9_]*)\(\);)");
        std::smatch match;
        if (!std::regex_search(content, match, markerRegex))
        {
            printWarning("No function marked with '" + marker + "' in file: " + filePath);
            return;
        }

        std::string functionName = match[1].str();

        // extract the function definition
        std::optional<size_t> startPos = content.find(functionName + "(");
        if (!startPos.has_value() || startPos == std::string::npos)
        {
            printError("Function '" + functionName + "' not found in file.");
            return;
        }

        std::optional<size_t> bracePos = content.find('{', startPos.value());
        if (!bracePos.has_value() || bracePos == std::string::npos)
        {
            printError("Opening brace '{' not found for function '" + functionName + "'.");
            return;
        }

        // count braces to find the closing brace
        int braceCount = 1;
        size_t endPos = bracePos.value() + 1;
        while (endPos < content.size() && braceCount > 0)
        {
            if (content[endPos] == '{') braceCount++;
            else if (content[endPos] == '}') braceCount--;
            endPos++;
        }

        if (braceCount != 0)
        {
            printError("Mismatched braces in function extraction.");
            return;
        }

        // extract text and ensure valid code
        std::string functionCode = content.substr(startPos.value(), endPos - startPos.value());
        std::regex arrayFixRegex(R"(\b(int|float|double|char)\s+\w+\s*\[.*\]\s*=\s*\{[^}]*\})");
        functionCode = std::regex_replace(functionCode, arrayFixRegex, "$&;");

        // write to temp file
        std::string tempFilePath = "temp_snippet.cpp";
        std::ofstream tempFile(tempFilePath);
        tempFile << functionCode;
        tempFile.close();

        printInfo("Extracted Function:\n" + functionCode);

        std::vector<std::string> sourcePaths = { tempFilePath };

        std::string tempPath =
#if defined(_WIN32)
                "C:\\Temp";
#else
                "/tmp";
#endif
        if (!fs::exists(tempPath)) fs::create_directory(tempPath);

        FixedCompilationDatabase EmptyDB(tempPath, compileArgs);
        ClangTool Tool(EmptyDB, sourcePaths);
        auto factory = std::make_unique<PointerReferenceFactory>();
        Tool.run(factory.get());

        if (auto action = factory->getLastAction())
        {
            stdOutContent += action->getOutput();
        }

        fs::remove(tempFilePath);
    }
    catch (const std::exception& e)
    {
        printError("Exception: " + std::string(e.what()));
        throw std::runtime_error("Exception: " + std::string(e.what()));
    }
}

std::string FileSystemManager::getStdOut()
{
    return stdOutContent;
}

std::string FileSystemManager::getStdErr()
{
    return stdErrContent;
}

void FileSystemManager::clearAllOut()
{
    stdOutContent.clear();
    stdErrContent.clear();
}
