#ifndef PTRVISION_PROJECTEXPLORER_H
#define PTRVISION_PROJECTEXPLORER_H

#include <TreeNode.h>
#include <string>
#include <utility>

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The ProjectExplorer class is responsible for displaying the project tree structure. \class ProjectExplorer
    class ProjectExplorer
    {
    public:
        /**
         * @brief Display the project tree structure.
         * @param node -> The root node of the tree to display.
         * @return -> A pair containing the project name and a pair of the file path and a pair of the file name and a boolean indicating if the file is a directory.
         */
        static std::pair<std::string, std::pair<std::string, std::pair<std::string, bool>>> DisplayTreeNode(TreeNode& node);
    };
}

#endif //PTRVISION_PROJECTEXPLORER_H
