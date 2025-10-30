#include <gtest/gtest.h>
#include "PtrVisionLib.h"

class BitwiseOperationsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BitwiseOperationsTest, DetectsBitwiseAnd) {
    std::string code = R"(
        void test() {
            int x = 5;
            int y = 3;
            int z = x & y;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::BitwiseAnd) {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_TRUE(hasBitwiseAnd);
}

TEST_F(BitwiseOperationsTest, DetectsBitwiseAndInExpression) {
    std::string code = R"(
        void test() {
            int result = (5 & 3) + 2;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::BitwiseAnd) {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_TRUE(hasBitwiseAnd);
}

TEST_F(BitwiseOperationsTest, CleanCodeNoBitwiseAnd) {
    std::string code = R"(
        void test() {
            int x = 5;
            int y = 3;
            int z = x + y;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::BitwiseAnd) {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_FALSE(hasBitwiseAnd);
}

TEST_F(BitwiseOperationsTest, DistinguishesLogicalAndFromBitwiseAnd) {
    std::string code = R"(
        void test() {
            bool a = true;
            bool b = false;
            bool c = a && b;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    // Logical AND (&&) should not be detected as bitwise AND
    bool hasBitwiseAnd = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::BitwiseAnd) {
            hasBitwiseAnd = true;
        }
    }
    EXPECT_FALSE(hasBitwiseAnd);
}
