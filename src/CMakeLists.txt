file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(cg1gpu ${SOURCES})
target_link_libraries(cg1gpu PRIVATE glm glfw glad imgui stb_image spdlog)