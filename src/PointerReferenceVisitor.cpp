#include <PointerReferenceVisitor.h>
#include <SyntaxHighlighter.h>
#include <clang/AST/Expr.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;

PointerReferenceVisitor::PointerReferenceVisitor(SourceManager &SM) : SM(SM)
{
}

bool PointerReferenceVisitor::VisitBinaryOperator(const BinaryOperator* BO) const
{
    const FullSourceLoc fullLoc(BO->getBeginLoc(), SM);

    if (BO->getLHS()->getType()->isPointerType() || BO->getRHS()->getType()->isPointerType())
    {
        const QualType lhsType = BO->getLHS()->getType();
        const QualType rhsType = BO->getRHS()->getType();

        if (lhsType->isVoidPointerType() || rhsType->isVoidPointerType())
        {
            llvm::outs() << "ERROR: Illegal void* arithmetic detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << " -> ";
            printHighlightedPointer(lhsType.getAsString());
            llvm::outs() << " " << BO->getOpcodeStr() << " ";
            printHighlightedPointer(rhsType.getAsString());
            llvm::outs() << " (Pointer arithmetic on void* is not allowed!)\n";
        }
        else
        {
            llvm::outs() << "Pointer arithmetic detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << " -> ";
            printHighlightedPointer(lhsType.getAsString());
            llvm::outs() << " " << BO->getOpcodeStr() << " ";
            printHighlightedPointer(rhsType.getAsString());
            llvm::outs() << "\n";
        }
    }

    if (BO->getOpcode() == BO_Add || BO->getOpcode() == BO_Sub)
    {
        if (BO->getLHS()->getType()->isPointerType() || BO->getRHS()->getType()->isPointerType())
        {
            llvm::outs() << "Pointer arithmetic detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << " -> ";
            printHighlightedPointer(BO->getLHS()->getType().getAsString());
            llvm::outs() << " " << (BO->getOpcode() == BO_Add ? "+" : "-") << " ";
            printHighlightedPointer(BO->getRHS()->getType().getAsString());
            llvm::outs() << "\n";
        }
    }

    if (BO->isCompoundAssignmentOp() && (BO->getOpcode() == BO_AddAssign || BO->getOpcode() == BO_SubAssign))
    {
        if (BO->getLHS()->getType()->isPointerType())
        {
            llvm::outs() << "Pointer compound assignment detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << " -> ";
            printHighlightedPointer(BO->getLHS()->getType().getAsString());
            llvm::outs() << " " << BO->getOpcodeStr() << " ";
            printHighlightedPointer(BO->getRHS()->getType().getAsString());
            llvm::outs() << " (Possible pointer arithmetic issue)\n";
        }
    }

    if (BO->getOpcode() == BO_Mul)
    {
        const bool lhsPointer = BO->getLHS()->getType()->isPointerType();
        const bool rhsPointer = BO->getRHS()->getType()->isPointerType();
        const bool lhsInteger = BO->getLHS()->getType()->isIntegerType();
        const bool rhsInteger = BO->getRHS()->getType()->isIntegerType();

        if (lhsInteger && rhsInteger)
        {
            llvm::outs() << "Multiplication detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << "\n";
        }
        else if (lhsPointer || rhsPointer)
        {
            if (!lhsInteger && !rhsInteger)
            {
                llvm::outs() << "ERROR: Suspicious pointer multiplication detected at "
                             << fullLoc.getSpellingLineNumber() << ":"
                             << fullLoc.getSpellingColumnNumber() << " -> ";
                printHighlightedPointer(lhsPointer ? BO->getLHS()->getType().getAsString()
                                                   : BO->getRHS()->getType().getAsString());
                llvm::outs() << " * ";
                printHighlightedPointer(rhsPointer ? BO->getRHS()->getType().getAsString()
                                                   : BO->getLHS()->getType().getAsString());
                llvm::outs() << " (Invalid operation!)\n";
            }
            else
            {
                llvm::outs() << "WARNING: Potential pointer scaling detected at "
                             << fullLoc.getSpellingLineNumber() << ":"
                             << fullLoc.getSpellingColumnNumber() << " -> ";
                printHighlightedPointer(lhsPointer ? BO->getLHS()->getType().getAsString()
                                                   : BO->getRHS()->getType().getAsString());
                llvm::outs() << " * integer\n";
            }
        }
    }

    if (BO->getOpcode() == BO_And)
    {
        if (BO->getLHS()->getType()->isIntegerType() && BO->getRHS()->getType()->isIntegerType())
        {
            llvm::outs() << "Bitwise AND detected at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << "\n";
        }
        else
        {
            llvm::outs() << "Suspicious AND operation? Possible misuse at "
                         << fullLoc.getSpellingLineNumber() << ":"
                         << fullLoc.getSpellingColumnNumber() << "\n";
        }
    }

    return true;
}


