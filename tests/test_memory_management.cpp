#include <gtest/gtest.h>
#include "PtrVisionLib.h"

using namespace ptrvision;

class MemoryManagementTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(MemoryManagementTest, DetectsNewOperator)
{
    const std::string code = R"(
        void test() {
            int* p = new int(5);
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(PtrVisionLib::hasForbiddenConstructs(code));
    
    bool hasNewOperator = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::NewOperator)
        {
            hasNewOperator = true;
        }
    }
    EXPECT_TRUE(hasNewOperator);
}

TEST_F(MemoryManagementTest, DetectsDeleteOperator)
{
    std::string code = R"(
        void test() {
            int* p = new int(5);
            delete p;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasNewOperator = false;
    bool hasDeleteOperator = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::NewOperator)
        {
            hasNewOperator = true;
        }
        if (issue.type == ptrvision::IssueType::DeleteOperator)
        {
            hasDeleteOperator = true;
        }
    }
    EXPECT_TRUE(hasNewOperator);
    EXPECT_TRUE(hasDeleteOperator);
}

TEST_F(MemoryManagementTest, DetectsNewArrayOperator)
{
    const std::string code = R"(
        void test() {
            int* arr = new int[10];
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasNewOperator = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::NewOperator)
        {
            hasNewOperator = true;
            EXPECT_NE(issue.description.find("[]"), std::string::npos);
        }
    }
    EXPECT_TRUE(hasNewOperator);
}

TEST_F(MemoryManagementTest, DetectsDeleteArrayOperator)
{
    std::string code = R"(
        void test() {
            int* arr = new int[10];
            delete[] arr;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasDeleteOperator = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::DeleteOperator)
        {
            hasDeleteOperator = true;
            EXPECT_NE(issue.description.find("[]"), std::string::npos);
        }
    }
    EXPECT_TRUE(hasDeleteOperator);
}

TEST_F(MemoryManagementTest, CleanCodeNoMemoryManagement)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int arr[10];
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasMemoryManagement = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::NewOperator ||
            issue.type == ptrvision::IssueType::DeleteOperator)
        {
            hasMemoryManagement = true;
        }
    }
    EXPECT_FALSE(hasMemoryManagement);
}
