#ifndef POINTER_REFERENCE_CONSUMER_H
#define POINTER_REFERENCE_CONSUMER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>


/// @brief The clang namespace \namespace clang
namespace clang
{
    /**
     * @brief PointerReferenceConsumer is an ASTConsumer that processes the AST of a translation unit \class PointerReferenceConsumer
     * 
     */
    class PointerReferenceConsumer final : public ASTConsumer
    {
    public:

        /**
         * @brief Handles the translation unit of the AST
         * 
         * @param Context -> ASTContext of the translation unit
         */
        void HandleTranslationUnit(ASTContext& Context) override;
    };
} // namespace clang

#endif // POINTER_REFERENCE_CONSUMER_H
