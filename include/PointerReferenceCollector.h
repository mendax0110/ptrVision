#ifndef POINTER_REFERENCE_COLLECTOR_H
#define POINTER_REFERENCE_COLLECTOR_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include "PtrVisionLib.h"
#include <vector>

/// @brief ptrvision namespace \namespace ptrvision
namespace clang
{
    /// @brief Pointer Reference Collector class \class PointerReferenceCollector
    class PointerReferenceCollector : public RecursiveASTVisitor<PointerReferenceCollector>
    {
    public:
        /**
         * @brief Explicit Constructor
         * @param SM The source manager
         */
        explicit PointerReferenceCollector(SourceManager &SM);

        /**
         * @brief Visit Binary Operator
         * @param BO The binary operator
         * @return A true to continue traversal
         */
        bool VisitBinaryOperator(const BinaryOperator* BO);

        /**
         * @brief Visit Unary Operator
         * @param UO The unary operator
         * @return A true to continue traversal
         */
        bool VisitUnaryOperator(const UnaryOperator* UO);

        /**
         * @brief Visit Decl Ref Expr
         * @param DRE The declaration reference expression
         * @return A true to continue traversal
         */
        bool VisitDeclRefExpr(const DeclRefExpr* DRE);

        /**
         * @brief Visit Member Expr
         * @param ME The member expression
         * @return A true to continue traversal
         */
        bool VisitMemberExpr(const MemberExpr* ME);

        /**
         * @brief Visit CXX New Expr
         * @param NE The C++ new expression
         * @return A true to continue traversal
         */
        bool VisitCXXNewExpr(const CXXNewExpr* NE);

        /**
         * @brief Visit CXX Delete Expr
         * @param DE The C++ delete expression
         * @return A true to continue traversal
         */
        bool VisitCXXDeleteExpr(const CXXDeleteExpr* DE);

        /**
         * @brief Visit Goto Stmt
         * @param GS The goto statement
         * @return A true to continue traversal
         */
        bool VisitGotoStmt(const GotoStmt* GS);

        /**
         * @brief Visit Var Decl
         * @param VD The variable declaration
         * @return A true to continue traversal
         */
        bool VisitVarDecl(const VarDecl* VD);

        /**
         * @brief Visit Array Subscript Expr
         * @param ASE The array subscript expression
         * @return A true to continue traversal
         */
        bool VisitArraySubscriptExpr(const ArraySubscriptExpr* ASE);

        /**
         * @brief Get collected issues
         * @return A reference to the vector of CodeIssue
         */
        [[nodiscard]] const std::vector<ptrvision::CodeIssue>& getIssues() const { return issues; }

    private:
        SourceManager &SM;
        std::vector<ptrvision::CodeIssue> issues;

        /**
         * @brief Add an issue to the collection
         * @param type The issue type
         * @param loc The source location
         * @param description The issue description
         */
        void addIssue(ptrvision::IssueType type, SourceLocation loc, const std::string& description);
    };
} // namespace clang

#endif // POINTER_REFERENCE_COLLECTOR_H
