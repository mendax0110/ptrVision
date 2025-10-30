#ifndef COLLECTOR_CONSUMER_H
#define COLLECTOR_CONSUMER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/SourceManager.h>
#include <vector>

#include "PointerReferenceCollector.h"
#include "PtrVisionLib.h"

namespace ptrvision
{
    /**
     * @brief Consumer that uses the collector to gather code issues
     */
    class CollectorConsumer : public clang::ASTConsumer
    {
    public:
        explicit CollectorConsumer(clang::SourceManager &SM);

        void HandleTranslationUnit(clang::ASTContext& Context) override;

        const std::vector<CodeIssue>& getIssues() const;

    private:
        clang::PointerReferenceCollector collector;
    };

} // namespace ptrvision

#endif // COLLECTOR_CONSUMER_H
