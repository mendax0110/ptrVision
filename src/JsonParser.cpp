//
// Created by Adrian Gössl on 19.04.25.
//
#include <JsonParser.h>
#include <iostream>
#include <filesystem>
#include <CliManager.h>

using namespace clang;

void JsonParser::Parse(TreeNode& treeNode, const std::string& basePath, const std::string& projectName, bool setProjectName)
{
    std::filesystem::path path(basePath);
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        treeNode = TreeNode(path.filename().string(), basePath, NodeType::Directory);
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            NodeType type = entry.is_directory() ? NodeType::Directory : NodeType::File;
            auto* childNode = new TreeNode(entry.path().filename().string(), entry.path().string(), type);
            treeNode.AddChild(childNode);
            if (type == NodeType::Directory)
            {
                Parse(*childNode, entry.path().string(), projectName, false);
            }
        }
    }
    else
    {
        CliManager::print(OutputLevel::ERROR, "Invalid directory path: ", basePath);
    }

    if (setProjectName)
    {
        treeNode = TreeNode(treeNode.GetName(), basePath, NodeType::Directory);
    }
}