#ifndef PTRVISION_LOGGER_H
#define PTRVISION_LOGGER_H

#include <llvm/Support/raw_os_ostream.h>
#include <string>
#include <filesystem>
#include <memory>
#include <mutex>

#if _WIN32
#include <windows.h>
#endif

/// @brief The clang namespace \namespace clang
namespace loggerInternals
{
    /// @brief Enum class LogLevel represents the different log levels. \enum LogLevel
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL,
        NONE
    };

    /// @brief Color enum class handles the color codes for different log levels. \enum Color
    enum class Color
    {
        RESET,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE
    };

    class Logger
    {
    public:

        /**
         * @Getter for the actual instance of the logger
         * @return -> Logger& -> reference to the logger instance
         */
        static Logger& getInstance();

        /**
         *
         * @param level
         */
        void setLogLevel(LogLevel level);

        /**
         *
         * @param os
         */
        void setOutputStream(llvm::raw_ostream& os);

        /**
         *
         * @param message
         */
        void debug(const std::string& message);

        /**
         *
         * @param message
         */
        void info(const std::string& message);

        /**
         *
         * @param message
         */
        void warning(const std::string& message);

        /**
         *
         * @param message
         */
        void error(const std::string& message);

        /**
         *
         * @param message
         */
        void critical(const std::string& message);

        /**
         *
         * @param type
         */
        void printHighlightedPointer(const std::string& type);

        /**
         *
         * @param type
         */
        void printHighlightedReference(const std::string& type);

        /**
         *
         * @param keyword
         */
        void printHighlightedKeyword(const std::string& keyword);

        /**
         *
         * @param type
         */
        void printHighlightedType(const std::string& type);

        /**
         *
         * @param castType
         */
        void printHighlightedCast(const std::string& castType);

        /**
         *
         * @param path
         */
        void printCanonicalPath(const std::filesystem::path& path);

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

    private:
        Logger();

        void log(LogLevel level, const std::string& message);
        void setColor(Color color);
        void resetColor();

        LogLevel currentLogLevel = LogLevel::INFO;
        llvm::raw_ostream* outputStream = &llvm::outs();
        std::mutex logMutex;

#ifdef _WIN32
        HANDLE hConsole;
        WORD defaultAttributes;
#endif
    };

    extern Logger& logger;
}


#endif //PTRVISION_LOGGER_H
