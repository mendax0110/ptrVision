#ifndef PTRVISIONLIB_H
#define PTRVISIONLIB_H

#include <string>
#include <vector>

namespace ptrvision
{
    /**
     * @brief Type of issue detected in the code
     */
    enum class IssueType
    {
        PointerArithmetic,
        PointerDereference,
        Reference,
        NewOperator,
        DeleteOperator,
        BitwiseAnd,
        GotoStatement,
        ArraySubscript,
        PointerDeclaration
    };

    /**
     * @brief Represents a single issue found in the analyzed code
     */
    struct CodeIssue
    {
        IssueType type;
        unsigned int line;
        unsigned int column;
        std::string description;
        
        CodeIssue() : type(IssueType::PointerArithmetic), line(0), column(0), description("") {}
        
        CodeIssue(IssueType t, unsigned int l, unsigned int c, const std::string& desc)
            : type(t), line(l), column(c), description(desc) {}
    };

    /**
     * @brief Result of code analysis
     */
    struct AnalysisResult
    {
        bool success;
        std::vector<CodeIssue> issues;
        std::string errorMessage;
        
        AnalysisResult() : success(false) {}
    };

    /**
     * @brief Main entry point for analyzing C++ code
     * 
     * @param code C++ code to analyze (as string)
     * @param filename Optional filename for better error messages (default: "input.cpp")
     * @return AnalysisResult containing detected issues or error information
     */
    AnalysisResult analyzeCode(const std::string& code, const std::string& filename = "input.cpp");

    /**
     * @brief Check if the code contains any forbidden constructs
     * 
     * @param code C++ code to check
     * @param filename Optional filename for better error messages
     * @return true if code contains forbidden constructs (pointers, references, etc.), false otherwise
     */
    bool hasForbiddenConstructs(const std::string& code, const std::string& filename = "input.cpp");

    /**
     * @brief Get human-readable string for issue type
     * 
     * @param type Issue type
     * @return String representation of the issue type
     */
    std::string issueTypeToString(IssueType type);

} // namespace ptrvision

#endif // PTRVISIONLIB_H
