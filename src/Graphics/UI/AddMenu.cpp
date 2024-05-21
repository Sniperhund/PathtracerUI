//
// Created by Lucas Täkker on 19/05/2024.
//

#include "AddMenu.h"

#include "../Window.h"
#include "Layers/SceneLayer.h"
#include "Objects/Sphere.h"
#include <iostream>

bool showWindow = false;
ImVec2 windowPos;

#define CREATE_BUTTON(Class, displayName) \
    if (ImGui::MenuItem(#displayName)) { \
        SceneObject* sceneObject = new SceneObject; \
        sceneObject->name = #displayName; \
        sceneObject->object = std::make_shared<Class>(); \
        SceneLayer::AddSceneObject(sceneObject);    \
        ImGui::CloseCurrentPopup();       \
    }

void AddMenu::ProcessAddMenu() {
    if (glfwGetKey(Window::GetGLFWWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(Window::GetGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        ImGui::OpenPopup("addMenu");
    }

    if (ImGui::BeginPopup("addMenu")) {
        if (ImGui::BeginMenu("Primitives")) {
            CREATE_BUTTON(Sphere, Sphere)

            ImGui::EndMenu();
        }

        ImGui::Separator();

        CREATE_BUTTON(SceneCamera, Camera)

        ImGui::EndPopup();
    }

    ImGui::ShowDemoWindow();
}
