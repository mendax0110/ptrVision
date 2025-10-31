#include <gtest/gtest.h>
#include "PtrVisionLib.h"

using namespace ptrvision;

class BitwiseOperationsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(BitwiseOperationsTest, DetectsBitwiseAnd)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int y = 3;
            int z = x & y;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(PtrVisionLib::hasForbiddenConstructs(code));
    
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::BitwiseAnd)
        {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_TRUE(hasBitwiseAnd);
}

TEST_F(BitwiseOperationsTest, DetectsBitwiseAndInExpression)
{
    const std::string code = R"(
        void test() {
            int result = (5 & 3) + 2;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::BitwiseAnd)
        {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_TRUE(hasBitwiseAnd);
}

TEST_F(BitwiseOperationsTest, CleanCodeNoBitwiseAnd)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int y = 3;
            int z = x + y;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::BitwiseAnd)
        {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_FALSE(hasBitwiseAnd);
}

TEST_F(BitwiseOperationsTest, DistinguishesLogicalAndFromBitwiseAnd)
{
    const std::string code = R"(
        void test() {
            bool a = true;
            bool b = false;
            bool c = a && b;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    // Logical AND (&&) should not be detected as bitwise AND
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::BitwiseAnd)
        {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_FALSE(hasBitwiseAnd);
}
