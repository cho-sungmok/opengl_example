#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

int main(int argc, const char** argv)
{
    SPDLOG_INFO("Start program(opengl)");

    SPDLOG_INFO("Initialize glfw");
    if(!glfwInit()) {
        const char* desc = nullptr;
        glfwGetError(&desc);
        SPDLOG_ERROR("failed to initialize glfw: {}", desc);
        return -1;
    }

    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if(!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    SPDLOG_INFO("Start main loop");
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }

    glfwTerminate();
    SPDLOG_INFO("Terminate");
    return 0;
}