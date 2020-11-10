#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

class Shader final
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	~Shader();

	void Bind();
	void UnBind();

	void SetMat4(const std::string& name, const glm::mat4x4& value)
	{
		int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
		if (uniformLocation == -1)
			assert(false && "Uniform 변수의 위치를 찾는데 실패하였습니다.");
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetVec4(const std::string& name, const glm::vec4& value)
	{
		int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
		if (uniformLocation == -1)
			assert(false && "Uniform 변수의 위치를 찾는데 실패하였습니다.");
		glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
	}

	void SetInt1(const std::string& name, unsigned int value)
	{
		int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
		if (uniformLocation == -1)
			assert(false && "Uniform 변수의 위치를 찾는데 실패하였습니다.");
		glUniform1i(uniformLocation, value);
	}

	

private:
	std::string readShaderFile(const std::string& filePath);
	unsigned int compileShader(const std::string& src, unsigned int glShaderType);
	unsigned int createShaderProgram(unsigned int vertexID, unsigned int fragmentID);

private:
	unsigned int m_ID;

};
