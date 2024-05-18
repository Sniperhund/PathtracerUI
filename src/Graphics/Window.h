//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
    static void CreateWindow(int width, int height);

private:
    static void RenderWindows();
    static void SetupDockspace();

    static GLFWwindow* s_window;
};
