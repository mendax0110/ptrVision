#include "PointerReferenceConsumer.h"
#include "PointerReferenceVisitor.h"
#include "PointerReferenceAction.h"

using namespace clang;

PointerReferenceConsumer::PointerReferenceConsumer(PointerReferenceAction* action)
        : action(action), visitor(nullptr) {}

void PointerReferenceConsumer::HandleTranslationUnit(ASTContext& Context)
{
    visitor = std::make_unique<PointerReferenceVisitor>(Context.getSourceManager());
    visitor->TraverseDecl(Context.getTranslationUnitDecl());

    if (action && visitor)
    {
        std::string visitorOutput = visitor->getOutput();
        if (!visitorOutput.empty())
        {
            action->appendOutput(visitorOutput);
        }
    }
}