// from https://www.khronos.org/opengl/wiki/Shader_Compilation

#include "Shader.h"
#include <assert.h>
#include <fstream>
#include <filesystem>
#include <GL/glew.h>

Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
	std::string vertexSource = readShaderFile(vsPath);
	std::string fragmentSource = readShaderFile(fsPath);

	unsigned int vertexID = compileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentID = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

	m_ID = createShaderProgram(vertexID, fragmentID);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Bind()
{
	glUseProgram(m_ID);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

std::string Shader::readShaderFile(const std::string& filePath)
{
	std::filesystem::path p(filePath);
	bool bIsExist = std::filesystem::exists(p);
	assert(bIsExist && "셰이더 파일을 찾을 수 없습니다.");

	int fileSize = std::filesystem::file_size(p);

	std::ifstream in(filePath);
	std::string src;
	src.resize(fileSize);
	in.read(src.data(), fileSize);

	return src;
}

unsigned int Shader::compileShader(const std::string& src, unsigned int glShaderType)
{
	unsigned int shader = glCreateShader(glShaderType);
	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const char* source = src.c_str();
	glShaderSource(shader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorMsg;
		errorMsg.resize(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, errorMsg.data());
		fprintf(stderr, "Shader Compilation Error: %s\n", errorMsg.c_str());

		// We don't need the shader anymore.
		glDeleteShader(shader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		assert(false && "정점 셰이더를 생성하는데 실패하였습니다.");
	}

	return shader;
}

unsigned int Shader::createShaderProgram(unsigned int vertexID, unsigned int fragmentID)
{
	unsigned int programID = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	// Link our program
	glLinkProgram(programID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorMsg;
		errorMsg.resize(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, errorMsg.data());
		fprintf(stderr, "Shader Compilation Error: %s\n", errorMsg.c_str());


		// We don't need the program anymore.
		glDeleteProgram(programID);
		// Don't leak shaders either.
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		assert(false && "셰이더 프로그램을 생성하는데 실패하였습니다.");

	}

	// Always detach shaders after a successful link.
	glDetachShader(programID, vertexID);
	glDetachShader(programID, fragmentID);

	return programID;
}
