//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include "../Layer.h"
#include "ViewportViewLayer.h"

// Add more settings (like depth etc)
struct RenderSettings {
    int width = 1280, height = 720;
    int viewportSamples = 1, renderSamples = 32;
    int maxThreads = 0;
    float viewportResolutionMultiplier = 1.0f;
    bool antialias = true;
};

class RenderSettingsLayer : public UI::Layer {
public:
    static std::shared_ptr<RenderSettings> GetRenderSettings() { return sm_renderSettings; }

    static void UpdateViewportRenderValues() {
        ViewportViewLayer::GetPathtracer()->samplesPerPixel = sm_renderSettings->viewportSamples;
        ViewportViewLayer::GetPathtracer()->antialias = sm_renderSettings->antialias;
    }
private:
    static std::shared_ptr<RenderSettings> sm_renderSettings;

    void Start() override {
        m_title = "Render Settings";
        m_active = true;
    }

    void Update() override {
        ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_NoHorizontalScroll;

        ImGui::Text("Output");
        ImGui::InputInt("Width", &sm_renderSettings->width, 1, 1, inputTextFlags);
        ImGui::InputInt("Height", &sm_renderSettings->height, 1, 1, inputTextFlags);

        ImGui::InputFloat("Viewport Resolution Multiplier", &sm_renderSettings->viewportResolutionMultiplier);

        ImGui::Separator();

        ImGui::Text("Render");
        ImGui::InputInt("Viewport Samples", &sm_renderSettings->viewportSamples, 1, 5);
        ImGui::InputInt("Render Samples", &sm_renderSettings->renderSamples, 1, 5);

        ImGui::Checkbox("Antialias", &sm_renderSettings->antialias);

        ImGui::Separator();

        ImGui::Text("Performance (WIP)");
        ImGui::InputInt("Max Threads", &sm_renderSettings->maxThreads);
    }
};