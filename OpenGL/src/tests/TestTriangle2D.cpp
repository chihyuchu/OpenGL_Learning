#include "TestTriangle2D.h"
#include "Renderer.h"
#include "imgui/imgui.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test 
{
	test::TestTriangle2D::TestTriangle2D()
	// initialization member variable
	:m_translation(480, 270, 0), brightness(0.5f), increment(0.05f),
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
	{
		float positions[] = {
			// coordinate	//layout
			-50.0f, -50.0f,	1.0f, 0.0f, 0.0f,
			 50.0f, -50.0f,	0.0f, 1.0f, 0.0f,
			  0.0f,  50.0f,	0.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2
		};

		// Blend to ensure alpha (transparent)
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Drawing an object process:
		1. bind shader
		2. bind vertex buffer
		3. setup vertex layout
		4. bind index buffer
		*/

		// initialization
		m_Shader = std::make_unique<Shader>("res/shaders/Triangle.shader");
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(positions, 5 * 3 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_IBO = std::make_unique<IndexBuffer>(indices, 3);

		// use sstream and fstream to read source shader file
		m_Shader->Bind();
		m_Shader->SetUniform1f("brightness", brightness);
	}

	test::TestTriangle2D::~TestTriangle2D()
	{
	}

	void test::TestTriangle2D::OnUpdate(float deltaTime)
	{
	}

	void test::TestTriangle2D::OnRender()
	{
		// Make my triangle shine ><!
		if (brightness > 1.0f)
			increment = -0.05f;
		else if (brightness < 0.0f)
			increment = 0.05f;
		brightness += increment;

		Renderer render;

		{	// Draw one object
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translation);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniform1f("brightness", brightness);
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			render.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}

	void test::TestTriangle2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation 1", &m_translation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
