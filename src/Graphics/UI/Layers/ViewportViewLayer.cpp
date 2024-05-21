//
// Created by Lucas Täkker on 18/05/2024.
//

#include "glad/glad.h"
#include "ViewportViewLayer.h"
#include "RenderSettingsLayer.h"
#include "StatsLayer.h"
#include "Objects/Sphere.h"
#include "Material/Lambertian.h"
#include "Material/Metal.h"
#include "SceneLayer.h"
#include "../../../Util/Graphics.h"
#include <atomic>
#include <vector>
#include <chrono>

std::shared_ptr<Pathtracer> ViewportViewLayer::sm_pathtracer = std::make_shared<Pathtracer>();
std::atomic<bool> ViewportViewLayer::m_shouldRender = true;
std::shared_ptr<RenderSettings> RenderSettingsLayer::sm_renderSettings = std::make_shared<RenderSettings>();
float StatsLayer::RenderFPS = 0.0f;

std::shared_ptr<Pathtracer> ViewportViewLayer::GetPathtracer() {
    return sm_pathtracer;
}

ViewportViewLayer::~ViewportViewLayer() {
    m_shouldRender = false;
    m_running = false;
    m_condVar.notify_all();
    if (m_renderThread.joinable()) {
        m_renderThread.join();
    }
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

void ViewportViewLayer::Start() {
    m_title = "Viewport";
    m_active = true;

    sm_pathtracer->samplesPerPixel = 1;
    sm_pathtracer->maxDepth = 3;
    sm_pathtracer->antialias = true;

    std::shared_ptr<Camera> camera = sm_pathtracer->GetCamera();
    camera->origin = Vector3(2, 0.5f, 0);
    camera->lookAt = Vector3(0, 0.5f, 0);

    m_renderThread = std::thread(&ViewportViewLayer::RenderThread, this);
}

void ViewportViewLayer::Update() {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    int width = static_cast<int>(windowSize.x * RenderSettingsLayer::GetRenderSettings()->viewportResolutionMultiplier);
    int height = static_cast<int>(windowSize.y * RenderSettingsLayer::GetRenderSettings()->viewportResolutionMultiplier);

    if (width <= 0 || height <= 0) {
        std::cerr << "Update error: Invalid viewport size (" << width << ", " << height << ")." << std::endl;
        return;
    }

    RenderSettingsLayer::UpdateViewportRenderValues();

    if (sm_pathtracer->GetWidth() != width || sm_pathtracer->GetHeight() != height) {
        sm_pathtracer->Resize(width, height);
    }

    if (m_newImageAvailable) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_textureID = CreateTextureFromRawPixels(m_pixels, sm_pathtracer->GetWidth(), sm_pathtracer->GetHeight());
        m_newImageAvailable = false;
        m_condVar.notify_one();
    } else if (!m_textureID && !m_previousPixels.empty()) {
        m_textureID = CreateTextureFromRawPixels(m_previousPixels, sm_pathtracer->GetWidth(), sm_pathtracer->GetHeight());
    }

    ImGui::Image((void*)(intptr_t)m_textureID, windowSize);
}

void ViewportViewLayer::RenderThread() {
    while (m_running) {
        if (!m_shouldRender)
            continue;

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<unsigned char> localPixels;
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            // Render the image
            sm_pathtracer->Render();
            localPixels = sm_pathtracer->GetImage()->GetRawPixels();
        }

        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_pixels = std::move(localPixels);

            m_newImageAvailable = true;
            m_condVar.notify_one();
        }

        // Wait for the main thread to process the pixels
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condVar.wait(lock, [this] { return !m_newImageAvailable || !m_shouldRender; });

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = end - start;
        StatsLayer::RenderFPS = 1.0f / elapsed.count();
    }
}

void ViewportViewLayer::SetShouldRender(bool value) {
    if (value == m_shouldRender) {
        return;
    }

    m_shouldRender = value;

    if (!value) {
        // If stopping the render, wait for the current render cycle to finish.
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condVar.notify_one();
        m_condVar.wait(lock, [this] { return m_newImageAvailable; });
    }
}