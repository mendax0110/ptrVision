#ifndef CLIMANAGER_H
#define CLIMANAGER_H

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringRef.h>

#ifdef _WIN32
#include <windows.h>
#endif

/// @brief Enum class for different output levels \enum OutputLevel
enum class OutputLevel
{
    PLAIN,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    STATUS
};

/// @brief Class for managing command line interface output and help messages \class CliManager
class CliManager
{
public:

    /**
     * @brief General method to print help information for the command line interface.
     */
    static void showHelp()
    {
        llvm::errs() << "Usage: ptrVision <file|directory> [marker] [options]\n";
        llvm::errs() << "Options:\n";
        llvm::errs() << "  -help\t\tShow this help message\n";
    }

    /**
     * @brief Prints a message to the console with the error output level.
     * @param errorMessage -> The error message to print, a llvm::Error object.
     */
    static void printError(llvm::Error errorMessage)
    {
        print(OutputLevel::ERROR, llvm::toString(std::move(errorMessage)));
    }

    /**
     * @brief Prints a message to the console with the error output level.
     * @param errorMessage -> The error message to print, a std::string object.
     */
    static void printError(const std::string& errorMessage)
    {
        print(OutputLevel::ERROR, errorMessage);
    }

    /**
     * @brief The print method, templated to take every type of argument.
     * @tparam Ts -> Variadic template parameter for multiple argument types
     * @param level -> The output level for the message, of type OutputLevel
     * @param args -> The arguments to print, can be of any type
     */
    template<typename... Ts>
    static void print(OutputLevel level, Ts&&... args)
    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::ostringstream oss;
        (oss << ... << std::forward<Ts>(args));
        outputFormatted(level, oss.str(), true);
    }

private:
    static inline std::mutex consoleMutex;

    /**
     * @brief Gets the current time formatted as a string.
     * @return -> A string representing the current time in "YYYY-MM-DD HH:MM:SS" format.
     */
    static std::string getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    /**
     * @brief Sets the console color based on the output level.
     * @param level -> The output level for which to set the color.
     */
    static void setConsoleColor(OutputLevel level)
    {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        switch (level) {
            case OutputLevel::ERROR:
            case OutputLevel::CRITICAL:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); break;
            case OutputLevel::WARNING:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
            case OutputLevel::INFO:
            case OutputLevel::STATUS:
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
            case OutputLevel::DEBUG:
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
            default: break;
        }
#else
        switch (level)
        {
            case OutputLevel::ERROR:
            case OutputLevel::CRITICAL:
                llvm::outs().changeColor(llvm::raw_ostream::RED, true); break;
            case OutputLevel::WARNING:
                llvm::outs().changeColor(llvm::raw_ostream::YELLOW, true); break;
            case OutputLevel::INFO:
            case OutputLevel::STATUS:
                llvm::outs().changeColor(llvm::raw_ostream::GREEN, true); break;
            case OutputLevel::DEBUG:
                llvm::outs().changeColor(llvm::raw_ostream::BLUE, true); break;
            default: break;
        }
#endif
    }

    /**
     * @brief Resets the console color to default.
     */
    static void resetConsoleColor()
    {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
        llvm::outs().resetColor();
#endif
    }

    /**
     * @brief Outputs a formatted message to the console with the specified output level.
     * @param level -> The output level for the message.
     * @param message -> The message to output.
     * @param newline -> Whether to append a newline at the end of the message.
     */
    static void outputFormatted(OutputLevel level, const std::string& message, bool newline)
    {
        std::ostringstream oss;

        if (level != OutputLevel::PLAIN)
        {
            oss << "[" << getCurrentTime() << "] ";
            switch (level) {
                case OutputLevel::DEBUG:    oss << "[DEBUG] "; break;
                case OutputLevel::INFO:     oss << "[INFO] "; break;
                case OutputLevel::WARNING:  oss << "[WARNING] "; break;
                case OutputLevel::ERROR:    oss << "[ERROR] "; break;
                case OutputLevel::CRITICAL: oss << "[CRITICAL] "; break;
                case OutputLevel::STATUS:   oss << "[STATUS] "; break;
                default: break;
            }
        }

        oss << message;

        setConsoleColor(level);
        llvm::outs() << oss.str();
        if (newline) llvm::outs() << "\n";
        resetConsoleColor();
    }
};

#endif // CLIMANAGER_H
