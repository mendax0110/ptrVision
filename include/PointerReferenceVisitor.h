#ifndef POINTER_REFERENCE_VISITOR_H
#define POINTER_REFERENCE_VISITOR_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>


/// @brief The clang namespace \namespace clang
namespace clang
{
    /**
     * @brief PointerReferenceVisitor is a RecursiveASTVisitor that visits the AST nodes of a translation unit \class PointerReferenceVisitor
     * 
     */
    class PointerReferenceVisitor : public RecursiveASTVisitor<PointerReferenceVisitor>
    {
    public:
        PointerReferenceVisitor(SourceManager &SM);

        /**
         * @brief Visit a BinaryOperator node, and check if it is a pointer arithmetic operation
         * 
         * @param BO -> BinaryOperator node
         * @return true (always returns true to continue traversal).
         */
        bool VisitBinaryOperator(const BinaryOperator* BO) const;

        /**
         * @brief Visit a UnaryOperator node, and check if it is a pointer dereference operation
         * 
         * @param UO -> UnaryOperator node
         * @return true (always returns true to continue traversal).
         */
        bool VisitUnaryOperator(const UnaryOperator* UO) const;

        /**
         * @brief Visit a DeclRefExpr node, and check if it is a pointer reference operation
         * 
         * @param DRE -> DeclRefExpr node
         * @return true (always returns true to continue traversal).
         */
        bool VisitDeclRefExpr(const DeclRefExpr* DRE) const;

        /**
         * @brief Visit a MemberExpr node, and check if it is a pointer reference operation
         * 
         * @param ME -> MemberExpr node
         * @return true (always returns true to continue traversal).
         */
        bool VisitMemberExpr(const MemberExpr* ME) const;

        /**
         * @brief Visit a CXXNewExpr node, and check if it is a pointer reference operation
         * @param NE -> CXXNewExpr node
         * @return true (always returns true to continue traversal).
         */
        bool VisitCXXNewExpr(const CXXNewExpr* NE) const;

        /**
         * @brief Visit a CXXDeleteExpr node, and check if it is a pointer reference operation
         * @param DE  -> CXXDeleteExpr node
         * @return true (always returns true to continue traversal).
         */
        bool VisitCXXDeleteExpr(const CXXDeleteExpr* DE) const;

        /**
         * @brief Visit a GotoStmt node, and check if it is a pointer reference operation
         * @param GS -> GotoStmt node
         * @return true (always returns true to continue traversal).
         */
        bool VisitGotoStmt(const GotoStmt* GS) const;

        /**
         * @brief Visit a VarDecl node to check if it is a pointer declaration.
         *
         * @param VD -> VarDecl node representing the variable declaration.
         * @return true (always returns true to continue traversal).
         */
        bool VisitVarDecl(const VarDecl* VD) const;

        /**
         * @brief Visit an ArraySubscriptExpr node to check if it involves a pointer.
         *
         * @param ASE -> ArraySubscriptExpr node representing an array subscript operation.
         * @return true (always returns true to continue traversal).
         */
        bool VisitArraySubscriptExpr(const ArraySubscriptExpr* ASE) const;

        /**
         * @brief Visit a CXXStaticCastExpr node to check for pointer casts using static_cast.
         *
         * @param CastExpr -> CXXStaticCastExpr node representing the static_cast operation.
         * @return true (always returns true to continue traversal).
         */
        //bool VisitCXXStaticCastExpr(const CXXStaticCastExpr* CastExpr) const;

        /**
         * @brief Visit a CXXReinterpretCastExpr node to check for pointer casts using reinterpret_cast.
         *
         * @param ReprCastExpr -> CXXReinterpretCastExpr node representing the reinterpret_cast operation.
         * @return true (always returns true to continue traversal).
         */
        //bool VisitCXXReinterpretCastExpr(const CXXReinterpretCastExpr* ReprCastExpr) const;

        /**
         * @brief Visit a CStyleCastExpr node to check for C-style pointer casts.
         *
         * @param CastExpr -> CStyleCastExpr node representing the C-style cast operation.
         * @return true (always returns true to continue traversal).
         */
        //bool VisitCStyleCastExpr(const CStyleCastExpr* CastExpr) const;

        /**
         * @brief Visit a CastExpr node to detect pointer-related cast operations.
         *
         * @param CastExpr -> CastExpr node representing the cast operation.
         * @return true (always returns true to continue traversal).
         */
        //bool VisitCastExpr(const CastExpr* CastExpr) const;


    private:
        SourceManager &SM;
    };
} // namespace clang

#endif // POINTER_REFERENCE_VISITOR_H
