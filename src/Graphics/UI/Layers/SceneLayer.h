//
// Created by Lucas Täkker on 19/05/2024.
//

#pragma once

#include <vector>
#include <memory>
#include "../Layer.h"
#include "Objects/Object.h"

struct SceneCamera : public Object {
    Vector3 position = Vector3(4);
    Vector3 focalPoint = Vector3();
    float vfov = 45;
    float defocusAngle = 0;
    float focusDist = 10;

    bool Hit(const Ray &ray, float rayTmin, float rayTmax, HitRecord &hit) const override { return false; }
    bool BoundingBox(AABB &outputBox) const override { return false; }
};

struct SceneObject {
    std::string name;
    std::shared_ptr<Object> object;
};

class SceneLayer : public UI::Layer {
public:
    static std::vector<SceneObject*> GetSceneObjects();
    static SceneObject* GetSelectedObject();
    static void AddSceneObject(SceneObject* sceneObject);
private:
    void Update() override;
    void Start() override;

    static std::vector<SceneObject*> m_sceneObjects;
    static int m_selectedIndex;
};