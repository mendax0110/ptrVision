#include "CollectorConsumer.h"
#include <clang/AST/ASTContext.h>

namespace ptrvision
{

CollectorConsumer::CollectorConsumer(clang::SourceManager &SM) : collector(SM)
{
}

void CollectorConsumer::HandleTranslationUnit(clang::ASTContext& Context)
{
    collector.TraverseDecl(Context.getTranslationUnitDecl());
}

const std::vector<CodeIssue>& CollectorConsumer::getIssues() const
{
    return collector.getIssues();
}

} // namespace ptrvision
