#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferSizeChnage(GLFWwindow* window, int width, int height)
{
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
	//auto context = (Context*)glfwGetWindowUserPointer(window);
	auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	context->Reshape(width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{})", key, scancode,
        action == GLFW_PRESS ? "pressed" :
        action == GLFW_RELEASE ? "release" :
        action == GLFW_REPEAT ? "repeat" : "unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void OnCursorPos(GLFWwindow* window, double x, double y)
{
	//auto context = (Context*)glfwGetWindowUserPointer(window);
	auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	//auto context = (Context*)glfwGetWindowUserPointer(window);
	auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	context->MouseButton(button, action, x, y);
}

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if (OS==OS_MAC)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if(!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    //const GLubyte* glVersion = glGetString(GL_VERSION);
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", *glVersion);

    auto context = Context::Create();
    if(!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
	glfwSetWindowUserPointer(window, context.get());

    OnFramebufferSizeChnage(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChnage);
    glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetCursorPosCallback(window, OnCursorPos);
	glfwSetMouseButtonCallback(window, OnMouseButton);

    SPDLOG_INFO("Start main loop");
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		context->ProcessInput(window);
        context->Render();
        glfwSwapBuffers(window);
    }
    context.reset();
    context = nullptr;

    glfwTerminate();
    SPDLOG_INFO("Terminate");
    return 0;
}
