#include <gtest/gtest.h>
#include "PtrVisionLib.h"

class MemoryManagementTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MemoryManagementTest, DetectsNewOperator) {
    std::string code = R"(
        void test() {
            int* p = new int(5);
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(ptrvision::hasForbiddenConstructs(code));
    
    bool hasNewOperator = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::NewOperator) {
            hasNewOperator = true;
        }
    }
    EXPECT_TRUE(hasNewOperator);
}

TEST_F(MemoryManagementTest, DetectsDeleteOperator) {
    std::string code = R"(
        void test() {
            int* p = new int(5);
            delete p;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasNewOperator = false;
    bool hasDeleteOperator = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::NewOperator) {
            hasNewOperator = true;
        }
        if (issue.type == ptrvision::IssueType::DeleteOperator) {
            hasDeleteOperator = true;
        }
    }
    EXPECT_TRUE(hasNewOperator);
    EXPECT_TRUE(hasDeleteOperator);
}

TEST_F(MemoryManagementTest, DetectsNewArrayOperator) {
    std::string code = R"(
        void test() {
            int* arr = new int[10];
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasNewOperator = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::NewOperator) {
            hasNewOperator = true;
            EXPECT_NE(issue.description.find("[]"), std::string::npos);
        }
    }
    EXPECT_TRUE(hasNewOperator);
}

TEST_F(MemoryManagementTest, DetectsDeleteArrayOperator) {
    std::string code = R"(
        void test() {
            int* arr = new int[10];
            delete[] arr;
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasDeleteOperator = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::DeleteOperator) {
            hasDeleteOperator = true;
            EXPECT_NE(issue.description.find("[]"), std::string::npos);
        }
    }
    EXPECT_TRUE(hasDeleteOperator);
}

TEST_F(MemoryManagementTest, CleanCodeNoMemoryManagement) {
    std::string code = R"(
        void test() {
            int x = 5;
            int arr[10];
        }
    )";
    
    auto result = ptrvision::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasMemoryManagement = false;
    for (const auto& issue : result.issues) {
        if (issue.type == ptrvision::IssueType::NewOperator ||
            issue.type == ptrvision::IssueType::DeleteOperator) {
            hasMemoryManagement = true;
        }
    }
    EXPECT_FALSE(hasMemoryManagement);
}
