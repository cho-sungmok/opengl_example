#include "context.h"

ContextUPtr Context::Create()
{
    auto context = ContextUPtr(new Context());
    if(!context->init())
        return nullptr;
    return std::move(context);
}

bool Context::init()
{
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if(!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vert shader id: {}", vertShader->Get());
    SPDLOG_INFO("frag shader id: {}", fragShader->Get());

     m_program = Program::Create({fragShader, vertShader});
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program->Get());
    glDrawArrays(GL_POINTS, 0, 1);
}

#include <iostream>
#include <stdio.h>
#if (OS==OS_WINDOW)
    #include <direct.h>
#elif (OS==OS_MAC)
    #include <filesystem>
#endif

Context::Context()
{
    if (__cplusplus == 202002L) std::cout << "C++20";
    else if (__cplusplus == 201703L) std::cout << "C++17";
    else if (__cplusplus == 201402L) std::cout << "C++14";
    else if (__cplusplus == 201103L) std::cout << "C++11";
    else if (__cplusplus == 199711L) std::cout << "C++98";
    else std::cout << "pre-standard C++";

    char curDir[1000];
#if (OS==OS_WINDOW)
    _getcwd(curDir, 1000);
#elif (OS==OS_MAC)
    getcwd(curDir, 1000);
#endif
    printf(" 현재 경로 %s\n", curDir);
}
