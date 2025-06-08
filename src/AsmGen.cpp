#include <AsmGen.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <fstream>
#include <mutex>
#include <CliManager.h>

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
        CliManager::print(OutputLevel::ERROR, "File not found: ", filename);
        return "";
    }

    CompilerInstance compiler;
    compiler.createDiagnostics(*llvm::vfs::getRealFileSystem());

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
        CliManager::print(OutputLevel::ERROR, "Failed to create target for triple: ", targetOpts.Triple);
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
        CliManager::print(OutputLevel::ERROR, "Failed to create temporary file: ", EC.message());
        return "";
    }
    compiler.getFrontendOpts().OutputFile = outputPath.str().str();

    auto action = std::make_unique<EmitAssemblyAction>();
    if (!compiler.ExecuteAction(*action))
    {
        CliManager::print(OutputLevel::ERROR, "Failed to generate assembly for file: ", filename);
        auto err = llvm::sys::fs::remove(outputPath);
        return err.message();
    }

    std::ifstream asmFile(outputPath.str().str());
    if (!asmFile.is_open())
    {
        CliManager::print(OutputLevel::ERROR, "Failed to open assembly file: ", outputPath.str().str());
        auto err = llvm::sys::fs::remove(outputPath);
        return err.message();
    }

    std::string assemblyStr((std::istreambuf_iterator<char>(asmFile)),
                            std::istreambuf_iterator<char>());
    asmFile.close();
    auto err = llvm::sys::fs::remove(outputPath);
    if (!err.operator bool())
    {
        CliManager::print(OutputLevel::DEBUG, "Temporary assembly file removed: ", outputPath.str().str());
        return err.message();
    }
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