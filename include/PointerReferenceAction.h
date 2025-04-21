#ifndef POINTER_REFERENCE_ACTION_H
#define POINTER_REFERENCE_ACTION_H

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <memory>
#include <string>

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief PointerRefernceConsumer class forward declaration \class PointerReferenceConsumer
    class PointerReferenceConsumer;

    /// @brief The PointerReferenceAction class is responsible for creating an AST consumer and handling the output. \class PointerReferenceAction
    class PointerReferenceAction final : public ASTFrontendAction
    {
    public:
        PointerReferenceAction() = default;

        /// @brief Movable constructor and assignment operator
        PointerReferenceAction(PointerReferenceAction&&) = delete; // was previously default
        PointerReferenceAction& operator=(PointerReferenceAction&&) = delete; // was previously default

        /// @brief Non-copyable constructor and assignment operator
        PointerReferenceAction(const PointerReferenceAction&) = delete;
        PointerReferenceAction& operator=(const PointerReferenceAction&) = delete;

        /**
         * @brief Create an AST consumer for the given CompilerInstance and file.
         * @param CI -> CompilerInstance object
         * @param file -> The name of the file being processed
         * @return -> A unique pointer to the ASTConsumer object
         */
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                       llvm::StringRef file) override;

        /**
         * @brief Forwards the stdout and stderr streams to the output file.
         * @return -> A String with the output
         */
        std::string getOutput() const { return output; }

        /**
         * @brief Appends the given string to the output.
         * @param str -> The string to append
         */
        void appendOutput(const std::string &str) { output += str; }

    private:
        std::string output;
    };
} // namespace clang

#endif // POINTER_REFERENCE_ACTION_H