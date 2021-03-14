#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayoutcpp.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // swap interval: the delay time of each frame

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	/* print out OpenGL version*/
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			// array coord		// texture coord
			0.0f,	0.0f,		0.0f, 0.0f,
			300.0f, 0.0f,		1.0f, 0.0f,
			300.0f, 300.0f,		1.0f, 1.0f,
			0.0f,	300.0f,		0.0f, 1.0f,
		};

		float positions_2[] = {
			// coordinate	// layout
			300.0f, 300.0f, 0.0f, 0.0f,
			600.0f, 300.0f, 1.0f, 0.0f,
			600.0f, 600.0f, 1.0f, 1.0f,
			300.0f, 600.0f, 0.0f, 1.0f,
		};

		float positions_3[] = {
			// coordinate	// layout
			700.0f, 100.0f,
			900.0f, 100.0f,
			900.0f, 300.0f,
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int indices_3[] = {
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

		VertexArray va1;
		VertexBuffer vb1(positions, 4 * 4 * sizeof(float));

		VertexArray va2;
		VertexBuffer vb2(positions_2, 4 * 4 * sizeof(float));

		VertexArray va3;
		VertexBuffer vb3(positions_3, 4 * 4 * sizeof(float));

		VertexBufferLayout layout1;
		layout1.Push<float>(2);
		layout1.Push<float>(2);
		va1.AddBuffer(vb1, layout1);

		VertexBufferLayout layout2;
		layout2.Push<float>(2);
		layout2.Push<float>(2);
		va2.AddBuffer(vb2, layout2);

		VertexBufferLayout layout3;
		layout3.Push<float>(2);
		va3.AddBuffer(vb3, layout3);

		IndexBuffer ib1(indices, 6);
		IndexBuffer ib2(indices, 6);
		IndexBuffer ib3(indices_3, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

		// use sstream and fstream to read source shader file
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", proj);
		
		// Setup texture coordination
		Texture textureA("res/textures/love_icon.png");
		textureA.Bind(0);
		shader.SetUniform1i("u_Texture", 0);

		Texture textureB("res/textures/terrain.png");
		textureB.Bind(1);
		shader.SetUniform1i("u_Texture", 1);

		// use sstream and fstream to read source shader file
		Shader shader2("res/shaders/Triangle.shader");
		shader2.Bind();
		shader2.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		shader2.SetUniformMat4f("u_MVP", proj);

		// Unbind object 1
		va1.Unbind();
		vb1.Unbind();
		ib1.Unbind();

		// Unbind object 2
		va2.Unbind();
		vb2.Unbind();
		ib2.Unbind();		
		
		// Unbind object 3
		va3.Unbind();
		vb3.Unbind();
		ib3.Unbind();

		shader.Unbind();
		shader2.Unbind();

		Renderer render;

		float r = 0.0f;
		float increment = 0.01f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* R channel increament*/
			if (r > 1.0f)
				increment = -0.01f;
			else if (r < 0.0f)
				increment = 0.01f;
			r += increment;

			/* Render here */
			render.Clear();

			// Using "shader" to draw texture A and texture B
			shader.Bind();
			// Draw texture A
			textureA.Bind(0);
			shader.SetUniform1i("u_Texture", 0);
			render.Draw(va1, ib1, shader);
			
			// Draw texture B
			textureB.Bind(0);
			shader.SetUniform1i("u_Texture", 1);
			render.Draw(va2, ib2, shader);
			
			// Release/unbind "shader"
			shader.Unbind();

			// Using "shader2" to draw Triangle C
			shader2.Bind();
			shader2.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			render.Draw(va3, ib3, shader2);

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());

		}
		
		shader.Unbind();
	}

	glfwTerminate();
	
	return 0;
}