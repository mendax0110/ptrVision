#include "PtrVisionLib.h"
#include <iostream>
#include <string>

using namespace ptrvision;

int main()
{
    // Example 1: Code with pointer arithmetic
    std::string code1 = R"(
        void test() {
            int arr[3] = {1, 2, 3};
            int* p = arr;
            p++;
            *p = 10;
        }
    )";

    std::cout << "=== Example 1: Code with pointer arithmetic ===" << std::endl;
    const AnalysisResult result1 = PtrVisionLib::analyzeCode(code1);
    
    if (result1.success)
    {
        std::cout << "Analysis completed successfully!" << std::endl;
        std::cout << "Found " << result1.issues.size() << " issue(s):" << std::endl;
        
        for (const auto& issue : result1.issues)
        {
            std::cout << "  - Line " << issue.line << ", Column " << issue.column 
                      << ": " << PtrVisionLib::issueTypeToString(issue.type)
                      << " - " << issue.description << std::endl;
        }
    }
    else
    {
        std::cout << "Analysis failed: " << result1.errorMessage << std::endl;
    }

    // Example 2: Code with new/delete
    std::string code2 = R"(
        void test() {
            int* p = new int(5);
            delete p;
        }
    )";

    std::cout << "\n=== Example 2: Code with new/delete ===" << std::endl;
    const AnalysisResult result2 = PtrVisionLib::analyzeCode(code2);
    
    if (result2.success)
    {
        std::cout << "Found " << result2.issues.size() << " issue(s):" << std::endl;
        for (const auto& issue : result2.issues)
        {
            std::cout << "  - " << PtrVisionLib::issueTypeToString(issue.type)
                      << ": " << issue.description << std::endl;
        }
    }

    // Example 3: Simple boolean check
    std::string code3 = R"(
        void test() {
            int x = 5;
            int y = x + 10;
        }
    )";

    std::cout << "\n=== Example 3: Clean code (no forbidden constructs) ===" << std::endl;
    if (PtrVisionLib::hasForbiddenConstructs(code3))
    {
        std::cout << "Code contains forbidden constructs - REJECTED!" << std::endl;
    }
    else
    {
        std::cout << "Code is clean - ACCEPTED!" << std::endl;
    }

    // Example 4: Code with goto
    std::string code4 = R"(
        void test() {
            int x = 0;
            label:
            x++;
            if (x < 10) goto label;
        }
    )";

    std::cout << "\n=== Example 4: Code with goto ===" << std::endl;
    if (PtrVisionLib::hasForbiddenConstructs(code4))
    {
        std::cout << "Code contains forbidden constructs - REJECTED!" << std::endl;
        const AnalysisResult result4 = PtrVisionLib::analyzeCode(code4);
        for (const auto& issue : result4.issues)
        {
            std::cout << "  - " << PtrVisionLib::issueTypeToString(issue.type) << std::endl;
        }
    }

    return 0;
}
