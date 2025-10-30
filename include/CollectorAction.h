#ifndef COLLECTOR_ACTION_H
#define COLLECTOR_ACTION_H

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <memory>
#include <vector>

#include "CollectorConsumer.h"
#include "PtrVisionLib.h"

namespace ptrvision
{
    /**
     * @brief Action that creates the consumer
     */
    class CollectorAction : public clang::ASTFrontendAction
    {
    public:
        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance &CI,
            llvm::StringRef file) override;

        const std::vector<CodeIssue>& getIssues() const;

    private:
        CollectorConsumer* consumer = nullptr;
        std::vector<CodeIssue> emptyIssues;
    };

} // namespace ptrvision

#endif // COLLECTOR_ACTION_H
