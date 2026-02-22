#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include <windows.h>
#include "resource.h"

#include <iostream>

#include "MyUI.h"

// Main code
// Main Function
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // GLFW initialisieren
    if (!glfwInit())
    {
        std::cerr << "Fehler: GLFW konnte nicht initialisiert werden!\n";
        return -1;
    }

    // OpenGL Version setzen (3.0+ für ImGui)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Fenster erzeugen
    GLFWwindow* window = glfwCreateWindow(1280, 720, "WINDOWS PATH VIEWER", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Fehler: Fenster konnte nicht erstellt werden!\n";
        glfwTerminate();
        return -1;
    }

    // Nach dem Fenster erstellen:
    HWND hwnd = glfwGetWin32Window(window); // Nur unter Windows verfügbar

    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APP_ICON));
    if (hIcon)
    {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync aktivieren

    // ImGui Context erzeugen
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;   // <- deaktiviert imgui.ini komplett

    // Style setzen
    ImGui::StyleColorsDark();

    // ImGui Backends initialisieren
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI rendern
        RenderPathViewer();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Aufräumen
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
