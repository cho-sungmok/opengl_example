#include "context.h"
#include "image.h"
#include <imgui.h>

ContextUPtr Context::Create()
{
    auto context = ContextUPtr(new Context());
    if(!context->init())
        return nullptr;
    return std::move(context);
}

void Context::Reshape(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, m_width, m_height);
}

void Context::ProcessInput(GLFWwindow* window)
{
	if(!m_cameraControl)
		return;

	const float cameraSpeed = 0.05f;
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
		m_cameraPos += cameraSpeed * m_cameraFront;
	if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
		m_cameraPos -= cameraSpeed * m_cameraFront;

	auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
		m_cameraPos += cameraSpeed * cameraRight;
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
		m_cameraPos -= cameraSpeed * cameraRight;

	auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
	if(glfwGetKey(window, GLFW_KEY_E)==GLFW_PRESS)
		m_cameraPos += cameraSpeed * cameraUp;
	if(glfwGetKey(window, GLFW_KEY_Q)==GLFW_PRESS)
		m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::MouseMove(double x, double y)
{
	if(!m_cameraControl)
		return;
		
	auto pos = glm::vec2((float)x, (float)y);
	auto deltaPos = pos - m_prevMousePos;
	m_prevMousePos = pos;

	const float cameraRotSpeed = 0.8f;
	m_cameraYaw -= deltaPos.x * cameraRotSpeed;
	m_cameraPitch -= deltaPos.y * cameraRotSpeed;

	if(m_cameraYaw < 0.0f)		m_cameraYaw += 360.0f;
	if(m_cameraYaw > 360.0f)	m_cameraYaw -= 360.0f;

	if(m_cameraPitch > 89.0f)	m_cameraPitch = 89.0f;
	if(m_cameraPitch < -89.0f)	m_cameraPitch = -89.0f;
}

void Context::MouseButton(int button, int action, double x, double y)
{
	//if(button==GLFW_MOUSE_BUTTON_LEFT)
	if(button==GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action==GLFW_PRESS)
		{
			m_prevMousePos = glm::vec2((float)x, (float)y);
			m_cameraControl = true;
		}
		else if(action==GLFW_RELEASE)
		{
			m_cameraControl = false;
		}
	}
}

bool Context::init()
{
	float vertices[] = { // pos.xyz, normal.xyz, texcoord.uv
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	};

	uint32_t indices[] = {
		0,  2,  1,  2,  0,  3,
		4,  5,  6,  6,  7,  4,
		8,  9, 10, 10, 11,  8,
		12, 14, 13, 14, 12, 15,
		16, 17, 18, 18, 19, 16,
		20, 22, 21, 22, 20, 23,
	};
	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
		GL_STATIC_DRAW, vertices, sizeof(float) * 8 * 6 * 4);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
		GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6 * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/lighting.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/lighting.fs", GL_FRAGMENT_SHADER);
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
	m_program->SetUniform("tex", 0);
	m_program->SetUniform("tex2", 1);

    return true;
}

void Context::Render()
{
	if (ImGui::Begin("ui window")) {
		if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor))) {
			glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		}
		ImGui::Separator();
		ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
		ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
		ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
		ImGui::Separator();
		if (ImGui::Button("reset camera")) {
			m_cameraYaw = 0.0f;
			m_cameraPitch = 0.0f;
			m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		}		
		if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::DragFloat3("light pos", glm::value_ptr(m_lightPos), 0.01f);
			ImGui::ColorEdit3("light color", glm::value_ptr(m_lightColor));
			ImGui::ColorEdit3("object color", glm::value_ptr(m_objectColor));
			ImGui::SliderFloat("ambient strength", &m_ambientStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("specular strength", &m_specularStrength, 0.0f, 1.0f);
			ImGui::DragFloat("specular shininess", &m_specularShininess, 1.0f, 1.0f, 256.0f);
		}
		ImGui::Checkbox("animation", &m_animation);
	}
	ImGui::End();

	std::vector<glm::vec3> cubePositions = {
		glm::vec3( 0.0f, 0.0f, 0.0f),
		glm::vec3( 2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f,-2.2f,-2.5f),
		glm::vec3(-3.8f,-2.0f,-12.3f),
		glm::vec3( 2.4f,-0.4f,-3.5f),
		glm::vec3(-1.7f, 3.0f,-7.5f),
		glm::vec3( 1.3f,-2.0f,-2.5f),
		glm::vec3( 1.5f, 2.0f,-2.5f),
		glm::vec3( 1.5f, 0.2f,-1.5f),
		glm::vec3(-1.3f, 1.0f,-1.5f),
	};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_cameraFront = 
		glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);// 벡터를 의미(평행이동 안됨)
		//glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);// 점을 의미

    auto projection = glm::perspective(glm::radians(45.0f),
		(float)m_width/(float)m_height, 0.01f, 30.0f);

	auto view = glm::lookAt(m_cameraPos,
		m_cameraPos + m_cameraFront, m_cameraUp);

	auto lightModelTransform =
		glm::translate(glm::mat4(1.0), m_lightPos) *
		glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
	m_program->Use();
	m_program->SetUniform("lightPos", m_lightPos);
	m_program->SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	m_program->SetUniform("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	m_program->SetUniform("ambientStrength", 1.0f);
	m_program->SetUniform("transform", projection * view * lightModelTransform);
	m_program->SetUniform("modelTransform", lightModelTransform);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	m_program->Use();
	m_program->SetUniform("viewPos", m_cameraPos);
	m_program->SetUniform("lightPos", m_lightPos);
	m_program->SetUniform("lightColor", m_lightColor);
	m_program->SetUniform("objectColor", m_objectColor);
	m_program->SetUniform("ambientStrength", m_ambientStrength);
	m_program->SetUniform("specularStrength", m_specularStrength);
	m_program->SetUniform("specularShininess", m_specularShininess);

	for(size_t i=0; i<cubePositions.size(); i++)
	{
		auto& pos = cubePositions[i];
		auto model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::rotate(model,
			glm::radians((m_animation ? (float)glfwGetTime() : 0.0f) * 120 + 20 * i),
			glm::vec3(1.0f, 0.5f, 0.0f));
		auto transform = projection * view * model;
		m_program->SetUniform("transform", transform);
		m_program->SetUniform("modelTransform", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);// 6 x 6
	}
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
