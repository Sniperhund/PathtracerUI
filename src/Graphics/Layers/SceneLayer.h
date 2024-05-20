//
// Created by Lucas Täkker on 19/05/2024.
//

#pragma once

#include <vector>
#include <memory>
#include "../Layer.h"

struct SceneObject {
    std::string name;
};

class SceneLayer : public UI::Layer {
public:
    static std::vector<SceneObject> GetSceneObjects();
    static SceneObject* GetSelectedObject();
private:
    void Update() override;
    void Start() override;

    static std::vector<SceneObject> m_sceneObjects;
    static int m_selectedIndex;
};