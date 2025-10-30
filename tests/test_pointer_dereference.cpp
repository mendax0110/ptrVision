#include <gtest/gtest.h>
#include "PtrVisionLib.h"

class PointerDereferenceTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PointerDereferenceTest, DetectsSimpleDereference) {
    std::string code = R"(
        void test() {
            int x = 5;
            int* p = &x;
            int y = *p;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    bool hasDereference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerDereference) {
            hasDereference = true;
        }
    }
    EXPECT_TRUE(hasDereference);
}

TEST_F(PointerDereferenceTest, DetectsArrowOperator) {
    std::string code = R"(
        struct Point { int x, y; };
        void test() {
            Point* p = nullptr;
            int val = p->x;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasDereference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerDereference) {
            hasDereference = true;
        }
    }
    EXPECT_TRUE(hasDereference);
}

TEST_F(PointerDereferenceTest, DetectsMultipleDereferences) {
    std::string code = R"(
        void test() {
            int x = 5;
            int* p = &x;
            *p = 10;
            int y = *p;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    int dereferenceCount = 0;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerDereference) {
            dereferenceCount++;
        }
    }
    EXPECT_GE(dereferenceCount, 2);
}

TEST_F(PointerDereferenceTest, CleanCodeNoDereference) {
    std::string code = R"(
        void test() {
            int x = 5;
            int y = x * 2;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasDereference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::PointerDereference) {
            hasDereference = true;
        }
    }
    EXPECT_FALSE(hasDereference);
}
