#include <gtest/gtest.h>
#include "PtrVisionLib.h"

using namespace ptrvision;

class ReferencesTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(ReferencesTest, DetectsReferenceDeclaration)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int& ref = x;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(PtrVisionLib::hasForbiddenConstructs(code));
    
    bool hasReference = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::Reference)
        {
            hasReference = true;
        }
    }
    EXPECT_TRUE(hasReference);
}

TEST_F(ReferencesTest, DetectsAddressOfOperator)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int* p = &x;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasReference = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::Reference)
        {
            hasReference = true;
        }
    }
    EXPECT_TRUE(hasReference);
}

TEST_F(ReferencesTest, DetectsConstReference)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            const int& ref = x;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasReference = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::Reference)
        {
            hasReference = true;
        }
    }
    EXPECT_TRUE(hasReference);
}

TEST_F(ReferencesTest, CleanCodeNoReferences)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int y = x;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasReference = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::Reference)
        {
            hasReference = true;
        }
    }
    EXPECT_FALSE(hasReference);
}
