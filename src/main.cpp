#include "Graphics/Window.h"
#include <memory>
#include "Graphics/UI/Layers/StatsLayer.h"
#include "Graphics/UI/Layers/RenderSettingsLayer.h"
#include "Graphics/UI/Layers/ViewportViewLayer.h"

std::shared_ptr<ViewportViewLayer> viewportViewLayer = std::make_shared<ViewportViewLayer>();

int main(int argc, char** argv) {
    CREATE_AND_ADD_LAYER(StatsLayer)
    CREATE_AND_ADD_LAYER(RenderSettingsLayer)
    Window::AddUILayer(viewportViewLayer);

    Window::CreateWindow(1280, 720);
}