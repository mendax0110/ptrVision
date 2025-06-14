#define IMGUI_DEFINE_MATH_OPERATORS
#define GL_SILENCE_DEPRECATION

#include <FileSystemManager.h>
#include <llvm/ADT/StringRef.h>
#include <GuiManager.h>
#include <TreeNode.h>
#include <JsonParser.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <array>
#include <filesystem>
#include <json.hpp>
#include "../external/dearimgui/examples/libs/glfw/include/GLFW/glfw3.h"
#include "../external/imgui_club/imgui_memory_editor/imgui_memory_editor.h"
#include "imgui_internal.h"
#include <AsmGen.h>
#include <CliManager.h>

using namespace clang;
namespace fs = std::filesystem;
using json = nlohmann::json;

ActiveDialog currentFileDialog = ActiveDialog::None;
static MemoryEditor mem_edit;

GuiManager::GuiManager()
        : rootNode(nullptr)
        , bgColor(0.45f, 0.55f, 0.60f, 1.00f)
        , data_size(0)

{
    glfwInit();
    window = glfwCreateWindow(1280, 720, "ptrVision", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = "../initialize/imgui.ini";

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120");

    std::ifstream file("project.json");
    if (file)
    {
        try
        {
            json fileTreeJson;
            file >> fileTreeJson;
            rootNode = new TreeNode("", "", NodeType::Directory);
            JsonParser::Parse(*rootNode, fileTreeJson["root"], fileTreeJson["project"]);
        }
        catch (const std::exception& e)
        {
            CliManager::print(OutputLevel::ERROR, "Failed to parse project.json: ", std::string(e.what()));
        }
    }
    else
    {
        CliManager::print(OutputLevel::WARNING, "project.json not found. Skipping load.");
    }

    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
}

GuiManager::~GuiManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    delete rootNode;
}

void GuiManager::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderDockingSpace();
        RenderProjectTree(rootNode);
        RenderMainMenu();
        RenderFileDialog();
        RenderActiveEditor();
        MemoryEditor();
        RenderOutputWindow();
        RenderAssemblyWindow();
        if (settingsOpen)
        {
            OpenSettings();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void GuiManager::RenderDockingSpace()
{
    auto& style = ImGui::GetStyle();
    auto& io = ImGui::GetIO();

    io.ConfigDockingTransparentPayload = true;
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.WindowBorderSize = 1.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_DockingEmptyBg] = style.Colors[ImGuiCol_WindowBg];
    style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.75f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.25f, 0.35f, 0.35f, 0.70f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    static bool dockspaceInitialized = false;
    if (!dockspaceInitialized && io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        dockspaceInitialized = true;

        ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, io.DisplaySize);

        ImGuiID dock_main_id = dockspaceID;
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);

        ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow("Project Tree", dock_id_left);
        ImGui::DockBuilderDockWindow("Editor", dock_main_id);
        ImGui::DockBuilderDockWindow("Memory Editor", dock_id_bottom);
        ImGui::DockBuilderDockWindow("Output Window", dock_id_bottom);
        ImGui::DockBuilderDockWindow("Assembly Window", dock_id_bottom);

        ImGui::DockBuilderFinish(dockspaceID);
    }

    ImGui::End();
}

void GuiManager::RenderMainMenu()
{
    if (currentFileDialog == ActiveDialog::None)
    {
        RenderMainMenuBar();
    }
    RenderPopups();
}

void GuiManager::RenderMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        RenderMenu("File",
        {
                {"New File", "Ctrl+N", [this](){ OpenFileDialog(ActiveDialog::CreateFile, "Create New File"); }},
                {"Open", "Ctrl+O", [this](){ OpenFileDialog(ActiveDialog::OpenFile, "Choose File"); }},
                {"Save", "Ctrl+S", [this]() { SaveFile(); }},
                {"Run File Analysis", "F5", [this]() {
                    std::vector<std::string> compileArgs = {};
                    RunAnalysis([this, &compileArgs](const std::string& filePath, std::vector<std::string>& args) {
                        FileSystemManager::processFile(filePath, args);
                    });
                }},
                {"Run Directory Analysis", "F6", [this]() {
                    std::vector<std::string> compileArgs = {};
                    RunAnalysis([this, &compileArgs](const std::string& filePath, std::vector<std::string>& args) {
                        std::filesystem::path path(filePath);
                        if (std::filesystem::is_directory(path))
                        {
                            FileSystemManager::processDirectory(filePath, args);
                        }
                        else
                        {
                            CliManager::print(OutputLevel::ERROR, "Selected path is not a directory: ", filePath);
                        }
                    });
                }},

        {"Exit", "Alt+F4", [this]() { ExitApp(); }}
        });

        RenderMenu("Output",
        {
                {"Clear Output", " ", [this]() { FileSystemManager::clearAllOut(); }}
        });

        RenderMenu("Assembly",
        {
                {"Generate Assembly", " ", [this]() { shouldGenerateAssembly = true; }},
                {"Clear Assembly", " ", [this]() { AsmGen::clearGeneratedAssembly(); }}
        });

        RenderMenu("Edit",
        {
                {"Undo", "Ctrl+Z", [this]() { Undo(); }, undoStack.size() > 1},
                {"Redo", "Ctrl+Y", [this]() { Redo(); }, !redoStack.empty()}
        });

        RenderMenu("Settings",
        {
                {"Open Settings", " ", [this]() { settingsOpen = true; OpenSettings();}}
        });

        RenderMenu("Help",
        {
                {"Documentation", " ", [this]() { OpenDocumentation(); }},
                {"About", " ", [this]() { ImGui::OpenPopup("About"); }}
        });

        ImGui::EndMainMenuBar();
    }
}

