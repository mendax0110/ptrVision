#include "PointerReferenceVisitor.h"
#include "SyntaxHighlighter.h"
#include <clang/AST/Expr.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;

PointerReferenceVisitor::PointerReferenceVisitor(SourceManager &SM) : SM(SM)
{
}

/*bool PointerReferenceVisitor::VisitBinaryOperator(BinaryOperator* BO)
{
    if (BO->getOpcode() == BO_Add || BO->getOpcode() == BO_Sub)
    {
        if (BO->getLHS()->getType()->isPointerType() || BO->getRHS()->getType()->isPointerType())
        {
            FullSourceLoc fullLoc(BO->getBeginLoc(), SM);
            llvm::outs() << "Pointer arithmetic detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << " -> ";
            printHighlightedPointer(BO->getLHS()->getType().getAsString() + " + " + BO->getRHS()->getType().getAsString());
            llvm::outs() << "\n";
        }
    }

    if (BO->getOpcode() == BO_Mul)
    {
        FullSourceLoc fullLoc(BO->getBeginLoc(), SM);
        llvm::outs() << "Multiplication operator detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(BO->getLHS()->getType().getAsString() + " * " + BO->getRHS()->getType().getAsString());
        llvm::outs() << "\n";
    }

    if (BO->getOpcode() == BO_And)
    {
        FullSourceLoc fullLoc(BO->getBeginLoc(), SM);
        llvm::outs() << "Bitwise AND operator detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedKeyword(BO->getLHS()->getType().getAsString() + " & " + BO->getRHS()->getType().getAsString());
        llvm::outs() << "\n";
    }

    return true;
}*/

bool PointerReferenceVisitor::VisitBinaryOperator(BinaryOperator* BO)
{
    if (BO->getOpcode() == BO_Add || BO->getOpcode() == BO_Sub)
    {
        if (BO->getLHS()->getType()->isPointerType() || BO->getRHS()->getType()->isPointerType())
        {
            FullSourceLoc fullLoc(BO->getBeginLoc(), SM);
            llvm::outs() << "Pointer arithmetic detected at "
                << fullLoc.getSpellingLineNumber() << ":"
                << fullLoc.getSpellingColumnNumber() << " -> ";

            if (BO->getLHS()->getType()->isPointerType())
                printHighlightedPointer(BO->getLHS()->getType().getAsString());
            else
                printHighlightedPointer(BO->getRHS()->getType().getAsString());

            llvm::outs() << " " << (BO->getOpcode() == BO_Add ? "+" : "-") << " ";

            if (BO->getRHS()->getType()->isPointerType())
                printHighlightedPointer(BO->getRHS()->getType().getAsString());
            else
                printHighlightedPointer(BO->getLHS()->getType().getAsString());

            llvm::outs() << "\n";
        }
    }

    if (BO->getOpcode() == BO_Mul)
    {
        FullSourceLoc fullLoc(BO->getBeginLoc(), SM);
        llvm::outs() << "Multiplication operator detected at "
            << fullLoc.getSpellingLineNumber() << ":"
            << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(BO->getLHS()->getType().getAsString());
        llvm::outs() << " * ";
        printHighlightedPointer(BO->getRHS()->getType().getAsString());
        llvm::outs() << "\n";
    }

    if (BO->getOpcode() == BO_And)
    {
        FullSourceLoc fullLoc(BO->getBeginLoc(), SM);
        llvm::outs() << "Bitwise AND operator detected at "
            << fullLoc.getSpellingLineNumber() << ":"
            << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedKeyword(BO->getLHS()->getType().getAsString());
        llvm::outs() << " & ";
        printHighlightedKeyword(BO->getRHS()->getType().getAsString());
        llvm::outs() << "\n";
    }

    return true;
}


bool PointerReferenceVisitor::VisitUnaryOperator(UnaryOperator* UO)
{
    FullSourceLoc fullLoc(UO->getBeginLoc(), SM);

    if (UO->getOpcode() == UO_Deref)
    {
        llvm::outs() << "Pointer dereference detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(UO->getType().getAsString());
        llvm::outs() << "\n";
    }
    else if (UO->getOpcode() == UO_AddrOf)
    {
        llvm::outs() << "Address-of operator detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(UO->getType().getAsString());
        llvm::outs() << "\n";
    }

    return true;
}

bool PointerReferenceVisitor::VisitDeclRefExpr(DeclRefExpr* DRE)
{
    FullSourceLoc fullLoc(DRE->getBeginLoc(), SM);

    if (DRE->getType()->isReferenceType())
    {
        llvm::outs() << "Reference detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedReference(DRE->getType().getAsString());
        llvm::outs() << "\n";
    }

    if (DRE->getType()->isPointerType())
    {
        llvm::outs() << "Pointer detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(DRE->getType().getAsString());
        llvm::outs() << "\n";
    }

    return true;
}

bool PointerReferenceVisitor::VisitMemberExpr(MemberExpr* ME)
{
    if (ME->getBase()->getType()->isPointerType())
    {
        FullSourceLoc fullLoc(ME->getBeginLoc(), SM);
        llvm::outs() << "Pointer dereference in member expression detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(ME->getBase()->getType().getAsString());
        llvm::outs() << "->" << ME->getMemberDecl()->getName() << "\n";
    }

    return true;
}

bool PointerReferenceVisitor::VisitCXXNewExpr(CXXNewExpr* NewExpr)
{
    FullSourceLoc fullLoc(NewExpr->getBeginLoc(), SM);
    llvm::outs() << "New expression detected at "
                 << fullLoc.getSpellingLineNumber() << ":"
                 << fullLoc.getSpellingColumnNumber() << " -> ";
    printHighlightedPointer(NewExpr->getAllocatedType().getAsString());
    llvm::outs() << "\n";

    return true;
}

bool PointerReferenceVisitor::VisitCXXDeleteExpr(CXXDeleteExpr* DeleteExpr)
{
    FullSourceLoc fullLoc(DeleteExpr->getBeginLoc(), SM);
    llvm::outs() << "Delete expression detected at "
                 << fullLoc.getSpellingLineNumber() << ":"
                 << fullLoc.getSpellingColumnNumber() << " -> ";
    printHighlightedPointer(DeleteExpr->getArgument()->getType().getAsString());
    llvm::outs() << "\n";

    return true;
}

bool PointerReferenceVisitor::VisitGotoStmt(GotoStmt* GS)
{
    FullSourceLoc fullLoc(GS->getBeginLoc(), SM);
    llvm::outs() << "Goto statement detected at "
                 << fullLoc.getSpellingLineNumber() << ":"
                 << fullLoc.getSpellingColumnNumber() << " -> ";
    printHighlightedKeyword(static_cast<std::string>(GS->getLabel()->getName()));
    llvm::outs() << "\n";

    return true;
}