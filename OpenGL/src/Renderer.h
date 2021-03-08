#pragma once

#include <GL/glew.h>
#include <iostream>

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