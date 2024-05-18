//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "Layer.h"

class Window {
public:
    static void CreateWindow(int width, int height);

    static void AddUILayer(std::shared_ptr<UI::Layer> layer);

private:
    static void RenderWindows();
    static void SetupDockspace();

    static GLFWwindow* sm_window;
    static std::vector<std::shared_ptr<UI::Layer>> sm_layers;
};
