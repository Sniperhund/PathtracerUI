//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <iostream>
#include "UI/Layer.h"

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

class Window {
public:
    static void CreateWindow(int width, int height);

    static void AddUILayer(std::shared_ptr<UI::Layer> layer);

    static float GetDeltaTime() { return sm_deltaTime; }
    static GLFWwindow* GetGLFWWindow() { return sm_window; }
private:
    static void RenderWindows();
    static void SetupDockspace();

    static GLFWwindow* sm_window;
    static std::vector<std::shared_ptr<UI::Layer>> sm_layers;

    static float sm_deltaTime;
};
