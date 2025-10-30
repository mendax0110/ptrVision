#include <gtest/gtest.h>
#include "PtrVisionLib.h"

class GotoStatementsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(GotoStatementsTest, DetectsSimpleGoto) {
    std::string code = R"(
        void test() {
            int x = 0;
            label:
            x++;
            if (x < 10) goto label;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    bool hasGoto = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::GotoStatement) {
            hasGoto = true;
        }
    }
    EXPECT_TRUE(hasGoto);
}

TEST_F(GotoStatementsTest, DetectsGotoInErrorHandling) {
    std::string code = R"(
        void test() {
            int* p = nullptr;
            if (!p) goto error;
            // do something
            error:
            return;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasGoto = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::GotoStatement) {
            hasGoto = true;
        }
    }
    EXPECT_TRUE(hasGoto);
}

TEST_F(GotoStatementsTest, CleanCodeNoGoto) {
    std::string code = R"(
        void test() {
            for (int i = 0; i < 10; i++) {
                if (i == 5) break;
            }
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasGoto = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::GotoStatement) {
            hasGoto = true;
        }
    }
    EXPECT_FALSE(hasGoto);
}