bool PointerReferenceVisitor::VisitUnaryOperator(const UnaryOperator* UO) const
{
    const FullSourceLoc fullLoc(UO->getBeginLoc(), SM);

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

bool PointerReferenceVisitor::VisitDeclRefExpr(const DeclRefExpr* DRE) const
{
    const FullSourceLoc fullLoc(DRE->getBeginLoc(), SM);

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

bool PointerReferenceVisitor::VisitVarDecl(const VarDecl *VD) const
{
    const FullSourceLoc fullLoc(VD->getBeginLoc(), SM);

    if (VD->getType()->isReferenceType())
    {
        llvm::outs() << "Reference variable declared at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " ->";
        printHighlightedReference(VD->getType().getAsString());
        llvm::outs() << " " << VD->getNameAsString() << "\n";
    }
    return true;
}

bool PointerReferenceVisitor::VisitMemberExpr(const MemberExpr* ME) const
{
    if (ME->getBase()->getType()->isPointerType())
    {
        const FullSourceLoc fullLoc(ME->getBeginLoc(), SM);
        llvm::outs() << "Pointer dereference in member expression detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";
        printHighlightedPointer(ME->getBase()->getType().getAsString());
        llvm::outs() << "->" << ME->getMemberDecl()->getName() << "\n";
    }
    return true;
}

bool PointerReferenceVisitor::VisitArraySubscriptExpr(const ArraySubscriptExpr* ASE) const
{
    const FullSourceLoc fullLoc(ASE->getBeginLoc(), SM);
    const QualType baseType = ASE->getBase()->getType().getCanonicalType();

    if (baseType->isPointerType() || baseType->isArrayType())
    {
        llvm::outs() << "Pointer array indexing detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << " -> ";

        QualType elementType;
        if (baseType->isArrayType())
        {
            elementType = baseType->getAsArrayTypeUnsafe()->getElementType();
        }
        else if (baseType->isPointerType())
        {
            elementType = baseType->getPointeeType();
        }

        printHighlightedPointer(elementType.getAsString() + "[index]");
        llvm::outs() << "\n";
    }
    return true;
}

bool PointerReferenceVisitor::VisitCXXNewExpr(const CXXNewExpr* NewExpr) const
{
    const FullSourceLoc fullLoc(NewExpr->getBeginLoc(), SM);
    llvm::outs() << "New expression detected at "
                 << fullLoc.getSpellingLineNumber() << ":"
                 << fullLoc.getSpellingColumnNumber() << " -> ";
    printHighlightedPointer(NewExpr->getAllocatedType().getAsString());
    llvm::outs() << "\n";

    return true;
}

bool PointerReferenceVisitor::VisitCXXDeleteExpr(const CXXDeleteExpr* DeleteExpr) const
{
    const FullSourceLoc fullLoc(DeleteExpr->getBeginLoc(), SM);
    llvm::outs() << "Delete expression detected at "
                 << fullLoc.getSpellingLineNumber() << ":"
                 << fullLoc.getSpellingColumnNumber() << " -> ";
    printHighlightedPointer(DeleteExpr->getArgument()->getType().getAsString());
    llvm::outs() << "\n";

    return true;
}

bool PointerReferenceVisitor::VisitGotoStmt(const GotoStmt* GS) const
{
    const FullSourceLoc fullLoc(GS->getBeginLoc(), SM);
    llvm::outs() << "Goto statement detected at "
                 << fullLoc.getSpellingLineNumber() << ":"
                 << fullLoc.getSpellingColumnNumber() << " -> ";
    printHighlightedKeyword(static_cast<std::string>(GS->getLabel()->getName()));
    llvm::outs() << "\n";

    return true;
}

/*bool PointerReferenceVisitor::VisitCastExpr(const CastExpr* CastExpr) const
{
    const FullSourceLoc fullLoc(CastExpr->getBeginLoc(), SM);

    if (CastExpr->getType()->isPointerType())
    {
        llvm::outs() << "Pointer cast detected at "
                     << fullLoc.getSpellingLineNumber() << ":"
                     << fullLoc.getSpellingColumnNumber() << "\n";

        std::string castType;

        if (llvm::isa<CStyleCastExpr>(CastExpr))
        {
            castType = "(C-Style cast)";
        }
        else if (llvm::isa<CXXStaticCastExpr>(CastExpr))
        {
            castType = "static_cast";
        }
        else if (llvm::isa<CXXReinterpretCastExpr>(CastExpr))
        {
            castType = "reinterpret_cast";
        }
        else
        {
            castType = "unknown_cast";
        }

        printHighlightedCast(castType);
        llvm::outs() << " ";
        printHighlightedType(CastExpr->getType().getAsString());
        llvm::outs() << "\n";
    }

    return true;
}

bool PointerReferenceVisitor::VisitCStyleCastExpr(const CStyleCastExpr* CastExpr) const
{
    return VisitCastExpr(CastExpr);
}

bool PointerReferenceVisitor::VisitCXXStaticCastExpr(const CXXStaticCastExpr* CastExpr) const
{
    return VisitCastExpr(CastExpr);
}

bool PointerReferenceVisitor::VisitCXXReinterpretCastExpr(const CXXReinterpretCastExpr *ReprCastExpr) const
{
    return VisitCastExpr(ReprCastExpr);
}*/

