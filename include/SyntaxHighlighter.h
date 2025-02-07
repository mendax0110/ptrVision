#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <llvm/Support/raw_ostream.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

// macOS and Linux
#define RESET_COLOR   "\033[0m"
#define RED_COLOR     "\033[31m"
#define GREEN_COLOR   "\033[32m"
#define YELLOW_COLOR  "\033[33m"
#define BLUE_COLOR    "\033[34m"
#define CYAN_COLOR    "\033[36m"
#define MAGENTA_COLOR "\033[35m"

// Windows
#ifdef _WIN32
inline void setConsoleColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#endif

/**
 * @brief Prints a highlighted pointer in cyan
 * @param type -> type of the pointer
 */
inline void printHighlightedPointer(const std::string &type)
{
#ifdef _WIN32
    setConsoleColor(11);
    llvm::outs() << type;
    setConsoleColor(7);
#else
    llvm::outs() << CYAN_COLOR << type << RESET_COLOR;
#endif
}

/**
 * @brief Prints a highlighted reference in green
 * @param type -> type of the reference
 */
inline void printHighlightedReference(const std::string &type)
{
#ifdef _WIN32
    setConsoleColor(10);
    llvm::outs() << type;
    setConsoleColor(7);
#else
    llvm::outs() << GREEN_COLOR << type << RESET_COLOR;
#endif
}

/**
 * @brief Prints a highlighted keyword in blue
 * @param keyword -> keyword to highlight
 */
inline void printHighlightedKeyword(const std::string &keyword)
{
#ifdef _WIN32
    setConsoleColor(9);
    llvm::outs() << keyword;
    setConsoleColor(7);
#else
    llvm::outs() << BLUE_COLOR << keyword << RESET_COLOR;
#endif
}

/**
 * @brief Prints an error message in red
 * @param message -> error message
 */
inline void printError(const std::string &message)
{
#ifdef _WIN32
    setConsoleColor(12);
    llvm::outs() << message << "\n";
    setConsoleColor(7);
#else
    llvm::errs() << RED_COLOR << message << RESET_COLOR << "\n";
#endif
}

/**
 * @brief Prints a warning message in yellow
 * @param message -> warning message
 */
inline void printWarning(const std::string &message)
{
#ifdef _WIN32
    setConsoleColor(14);
    llvm::outs() << message << "\n";
    setConsoleColor(7);
#else
    llvm::outs() << YELLOW_COLOR << message << RESET_COLOR << "\n";
#endif
}

/**
 * @brief Prints an information message in magenta
 * @param message -> information message
 */
inline void printInfo(const std::string &message)
{
#ifdef _WIN32
    setConsoleColor(13);
    llvm::outs() << message << "\n";
    setConsoleColor(7);
#else
    llvm::outs() << MAGENTA_COLOR << message << RESET_COLOR << "\n";
#endif
}

#endif // SYNTAXHIGHLIGHTER_H
