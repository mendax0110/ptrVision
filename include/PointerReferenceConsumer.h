#ifndef POINTER_REFERENCE_CONSUMER_H
#define POINTER_REFERENCE_CONSUMER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <memory>
#include <string>
#include "PointerReferenceVisitor.h"

/// @brief Forward declaration of the PointerReferenceAction class
namespace clang
{
    class PointerReferenceAction;
}

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The PointerReferenceConsumer class is responsible for consuming the AST and processing pointer references. \class PointerReferenceConsumer
    class PointerReferenceConsumer final : public ASTConsumer
    {
    public:
        /// @brief Constructor that takes a PointerReferenceAction pointer
        explicit PointerReferenceConsumer(PointerReferenceAction* action);

        /**
         * @brief Handles the translation unit of the AST
         *
         * @param Context -> ASTContext of the translation unit
         */
        void HandleTranslationUnit(ASTContext& Context) override;

        /**
         * @brief Returns the output of the visitor
         *
         * @return -> A string with the output
         */
        [[nodiscard]] std::string getOutput() const
        {
            return visitor ? visitor->getOutput() : "";
        }

    private:
        PointerReferenceAction* action;
        std::unique_ptr<PointerReferenceVisitor> visitor;
    };
} // namespace clang

#endif // POINTER_REFERENCE_CONSUMER_H