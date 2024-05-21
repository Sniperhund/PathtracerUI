//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include "../Layer.h"
#include "../../Window.h"
#include <format>

class StatsLayer : public UI::Layer {
public:
    StatsLayer() = default;

    static float RenderFPS;

private:
    void Start() override {
        m_title = "StatsLayer";
        m_active = true;
    }

    void Update() override {
        ImGui::Text("Application FPS: %s", std::to_string(1.0f / Window::GetDeltaTime()).c_str());
        ImGui::Text("Render FPS: %s", std::to_string(RenderFPS).c_str());
    }
};
