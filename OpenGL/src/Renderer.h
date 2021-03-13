#pragma once

#include <GL/glew.h>
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!x) __debugbreak();
#ifdef DEBUG
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif


// Deal Error funciton

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


/*
Renderer: the class for render data and manage all stuffs
*/
class Renderer
{
public:
	static Renderer& getInstace() {
		static Renderer sInstance;
		return sInstance;
	}
	void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;

};