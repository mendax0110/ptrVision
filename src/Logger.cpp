#include <Logger.h>

using namespace loggerInternals;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger& loggerInternals::logger = Logger::getInstance();

Logger::Logger()
{
#ifdef _WIN32
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    defaultAttributes = consoleInfo.wAttributes;
#endif
}

void Logger::setLogLevel(loggerInternals::LogLevel level)
{
    std::lock_guard<std::mutex> lock(logMutex);
    currentLogLevel = level;
}

void Logger::setOutputStream(llvm::raw_ostream &os)
{
    std::lock_guard<std::mutex> lock(logMutex);
    outputStream = &os;
}

void Logger::log(loggerInternals::LogLevel level, const std::string &message)
{
    if (level < currentLogLevel) return;

    std::lock_guard<std::mutex> lock(logMutex);

    switch (level)
    {
        case LogLevel::DEBUG:
            setColor(Color::CYAN);
            break;
        case LogLevel::INFO:
            setColor(Color::MAGENTA);
            break;
        case LogLevel::WARNING:
            setColor(Color::YELLOW);
            break;
        case LogLevel::ERROR:
            setColor(Color::RED);
            break;
        case LogLevel::CRITICAL:
            setColor(Color::RED);
            break;
        case LogLevel::NONE:
            setColor(Color::RESET);
            break;
    }

    switch (level)
    {
        case LogLevel::DEBUG:
            *outputStream << "[DEBUG] ";
            break;
        case LogLevel::INFO:
            *outputStream << "[INFO] ";
            break;
        case LogLevel::WARNING:
            *outputStream << "[WARNING] ";
            break;
        case LogLevel::ERROR:
            *outputStream << "[ERROR] ";
            break;
        case LogLevel::CRITICAL:
            *outputStream << "[CRITICAL] ";
            break;
        case LogLevel::NONE:
            *outputStream << "[NONE] ";
            break;
    }

    *outputStream << message << "\n";
    resetColor();
}

void Logger::debug(const std::string &message)
{
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string &message)
{
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string &message)
{
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string &message)
{
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string &message)
{
    log(LogLevel::CRITICAL, message);
}

void Logger::setColor(Color color)
{
#ifdef _WIN32
    WORD attributes = defaultAttributes;
    switch (color)
    {
        case Color::RED:
            attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case Color::GREEN:
            attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Color::YELLOW:
            attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Color::BLUE:
            attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case Color::MAGENTA:
            attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case Color::CYAN:
            attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case Color::WHITE:
            attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case Color::RESET:
            attributes = defaultAttributes;
            break;
    }
    SetConsoleTextAttribute(hConsole, attributes);
#else
    switch (color)
    {
        case Color::RED:
            *outputStream << "\033[31m";
            break;
        case Color::GREEN:
            *outputStream << "\033[32m";
            break;
        case Color::YELLOW:
            *outputStream << "\033[33m";
            break;
        case Color::BLUE:
            *outputStream << "\033[34m";
            break;
        case Color::MAGENTA:
            *outputStream << "\033[35m";
            break;
        case Color::CYAN:
            *outputStream << "\033[36m";
            break;
        case Color::WHITE:
            *outputStream << "\033[37m";
            break;
        case Color::RESET:
            *outputStream << "\033[0m";
            break;
    }
#endif
}

void Logger::resetColor()
{
    setColor(Color::RESET);
}

void Logger::printHighlightedPointer(const std::string &type)
{
    std::lock_guard<std::mutex> lock(logMutex);
    setColor(Color::CYAN);
    *outputStream << type;
    resetColor();
}

void Logger::printHighlightedReference(const std::string &type)
{
    std::lock_guard<std::mutex> lock(logMutex);
    setColor(Color::GREEN);
    *outputStream << type;
    resetColor();
}

void Logger::printHighlightedKeyword(const std::string &keyword)
{
    std::lock_guard<std::mutex> lock(logMutex);
    setColor(Color::BLUE);
    *outputStream << keyword;
    resetColor();
}

void Logger::printHighlightedType(const std::string &type)
{
    std::lock_guard<std::mutex> lock(logMutex);
    setColor(Color::CYAN);
    *outputStream << type;
    resetColor();
}

void Logger::printHighlightedCast(const std::string &castType)
{
    std::lock_guard<std::mutex> lock(logMutex);
    setColor(Color::BLUE);
    *outputStream << castType;
    resetColor();
}

void Logger::printCanonicalPath(const std::filesystem::path &path)
{
    std::lock_guard<std::mutex> lock(logMutex);
    try
    {
        std::string pathStr = std::filesystem::canonical(path).string();
        const Color rainbowColors[] = {
            Color::RED, Color::GREEN, Color::YELLOW,
            Color::BLUE, Color::MAGENTA, Color::CYAN
        };

        for (size_t i = 0; i < pathStr.size(); ++i)
        {
            setColor(rainbowColors[i % (sizeof(rainbowColors) / sizeof(Color))]);
            *outputStream << pathStr[i];
        }

        *outputStream << "\n";
        resetColor();
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        error("Resolving canonical path: " + std::string(e.what()));
    }
}