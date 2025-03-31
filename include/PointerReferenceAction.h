#ifndef POINTER_REFERENCE_ACTION_H
#define POINTER_REFERENCE_ACTION_H

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include "PointerReferenceConsumer.h"

namespace clang
{
    /**
     * @brief PointerReferenceAction is an ASTFrontendAction that creates an ASTConsumer
     * 
     */
    class PointerReferenceAction final : public ASTFrontendAction
    {
    public:

        /**
         * @brief Creates the ASTConsumer for the translation unit, which processes the AST
         * 
         * @param CI -> CompilerInstance
         * @param file -> file name
         * @return std::unique_ptr<ASTConsumer> -> A unique pointer to the ASTConsumer
         */
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef file) override
        {
            return std::make_unique<PointerReferenceConsumer>();
        }
    };
} // namespace clang

#endif // POINTER_REFERENCE_ACTION_H
