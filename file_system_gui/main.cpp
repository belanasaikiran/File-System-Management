#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "file_operations.h"

// Variables for user input
char dirName[64] = "";
char fileName[64] = "";
char fileContent[1024] = "";
char statusMessage[256] = "Welcome to the File System GUI!"; // Holds the latest status message
std::string dirContents;

char oldName[64] = "";
char newName[64] = "";
char moveSource[64] = "";
char moveDestination[64] = "";
char copySource[64] = "";
char copyDestination[64] = "";
// Declare a string to hold the mode as a string for display
char newPermissionsStr[16];  // Buffer to hold permission string (e.g., "0644")
mode_t newPermissions = 0644; // Default permissions


int main() {
    snprintf(newPermissionsStr, sizeof(newPermissionsStr), "%o", newPermissions);
    // Setup GLFW
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "File System Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader (glad, glew, etc.) before initializing Dear ImGui
    // Example for GLAD: if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))

    // Initialize Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

  // Load and scale default font
    ImFont* font = io.Fonts->AddFontDefault();

    if (font) {
        font->Scale = 1.5f;  // Scale font by 1.5x
    }
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");



    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // adding Docking space
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

        // GUI for file system operations
        ImGui::Begin("File System Operations");

        // Display status message
        // ImGui::TextWrapped(statusMessage);
        ImGui::Text("%s", statusMessage);

        // Directory creation
        ImGui::InputText("Directory Name", dirName, IM_ARRAYSIZE(dirName));
        if (ImGui::Button("Create Directory")) {
            int result = create_directory(dirName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Directory created successfully: %s", dirName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error creating directory: %s (%s)", dirName, strerror(result));
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete Directory")) {
            int result = delete_directory(dirName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Directory deleted successfully: %s", dirName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error deleting directory: %s (%s)", dirName, strerror(result));
            }
        }

        // File creation
        ImGui::InputText("File Name", fileName, IM_ARRAYSIZE(fileName));
        if (ImGui::Button("Create File")) {
            int result = create_file(fileName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File created successfully: %s", fileName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error creating file: %s (%s)", fileName, strerror(result));
            }
        }

        ImGui::SameLine();
        // delete file
        if (ImGui::Button("Delete File")) {
            int result = delete_file(fileName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File deleted successfully: %s", fileName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error deleting file: %s (%s)", fileName, strerror(result));
            }
        }

        // Write to file
        ImGui::InputTextMultiline("Content to Read/Write", fileContent, IM_ARRAYSIZE(fileContent));
        if (ImGui::Button("Write to File")) {
            int result = write_to_file(fileName, fileContent);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Data written to file: %s", fileName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error writing to file: %s (%s)", fileName, strerror(result));
            }
        }

        ImGui::SameLine();
        // Read from file
        if (ImGui::Button("Read File")) {
            std::string content = read_file(fileName);
            if (content.rfind("Error:",0) == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File read successfully: %s (check console for content)", fileName);
                // fileContents.clear(); // clear if there was an error
                fileContent[0] = '\0';
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "File read successfully: %s", fileName);
                strncpy(fileContent, content.c_str(), IM_ARRAYSIZE(fileContent) - 1); // load content into buffer
                fileContent[IM_ARRAYSIZE(fileContent) - 1] = '\0'; // null-terminate
            }
        }

         // List directory contents
        if (ImGui::Button("List Directory Contents")) {
            dirContents = list_directory_contents(dirName);  // Get contents of the specified directory
            if (dirContents.rfind("Error:", 0) == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "%s", dirContents.c_str());
                dirContents.clear();  // Clear if it's an error message
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Directory contents for: %s", dirName);
            }
        }

        // Display directory contents if available
        ImGui::Text("Directory Contents:");
        
        // Add a dummy item to push the "Clear" button to the right
        ImGui::SameLine(ImGui::GetWindowWidth() - 70); // Adjust the value to position the button correctly
        if (ImGui::Button("Clear")) {
            dirContents.clear();
        }
        ImGui::InputTextMultiline("##dirContents", &dirContents[0], dirContents.size() + 1, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);

        // New Operations: Rename, Move, Copy, Permissions, etc.

        // Rename File/Directory
        ImGui::InputText("Old Name (Rename)", oldName, IM_ARRAYSIZE(oldName));
        ImGui::InputText("New Name (Rename)", newName, IM_ARRAYSIZE(newName));
        if (ImGui::Button("Rename")) {
            int result = rename_file_or_directory(oldName, newName);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Renamed successfully: %s -> %s", oldName, newName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error renaming: %s -> %s", oldName, newName);
            }
        }

        // Move File/Directory
        ImGui::InputText("Move Source", moveSource, IM_ARRAYSIZE(moveSource));
        ImGui::InputText("Move Destination", moveDestination, IM_ARRAYSIZE(moveDestination));
        if (ImGui::Button("Move")) {
            int result = move_file_or_directory(moveSource, moveDestination);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Moved successfully: %s -> %s", moveSource, moveDestination);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error moving: %s -> %s", moveSource, moveDestination);
            }
        }

        // Copy File
        ImGui::InputText("Copy Source", copySource, IM_ARRAYSIZE(copySource));
        ImGui::InputText("Copy Destination", copyDestination, IM_ARRAYSIZE(copyDestination));
        if (ImGui::Button("Copy")) {
            int result = copy_file(copySource, copyDestination);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Copied successfully: %s -> %s", copySource, copyDestination);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error copying: %s -> %s", copySource, copyDestination);
            }
        }

        // Change File Permissions
        ImGui::InputText("New Permissions", newPermissionsStr, sizeof(newPermissionsStr));
        if (ImGui::Button("Change Permissions")) {
            // Convert the string back to mode_t
            mode_t parsedPermissions = strtol(newPermissionsStr, nullptr, 8);  // Convert octal string to mode_t

            int result = change_permissions(fileName, parsedPermissions);
            if (result == 0) {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Permissions changed successfully: %s", fileName);
            } else {
                snprintf(statusMessage, IM_ARRAYSIZE(statusMessage), "Error changing permissions: %s", strerror(result));
            }
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

