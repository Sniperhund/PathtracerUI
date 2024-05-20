//
// Created by Lucas Täkker on 18/05/2024.
//

#include "Window.h"
#include "AddMenu.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdlib>
#include <stdexcept>

GLFWwindow* Window::sm_window = nullptr;
std::vector<std::shared_ptr<UI::Layer>> Window::sm_layers = std::vector<std::shared_ptr<UI::Layer>>();
float Window::sm_deltaTime = 0;

void Window::CreateWindow(int width, int height) {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW could not be initialized");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    sm_window = glfwCreateWindow(width, height, "Pathtracer UI", NULL, NULL);
    if (!sm_window) {
        glfwTerminate();
        throw std::runtime_error("GLFW Window could not be created");
    }

    glfwMakeContextCurrent(sm_window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("GLAD could not be initialized");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(sm_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float lastFrame = 0, lastUpdateTime = 0;
    const float fpsLimit = 1.0f / 60.0f;
    while (!glfwWindowShouldClose(sm_window))
    {
        float currentFrame = glfwGetTime();
        sm_deltaTime = currentFrame - lastFrame;

        glfwPollEvents();

        if ((currentFrame - lastFrame) >= fpsLimit) {
            glClear(GL_COLOR_BUFFER_BIT);

            RenderWindows();

            glfwSwapBuffers(sm_window);

            lastFrame = currentFrame;
        }

        lastUpdateTime = currentFrame;
    }
}

void Window::SetupDockspace() {
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Dockspace", nullptr, windowFlags);

    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("Dockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0, 0));

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("View")) {
            for (std::shared_ptr<UI::Layer> layer : sm_layers) {
                ImGui::MenuItem(layer->m_title.c_str(), NULL, &layer->m_active);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void Window::RenderWindows() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    SetupDockspace();

    for (std::shared_ptr<UI::Layer> layer : sm_layers) {
        layer->InternalUpdate();
    }

    AddMenu::ProcessAddMenu();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::AddUILayer(std::shared_ptr<UI::Layer> layer) {
    sm_layers.push_back(layer);
    layer->InternalStart();
}
