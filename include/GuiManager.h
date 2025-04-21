#ifndef PTRVISION_GUIMANAGER_H
#define PTRVISION_GUIMANAGER_H

#include <string>
#include <atomic>
#include <imgui.h>
#include "../external/dearimgui/backends/imgui_impl_glfw.h"
#include "../external/dearimgui/backends/imgui_impl_opengl3.h"
#include <ProjectExplorer.h>
#include <JsonParser.h>
#include <FileHandler.h>
#include <thread>
#include <stack>
#include <mutex>
#include "../external/ImGuiFileDialog/ImGuiFileDialog.h"
#include "../external/ImGuiColorTextEdit/TextEditor.h"

/// @brief The clang namespace \namespace clang
namespace clang
{
    /// @brief The ActiveDialog enum class represents the type of active dialog in the GUI. \enum ActiveDialog
    enum class ActiveDialog
    {
        None,
        OpenFile,
        CreateFile,
        SaveFile
    };

    /// @brief The GuiManager class manages the GUI of the application. \class GuiManager
    class GuiManager
    {
    public:
        GuiManager();
        ~GuiManager();

        /**
         * @brief Initialize the GUI manager.
         */
        void Run();

    private:

        /**
         * @brief Render the main menu.
         */
        void RenderMainMenu();

        /**
         * @brief Render the file dialog.
         */
        void RenderFileDialog();

        /**
         * @brief Render the project explorer.
         *
         * @param filePath -> The path to the file.
         */
        void HandleProjectExplorerFileClick(const std::filesystem::path& filePath);

        /**
         * @brief Redo the last action.
         */
        void Redo();

        /**
         * @brief Undo the last action.
         */
        void Undo();

        /**
         * @brief Save the current file.
         */
        void SaveFile();

        /**
         * @brief Render the Popups.
         */
        static void RenderPopups();

        /**
         * @brief Render the main menu bar.
         */
        void RenderMainMenuBar();

        /**
         * @brief Handles the file open event.
         * @param fileName -> The name of the file to open.
         * @param filePath -> The path to the file to open.
         * @param selectedPath -> The selected path in the file dialog.
         */
        void HandleFileOpen(const std::string& fileName,
                            const std::string& filePath,
                            const std::filesystem::path& selectedPath);

        /**
         * @brief render the Docking space.
         */
        static void RenderDockingSpace();

        /**
         * @brief Open the Documentation.
         */
        static void OpenDocumentation();

        /**
         * @brief Check if a file is open.
         * @param filePath -> The path to the file.
         * @param fileName -> The name of the file.
         * @return -> True if the file is open, false otherwise.
         */
        bool IsFileOpen(const std::string& filePath, const std::string& fileName);

        /// @brief The OpenFile struct represents an open file in the GUI. \struct OpenFile
        struct OpenFile
        {
            std::string fileName;
            std::string filePath;
            std::string content;
            TextEditor editor;
            std::string fileData;
        };

        /**
         * @brief Create a new file.
         * @param fileName -> The name of the file to create.
         * @param filePath -> The path where the file will be created.
         * @param selectedPath -> The selected path in the file dialog.
         * @return -> An OpenFile object representing the new file.
         */
        static OpenFile CreateNewFile(const std::string& fileName,
                                      const std::string& filePath,
                                      const std::filesystem::path& selectedPath);

        /// @brief The MenuItem struct represents an item in the menu. \struct MenuItem
        struct MenuItem
        {
            const char* name;
            const char* shortcut;
            std::function<void()> action;
            bool enabled = true;
        };

        /**
         * @brief Render the menu.
         * @param menuName -> The name of the menu.
         * @param menuItems -> The items in the menu.
         */
        static void RenderMenu(const char* menuName, const std::vector<MenuItem>& menuItems);

        /**
         * @brief Open the file dialog.
         * @param dialogType -> The type of dialog to open.
         * @param dialogTitle -> The title of the dialog.
         */
        void OpenFileDialog(clang::ActiveDialog dialogType, const char* dialogTitle);

        /**
         * @brief Run the analysis function.
         * @param analysisFunc -> The analysis function to run.
         */
        void RunAnalysis(const std::function<void(const std::string&, std::vector<std::string>&)>& analysisFunc);

        /**
         * @brief Render the active editor.
         */
        void RenderActiveEditor();

        /**
         * @brief Render the output window.
         */
        static void RenderOutputWindow();

        /**
         * @brief Render the Assembly window.
         */
        void RenderAssemblyWindow();

        /**
         * @brief Render the project tree.
         * @param node -> The root node of the project tree.
         */
        void RenderProjectTree(TreeNode* node);

        /**
         * @brief Exits the application GUI.
         */
        void ExitApp();

        /**
         * @brief Handles the file dialog events.
         * @param dialogType -> The type of dialog to handle.
         */
        void HandleFileDialog(ActiveDialog dialogType);

        /**
         * @brief Build the project tree.
         * @param rootPath -> The root path of the project.
         * @return -> A pointer to the root node of the project tree.
         */
        TreeNode* BuildProjectTree(const std::string& rootPath);

        /**
         * @brief Render the memory editor.
         */
        void MemoryEditor();

        FileSystemManager fileSystemManager;

        std::vector<OpenFile> openFiles;
        int currentFileIndex = -1;

        GLFWwindow* window;
        ImGuiFileDialog fileDialog;
        std::string filePathName;
        std::string filePath;
        std::string text;
        std::string output;
        bool settingsOpen;
        std::stack<std::string> undoStack;
        std::stack<std::string> redoStack;
        TreeNode* rootNode;
        ImVec4 bgColor;
        int windowWidth{};
        int windowHeight{};
        std::mutex outputMutex;
        std::string executableName;
        std::string buildOutput;
        TextEditor editor;
        bool isNewFileLoaded{};
        std::string rootDirectory;
        FileHandler fileHandler;

        std::vector<uint8_t> data;
        size_t data_size;

        std::stringstream outputBuffer;

        std::vector<std::string> compileArgs = {};

    };
}

#endif //PTRVISION_GUIMANAGER_H
