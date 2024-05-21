//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>

#include "glad/glad.h"
#include "../Layer.h"

#include "Pathtracer.h"

class ViewportViewLayer : public UI::Layer {
public:
    ViewportViewLayer() = default;
    ~ViewportViewLayer();

    void Start() override;
    void Update() override;

    void SetShouldRender(bool value);

    static std::shared_ptr<Pathtracer> GetPathtracer();
private:
    static std::shared_ptr<Pathtracer> sm_pathtracer;

    void RenderThread();

    std::thread m_renderThread;
    static std::atomic<bool> m_shouldRender;
    std::mutex m_mutex;
    std::condition_variable m_condVar;
    std::vector<unsigned char> m_pixels;
    std::vector<unsigned char> m_previousPixels;
    bool m_pixelsReady = false;
    std::atomic<bool> m_running = true;

    GLuint m_textureID = 0;
    bool m_newImageAvailable = false;
};