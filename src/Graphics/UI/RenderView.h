//
// Created by Lucas on 20-05-2024.
//

#pragma once

#include "Pathtracer.h"

class RenderView {
public:
    static void Update();
    static void Render();
    static void SetActive(bool active);

    static bool active;

private:
    static std::shared_ptr<Pathtracer> sm_pathtracer;
    static GLuint sm_textureID;
};