void GuiManager::RenderMenu(const char* menuName, const std::vector<GuiManager::MenuItem>& menuItems)
{
    if (ImGui::BeginMenu(menuName))
    {
        for (const auto& item : menuItems)
        {
            if (ImGui::MenuItem(item.name, item.shortcut, item.enabled))
            {
                item.action();
            }
        }
        ImGui::EndMenu();
    }
}

void GuiManager::OpenSettings()
{
    if (ImGui::Begin("Settings", &settingsOpen))
    {
        ImGui::Text("ImGui Settings");
        ImGui::Checkbox("Enable Anti-Aliasing", &ImGui::GetStyle().AntiAliasedLines);
        ImGui::Checkbox("Enable Anti-Aliasing for Lines", &ImGui::GetStyle().AntiAliasedLinesUseTex);
        ImGui::Checkbox("Enable Anti-Aliasing for Polygons", &ImGui::GetStyle().AntiAliasedFill);
        ImGui::ColorEdit3("Background Color", (float*)&bgColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::Text("Window Size: %d x %d", windowWidth, windowHeight);
        if (ImGui::Button("Save Settings"))
        {
            CliManager::print(OutputLevel::INFO, "Settings saved.");
            auto saveSettings = [this]() {
                std::ofstream settingsFile("settings.json");
                if (settingsFile.is_open())
                {
                    json settingsJson;
                    settingsJson["bgColor"] = { bgColor.x, bgColor.y, bgColor.z, bgColor.w };
                    settingsJson["windowWidth"] = windowWidth;
                    settingsJson["windowHeight"] = windowHeight;
                    settingsFile << settingsJson.dump(4);
                    settingsFile.close();
                    CliManager::print(OutputLevel::INFO, "Settings saved to settings.json");
                }
                else
                {
                    CliManager::print(OutputLevel::ERROR, "Failed to open settings file for writing.");
                }
            };

            saveSettings();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Close"))
        {
            settingsOpen = false;
            ImGui::CloseCurrentPopup();
        }
    }

    ImGui::End();
}

void GuiManager::OpenDocumentation()
{
    std::string rootPath = std::filesystem::current_path().parent_path().string();
    std::string documentationPath = rootPath + "/docs/doxygen/html/index.html";

    if (std::filesystem::exists(documentationPath))
    {
        CliManager::print(OutputLevel::INFO, "Opening documentation: ", documentationPath);
        #ifdef _WIN32
        std::string command = "start \"\" \"" + documentationPath + "\"";
        #elif __APPLE__
        std::string command = "open \"" + documentationPath + "\"";
        #else // For Linux and other Unix-like systems
        std::string command = "xdg-open \"" + documentationPath + "\"";
        #endif
        system(command.c_str());
    }
    else
    {
        CliManager::print(OutputLevel::ERROR, "Documentation file not found: ", documentationPath);
    }
}

void GuiManager::OpenFileDialog(clang::ActiveDialog dialogType, const char* dialogTitle)
{
    currentFileDialog = dialogType;
    const char* dialogName = dialogType == ActiveDialog::OpenFile ? "ChooseFileDlg" : "CreateNewFileDlg";

    if (!fileDialog.IsOpened())
    {
        // Create config
        IGFD::FileDialogConfig config;
        config.flags = ImGuiFileDialogFlags_None;
        config.sidePaneWidth = 250.0f; // Optional: set side pane width

        // Open dialog with config
        fileDialog.OpenDialog(
                dialogName,        // Key
                dialogTitle,       // Title
                ".cpp,.h,.hpp,.txt,.bin,.out,*",  // Filters
                config             // Configuration
        );

        // Set the size using ImGui::SetNextWindowSize
        ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_FirstUseEver);

        CliManager::print(OutputLevel::INFO, "Opening file dialog: ", dialogTitle);
    }
}

void GuiManager::RunAnalysis(const std::function<void(const std::string&, std::vector<std::string>&)>& analysisFunc)
{
    if (currentFileIndex != -1)
    {
        const auto& file = openFiles[currentFileIndex];
        std::vector<std::string> compileArgs;
        analysisFunc(file.filePath + "/" + file.fileName, compileArgs);
    }
    else
    {
        ImGui::OpenPopup("No File Selected");
    }
}

void GuiManager::ExitApp()
{
    CliManager::print(OutputLevel::INFO, "Exiting application...");
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GuiManager::RenderPopups()
{
    auto SimplePopup = [](const char* id, const char* message) {
        if (ImGui::BeginPopup(id))
        {
            ImGui::Text("%s", message);
            if (ImGui::Button("OK"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    };

    SimplePopup("No File Selected", "No file selected for compilation.");
    SimplePopup("Invalid File Type", "Only .cpp files can be compiled.");
    SimplePopup("No Executable", "No executable found. Please compile your code.");
    SimplePopup("About", "TextEditorApp v1.0. Built with ImGui & love.");
}

void GuiManager::RenderActiveEditor()
{
    ImGui::Begin("Editor");

    if (currentFileIndex >= 0 && currentFileIndex < openFiles.size())
    {
        OpenFile& currentFile = openFiles[currentFileIndex];
        std::string windowTitle = "Editor: " + currentFile.fileName;
        currentFile.editor.Render(windowTitle.c_str(), ImVec2(0, 0), true);
    }
    else
    {
        ImGui::Text("No file is currently open.");
    }

    ImGui::End();
}

void GuiManager::SaveFile()
{
    OpenFile & currentFile = openFiles[currentFileIndex];
    std::string filePath = currentFile.filePath;
    std::string filePathName = currentFile.fileName;

    if (filePathName.empty())
    {
        CliManager::print(OutputLevel::ERROR, "No file name available for saving.");
        return;
    }

    std::ofstream ofs(filePath + "/" + filePathName, std::ofstream::trunc);
    if (ofs)
    {
        text = editor.GetText();
        ofs << text;
        CliManager::print(OutputLevel::INFO, "File saved successfully: ", filePath, "/", filePathName);
    }
    else
    {
        CliManager::print(OutputLevel::ERROR, "Failed to open file for saving: ", filePath, "/", filePathName);
    }
}

void GuiManager::Undo()
{
    if (!undoStack.empty())
    {
        redoStack.push(text);
        undoStack.pop();
        text = undoStack.top();
        CliManager::print(OutputLevel::INFO, "Undo operation performed.");
    }
}

void GuiManager::Redo()
{
    if (!redoStack.empty())
    {
        undoStack.push(text);
        text = redoStack.top();
        redoStack.pop();
        CliManager::print(OutputLevel::INFO, "Redo operation performed.");
    }
}

void GuiManager::RenderFileDialog()
{
    switch (currentFileDialog)
    {
        case ActiveDialog::OpenFile:
        case ActiveDialog::CreateFile:
            HandleFileDialog(currentFileDialog);
            break;
        default:
            break;
    }
}

void GuiManager::HandleFileDialog(ActiveDialog dialogType)
{
    const char* dialogName = dialogType == ActiveDialog::OpenFile ? "ChooseFileDlg" : "CreateNewFileDlg";

    if (fileDialog.Display(dialogName))
    {
        if (fileDialog.IsOk())
        {
            std::filesystem::path selectedPath = fileDialog.GetFilePathName();
            HandleFileOpen(selectedPath.filename().string(), selectedPath.parent_path().string(), selectedPath);

            if (fs::is_regular_file(selectedPath))
            {
                std::string selectedDirectory = selectedPath.parent_path().string();
                delete rootNode;
                rootNode = rootNode->BuildProjectTree(selectedDirectory);
                CliManager::print(OutputLevel::INFO, "Rebuilt project tree for: ", selectedDirectory);
            }
        }

        fileDialog.Close();
        currentFileDialog = ActiveDialog::None;
    }
}

bool GuiManager::IsFileOpen(const std::string& filePath,
                            const std::string& fileName)
{
    CliManager::print(OutputLevel::INFO, "Checking if file is open: ", filePath, " ", fileName);
    return std::any_of(openFiles.begin(), openFiles.end(),
                       [&filePath, &fileName](const OpenFile& file)
                       { return file.filePath == filePath && file.fileName == fileName; });
}

void GuiManager::HandleFileOpen(const std::string& fileName,
                                const std::string& filePath,
                                const std::filesystem::path& selectedPath)
{
    if (!IsFileOpen(filePath, fileName))
    {
        OpenFile newFile = CreateNewFile(fileName, filePath, selectedPath);
        if (newFile.fileName.empty())
        {
            CliManager::print(OutputLevel::ERROR, "Failed to open file: ", fileName);
        }
        else
        {
            openFiles.push_back(newFile);
            currentFileIndex = openFiles.size() - 1;
        }
    }
}

GuiManager::OpenFile GuiManager::CreateNewFile(const std::string& fileName,
                                               const std::string& filePath,
                                               const std::filesystem::path& selectedPath)
{
    OpenFile newFile{fileName, filePath};
    std::ifstream inFile(selectedPath, std::ios::in | std::ios::binary);

    if (inFile)
    {
        std::ostringstream ss;
        ss << inFile.rdbuf();
        newFile.content = ss.str();

        newFile.editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
        newFile.editor.SetText(newFile.content);
    }
    else
    {
        newFile.fileName.clear();
    }

    return newFile;
}

void GuiManager::RenderOutputWindow()
{
    ImGui::Begin("Output Window");

    std::string output = FileSystemManager::getStdOut();
    std::string errors = FileSystemManager::getStdErr();

    ImGui::BeginChild("OutputContent", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::TextUnformatted(output.c_str());

    if (!errors.empty())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
        ImGui::TextUnformatted(errors.c_str());
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}

void GuiManager::RenderAssemblyWindow()
{
    ImGui::Begin("Assembly Window");

    if (currentFileIndex >= 0 && currentFileIndex < openFiles.size())
    {
        OpenFile& currentFile = openFiles[currentFileIndex];
        std::string filePath = currentFile.filePath + "/" + currentFile.fileName;

        static std::string cachedAssembly;

        if (shouldGenerateAssembly)
        {
            if (AsmGen::generateStoreAssembly(filePath))
            {
                cachedAssembly = AsmGen::getGeneratedAssembly();
                if (cachedAssembly.empty())
                {
                    CliManager::print(OutputLevel::ERROR, "Failed to generate assembly code for: ", filePath);
                }
            }
            shouldGenerateAssembly = false;
        }

        if (!cachedAssembly.empty())
        {
            ImGui::BeginChild("AssemblyCode", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::TextUnformatted(cachedAssembly.c_str());
            ImGui::EndChild();
        }
        else
        {
            ImGui::Text("No assembly available.");
        }
    }
    else
    {
        ImGui::Text("No file selected for assembly generation.");
    }

    ImGui::End();
}

void GuiManager::RenderProjectTree(TreeNode* node)
{
    if (!node) return;

    ImGui::Begin("Project Tree");

    for (auto& child : node->GetChildren())
    {
        if (child->GetType() == NodeType::Directory)
        {
            if (ImGui::TreeNode(child->GetName().c_str()))
            {
                RenderProjectTree(child);
                ImGui::TreePop();
            }
        }
        else
        {
            std::filesystem::path filePath = child->GetPath();
            if (ImGui::Selectable(child->GetName().c_str()))
            {
                HandleProjectExplorerFileClick(filePath);
            }
        }
    }

    ImGui::End();
}

void GuiManager::HandleProjectExplorerFileClick(const std::filesystem::path& filePath)
{
    if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
    {
        CliManager::print(OutputLevel::ERROR, "Path is not a valid file: ", filePath.string());
        return;
    }

    const std::string fileName = filePath.filename().string();
    const std::string parentPath = filePath.parent_path().string();

    // Check if file is already open
    auto it = std::find_if(openFiles.begin(), openFiles.end(),
                           [&filePath](const OpenFile& file) {
                               return (file.filePath + "/" + file.fileName) == filePath.string();
                           });

    if (it != openFiles.end())
    {
        currentFileIndex = std::distance(openFiles.begin(), it);
        CliManager::print(OutputLevel::INFO, "File already open, switching to tab: ", fileName);
    }
    else
    {
        OpenFile newFile;
        newFile.fileName = fileName;
        newFile.filePath = parentPath;

        std::ifstream inFile(filePath, std::ios::binary);
        if (inFile)
        {
            std::ostringstream ss;
            ss << inFile.rdbuf();
            newFile.content = ss.str();

            newFile.editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
            newFile.editor.SetText(newFile.content);

            openFiles.push_back(newFile);
            currentFileIndex = openFiles.size() - 1;
            CliManager::print(OutputLevel::INFO, "Opened new file: ", fileName);
        }
        else
        {
            CliManager::print(OutputLevel::ERROR, "Failed to open file: ", filePath.string());
            return;
        }
    }

    ImGui::SetWindowFocus("Editor");
}

void GuiManager::MemoryEditor()
{
    ImGui::Begin("Memory Editor");
    ImGui::Text("Memory Editor");

    if (currentFileIndex >= 0 && currentFileIndex < openFiles.size())
    {
        OpenFile& currentFile = openFiles[currentFileIndex];
        mem_edit.DrawWindow("Memory Editor", currentFile.content.data(), currentFile.content.size());
    }
    else
    {
        CliManager::print(OutputLevel::ERROR, "No file is currently open for memory editing.");
    }

    ImGui::End();
}