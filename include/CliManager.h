#ifndef CLIMANAGER_H
#define CLIMANAGER_H

#include <iostream>
#include <string>
#include <llvm/Support/FileSystem.h>
#include <SyntaxHighlighter.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Error.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringExtras.h>

/// @brief Class to manage the command line interface \class CliManager
class CliManager
{
public:

    /**
     * @brief Method to show the help message
     * 
     */
    static void showHelp();

    /**
     * @brief Method to print error messages in red
     * @param errorMessage -> error message to print (llvm::Error)
     */
    static void printError(llvm::Error errorMessage);

    /**
     * @brief Method to print error messages in red
     * @param errorMessage -> error message to print (std::string&)
     */
    static void printError(const std::string& errorMessage);


private:
    std::string helpText;
    std::string helpFilePath;
};

#endif // CLIMANAGER_H
