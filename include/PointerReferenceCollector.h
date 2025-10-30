#ifndef POINTER_REFERENCE_COLLECTOR_H
#define POINTER_REFERENCE_COLLECTOR_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include "PtrVisionLib.h"
#include <vector>

namespace clang
{
    /**
     * @brief Visitor that collects pointer and reference issues instead of printing them
     */
    class PointerReferenceCollector : public RecursiveASTVisitor<PointerReferenceCollector>
    {
    public:
        explicit PointerReferenceCollector(SourceManager &SM);

        bool VisitBinaryOperator(const BinaryOperator* BO);
        bool VisitUnaryOperator(const UnaryOperator* UO);
        bool VisitDeclRefExpr(const DeclRefExpr* DRE);
        bool VisitMemberExpr(const MemberExpr* ME);
        bool VisitCXXNewExpr(const CXXNewExpr* NE);
        bool VisitCXXDeleteExpr(const CXXDeleteExpr* DE);
        bool VisitGotoStmt(const GotoStmt* GS);
        bool VisitVarDecl(const VarDecl* VD);
        bool VisitArraySubscriptExpr(const ArraySubscriptExpr* ASE);

        const std::vector<ptrvision::CodeIssue>& getIssues() const { return issues; }

    private:
        SourceManager &SM;
        std::vector<ptrvision::CodeIssue> issues;

        void addIssue(ptrvision::IssueType type, SourceLocation loc, const std::string& description);
    };
} // namespace clang

#endif // POINTER_REFERENCE_COLLECTOR_H
