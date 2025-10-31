#include <gtest/gtest.h>
#include "PtrVisionLib.h"

using namespace ptrvision;

class ErrorHandlingTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(ErrorHandlingTest, HandlesInvalidSyntax)
{
    const std::string code = R"(
        void test() {
            int x = ;  // Invalid syntax
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    // Should fail to parse but not crash
    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST_F(ErrorHandlingTest, HandlesEmptyCode)
{
    const std::string code = "";

    const auto result = PtrVisionLib::analyzeCode(code);
    // Empty code should parse successfully with no issues
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.issues.size(), 0);
}

TEST_F(ErrorHandlingTest, HandlesOnlyWhitespace)
{
    const std::string code = "   \n\t\n   ";

    const auto result = PtrVisionLib::analyzeCode(code);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.issues.size(), 0);
}

TEST_F(ErrorHandlingTest, HandlesComments)
{
    const std::string code = R"(
        // This is a comment
        /* Multi-line
           comment */
        void test() {
            int x = 5;  // Inline comment
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_FALSE(PtrVisionLib::hasForbiddenConstructs(code));
}

TEST_F(ErrorHandlingTest, HandlesLargeCode)
{
    std::string code = "void test() {\n";
    // Generate large but valid code
    for (int i = 0; i < 1000; i++)
    {
        code += "    int x" + std::to_string(i) + " = " + std::to_string(i) + ";\n";
    }
    code += "}\n";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_FALSE(PtrVisionLib::hasForbiddenConstructs(code));
}

TEST_F(ErrorHandlingTest, HandlesTemplateCode)
{
    const std::string code = R"(
        template<typename T>
        void test() {
            T value;
        }
    )";
    
    auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
}

TEST_F(ErrorHandlingTest, HandlesNamespaces)
{
    const std::string code = R"(
        namespace MyNamespace {
            void test() {
                int x = 5;
            }
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_FALSE(PtrVisionLib::hasForbiddenConstructs(code));
}

TEST_F(ErrorHandlingTest, IssueTypeToStringCoversAllTypes)
{
    // Verify all issue types have string representations
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::PointerArithmetic).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::PointerDereference).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::Reference).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::NewOperator).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::DeleteOperator).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::BitwiseAnd).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::GotoStatement).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::ArraySubscript).empty());
    EXPECT_FALSE(PtrVisionLib::issueTypeToString(ptrvision::IssueType::PointerDeclaration).empty());
}

TEST_F(ErrorHandlingTest, ThreadSafetyBasicTest)
{
    // Basic test - real thread safety testing would require more complex setup
    const std::string code1 = R"(void test1() { int x = 5; })";
    const std::string code2 = R"(void test2() { int* p = nullptr; })";

    const auto result1 = PtrVisionLib::analyzeCode(code1);
    const auto result2 = PtrVisionLib::analyzeCode(code2);
    
    EXPECT_TRUE(result1.success);
    EXPECT_TRUE(result2.success);
    EXPECT_EQ(result1.issues.size(), 0);
    EXPECT_GT(result2.issues.size(), 0);
}
