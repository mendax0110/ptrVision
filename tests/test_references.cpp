#include <gtest/gtest.h>
#include "PtrVisionLib.h"

class ReferencesTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ReferencesTest, DetectsReferenceDeclaration) {
    std::string code = R"(
        void test() {
            int x = 5;
            int& ref = x;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    bool hasReference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::Reference) {
            hasReference = true;
        }
    }
    EXPECT_TRUE(hasReference);
}

TEST_F(ReferencesTest, DetectsAddressOfOperator) {
    std::string code = R"(
        void test() {
            int x = 5;
            int* p = &x;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasReference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::Reference) {
            hasReference = true;
        }
    }
    EXPECT_TRUE(hasReference);
}

TEST_F(ReferencesTest, DetectsConstReference) {
    std::string code = R"(
        void test() {
            int x = 5;
            const int& ref = x;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasReference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::Reference) {
            hasReference = true;
        }
    }
    EXPECT_TRUE(hasReference);
}

TEST_F(ReferencesTest, CleanCodeNoReferences) {
    std::string code = R"(
        void test() {
            int x = 5;
            int y = x;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasReference = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::Reference) {
            hasReference = true;
        }
    }
    EXPECT_FALSE(hasReference);
}
