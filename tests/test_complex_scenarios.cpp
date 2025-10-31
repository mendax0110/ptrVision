#include <gtest/gtest.h>
#include "PtrVisionLib.h"

using namespace ptrvision;

class ComplexScenariosTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(ComplexScenariosTest, DetectsMultipleViolations)
{
    const std::string code = R"(
        void test() {
            int* p = new int(5);
            p++;
            *p = 10;
            delete p;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(PtrVisionLib::hasForbiddenConstructs(code));
    
    // Should detect: new, pointer arithmetic, dereference, delete
    EXPECT_GE(result.issues.size(), 4);
    
    std::map<ptrvision::IssueType, int> issueCounts;
    for (const auto& issue : result.issues)
    {
        issueCounts[issue.type]++;
    }
    
    EXPECT_GT(issueCounts[ptrvision::IssueType::NewOperator], 0);
    EXPECT_GT(issueCounts[ptrvision::IssueType::PointerArithmetic], 0);
    EXPECT_GT(issueCounts[ptrvision::IssueType::PointerDereference], 0);
    EXPECT_GT(issueCounts[ptrvision::IssueType::DeleteOperator], 0);
}

TEST_F(ComplexScenariosTest, DetectsLinkedListOperations)
{
    const std::string code = R"(
        struct Node {
            int data;
            Node* next;
        };
        
        void test() {
            Node* head = new Node();
            head->data = 5;
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_TRUE(PtrVisionLib::hasForbiddenConstructs(code));
    
    // Should detect multiple pointer operations
    EXPECT_GT(result.issues.size(), 0);
}

TEST_F(ComplexScenariosTest, DetectsPointerArithmeticInLoop)
{
    const std::string code = R"(
        void test() {
            int arr[10];
            int* p = arr;
            for (int i = 0; i < 10; i++) {
                *p = i;
                p++;
            }
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasPointerArithmetic = false;
    bool hasDereference = false;
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::PointerArithmetic)
        {
            hasPointerArithmetic = true;
        }
        if (issue.type == ptrvision::IssueType::PointerDereference)
        {
            hasDereference = true;
        }
    }
    EXPECT_TRUE(hasPointerArithmetic);
    EXPECT_TRUE(hasDereference);
}

TEST_F(ComplexScenariosTest, CleanComplexCode)
{
    const std::string code = R"(
        struct Data {
            int value;
            double score;
        };
        
        void test() {
            Data d;
            d.value = 42;
            d.score = 3.14;
            
            int sum = 0;
            for (int i = 0; i < 10; i++) {
                sum += i;
            }
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    EXPECT_FALSE(PtrVisionLib::hasForbiddenConstructs(code));
}

TEST_F(ComplexScenariosTest, DetectsMixedBitwiseAndPointerOps)
{
    const std::string code = R"(
        void test() {
            int x = 5;
            int y = 3;
            int z = x & y;
            
            int* p = &x;
            *p = 10;
        }
    )";

    const auto result = PtrVisionLib::analyzeCode(code);
    ASSERT_TRUE(result.success);
    
    bool hasBitwise = false;
    bool hasReference = false;
    bool hasDereference = false;
    
    for (const auto& issue : result.issues)
    {
        if (issue.type == ptrvision::IssueType::BitwiseAnd) hasBitwise = true;
        if (issue.type == ptrvision::IssueType::Reference) hasReference = true;
        if (issue.type == ptrvision::IssueType::PointerDereference) hasDereference = true;
    }
    
    EXPECT_TRUE(hasBitwise);
    EXPECT_TRUE(hasReference);
    EXPECT_TRUE(hasDereference);
}
