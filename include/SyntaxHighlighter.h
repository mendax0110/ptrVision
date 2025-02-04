#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <llvm/Support/raw_ostream.h>
#include <string>

#define RESET_COLOR   "\033[0m"
#define RED_COLOR     "\033[31m"
#define GREEN_COLOR   "\033[32m"
#define YELLOW_COLOR  "\033[33m"
#define BLUE_COLOR    "\033[34m"
#define CYAN_COLOR    "\033[36m"
#define MAGENTA_COLOR "\033[35m"

/**
 * @brief Prints a highlighted pointer in cyan
 * @param type -> type of the pointer
 */
inline void printHighlightedPointer(const std::string &type)
{
    llvm::outs() << CYAN_COLOR << type << RESET_COLOR;
}

/**
 * @brief Prints a highlighted reference in green
 * @param type -> type of the reference
 */
inline void printHighlightedReference(const std::string &type)
{
    llvm::outs() << GREEN_COLOR << type << RESET_COLOR;
}

/**
 * @brief Prints a highlighted keyword in blue
 * @param keyword -> keyword to highlight
 */
inline void printHighlightedKeyword(const std::string &keyword)
{
    llvm::outs() << BLUE_COLOR << keyword << RESET_COLOR;
}

/**
 * @brief Prints an error message in red
 * @param message -> error message
 */
inline void printError(const std::string &message)
{
    llvm::errs() << RED_COLOR << message << RESET_COLOR << "\n";
}

/**
 * @brief Prints a warning message in yellow
 * @param message -> warning message
 */
inline void printWarning(const std::string &message)
{
    llvm::outs() << YELLOW_COLOR << message << RESET_COLOR << "\n";
}

/**
 * @brief Prints an information message in magenta
 * @param message -> information message
 */
inline void printInfo(const std::string &message)
{
    llvm::outs() << MAGENTA_COLOR << message << RESET_COLOR << "\n";
}

#endif // SYNTAXHIGHLIGHTER_H
