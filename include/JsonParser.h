#ifndef PTRVISION_JSONPARSER_H
#define PTRVISION_JSONPARSER_H

#include <TreeNode.h>
#include <string>

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The JsonParser class is responsible for parsing JSON files and populating the TreeNode structure. \class JsonParser
    class JsonParser
    {
    public:
        /**
         * @brief Parse a JSON file and populate the TreeNode structure.
         * @param treeNode -> The TreeNode to populate with the parsed data.
         * @param basePath -> The base path for the project.
         * @param projectName -> The name of the project.
         * @param setProjectName -> Flag to set the project name.
         */
        static void Parse(TreeNode& treeNode, const std::string& basePath, const std::string& projectName, bool setProjectName = true);
    };
}

#endif //PTRVISION_JSONPARSER_H
