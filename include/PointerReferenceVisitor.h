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
         * @return true if the node is a pointer arithmetic operation
         * @return false if the node is not a pointer arithmetic operation
         */
        bool VisitBinaryOperator(BinaryOperator* BO);

        /**
         * @brief Visit a UnaryOperator node, and check if it is a pointer dereference operation
         * 
         * @param UO -> UnaryOperator node
         * @return true 
         * @return false 
         */
        bool VisitUnaryOperator(UnaryOperator* UO);

        /**
         * @brief Visit a DeclRefExpr node, and check if it is a pointer reference operation
         * 
         * @param DRE -> DeclRefExpr node
         * @return true 
         * @return false
         */
        bool VisitDeclRefExpr(DeclRefExpr* DRE);

        /**
         * @brief Visit a MemberExpr node, and check if it is a pointer reference operation
         * 
         * @param ME -> MemberExpr node
         * @return true
         * @return false
         */
        bool VisitMemberExpr(MemberExpr* ME);

        /**
         * @brief Visit a CXXNewExpr node, and check if it is a pointer reference operation
         * @param NE -> CXXNewExpr node
         * @return
         */
        bool VisitCXXNewExpr(CXXNewExpr* NE);

        /**
         * @brief Visit a CXXDeleteExpr node, and check if it is a pointer reference operation
         * @param DE  -> CXXDeleteExpr node
         * @return
         */
        bool VisitCXXDeleteExpr(CXXDeleteExpr* DE);

        /**
         * @brief Visit a GotoStmt node, and check if it is a pointer reference operation
         * @param GS -> GotoStmt node
         * @return
         */
        bool VisitGotoStmt(GotoStmt* GS);

    private:
        SourceManager &SM;
    };
} // namespace clang

#endif // POINTER_REFERENCE_VISITOR_H
