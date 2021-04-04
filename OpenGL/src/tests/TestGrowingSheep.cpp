#include "TestGrowingSheep.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include <thread>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test 
{
	test::TestGrowingSheep::TestGrowingSheep()
		// initialization member variable
		:m_translation(50, 270, 0), 
		m_Scale(1.0f),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
	{
		float positions[] = {
			// coordinate	// layout
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f,
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
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
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>("res/textures/sheep1.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	test::TestGrowingSheep::~TestGrowingSheep()
	{
	}

	void test::TestGrowingSheep::OnUpdate(float deltaTime)
	{
	}

	void test::TestGrowingSheep::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.5f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer render;

		m_Texture->Bind();
		if (m_translation.x > 910)
		{
			m_translation.x = 50.0f;
			m_Scale = 1.0f;
		}
		else
		{
			m_translation.x += 20.0f;
			m_Scale += 0.05f;
		}

		{	// Draw first object
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, m_Scale, m_Scale));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translation);
			glm::mat4 mvp = Scale * m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			render.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
		// Slow down the frame rate
;		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	void test::TestGrowingSheep::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
