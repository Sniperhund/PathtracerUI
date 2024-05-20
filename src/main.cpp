#include "GlobalSettings.h"
#include "Graphics/Window.h"
#include <memory>
#include "Graphics/Layers/StatsLayer.h"
#include "Graphics/Layers/RenderSettingsLayer.h"
#include "Graphics/Layers/RenderViewLayer.h"

int main(int argc, char** argv) {
    CREATE_AND_ADD_LAYER(StatsLayer)
    CREATE_AND_ADD_LAYER(RenderSettingsLayer)
    CREATE_AND_ADD_LAYER(RenderViewLayer)

    Window::CreateWindow(1280, 720);
}