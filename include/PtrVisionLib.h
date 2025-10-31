#ifndef PTRVISIONLIB_H
#define PTRVISIONLIB_H

#include <string>
#include <vector>

/// @brief ptrvision namespace \namespace ptrvision
namespace ptrvision
{
    /// @brief Types of issues that can be detected \enum IssueType
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

    /// @brief Representation of a code issue \struct CodeIssue
    struct CodeIssue
    {
        IssueType type;
        unsigned int line;
        unsigned int column;
        std::string description;

        /**
         * @brief Default constructor
         */
        CodeIssue()
            : type(IssueType::PointerArithmetic)
            , line(0)
            , column(0)
            , description(  "")
        {

        }

        /**
         * @brief Parameterized constructor
         * @param t Issue type
         * @param l Line number
         * @param c Column number
         * @param desc Description of the issue
         */
        CodeIssue(const IssueType t, const unsigned int l, const unsigned int c, const std::string& desc)
            : type(t)
            , line(l)
            , column(c)
            , description(desc)
        {

        }
    };

    /// @brief Result of code analysis \struct AnalysisResult
    struct AnalysisResult
    {
        bool success;
        std::vector<CodeIssue> issues;
        std::string errorMessage;

        /**
         * @brief Default constructor
         */
        AnalysisResult() : success(false)
        {

        }
    };

    class PtrVisionLib
    {
    public:
        PtrVisionLib() = default;
        ~PtrVisionLib() = default;

        /**
         * @brief Main entry point for analyzing C++ code
         * @param code C++ code to analyze (as string)
         * @param filename Optional filename for better error messages (default: "input.cpp")
         * @return AnalysisResult containing detected issues or error information
         */
        static AnalysisResult analyzeCode(const std::string& code, const std::string& filename = "input.cpp");

        /**
         * @brief Check if the code contains any forbidden constructs
         * @param code C++ code to check
         * @param filename Optional filename for better error messages
         * @return true if code contains forbidden constructs (pointers, references, etc.), false otherwise
         */
        static bool hasForbiddenConstructs(const std::string& code, const std::string& filename = "input.cpp");

        /**
         * @brief Get human-readable string for issue type
         * @param type Issue type
         * @return String representation of the issue type
         */
        static std::string issueTypeToString(IssueType type);
    };

} // namespace ptrvision

#endif // PTRVISIONLIB_H
