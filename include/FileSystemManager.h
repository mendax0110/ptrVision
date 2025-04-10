#ifndef FILE_SYSTEM_MANAGER_H
#define FILE_SYSTEM_MANAGER_H

#include <vector>
#include <filesystem>
#include <PointerReferenceVisitor.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/ADT/StringRef.h>


/// @brief The clang namespace \namespace clang
namespace clang
{
    /**
     * @brief FileSystemManager is a class that processes files and directories \class FileSystemManager
     * 
     */
    class FileSystemManager
    {
    public:

        /**
         * @brief Processes a file, creating a ClangTool to run the PointerReferenceAction
         * 
         * @param filePath -> file path
         * @param compileArgs -> compilation arguments
         */
        static void processFile(const std::string& filePath, std::vector<std::string>& compileArgs);

        /**
         * @brief Processes a directory, creating a ClangTool to run the PointerReferenceAction
         * 
         * @param directoryPath -> directory path
         * @param compileArgs -> compilation arguments
         */
        static void processDirectory(const std::string& directoryPath, std::vector<std::string>& compileArgs);

        /**
         * @brief Processes a directory, creating a ClangTool to run the PointerReferenceAction
         * 
         * @param filePath -> path to the file
         * @param compileArgs -> compilation arguments
         * @param marker -> marker to identify the snippet, which will be processed
         */
        static void processSnippetFromSpecifiedFile(const std::string& filePath, const std::string& marker);
    };
} // namespace clang

#endif // FILE_SYSTEM_MANAGER_H