#ifndef PTRVISION_TREENODE_H
#define PTRVISION_TREENODE_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The NodeType enum class represents the type of a node in the file system tree. \enum NodeType
    enum class NodeType
    {
        File,
        Directory
    };

    /// @brief The TreeNode class represents a node in the file system tree. \class TreeNode
    class TreeNode
    {
    public:
        TreeNode(std::string name, const std::string& basePath, NodeType type);
        ~TreeNode();

        /**
         * @brief Get the name of the node.
         * @return -> The name of the node.
         */
        [[nodiscard]] const std::string& GetName() const { return name; }

        /**
         * @brief Get the path of the node.
         * @return -> The path of the node.
         */
        [[nodiscard]] const std::string& GetPath() const { return path; }

        /**
         * @brief Get the type of the node.
         * @return -> The type of the node.
         */
        [[nodiscard]] NodeType GetType() const { return type; }

        /**
         * @brief Add a child node to the current node.
         * @param child -> The child node to add.
         */
        void AddChild(TreeNode* child);

        /**
         * @brief Get the children of the node.
         * @return -> A vector of pointers to the child nodes.
         */
        [[nodiscard]] const std::vector<TreeNode*>& GetChildren() const { return children; }

        /**
         * @brief Scan a directory and create a TreeNode for each file and subdirectory.
         * @param dirPath -> The path of the directory to scan.
         * @return -> A pointer to the root TreeNode of the scanned directory.
         */
        TreeNode* ScanDirectory(const fs::path& dirPath);

        /**
         * @brief Build a project tree from the given root path.
         * @param rootPath -> The root path of the project.
         * @return -> A pointer to the root TreeNode of the project tree.
         */
        TreeNode* BuildProjectTree(const std::string& rootPath);

    private:
        std::string path;
        std::string name;
        NodeType type;
        std::vector<TreeNode*> children;
    };
}

#endif //PTRVISION_TREENODE_H
