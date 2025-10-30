# PointerVision Library

PointerVision is a C++14-compatible static analysis library designed to help developers detect and reject C++ code containing pointer and reference operations, memory management constructs, and other potentially unsafe operations.

## Features

The library can detect:

- **Pointer Arithmetic:** Operations like `ptr + offset`, `ptr++`, etc.
- **Pointer Dereference:** Operations like `*ptr`, `ptr->member`
- **References:** Reference declarations and usage (`&`)
- **Memory Management:** `new` and `delete` operators
- **Bitwise AND:** Bitwise AND operator (`&`)
- **Goto Statements:** Usage of `goto`
- **Pointer Declarations:** Any pointer variable declarations

## Use Case

This library is ideal for environments where you need to validate user-submitted code and reject code containing certain constructs. For example:
- Online code judges or learning platforms
- Sandboxed execution environments
- Code review automation tools

## Building the Library

### Prerequisites

- CMake 3.10 or higher
- Clang/LLVM 14 or higher
- C++14 compatible compiler

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/mendax0110/ptrVision.git
cd ptrVision

# Create build directory
mkdir build && cd build

# Configure with the library-only CMakeLists
cmake -DCMAKE_BUILD_TYPE=Release ..
# Or use the library-specific CMakeLists:
# cmake -DCMAKE_BUILD_TYPE=Release -C ../CMakeLists_library.txt ..

# Build
cmake --build .
```

## Integration into Your Project

### Option 1: Link Against the Library

After building, you'll have `libptrVision.so` (Linux), `libptrVision.dylib` (macOS), or `ptrVision.dll` (Windows).

**CMakeLists.txt:**
```cmake
find_library(PTRVISION_LIB ptrVision PATHS /path/to/library)
include_directories(/path/to/include)
target_link_libraries(your_target ${PTRVISION_LIB})
```

### Option 2: Include Source Files

Add the following files to your project:
- `include/PtrVisionLib.h`
- `include/PointerReferenceCollector.h`
- `src/PtrVisionLib.cpp`
- `src/PointerReferenceCollector.cpp`

Ensure you link against Clang/LLVM libraries.

## Usage Example

```cpp
#include "PtrVisionLib.h"
#include <iostream>
#include <string>

int main()
{
    // Code to analyze (e.g., user input)
    std::string userCode = R"(
        void myFunction() {
            int* ptr = new int(5);
            delete ptr;
        }
    )";

    // Quick check: Does code contain forbidden constructs?
    if (ptrvision::hasForbiddenConstructs(userCode))
    {
        std::cout << "ERROR: Code contains forbidden constructs!" << std::endl;
        return 1;
    }

    // Detailed analysis
    ptrvision::AnalysisResult result = ptrvision::analyzeCode(userCode);
    
    if (result.success)
    {
        if (result.issues.empty())
        {
            std::cout << "Code is clean - ACCEPTED" << std::endl;
        }
        else
        {
            std::cout << "Code REJECTED - Found " << result.issues.size() 
                      << " issue(s):" << std::endl;
            
            for (const auto& issue : result.issues)
            {
                std::cout << "  Line " << issue.line 
                          << ": " << ptrvision::issueTypeToString(issue.type)
                          << " - " << issue.description << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Analysis failed: " << result.errorMessage << std::endl;
    }

    return 0;
}
```

## API Reference

### Main Functions

#### `analyzeCode()`
```cpp
AnalysisResult analyzeCode(const std::string& code, 
                           const std::string& filename = "input.cpp");
```
Analyzes the provided C++ code and returns detailed information about detected issues.

**Parameters:**
- `code`: C++ source code as a string
- `filename`: Optional filename for error reporting (default: "input.cpp")

**Returns:** `AnalysisResult` containing:
- `success`: Whether analysis completed successfully
- `issues`: Vector of detected `CodeIssue` objects
- `errorMessage`: Error description if analysis failed

#### `hasForbiddenConstructs()`
```cpp
bool hasForbiddenConstructs(const std::string& code, 
                            const std::string& filename = "input.cpp");
```
Quick check to determine if code contains any forbidden constructs.

**Returns:** `true` if forbidden constructs are detected, `false` otherwise.

#### `issueTypeToString()`
```cpp
std::string issueTypeToString(IssueType type);
```
Converts an `IssueType` enum to a human-readable string.

### Data Structures

#### `CodeIssue`
Represents a single detected issue:
```cpp
struct CodeIssue
{
    IssueType type;           // Type of issue
    unsigned int line;         // Line number
    unsigned int column;       // Column number
    std::string description;   // Detailed description
};
```

#### `IssueType` Enum
```cpp
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
```

#### `AnalysisResult`
Contains the result of code analysis:
```cpp
struct AnalysisResult
{
    bool success;                      // Analysis success flag
    std::vector<CodeIssue> issues;     // Detected issues
    std::string errorMessage;          // Error message if failed
};
```

## C++17 Requirement

This library requires C++17 as the minimum standard:
- LLVM 18+ requires C++17 for its headers and APIs
- The library implementation uses C++17 features
- Your codebase integrating this library should use C++17 or later
- C++17 is widely supported (GCC 7+, Clang 5+, MSVC 2017+)

## Why Clang/LLVM?

While the std library provides basic string manipulation, properly parsing and analyzing C++ code requires understanding its syntax tree. Clang/LLVM provides:
- Complete C++ parser
- Abstract Syntax Tree (AST) representation
- Robust handling of complex C++ constructs
- Industry-standard compiler infrastructure

Attempting to use regex or simple parsing would miss many edge cases and provide unreliable results.

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please submit issues and pull requests on GitHub.
