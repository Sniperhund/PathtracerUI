//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include <string>

#include <imgui.h>

#define CREATE_AND_ADD_LAYER(LayerType) \
    { \
        std::shared_ptr<UI::Layer> layer = std::make_shared<LayerType>(); \
        Window::AddUILayer(layer); \
    }

namespace UI {
    class Layer {
    public:
        bool m_active = false;
        std::string m_title;

        Layer() = default;
        ~Layer() = default;

        void InternalStart() {
            Start();
        }

        void InternalUpdate() {
            if (!m_active) {
                return;
            }

            ImGui::Begin(m_title.c_str(), &m_active, m_flags);

            Update();

            ImGui::End();
        }

    protected:
        ImGuiConfigFlags m_flags;

        virtual void Start() = 0;
        virtual void Update() = 0;
    };
} // UI