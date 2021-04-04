#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayoutcpp.h"
#include "Texture.h"
#include <memory>

namespace test {
	class TestGrowingSheep : public Test
	{
	public:
		TestGrowingSheep();
		~TestGrowingSheep();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::vec3 m_translation;
		glm::mat4 m_Proj, m_View;
		float m_Scale;
	};
}