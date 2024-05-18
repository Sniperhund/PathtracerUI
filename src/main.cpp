#include "GlobalSettings.h"
#include "Graphics/Window.h"
#include <memory>
#include "Graphics/Layers/Stats.h"

int main(int argc, char** argv) {
    std::shared_ptr<UI::Layer> stats = std::make_shared<Stats>();
    Window::AddUILayer(stats);

    Window::CreateWindow(1280, 720);
}