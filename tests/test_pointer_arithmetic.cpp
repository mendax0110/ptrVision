#include <gtest/gtest.h>
#include "PtrVisionLib.h"

class PointerArithmeticTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PointerArithmeticTest, DetectsPointerIncrement) {
    std::string code = R"(
        void test() {
            int arr[3] = {1, 2, 3};
            int* p = arr;
            p++;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    // Should detect pointer declaration and pointer arithmetic
    bool hasPointerArithmetic = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerArithmetic) {
            hasPointerArithmetic = true;
            EXPECT_EQ(issue.line, 5);
        }
    }
    EXPECT_TRUE(hasPointerArithmetic);
}

TEST_F(PointerArithmeticTest, DetectsPointerDecrement) {
    std::string code = R"(
        void test() {
            int arr[3] = {1, 2, 3};
            int* p = arr;
            p--;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    bool hasPointerArithmetic = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerArithmetic) {
            hasPointerArithmetic = true;
        }
    }
    EXPECT_TRUE(hasPointerArithmetic);
}

TEST_F(PointerArithmeticTest, DetectsPointerAddition) {
    std::string code = R"(
        void test() {
            int arr[10];
            int* p = arr + 5;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasPointerArithmetic = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerArithmetic) {
            hasPointerArithmetic = true;
        }
    }
    EXPECT_TRUE(hasPointerArithmetic);
}

TEST_F(PointerArithmeticTest, DetectsPointerSubtraction) {
    std::string code = R"(
        void test() {
            int arr[10];
            int* p = arr;
            int* q = p - 2;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasPointerArithmetic = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerArithmetic) {
            hasPointerArithmetic = true;
        }
    }
    EXPECT_TRUE(hasPointerArithmetic);
}

TEST_F(PointerArithmeticTest, DetectsCompoundAssignment) {
    std::string code = R"(
        void test() {
            int arr[10];
            int* p = arr;
            p += 3;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasPointerArithmetic = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerArithmetic) {
            hasPointerArithmetic = true;
        }
    }
    EXPECT_TRUE(hasPointerArithmetic);
}

TEST_F(PointerArithmeticTest, CleanCodeNoPointerArithmetic) {
    std::string code = R"(
        void test() {
            int x = 5;
            int y = x + 10;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_FALSE(ptrvision::hasForbiddenConstructs(code));
}
