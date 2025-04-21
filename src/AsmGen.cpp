#include <AsmGen.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <fstream>
#include <mutex>

using namespace clang;

std::string AsmGen::generatedAssembly;

AsmGen::AsmGen()
{
    static std::once_flag initialized;
    std::call_once(initialized, []() {
        // Initialize all supported architectures
        LLVMInitializeX86TargetInfo();
        LLVMInitializeX86TargetMC();
        LLVMInitializeX86AsmPrinter();

        LLVMInitializeAArch64TargetInfo();
        LLVMInitializeAArch64TargetMC();
        LLVMInitializeAArch64AsmPrinter();

        LLVMInitializeARMTargetInfo();
        LLVMInitializeARMTargetMC();
        LLVMInitializeARMAsmPrinter();

        // Initialize support components
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        llvm::InitializeAllAsmParsers();
    });
}

AsmGen::~AsmGen() = default;

std::string AsmGen::generateAssembly(const std::string& filename)
{
    if (!llvm::sys::fs::exists(filename))
    {
        llvm::errs() << "File not found: " << filename << "\n";
        return "";
    }

    CompilerInstance compiler;
    compiler.createDiagnostics();

    auto& targetOpts = compiler.getTargetOpts();
    targetOpts.Triple = llvm::sys::getDefaultTargetTriple();
    targetOpts.CPU = llvm::sys::getHostCPUName().str();
    targetOpts.ABI = "";
    targetOpts.CodeModel = "default";

    if (targetOpts.Triple.find("apple") != std::string::npos)
    {
        targetOpts.ABI = "darwinpcs";
        if (targetOpts.Triple.find("arm64") != std::string::npos)
        {
            targetOpts.CodeModel = "small";
        }
    }
    else if (targetOpts.Triple.find("linux") != std::string::npos)
    {
        if (targetOpts.Triple.find("aarch64") != std::string::npos)
        {
            targetOpts.CodeModel = "small";
        }
    }

    llvm::errs() << "Target Configuration:\n";
    llvm::errs() << "  Triple: " << targetOpts.Triple << "\n";
    llvm::errs() << "  CPU: " << targetOpts.CPU << "\n";
    llvm::errs() << "  ABI: " << (targetOpts.ABI.empty() ? "(default)" : targetOpts.ABI) << "\n";
    llvm::errs() << "  Code Model: " << targetOpts.CodeModel << "\n";

    compiler.setTarget(TargetInfo::CreateTargetInfo(
            compiler.getDiagnostics(),
            std::make_shared<TargetOptions>(targetOpts)));

    if (!compiler.hasTarget())
    {
        llvm::errs() << "Failed to create target for triple: " << targetOpts.Triple << "\n";
        return "";
    }

    compiler.createFileManager();
    compiler.createSourceManager(compiler.getFileManager());

    FrontendInputFile input(filename, Language::CXX);
    compiler.getFrontendOpts().Inputs.push_back(input);

    llvm::SmallString<128> outputPath;
    std::error_code EC = llvm::sys::fs::createTemporaryFile("asm_output", "s", outputPath);
    if (EC)
    {
        llvm::errs() << "Failed to create temporary file: " << EC.message() << "\n";
        return "";
    }
    compiler.getFrontendOpts().OutputFile = outputPath.str().str();

    auto action = std::make_unique<EmitAssemblyAction>();
    if (!compiler.ExecuteAction(*action))
    {
        llvm::errs() << "Failed to generate assembly\n";
        llvm::sys::fs::remove(outputPath);
        return "";
    }

    std::ifstream asmFile(outputPath.str().str());
    if (!asmFile.is_open())
    {
        llvm::errs() << "Failed to open assembly file\n";
        llvm::sys::fs::remove(outputPath);
        return "";
    }

    std::string assemblyStr((std::istreambuf_iterator<char>(asmFile)),
                            std::istreambuf_iterator<char>());
    asmFile.close();
    llvm::sys::fs::remove(outputPath);

    return assemblyStr;
}

bool AsmGen::generateStoreAssembly(const std::string &filePath)
{
    generatedAssembly = generateAssembly(filePath);
    return !generatedAssembly.empty();
}

std::string AsmGen::getGeneratedAssembly()
{
    return generatedAssembly;
}

void AsmGen::clearGeneratedAssembly()
{
    generatedAssembly.clear();
}