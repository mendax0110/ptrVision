#include "TreeNode.h"
#include <filesystem>
#include <iostream>
#include <utility>

using namespace clang;
namespace fs = std::filesystem;

TreeNode::TreeNode(std::string name, const std::string& basePath, NodeType type)
        : name(std::move(name)), type(type)
{
    path = basePath;
    if (type == NodeType::Directory && !path.empty() && path.back() != '/')
    {
        path += "/";
    }
    std::cout << "Creating node: " << name << " with path: " << path << std::endl;
}

TreeNode::~TreeNode()
{
    for (auto& child : children)
    {
        delete child;
    }
}

void TreeNode::AddChild(TreeNode* child)
{
    children.push_back(child);
}

TreeNode* TreeNode::ScanDirectory(const fs::path& dirPath)
{
    fs::path absolutePath = fs::absolute(dirPath);
    auto* node = new TreeNode(absolutePath.filename().string(),
                                  absolutePath.parent_path().string(),
                                  NodeType::Directory);

    try
    {
        for (const auto& entry : fs::directory_iterator(absolutePath))
        {
            if (fs::is_directory(entry))
            {
                TreeNode* subDirNode = ScanDirectory(entry.path());
                node->AddChild(subDirNode);
            }
            else if (fs::is_regular_file(entry))
            {
                auto* fileNode = new TreeNode(entry.path().filename().string(),
                                              entry.path().string(),
                                              NodeType::File);
                node->AddChild(fileNode);
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "[ERROR] Directory scan failed: " << e.what() << std::endl;
    }

    return node;
}

TreeNode* TreeNode::BuildProjectTree(const std::string &rootPath)
{
    fs::path startPath(rootPath);
    fs::path absolutePath = fs::absolute(startPath);
    if (!fs::exists(absolutePath))
    {
        std::cerr << "[ERROR] Path does not exist: " << absolutePath << std::endl;
        return nullptr;
    }
    return ScanDirectory(absolutePath);
}
