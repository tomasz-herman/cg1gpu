#include <cstdlib>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Texture.h"

int main() {
    if (!glfwInit()) {
        spdlog::error("Failed to initialize glfw");
        return EXIT_FAILURE;
    }

    auto window = glfwCreateWindow(1280, 800, "CG1GPU", nullptr, nullptr);

    if (!window) {
        spdlog::error("Failed to create glfw window");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL();
    if (version == 0) {
        spdlog::error("Failed to create initialize glad");
        return EXIT_FAILURE;
    }

    const GLubyte *glRenderer = glGetString(GL_RENDERER);
    const GLubyte *glVersion = glGetString(GL_VERSION);
    spdlog::info("Current device: {}", std::string(reinterpret_cast<char const *>(glRenderer)));
    spdlog::info("OpenGL version: {}", std::string(reinterpret_cast<char const *>(glVersion)));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Texture texture = Texture("res/lena.png");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("CG1");

        ImGui::Image(reinterpret_cast<ImTextureID>(texture.Handle), {512, 512});

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return EXIT_SUCCESS;
}