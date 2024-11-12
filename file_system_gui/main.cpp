#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "file_operations.h"
// Include your file system functions here
// #include "filesystem_simulation.h"

// Example functions (replace these with actual implementations)
// void create_directory(const char* name) { /* Your implementation */ }
// void delete_directory(const char* name) { /* Your implementation */ }
// void create_file(const char* name) { /* Your implementation */ }
// void write_to_file(const char* name, const char* content) { /* Your implementation */ }
// void read_file(const char* name) { /* Your implementation */ }

int main() {
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

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Variables for user input
    char dirName[64] = "";
    char fileName[64] = "";
    char fileContent[256] = "";

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // GUI for file system operations
        ImGui::Begin("File System Operations");

        // Directory creation
        ImGui::InputText("Directory Name", dirName, IM_ARRAYSIZE(dirName));
        if (ImGui::Button("Create Directory")) {
            create_directory(dirName);
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete Directory")) {
            delete_directory(dirName);
        }

        // File creation
        ImGui::InputText("File Name", fileName, IM_ARRAYSIZE(fileName));
        if (ImGui::Button("Create File")) {
            create_file(fileName);
        }

        // Write to file
        ImGui::InputTextMultiline("Content to Write", fileContent, IM_ARRAYSIZE(fileContent));
        if (ImGui::Button("Write to File")) {
            write_to_file(fileName, fileContent);
        }

        // Read from file
        if (ImGui::Button("Read File")) {
            read_file(fileName);
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

