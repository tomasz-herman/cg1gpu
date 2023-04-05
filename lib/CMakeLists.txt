add_definitions(-DGLM_FORCE_SSE2)
add_subdirectory(glm)
get_target_property(_inc glm INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(glm SYSTEM INTERFACE ${_inc})

add_subdirectory(glfw)
add_subdirectory(spdlog)

add_library(glad)
target_include_directories(glad PUBLIC glad/include)
target_sources(glad PRIVATE glad/src/glad.c)

add_library(imgui)
target_include_directories(imgui PUBLIC imgui imgui/backends)
target_sources(imgui PRIVATE imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp imgui/imgui.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_glfw.cpp)
target_link_libraries(imgui PRIVATE glfw glad)

add_library(stb_image)
target_sources(stb_image PRIVATE stb/stb_image.h)
target_include_directories(stb_image PUBLIC stb)
set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(stb_image PRIVATE STB_IMAGE_IMPLEMENTATION)