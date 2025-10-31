#include "CollectorAction.h"

using namespace ptrvision;

std::unique_ptr<clang::ASTConsumer> CollectorAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file)
{
    consumer = new CollectorConsumer(CI.getSourceManager());
    return std::unique_ptr<clang::ASTConsumer>(consumer);
}

const std::vector<CodeIssue>& CollectorAction::getIssues() const
{
    return consumer ? consumer->getIssues() : emptyIssues;
}
