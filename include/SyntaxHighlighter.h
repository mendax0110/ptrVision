#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <llvm/Support/raw_ostream.h>
#include <string>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

// macOS and Linux
#define RESET_COLOR   "\033[0m"
#define RED_COLOR     "\033[31m"
#define GREEN_COLOR   "\033[32m"
#define YELLOW_COLOR  "\033[33m"
#define BLUE_COLOR    "\033[34m"
#define MAGENTA_COLOR "\033[35m"
#define CYAN_COLOR    "\033[36m"

// Windows
#define RED_WIN_COLOR     4
#define GREEN_WIN_COLOR   2
#define YELLOW_WIN_COLOR  14
#define BLUE_WIN_COLOR    9
#define MAGENTA_WIN_COLOR 13
#define CYAN_WIN_COLOR    11
#define WHITE_WIN_COLOR   15

// Windows
#ifdef _WIN32
inline void setConsoleColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#endif

const std::string rainbowColors[] ={
    RED_COLOR, GREEN_COLOR, YELLOW_COLOR, BLUE_COLOR, MAGENTA_COLOR, CYAN_COLOR
};

const int rainbowWinColors[] = {
    RED_WIN_COLOR, GREEN_WIN_COLOR, YELLOW_WIN_COLOR, BLUE_WIN_COLOR, MAGENTA_WIN_COLOR, CYAN_WIN_COLOR
};


/**
 * @brief Prints a highlighted pointer in cyan
 * @param type -> type of the pointer
 */
inline void printHighlightedPointer(const std::string& type)
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
inline void printHighlightedReference(const std::string& type)
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
inline void printHighlightedKeyword(const std::string& keyword)
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
inline void printError(const std::string& message)
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
inline void printWarning(const std::string& message)
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
inline void printInfo(const std::string& message)
{
#ifdef _WIN32
    setConsoleColor(13);
    llvm::outs() << message << "\n";
    setConsoleColor(7);
#else
    llvm::outs() << MAGENTA_COLOR << message << RESET_COLOR << "\n";
#endif
}

/**
 * @brief Prints the type of cast in blue
 * @param castType -> the cast type
 */
inline void printHighlightedCast(const std::string& castType)
{
#ifdef _WIN32
    setConsoleColor(9);
    llvm::outs() << castType;
    setConsoleColor(7);
#else
    llvm::outs() << BLUE_COLOR << castType << RESET_COLOR;
#endif
}

/**
 * @brief Prints a highlighted type (e.g., int*, MyClass*)
 * @param type -> type being highlighted
 */
inline void printHighlightedType(const std::string& type)
{
#ifdef _WIN32
    setConsoleColor(36);  // Cyan for types
    llvm::outs() << type;
    setConsoleColor(7);
#else
    llvm::outs() << CYAN_COLOR << type << RESET_COLOR;
#endif
}

/**
 * @brief Prints the canonical path of a directory with rainbow color
 * @param directoryPath -> the path of the directory to resolve
 */
inline void printCanonicalDirectoryPath(const std::string& directoryPath)
{
    try
    {
        std::filesystem::path canonicalPath = std::filesystem::canonical(directoryPath);
#ifdef _WIN32
        for (size_t i = 0; i < canonicalPath.string().size(); ++i)
        {
            setConsoleColor(rainbowWinColors[i % 6]);
            llvm::outs() << canonicalPath.string()[i];
        }
        setConsoleColor(WHITE_WIN_COLOR);
        llvm::outs() << "\n";
#else
        for (size_t i = 0; i < canonicalPath.string().size(); ++i)
        {
            llvm::outs() << rainbowColors[i % 6] << canonicalPath.string()[i] << RESET_COLOR;
        }
        llvm::outs() << "\n";
#endif
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        printError("Error resolving canonical path: " + std::string(e.what()));
    }
}

/**
 * @brief Prints the canonical path of a file with rainbow color
 * @param filePath -> the path of the file to resolve
 */
inline void printCanonicalFilePath(const std::string& filePath)
{
    try
    {
        std::filesystem::path canonicalPath = std::filesystem::canonical(filePath);
#ifdef _WIN32
        for (size_t i = 0; i < canonicalPath.string().size(); ++i)
        {
            setConsoleColor(rainbowWinColors[i % 6]);
            llvm::outs() << canonicalPath.string()[i];
        }
        setConsoleColor(WHITE_WIN_COLOR);
        llvm::outs() << "\n";
#else
        for (size_t i = 0; i < canonicalPath.string().size(); ++i)
        {
            llvm::outs() << rainbowColors[i % 6] << canonicalPath.string()[i] << RESET_COLOR;
        }
        llvm::outs() << "\n";
#endif
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        printError("Error resolving canonical path: " + std::string(e.what()));
    }
}

#endif // SYNTAXHIGHLIGHTER_H
