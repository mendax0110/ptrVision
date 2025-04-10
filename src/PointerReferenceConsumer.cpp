#include <PointerReferenceConsumer.h>
#include <PointerReferenceVisitor.h>

using namespace clang;

void PointerReferenceConsumer::HandleTranslationUnit(ASTContext& Context)
{
    PointerReferenceVisitor Visitor(Context.getSourceManager());
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
}
