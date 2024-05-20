//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include "../Layer.h"
#include <memory>
#define PATHTRACER_IGNORE_GLOBALSETTINGS 1
#include <Pathtracer.h>

class RenderViewLayer : public UI::Layer {
public:
    RenderViewLayer() = default;
    ~RenderViewLayer();

    void Start() override;
    void Update() override;

    static std::shared_ptr<Pathtracer> GetPathtracer();
private:
    static std::shared_ptr<Pathtracer> sm_pathtracer;

    void RenderThread();

    std::thread m_renderThread;
    std::atomic<bool> m_shouldRender{true};
    static std::atomic<bool> m_continueRender;
    std::mutex m_mutex;
    std::condition_variable m_condVar;
    std::vector<unsigned char> m_pixels;
    std::vector<unsigned char> m_previousPixels;
    bool m_pixelsReady = false;

    GLuint m_textureID = 0;
    bool m_newImageAvailable = false;
};