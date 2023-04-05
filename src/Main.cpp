#include <cstdlib>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Texture.h"
#include "Shader.h"

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

    Texture inputTex = Texture("res/lena.png");
    Texture outputTex = Texture("res/lena.png");
    Shader shader = Shader({{"res/shader.cs", ShaderType::COMPUTE_SHADER}});

    glm::mat3 kernel = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    float divisor = 9.0f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Input Image");
        ImGui::Image(reinterpret_cast<ImTextureID>(inputTex.Handle), {512, 512});
        ImGui::End();

        ImGui::Begin("Output Image");
        ImGui::Image(reinterpret_cast<ImTextureID>(outputTex.Handle), {512, 512});
        ImGui::End();

        ImGui::Begin("Controls");
        if(ImGui::Button("Apply Filter")) {
            shader.Use();

            shader.LoadMatrix3("kernel", kernel);
            shader.LoadFloat("divisor", divisor);
            shader.LoadInteger("imgSize", 512);

            glBindImageTexture(0, inputTex.Handle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
            glBindImageTexture(1, outputTex.Handle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

            Shader::Dispatch(512, 512, 1);

            Shader::Wait();
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset")) {
            outputTex.Bind();
            outputTex.LoadDataFromFile("res/lena.png");
            Texture::Unbind();
        }
        ImGui::InputFloat3("C0", glm::value_ptr(kernel[0]));
        ImGui::InputFloat3("C1", glm::value_ptr(kernel[1]));
        ImGui::InputFloat3("C2", glm::value_ptr(kernel[2]));
        ImGui::InputFloat("Divisor", &divisor);
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