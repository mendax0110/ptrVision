#include "PointerReferenceCollector.h"
#include <clang/AST/Expr.h>
#include <clang/Frontend/CompilerInstance.h>
#include <sstream>

using namespace clang;

PointerReferenceCollector::PointerReferenceCollector(SourceManager &SM) : SM(SM)
{
}

void PointerReferenceCollector::addIssue(ptrvision::IssueType type, SourceLocation loc, const std::string& description)
{
    FullSourceLoc fullLoc(loc, SM);
    if (fullLoc.isValid())
    {
        unsigned int line = fullLoc.getSpellingLineNumber();
        unsigned int column = fullLoc.getSpellingColumnNumber();
        issues.push_back(ptrvision::CodeIssue(type, line, column, description));
    }
}

bool PointerReferenceCollector::VisitBinaryOperator(const BinaryOperator* BO)
{
    // Check for pointer arithmetic operations
    if ((BO->getOpcode() == BO_Add || BO->getOpcode() == BO_Sub) &&
        (BO->getLHS()->getType()->isPointerType() || BO->getRHS()->getType()->isPointerType()))
    {
        QualType lhsType = BO->getLHS()->getType();
        QualType rhsType = BO->getRHS()->getType();

        std::ostringstream desc;
        desc << lhsType.getAsString() 
             << " " << (BO->getOpcode() == BO_Add ? "+" : "-") << " "
             << rhsType.getAsString();
        
        if (lhsType->isVoidPointerType() || rhsType->isVoidPointerType())
        {
            desc << " (Pointer arithmetic on void* is not allowed!)";
        }
        
        addIssue(ptrvision::IssueType::PointerArithmetic, BO->getBeginLoc(), desc.str());
    }
    // Check for compound assignment with pointers
    else if (BO->isCompoundAssignmentOp() && 
             (BO->getOpcode() == BO_AddAssign || BO->getOpcode() == BO_SubAssign) &&
             BO->getLHS()->getType()->isPointerType())
    {
        std::ostringstream desc;
        desc << BO->getLHS()->getType().getAsString() 
             << " " << BO->getOpcodeStr().str() << " "
             << BO->getRHS()->getType().getAsString();
        addIssue(ptrvision::IssueType::PointerArithmetic, BO->getBeginLoc(), desc.str());
    }
    // Check for bitwise AND
    else if (BO->getOpcode() == BO_And)
    {
        std::ostringstream desc;
        desc << "Bitwise AND operation: " << BO->getLHS()->getType().getAsString() 
             << " & " << BO->getRHS()->getType().getAsString();
        addIssue(ptrvision::IssueType::BitwiseAnd, BO->getBeginLoc(), desc.str());
    }

    return true;
}

bool PointerReferenceCollector::VisitUnaryOperator(const UnaryOperator* UO)
{
    if (UO->getOpcode() == UO_Deref)
    {
        std::ostringstream desc;
        desc << "Pointer dereference: *" << UO->getSubExpr()->getType().getAsString();
        addIssue(ptrvision::IssueType::PointerDereference, UO->getBeginLoc(), desc.str());
    }

    if (UO->getOpcode() == UO_AddrOf)
    {
        if (!UO->getType()->isFunctionPointerType())
        {
            std::ostringstream desc;
            desc << "Address-of operator: &" << UO->getSubExpr()->getType().getAsString();
            addIssue(ptrvision::IssueType::Reference, UO->getBeginLoc(), desc.str());
        }
    }

    if ((UO->getOpcode() == UO_PreInc || UO->getOpcode() == UO_PreDec ||
         UO->getOpcode() == UO_PostInc || UO->getOpcode() == UO_PostDec) &&
        UO->getSubExpr()->getType()->isPointerType())
    {
        std::ostringstream desc;
        desc << "Pointer increment/decrement on " << UO->getSubExpr()->getType().getAsString();
        addIssue(ptrvision::IssueType::PointerArithmetic, UO->getBeginLoc(), desc.str());
    }

    return true;
}

bool PointerReferenceCollector::VisitDeclRefExpr(const DeclRefExpr* DRE)
{
    if (DRE->getType()->isReferenceType())
    {
        std::ostringstream desc;
        desc << "Reference usage: " << DRE->getType().getAsString();
        addIssue(ptrvision::IssueType::Reference, DRE->getBeginLoc(), desc.str());
    }
    return true;
}

bool PointerReferenceCollector::VisitMemberExpr(const MemberExpr* ME)
{
    if (ME->isArrow())
    {
        std::ostringstream desc;
        desc << "Pointer member access: " << ME->getBase()->getType().getAsString() 
             << "->" << ME->getMemberDecl()->getNameAsString();
        addIssue(ptrvision::IssueType::PointerDereference, ME->getBeginLoc(), desc.str());
    }
    return true;
}

bool PointerReferenceCollector::VisitCXXNewExpr(const CXXNewExpr* NE)
{
    std::ostringstream desc;
    desc << "Memory allocation (new): " << NE->getAllocatedType().getAsString();
    if (NE->isArray())
    {
        desc << "[]";
    }
    addIssue(ptrvision::IssueType::NewOperator, NE->getBeginLoc(), desc.str());
    return true;
}

bool PointerReferenceCollector::VisitCXXDeleteExpr(const CXXDeleteExpr* DE)
{
    std::ostringstream desc;
    desc << "Memory deallocation (delete";
    if (DE->isArrayForm())
    {
        desc << "[]";
    }
    desc << ")";
    addIssue(ptrvision::IssueType::DeleteOperator, DE->getBeginLoc(), desc.str());
    return true;
}

bool PointerReferenceCollector::VisitGotoStmt(const GotoStmt* GS)
{
    std::ostringstream desc;
    desc << "Goto statement to label: " << GS->getLabel()->getName().str();
    addIssue(ptrvision::IssueType::GotoStatement, GS->getBeginLoc(), desc.str());
    return true;
}

bool PointerReferenceCollector::VisitVarDecl(const VarDecl* VD)
{
    if (VD->getType()->isPointerType())
    {
        std::ostringstream desc;
        desc << "Pointer declaration: " << VD->getType().getAsString() 
             << " " << VD->getNameAsString();
        addIssue(ptrvision::IssueType::PointerDeclaration, VD->getBeginLoc(), desc.str());
    }

    if (VD->getType()->isReferenceType())
    {
        std::ostringstream desc;
        desc << "Reference declaration: " << VD->getType().getAsString() 
             << " " << VD->getNameAsString();
        addIssue(ptrvision::IssueType::Reference, VD->getBeginLoc(), desc.str());
    }

    return true;
}

bool PointerReferenceCollector::VisitArraySubscriptExpr(const ArraySubscriptExpr* ASE)
{
    if (ASE->getBase()->getType()->isPointerType())
    {
        std::ostringstream desc;
        desc << "Array subscript with pointer: " << ASE->getBase()->getType().getAsString();
        addIssue(ptrvision::IssueType::ArraySubscript, ASE->getBeginLoc(), desc.str());
    }
    return true;
}
