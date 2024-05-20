//
// Created by Lucas Täkker on 19/05/2024.
//

#include "AddMenu.h"

#include "Window.h"
#include <iostream>

bool showWindow = false;
ImVec2 windowPos;

void AddMenu::ProcessAddMenu() {
    if (glfwGetKey(Window::GetGLFWWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(Window::GetGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);
        windowPos = ImVec2((float)mouseX, (float)mouseY);
        showWindow = true;
    }

    if (showWindow) {
        ImGui::SetNextWindowPos(windowPos);
        ImGui::Begin("Add Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (ImGui::Button("Sphere")) {
            std::cout << "Add sphere" << std::endl;
            showWindow = false;
        }

        ImVec2 windowSize = ImGui::GetWindowSize();

        ImGui::End();

        if (showWindow && ImGui::IsMouseClicked(0)) {
            ImVec2 mousePos = ImGui::GetMousePos();
            bool clickedOutside = mousePos.x < windowPos.x || mousePos.x > windowPos.x + windowSize.x ||
                                  mousePos.y < windowPos.y || mousePos.y > windowPos.y + windowSize.y;
            if (clickedOutside) {
                showWindow = false;
            }
        }
    }
}
