#include "TreeNode.h"
#include <filesystem>
#include <iostream>
#include <utility>
#include <CliManager.h>

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
    CliManager::print(OutputLevel::DEBUG, "TreeNode", "Creating node: ", name, " with path: ", path);
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
        CliManager::print(OutputLevel::ERROR, "Failed to scan directory: ", absolutePath.string(), " Error: ", e.what());
    }

    return node;
}

TreeNode* TreeNode::BuildProjectTree(const std::string &rootPath)
{
    fs::path startPath(rootPath);
    fs::path absolutePath = fs::absolute(startPath);
    if (!fs::exists(absolutePath))
    {
        CliManager::print(OutputLevel::ERROR, "Path does not exist: ", absolutePath.string());
        return nullptr;
    }
    return ScanDirectory(absolutePath);
}
