#ifndef COLLECTOR_ACTION_H
#define COLLECTOR_ACTION_H

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <memory>
#include <vector>

#include "CollectorConsumer.h"
#include "PtrVisionLib.h"

/// @brief ptrvision namespace \namespace ptrvision
namespace ptrvision
{
    /// @brief Collector Frontend Action class \class CollectorAction
    class CollectorAction final : public clang::ASTFrontendAction
    {
    public:
        /**
         * @brief Create AST Consumer
         * @param CI The compiler instance
         * @param file The source file
         * @return A unique pointer to the ASTConsumer
         */
        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) override;

        /**
         * @brief Get collected issues
         * @return A reference to the vector of CodeIssue
         */
        [[nodiscard]] const std::vector<CodeIssue>& getIssues() const;

    private:
        CollectorConsumer* consumer = nullptr;
        std::vector<CodeIssue> emptyIssues;
    };

} // namespace ptrvision

#endif // COLLECTOR_ACTION_H
