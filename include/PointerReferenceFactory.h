#ifndef POINTER_REFERENCE_FACTORY_H
#define POINTER_REFERENCE_FACTORY_H

#include "PointerReferenceAction.h"
#include <memory>

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief PointerReferenceFactory class is responsible for creating and managing PointerReferenceAction instances. \class PointerReferenceFactory
    class PointerReferenceFactory : public clang::tooling::FrontendActionFactory
    {
    public:

        /**
         * @brief Creaets a new instance of PointerReferenceAction and stores it in shared storage.
         * @return -> A unique pointer to the created PointerReferenceAction instance
         */
        std::unique_ptr<clang::FrontendAction> create() override
        {
            auto action = std::make_unique<clang::PointerReferenceAction>();
            lastAction = action.get();
            actions.push_back(std::move(action));
            return std::make_unique<ForwardingAction>(lastAction);
        }

        /**
         * @brief Returns the last created PointerReferenceAction instance.
         * @return -> A pointer to the last created PointerReferenceAction instance
         */
        clang::PointerReferenceAction* getLastAction()
        {
            return lastAction;
        }

    private:

        /// @brief ForwardingAction class is a wrapper around PointerReferenceAction to forward the AST consumer creation. \class ForwardingAction
        class ForwardingAction : public clang::ASTFrontendAction
        {
        public:
            explicit ForwardingAction(clang::PointerReferenceAction* target) : target(target) {}

            /**
             * @brief Creates an AST consumer for the given CompilerInstance and file.
             * @param CI -> CompilerInstance object
             * @param file -> The name of the file being processed
             * @return -> A unique pointer to the ASTConsumer object
             */
            std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                    clang::CompilerInstance &CI, llvm::StringRef file) override
            {
                return target->CreateASTConsumer(CI, file);
            }

        private:
            clang::PointerReferenceAction* target;
        };

        std::vector<std::unique_ptr<clang::PointerReferenceAction>> actions;
        clang::PointerReferenceAction* lastAction = nullptr;
    };
}

#endif // POINTER_REFERENCE_FACTORY_H