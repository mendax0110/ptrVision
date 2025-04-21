#include <LibExporter.h>
#include <FileSystemManager.h>

// C-compatible interface
void processFile(const char* filePath, const char* marker)
{
    std::vector<std::string> markers = { marker };
    clang::FileSystemManager::processFile(std::string(filePath), markers);
}

void processDirectory(const char* dirPath, const char* marker)
{
    std::vector<std::string> markers = { marker };
    clang ::FileSystemManager::processDirectory(std::string(dirPath), markers);
}

void processSnippetFromSpecifiedFile(const char* filePath, const char* marker)
{
    clang ::FileSystemManager::processSnippetFromSpecifiedFile(std::string(filePath), std::string(marker));
}

// C++ class interface implementation
LibExporter::LibExporter() = default;

void LibExporter::ProcessFile(const std::string& filePath, const std::string& marker)
{
    std::vector<std::string> markers = { marker };
    clang ::FileSystemManager::processFile(filePath, markers);
}

void LibExporter::ProcessDirectory(const std::string& dirPath, const std::string& marker)
{
    std::vector<std::string> markers = { marker };
    clang ::FileSystemManager::processDirectory(dirPath, markers);
}

void LibExporter::ProcessSnippetFromSpecifiedFile(const std::string& filePath, const std::string& marker)
{
    clang ::FileSystemManager::processSnippetFromSpecifiedFile(filePath, marker);
}