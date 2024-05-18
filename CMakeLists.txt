cmake_minimum_required(VERSION 3.28)
project(PathtracerUI)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(Libs/glfw)
add_subdirectory(Libs/Pathtracer)

set(IMGUI_ROOT Libs/imgui)
set(IMGUI_SOURCE ${IMGUI_ROOT}/imgui.cpp ${IMGUI_ROOT}/imgui.h ${IMGUI_ROOT}/imstb_rectpack.h
        ${IMGUI_ROOT}/imstb_textedit.h ${IMGUI_ROOT}/imstb_truetype.h ${IMGUI_ROOT}/imgui_demo.cpp
        ${IMGUI_ROOT}/imgui_draw.cpp ${IMGUI_ROOT}/imgui_internal.h ${IMGUI_ROOT}/imgui_tables.cpp
        ${IMGUI_ROOT}/imgui_widgets.cpp ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp
        ${IMGUI_ROOT}/backends/imgui_impl_glfw.h ${IMGUI_ROOT}/backends/imgui_impl_opengl3.h
        ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
        src/Graphics/Window.cpp
        src/Graphics/Window.h
        src/Graphics/Layer.h)

add_executable(PathtracerUI ${IMGUI_SOURCE} src/main.cpp
        src/Graphics/Window.cpp
        src/Graphics/Window.h
        src/Graphics/Layer.h)

find_package(OpenGL REQUIRED)

target_include_directories(PathtracerUI PUBLIC Libs/imgui Libs/imgui/backends Libs/glfw)
target_link_libraries(PathtracerUI glfw OpenGL::GL Pathtracer)