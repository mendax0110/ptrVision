#include "PtrVisionLib.h"
#include "CollectorActionWithStorage.h"
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace ptrvision
{

AnalysisResult analyzeCode(const std::string& code, const std::string& filename)
{
    AnalysisResult result;
    
    try
    {
        // Clear any previous issues
        g_collectedIssues.clear();

        // Create action
        auto action = std::make_unique<CollectorActionWithStorage>();

        // Run the tool on the code
        std::vector<std::string> args = {"-fsyntax-only", "-std=c++14"};
        bool success = clang::tooling::runToolOnCodeWithArgs(
            std::move(action), code, args, filename);
        
        if (success)
        {
            result.success = true;
            result.issues = g_collectedIssues;
        }
        else
        {
            result.success = false;
            result.errorMessage = "Failed to parse code";
        }
    }
    catch (const std::exception& e)
    {
        result.success = false;
        result.errorMessage = std::string("Exception during analysis: ") + e.what();
    }
    catch (...)
    {
        result.success = false;
        result.errorMessage = "Unknown exception during analysis";
    }

    return result;
}

bool hasForbiddenConstructs(const std::string& code, const std::string& filename)
{
    AnalysisResult result = analyzeCode(code, filename);
    return result.success && !result.issues.empty();
}

std::string issueTypeToString(IssueType type)
{
    switch (type)
    {
        case IssueType::PointerArithmetic:
            return "Pointer Arithmetic";
        case IssueType::PointerDereference:
            return "Pointer Dereference";
        case IssueType::Reference:
            return "Reference";
        case IssueType::NewOperator:
            return "New Operator";
        case IssueType::DeleteOperator:
            return "Delete Operator";
        case IssueType::BitwiseAnd:
            return "Bitwise AND";
        case IssueType::GotoStatement:
            return "Goto Statement";
        case IssueType::ArraySubscript:
            return "Array Subscript";
        case IssueType::PointerDeclaration:
            return "Pointer Declaration";
        default:
            return "Unknown";
    }
}

} // namespace ptrvision
