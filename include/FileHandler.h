#ifndef PTRVISION_FILEHANDLER_H
#define PTRVISION_FILEHANDLER_H

#include <string>

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The FileHandler class handles file operations such as creating files and checking if a file exists. \class FileHandler
    class FileHandler
    {
    public:
        FileHandler();
        ~FileHandler();

        /**
         * @brief Create a file with the given name and path.
         * @param filePath -> The path where the file will be created.
         * @param fileName -> The name of the file to be created.
         * @return -> True if the file was created successfully, false otherwise.
         */
        bool CreateFile(const std::string& filePath, const std::string& fileName);

        /**
         * @brief Check if a file exists at the given path.
         * @param filePath -> The path to check for the file.
         * @return -> True if the file exists, false otherwise.
         */
        static bool FileExists(const std::string& filePath);

    private:
        std::string fileContentTemplate;
    };
}

#endif //PTRVISION_FILEHANDLER_H
