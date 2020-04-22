#include <cassert>

#include "glad/glad.h"
#include "glfw3.h"
#include "fmt/format.h"
#include "fmt/printf.h"
#include "stb_image.h"

#include "Image.hpp"
#include "ImageRenderer.hpp"
#include "ResourceManager.hpp"
#include "Specimen.hpp"

#include "Application.hpp"


Application* g_application;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

Application::Application()
{
    assert(!g_application);
    g_application = this;
}

Application::~Application()
{
    m_CleanUp();

    assert(g_application == this);
    g_application = nullptr;
}

void Application::m_CleanUp()
{
    ResourceManager::Clear();
}

void Application::SetCommandArgs(int count, char* args[])
{
    this->args = std::vector<std::string>(args, args + count);
}

int Application::Run()
{
    int error = Application::m_Init();
    if (error)
        return error;

    return m_Main();
}

// TODO: Add an enumeration with success / failure, instead of 0 and 1.
int Application::m_Init()
{

    try {
        m_window.Init(600, 600, "Hello!");
    } catch (const std::exception& e) {
        fmt::print(stderr, "error: {}\n", e.what());
        return 1;
    }

    ResourceManager::Init(&m_window);
    ResourceManager::LoadShader("image");
    ResourceManager::LoadShader("specimen_gen");

    stbi_set_flip_vertically_on_load(true);
    ImageRenderer::Init();
    return 0;
}

int Application::m_Main()
{
    Image img({0, 0}, {500, 600}, "test.jpeg");

    Specimen spec;
    spec.InitGenome(1);
    spec.Generate({500, 600});
    img.SetTexture(spec.GetTexture());

    double t = glfwGetTime();
    int frameCount = 0;
    while (!m_window.ShouldClose()) {
        m_window.PollEvents();

        img.SetSize(m_window.Size());

        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        img.Draw();

        m_window.SwapBuffers();

        if (m_window.KeyPressed(GLFW_KEY_N) == ButtonState::HOLD) {
            spec.InitGenome(5000);
            spec.Generate({500, 600});
            img.SetTexture(spec.GetTexture());
        }

        frameCount++;
        double nt = glfwGetTime();
        if (nt - t >= 1) {
            std::string statusLine = fmt::format("{} fps", frameCount);
            m_window.SetTitle(statusLine);
            t = nt;
            frameCount = 0;
        }
    }

    return 0;
}