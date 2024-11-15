#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "process_scheduler.h"
#include <vector>

std::vector<Process> processes;
int quantum = 2;
std::string scheduleOutput;

int main() {
    // Initialize GLFW and ImGui (set up OpenGL context, etc.)
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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Process Scheduler Simulator");

        static int pid = 1;
        static int arrivalTime = 0;
        static int burstTime = 0;
        static int priority = 0;

        ImGui::InputInt("Arrival Time", &arrivalTime);
        ImGui::InputInt("Burst Time", &burstTime);
        ImGui::InputInt("Priority", &priority);

        if (ImGui::Button("Add Process")) {
            processes.push_back({pid++, arrivalTime, burstTime, priority});
        }

        ImGui::Separator();
        ImGui::Text("Select Scheduling Algorithm");

        if (ImGui::Button("First-Come, First-Served")) {
            fcfs(processes);
            scheduleOutput = display_schedule(processes);
        }

        ImGui::SameLine();
        if (ImGui::Button("Round Robin")) {
            ImGui::InputInt("Quantum", &quantum);
            round_robin(processes, quantum);
            scheduleOutput = display_schedule(processes);
        }

        ImGui::SameLine();
        if (ImGui::Button("Shortest Job First")) {
            sjf(processes);
            scheduleOutput = display_schedule(processes);
        }

        ImGui::SameLine();
        if (ImGui::Button("Priority Scheduling")) {
            priority_scheduling(processes);
            scheduleOutput = display_schedule(processes);
        }

        // Display the scheduling output
        ImGui::Text("Schedule Output:");
        ImGui::InputTextMultiline("##ScheduleOutput", &scheduleOutput[0], scheduleOutput.size() + 1, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);

        ImGui::End();

        // Render ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup code
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
