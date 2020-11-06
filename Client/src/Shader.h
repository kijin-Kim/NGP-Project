#pragma once
#include <string>

class Shader final
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	~Shader();

	void Bind();
	void UnBind();

private:
	std::string readShaderFile(const std::string& filePath);
	unsigned int compileShader(const std::string& src, unsigned int glShaderType);
	unsigned int createShaderProgram(unsigned int vertexID, unsigned int fragmentID);

private:
	unsigned int m_ID;

};
