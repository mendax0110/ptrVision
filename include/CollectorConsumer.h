#ifndef COLLECTOR_CONSUMER_H
#define COLLECTOR_CONSUMER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/SourceManager.h>
#include <vector>

#include "PointerReferenceCollector.h"
#include "PtrVisionLib.h"

/// @brief ptrvision namespace \namespace ptrvision
namespace ptrvision
{
    /// @brief Collector AST Consumer class \class CollectorConsumer
    class CollectorConsumer final : public clang::ASTConsumer
    {
    public:
        /**
         * @brief Constructor
         * @param SM The source manager
         */
        explicit CollectorConsumer(clang::SourceManager &SM);

        /**
         * @brief Handle Translation Unit
         * @param Context The AST context
         */
        void HandleTranslationUnit(clang::ASTContext& Context) override;

        /**
         * @brief Get collected issues
         * @return A reference to the vector of CodeIssue
         */
        const std::vector<CodeIssue>& getIssues() const;

    private:
        clang::PointerReferenceCollector collector;
    };

} // namespace ptrvision

#endif // COLLECTOR_CONSUMER_H
