#include "PointerReferenceAction.h"
#include "PointerReferenceConsumer.h"

using namespace clang;

std::unique_ptr<ASTConsumer> PointerReferenceAction::CreateASTConsumer(
        CompilerInstance &CI,
        llvm::StringRef file)
{
    return std::make_unique<PointerReferenceConsumer>(this);
}