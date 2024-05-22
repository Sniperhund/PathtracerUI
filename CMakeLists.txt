cmake_minimum_required(VERSION 3.28)
project(PathtracerUI)

set(CMAKE_CXX_STANDARD 26)
#set(CMAKE_EXE_LINKER_FLAGS -static)

IF(APPLE)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF()

add_subdirectory(Libs/glfw)
add_subdirectory(Libs/Pathtracer)
add_subdirectory(Libs/glad)
add_subdirectory(Libs/nativefiledialog-extended)

set(IMGUI_ROOT Libs/imgui)
set(IMGUI_SOURCE ${IMGUI_ROOT}/imgui.cpp ${IMGUI_ROOT}/imgui.h ${IMGUI_ROOT}/imstb_rectpack.h
        ${IMGUI_ROOT}/imstb_textedit.h ${IMGUI_ROOT}/imstb_truetype.h ${IMGUI_ROOT}/imgui_demo.cpp
        ${IMGUI_ROOT}/imgui_draw.cpp ${IMGUI_ROOT}/imgui_internal.h ${IMGUI_ROOT}/imgui_tables.cpp
        ${IMGUI_ROOT}/imgui_widgets.cpp ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp
        ${IMGUI_ROOT}/backends/imgui_impl_glfw.h ${IMGUI_ROOT}/backends/imgui_impl_opengl3.h
        ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp)

add_executable(PathtracerUI ${IMGUI_SOURCE} src/main.cpp
        src/Graphics/Window.cpp
        src/Graphics/Window.h
        src/Graphics/UI/Layer.h
        src/Graphics/UI/Layers/StatsLayer.h
        src/Graphics/UI/Layers/ViewportViewLayer.h
        src/Graphics/UI/Layers/ViewportViewLayer.cpp
        src/Graphics/UI/Layers/RenderSettingsLayer.h
        src/Graphics/UI/Layers/SceneLayer.h
        src/Graphics/UI/Layers/SceneLayer.cpp
        src/Graphics/UI/AddMenu.cpp
        src/Graphics/UI/AddMenu.h
        src/Graphics/UI/RenderView.cpp
        src/Graphics/UI/RenderView.h
        src/Util/Graphics.h
        src/Util/Graphics.cpp)

find_package(OpenGL REQUIRED)

target_include_directories(PathtracerUI PUBLIC Libs/imgui Libs/imgui/backends Libs/glfw)
target_link_libraries(PathtracerUI glfw OpenGL::GL Pathtracer glad nfd)