#include <CliManager.h>


void CliManager::showHelp()
{
    llvm::errs() << "Usage: ptrVision <file|directory> [marker] [options]\n";
    llvm::errs() << "Options:\n";
    llvm::errs() << "  -help\t\tShow this help message\n";
}

void CliManager::printError(llvm::Error errorMessage)
{
#ifdef _WIN32
    setConsoleColor(4);
    llvm::errs() << "[ERROR] " << llvm::toString(std::move(errorMessage)) << "\n";
    setConsoleColor(RESET_WIN_COLOR);
#else
    llvm::outs() << RED_COLOR << "[ERROR] " << llvm::toString(std::move(errorMessage)) << RESET_COLOR << "\n";
#endif
}

void CliManager::printError(const std::string& errorMessage)
{
#ifdef _WIN32
    setConsoleColor(RED_WIN_COLOR);
	llvm::errs() << "[ERROR] " << errorMessage << "\n";
	setConsoleColor(RESET_WIN_COLOR);
#else
	llvm::outs() << RED_COLOR << "[ERROR] " << errorMessage << RESET_COLOR << "\n";
#endif
}

