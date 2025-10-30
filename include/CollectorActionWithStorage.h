#ifndef COLLECTOR_ACTION_WITH_STORAGE_H
#define COLLECTOR_ACTION_WITH_STORAGE_H

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <memory>
#include <vector>

#include "CollectorConsumer.h"
#include "PtrVisionLib.h"

namespace ptrvision
{
    /**
     * @brief Custom action that stores issues in thread-local storage
     * 
     * This class extends CollectorAction to work around lifetime issues
     * by storing collected issues in thread-local storage before the
     * consumer is destroyed.
     */
    class CollectorActionWithStorage : public clang::ASTFrontendAction
    {
    public:
        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance &CI,
            llvm::StringRef file) override;

        void EndSourceFileAction() override;

    private:
        CollectorConsumer* consumer = nullptr;
    };

    // Thread-local storage for issues to avoid lifetime issues
    extern thread_local std::vector<CodeIssue> g_collectedIssues;

} // namespace ptrvision

#endif // COLLECTOR_ACTION_WITH_STORAGE_H
