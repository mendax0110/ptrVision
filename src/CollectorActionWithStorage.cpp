#include "CollectorActionWithStorage.h"

namespace ptrvision
{

// Thread-local storage for issues to avoid lifetime issues
thread_local std::vector<CodeIssue> g_collectedIssues;

std::unique_ptr<clang::ASTConsumer> CollectorActionWithStorage::CreateASTConsumer(
    clang::CompilerInstance &CI,
    llvm::StringRef file)
{
    // Clear previous issues
    g_collectedIssues.clear();
    consumer = new CollectorConsumer(CI.getSourceManager());
    return std::unique_ptr<clang::ASTConsumer>(consumer);
}

void CollectorActionWithStorage::EndSourceFileAction()
{
    // Copy issues to thread-local storage before consumer is destroyed
    if (consumer)
    {
        g_collectedIssues = consumer->getIssues();
    }
}

} // namespace ptrvision
