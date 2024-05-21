//
// Created by Lucas on 20-05-2024.
//

#include "glad/glad.h"
#include "RenderView.h"
#include "imgui.h"
#include "nfd.h"

#include "Layers/SceneLayer.h"
#include "Layers/RenderSettingsLayer.h"
#include "../../Util/Graphics.h"

std::shared_ptr<Pathtracer> RenderView::sm_pathtracer = std::make_shared<Pathtracer>();
bool RenderView::active = false;
GLuint RenderView::sm_textureID = 0;

void RenderView::Update() {
    if (!active) {
        return;
    }

    static float zoomFactor = 1.0f;
    static ImVec2 imageSize;

    ImGui::Begin("Render", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Image")) {
                nfdchar_t* outPath;
                NFD_SaveDialogU8(&outPath, NULL, 0, NULL, "image.png");

                sm_pathtracer->GetImage()->Save(outPath);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Calculate the zoomed size of the image
    ImVec2 zoomedSize = ImVec2(sm_pathtracer->GetWidth() * zoomFactor, sm_pathtracer->GetHeight() * zoomFactor);

    // Calculate the position offset to keep the image centered
    ImVec2 offset = ImVec2((ImGui::GetWindowSize().x - zoomedSize.x) * 0.5f, (ImGui::GetWindowSize().y - zoomedSize.y) * 0.5f);

    // Store the actual size of the image
    imageSize = ImVec2(sm_pathtracer->GetWidth(), sm_pathtracer->GetHeight());

    // Display the image with zooming and offset
    ImGui::SetCursorPos(offset);
    ImGui::Image((void*)(intptr_t)sm_textureID, zoomedSize);

    // Handle zoom controls
    if (ImGui::IsWindowHovered() && ImGui::GetIO().MouseWheel != 0.0f) {
        zoomFactor += ImGui::GetIO().MouseWheel * 0.1f; // You can adjust the zoom speed here
        zoomFactor = std::max(0.1f, zoomFactor); // Prevent zooming out too much
    }

    ImGui::End();

}

void RenderView::SetActive(bool value) {
    active = value;
}

extern std::shared_ptr<ViewportViewLayer> viewportViewLayer;

void RenderView::Render() {
    viewportViewLayer->SetShouldRender(false);

    std::shared_ptr<RenderSettings> renderSettings = RenderSettingsLayer::GetRenderSettings();
    std::vector<SceneObject*> sceneObjects = SceneLayer::GetSceneObjects();

    sm_pathtracer->GetScene()->Clear();

    for (SceneObject* sceneObject : sceneObjects) {
        if (std::dynamic_pointer_cast<SceneCamera>(sceneObject->object) != nullptr) {
            std::shared_ptr<Camera> internalCamera = sm_pathtracer->GetCamera();
            std::shared_ptr<SceneCamera> camera = std::dynamic_pointer_cast<SceneCamera>(sceneObject->object);

            internalCamera->origin = camera->position;
            internalCamera->lookAt = camera->focalPoint;
            internalCamera->vfov = camera->vfov;
            internalCamera->defocusAngle = camera->defocusAngle;
            internalCamera->focusDist = camera->focusDist;

            continue;
        }

        sm_pathtracer->GetScene()->AddObject(sceneObject->object);
    }

    sm_pathtracer->GetScene()->BuildBVH();

    sm_pathtracer->Resize(renderSettings->width, renderSettings->height);

    sm_pathtracer->samplesPerPixel = RenderSettingsLayer::GetRenderSettings()->renderSamples;

    // TODO: Show this to the window instead and have a menuItem in the top where the user can save it to an image.
    sm_pathtracer->Render("../output.png");

    std::vector<unsigned char> pixels = sm_pathtracer->GetImage()->GetRawPixels();
    sm_textureID = CreateTextureFromRawPixels(pixels, sm_pathtracer->GetWidth(), sm_pathtracer->GetHeight());

    viewportViewLayer->SetShouldRender(true);
}
