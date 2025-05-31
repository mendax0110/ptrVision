//
// Created by Adrian Gössl on 19.04.25.
//
#include <FileHandler.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <CliManager.h>

using namespace clang;

FileHandler::FileHandler() : fileContentTemplate("// New file template\n\nint main() {\n    return 0;\n}\n")
{

}

FileHandler::~FileHandler() = default;

bool FileHandler::CreateFile(const std::string &filePath, const std::string &fileName)
{
    std::filesystem::path fullPath;

    if (filePath.empty())
    {
        fullPath = std::filesystem::current_path() / fileName;
    }
    else
    {
        fullPath = std::filesystem::path(filePath) / fileName;
    }

    if (FileExists(fullPath.string()))
    {
        CliManager::print(OutputLevel::ERROR, "File already exists: ", fullPath.string());
        return false;
    }

    std::ofstream newFile(fullPath);
    if (newFile.is_open())
    {
        newFile << fileContentTemplate;
        newFile.close();
        CliManager::print(OutputLevel::INFO, "File created successfully: ", fullPath.string());
        return true;
    }
    else
    {
        CliManager::print(OutputLevel::ERROR, "Failed to create file: ", fullPath.string());
        return false;
    }
}

bool FileHandler::FileExists(const std::string &filePath)
{
    return std::filesystem::exists(filePath);
}