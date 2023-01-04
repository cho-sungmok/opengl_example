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
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,	// top right
		 0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,	// bottom left
		-0.5f,  0.5f, 0.0f,	// top left
	};
	uint32_t indices[] = {
		0, 1, 3,	// first
		1, 2, 3		// second
	};
	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
		GL_STATIC_DRAW, vertices, sizeof(float) * 12);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
		GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if(!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vert shader id: {}", vertShader->Get());
    SPDLOG_INFO("frag shader id: {}", fragShader->Get());

     m_program = Program::Create({fragShader, vertShader});
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	m_program->Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
