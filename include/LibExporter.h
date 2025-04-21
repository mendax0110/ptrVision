#ifndef LIBEXPORTER_H
#define LIBEXPORTER_H

#include <string>
#include <FileSystemManager.h>

#ifdef _WIN32
#define NOMINMAX   
#define EXPORT_API __declspec(dllexport)
#define EXPORT_C extern "C" __declspec(dllexport)
#else
#define EXPORT_API __attribute__((visibility("default")))
#define EXPORT_C extern "C" __attribute__((visibility("default")))
#endif

/// @brief LibExporter is a class that provides methods to process files and directories. \class LibExporter
class EXPORT_API LibExporter
{
public:
    LibExporter();

    /**
     * @brief Processes a single file.
     * @param filePath The path to the file to process.
     * @param marker The marker to search for in the file.
     * 
     */
    void ProcessFile(const std::string& filePath, const std::string& marker);

    /**
     * @brief Processes all .cpp files in a specified directory.
     * @param dirPath The path to the directory to process.
     * @param marker The marker to search for in the files.
     */
    void ProcessDirectory(const std::string& dirPath, const std::string& marker);


    /**
     * @brief Processes a snippet from a specified file.
     * @param filePath The path to the file to process.
     * @param marker The marker to search for in the file.
     */
    void ProcessSnippetFromSpecifiedFile(const std::string& filePath, const std::string& marker);
};

/// C-compatible exported API
EXPORT_C void processFile(const char* filePath, const char* marker);
EXPORT_C void processDirectory(const char* dirPath, const char* marker);
EXPORT_C void processSnippetFromSpecifiedFile(const char* filePath, const char* marker);

#endif // LIBEXPORTER_H
