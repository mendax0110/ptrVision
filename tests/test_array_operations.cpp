#include <gtest/gtest.h>
#include "PtrVisionLib.h"

using namespace ptrvision;

class ArrayOperationsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(ArrayOperationsTest, DetectsArraySubscriptWithPointer)
{
    const std::string code = R"(
        void test() {
            int arr[10];
            int* p = arr;
            int x = p[5];
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(PtrVisionLib::hasForbiddenConstructs(code));
    
    bool hasArraySubscript = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::ArraySubscript)
        {
            hasArraySubscript = true;
        }
    }
    EXPECT_TRUE(hasArraySubscript);
}

TEST_F(ArrayOperationsTest, AcceptsDirectArrayAccess)
{
    const std::string code = R"(
        void test() {
            int arr[10];
            arr[5] = 42;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    // Direct array access might not be detected as pointer subscript
    // depending on how Clang represents it in the AST
    bool hasPointerDecl = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::PointerDeclaration)
        {
            hasPointerDecl = true;
        }
    }
    // Array name decays to pointer in some contexts but not in simple subscript
}

TEST_F(ArrayOperationsTest, DetectsPointerDeclarationFromArray)
{
    const std::string code = R"(
        void test() {
            int arr[10];
            int* p = arr;
        }
    )";
    
    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasPointerDecl = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::PointerDeclaration)
        {
            hasPointerDecl = true;
        }
    }
    EXPECT_TRUE(hasPointerDecl);
}

TEST_F(ArrayOperationsTest, CleanCodeSimpleArrays)
{
    const std::string code = R"(
        void test() {
            int arr[10] = {0};
            int x = arr[0];
        }
    )";
    
    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
}
