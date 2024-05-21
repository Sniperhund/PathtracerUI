//
// Created by Lucas Täkker on 19/05/2024.
//

#include "SceneLayer.h"
#include "ViewportViewLayer.h"

std::vector<SceneObject*> SceneLayer::m_sceneObjects = std::vector<SceneObject*>();
int SceneLayer::m_selectedIndex = -1;

std::vector<SceneObject*> SceneLayer::GetSceneObjects() {
    return m_sceneObjects;
}

SceneObject* SceneLayer::GetSelectedObject() {
    if (m_selectedIndex >= 0 && m_selectedIndex < m_sceneObjects.size())
        return m_sceneObjects[m_selectedIndex];
    return nullptr;
}

extern std::shared_ptr<ViewportViewLayer> viewportViewLayer;

void SceneLayer::AddSceneObject(SceneObject *sceneObject) {
    m_sceneObjects.push_back(sceneObject);

    std::shared_ptr<Scene> scene = ViewportViewLayer::GetPathtracer()->GetScene();

    viewportViewLayer->SetShouldRender(false);
    scene->AddObject(sceneObject->object);
    viewportViewLayer->SetShouldRender(true);
}

void SceneLayer::Update() {
    for (int i = 0; i < m_sceneObjects.size(); i++) {
        if (ImGui::Selectable(m_sceneObjects[i]->name.c_str(), m_selectedIndex == i))
            m_selectedIndex = i;
    }
}

void SceneLayer::Start() {

}