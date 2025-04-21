#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The AsmGen class generates assembly code from C++ source files. \class AsmGen
    class AsmGen
    {
    public:
        AsmGen();
        ~AsmGen();

        /**
         * @brief Generate assembly code from a C++ source file.
         * @param filename -> The path to the C++ source file.
         * @return -> The generated assembly code as a string.
         */
        static std::string generateAssembly(const std::string& filename);

        /**
         * @brief Generate assembly code from a C++ source file and store it in a file.
         * @param filePath -> The path to the C++ source file.
         * @return -> True if the assembly code was generated and stored successfully, false otherwise.
         */
        static bool generateStoreAssembly(const std::string& filePath);

        /**
         * @brief Get the generated assembly code.
         * @return -> The generated assembly code as a string.
         */
        static std::string getGeneratedAssembly();

        /**
         * @brief Clear the generated assembly code.
         */
        static void clearGeneratedAssembly();

    private:
        static std::string generatedAssembly;
    };
}