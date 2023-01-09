#include "context.h"
#include "image.h"

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
		 0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// top right,      red
		 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right,   green
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left,    blue
		-0.5f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left,       yellow
	};
	uint32_t indices[] = {
		0, 1, 3,	// first
		1, 2, 3		// second
	};
	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
		GL_STATIC_DRAW, vertices, sizeof(float) * 32);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
		GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if(!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vert shader id: {}", vertShader->Get());
    SPDLOG_INFO("frag shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if(!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

#if 1
    auto image = Image::Load("./image/container.jpg");
    if(!image)
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels",
        image->GetWidth(), image->GetHeight(), image->GetChannelCount());
#else
    auto image = Image::Create(512, 512);
    image->SetCheckImage(16, 16);
#endif
    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.01f, 10.0f);
    auto transform = projection * view * model;
    auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float time = 0.0f;
    float t = sinf(time) * 0.5f + 0.5f;
    auto loc = glGetUniformLocation(m_program->Get(), "color");
	m_program->Use();
    glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    time += 0.016f;
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
    printf(" curr path 현재 경로 %s\n", curDir);
}
