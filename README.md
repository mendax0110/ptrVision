# PointerVision Library

PointerVision is a C++17-compatible static analysis library designed to help developers detect and reject C++ code containing pointer and reference operations, memory management constructs, and other potentially unsafe operations. It can be integrated into existing C++ codebases to validate user-submitted code.

## Features

The library can detect:

- **Pointer Arithmetic:** Operations like `ptr + offset`, `ptr++`, etc.
- **Pointer Dereference:** Operations like `*ptr`, `ptr->member`
- **References:** Reference declarations and usage (`&`)
- **Memory Management:** `new` and `delete` operators
- **Bitwise AND:** Bitwise AND operator (`&`)
- **Goto Statements:** Usage of `goto`
- **Pointer Declarations:** Any pointer variable declarations
- **Array Subscript:** Array subscript operations with pointers

## Use Case

This library is ideal for environments where you need to validate user-submitted code and reject code containing certain constructs. For example:
- Online code judges or learning platforms
- Sandboxed execution environments
- Code review automation tools
- Educational platforms teaching safe C++ practices

## Building the Library

### Prerequisites

- CMake 3.10 or higher
- Clang/LLVM 14 or higher (18+ recommended)
- C++17 compatible compiler
- libclang-cpp development headers

### Installation on Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y llvm-18-dev libclang-18-dev libclang-cpp18-dev clang-18
```

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/mendax0110/ptrVision.git
cd ptrVision

# Create build directory
mkdir build && cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
cmake --build . -j$(nproc)

# Test
./ptrVision_example
```

## Integration into Your Project

### Example Usage

```cpp
#include "PtrVisionLib.h"
#include <iostream>

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

    return 0;
}
```

### API Reference

See [README_LIBRARY.md](README_LIBRARY.md) for detailed API documentation.

## C++17 Requirement

This library requires C++17 as the minimum standard because:
- LLVM 18+ requires C++17 for its headers
- C++17 is widely supported and compatible with most modern codebases
- The library interface itself is C++14-compatible, but the implementation uses LLVM 18
