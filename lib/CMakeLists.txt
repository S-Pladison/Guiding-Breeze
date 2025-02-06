# -[fmt]--------------------------------------------------------------------

add_subdirectory(fmt)

# -[ImGui]------------------------------------------------------------------

add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    imgui/backends/imgui_impl_sdl2.cpp
    imgui/backends/imgui_impl_sdlrenderer2.cpp

    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_opengl3_loader.h
)

target_include_directories(imgui PUBLIC
    imgui/
    imgui/backends/
)

# -[SDL2]-------------------------------------------------------------------

add_subdirectory(sdl2)

# -[Линковка]---------------------------------------------------------------

target_link_libraries(imgui
    SDL2::SDL2
    SDL2::SDL2main
)

target_link_libraries(${PROJECT_NAME}
    fmt
    SDL2::SDL2
    SDL2::SDL2main
    imgui
)