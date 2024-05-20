//
// Created by Lucas Täkker on 18/05/2024.
//

#include "glad/glad.h"
#include "RenderViewLayer.h"
#include "RenderSettingsLayer.h"
#include "StatsLayer.h"
#include "Objects/Sphere.h"
#include "Material/Lambertian.h"
#include "Material/Metal.h"
#include <atomic>
#include <vector>
#include <chrono>

std::shared_ptr<Pathtracer> RenderViewLayer::sm_pathtracer = std::make_shared<Pathtracer>();
std::shared_ptr<RenderSettings> RenderSettingsLayer::sm_renderSettings = std::make_shared<RenderSettings>();
float StatsLayer::RenderFPS = 0.0f;

std::shared_ptr<Pathtracer> RenderViewLayer::GetPathtracer() {
    return sm_pathtracer;
}

RenderViewLayer::~RenderViewLayer() {
    m_shouldRender = false;
    m_condVar.notify_all();
    if (m_renderThread.joinable()) {
        m_renderThread.join();
    }
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

void RenderViewLayer::Start() {
    m_title = "Viewport";
    m_active = true;

    GlobalSettings::samplesPerPixel = 2;
    GlobalSettings::maxDepth = 3;
    GlobalSettings::antialias = true;

    std::shared_ptr<Scene> scene = sm_pathtracer->GetScene();

    // Ground
    scene->AddObject(std::make_shared<Sphere>(Vector3(0, -1000, -1), 1000,
                                              std::make_shared<Lambertian>(Vector3(0.5f, 0.5f, 0.5f))));

    scene->AddObject(std::make_shared<Sphere>(Vector3(-4, 1, 0), 1,
                                              std::make_shared<Metal>(Vector3(0.8f, 0.6f, 0.2f), 0.3f)));

    std::shared_ptr<Camera> camera = sm_pathtracer->GetCamera();
    camera->origin = Vector3(2, 0.5f, 0);
    camera->lookAt = Vector3(0, 0.5f, 0);

    m_renderThread = std::thread(&RenderViewLayer::RenderThread, this);
}

GLuint CreateTextureFromRawPixels(const std::vector<unsigned char>& pixels, int width, int height) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the raw pixel data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void RenderViewLayer::Update() {
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    int width = windowSize.x * RenderSettingsLayer::GetRenderSettings()->viewportResolutionMultiplier;
    int height = windowSize.y * RenderSettingsLayer::GetRenderSettings()->viewportResolutionMultiplier;

    RenderSettingsLayer::UpdateViewportRenderValues();

    if (GlobalSettings::width != width || GlobalSettings::height != height) {
        sm_pathtracer->Resize(width, height);
    }

    if (m_newImageAvailable) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_textureID = CreateTextureFromRawPixels(m_pixels, GlobalSettings::width, GlobalSettings::height);
            m_newImageAvailable = false;
        }
        m_condVar.notify_one();
    } else if (!m_textureID && !m_previousPixels.empty()) {
        m_textureID = CreateTextureFromRawPixels(m_previousPixels, GlobalSettings::width, GlobalSettings::height);
    }

    ImGui::Image((void*)(intptr_t)m_textureID, windowSize);
}

void RenderViewLayer::RenderThread() {
    while (m_shouldRender) {
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