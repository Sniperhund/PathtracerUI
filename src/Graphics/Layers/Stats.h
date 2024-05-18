//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

#include "../Layer.h"

class Stats : public UI::Layer {
public:
    Stats() = default;

private:
    void Start() override {
        m_title = "Stats";
        m_active = false;
    }

    void Update() override {

    }
};
