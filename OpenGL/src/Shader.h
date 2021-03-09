#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RenderID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniform
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);

private:
	ShaderProgramSource ParseShader(const std::string & filePath);
	unsigned int CompileShader(unsigned int type, const std::string & source);
	unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

	int GetUniformLocation(const std::string& name);
};