#include "Shader.h"

#include <fstream>
#include <iostream>

using namespace std;

namespace GLModule
{

	Shader::Shader()
	{
	}

	Shader::~Shader()
	{
		if (m_vertShaderID != -1)
		{
			glDeleteShader(m_vertShaderID);
		}
		if (m_fragShaderID != -1)
		{
			glDeleteShader(m_fragShaderID);
		}
		if (m_programID != -1)
		{
			glDeleteProgram(m_programID);
		}
	}

	void Shader::use()
	{
		glUseProgram(m_programID);
	}

	void Shader::unuse()
	{
		glUseProgram(0);
	}

	bool Shader::initialize()
	{
		if (m_shaderPath.empty())
			return false;
		return initShaderSource(m_shaderPath);
	}

	bool Shader::setShaderSource(const string& shaderPath)
	{
		m_shaderPath = shaderPath;
		return true;
	}

	bool Shader::initShaderSource(const string& shaderPath)
	{
		if (!parserShaderFile(shaderPath, m_vertShaderSource, m_fragShaderSource))
			return false;

		std::string errMsg;
		if (!m_vertShaderSource.empty())
		{
			m_vertShaderID = createShader(m_vertShaderSource, "vert", errMsg);
			if (m_vertShaderID == -1)
			{
				cout << "create vert shader failed, error=" << errMsg;
				return false;
			}
		}
		if (!m_fragShaderSource.empty())
		{
			m_fragShaderID = createShader(m_fragShaderSource, "frag", errMsg);
			if (m_fragShaderID == -1)
			{
				cout << "create frag shader failed, error=" << errMsg;
				return false;
			}
		}

		m_programID = createProgram(m_vertShaderID, m_fragShaderID, errMsg);
		if (m_programID == -1)
		{
			cout << "create program failed, error=" << errMsg;
			return false;
		}
		return true;
	}

	bool Shader::parserShaderFile(const string& shaderPath, string& vertShaderSource, string& fragShaderSource)
	{
		ifstream iFileStream;
		iFileStream.open(shaderPath, ios_base::in);
		if (!iFileStream.is_open())
		{
			cout << "open " << shaderPath << " failed" << endl;
			return false;
		}

		char buff[1024];
		string* target = nullptr;
		while (!iFileStream.eof())
		{
			memset(buff, 0, sizeof(buff));
			iFileStream.getline(&buff[0], sizeof(buff));
			string tmp(buff);
			if (tmp.empty())
				continue;
			if (tmp.compare("[vert]") == 0)
			{
				target = &vertShaderSource;
			}
			else if (tmp.compare("[frag]") == 0)
			{
				target = &fragShaderSource;
			}
			else
			{
				*target += tmp + "\n";
			}
		}
		iFileStream.close();
		return true;
	}

	GLuint Shader::createShader(const string& shaderSrc, const string& shaderType, string& errorMsg)
	{
		GLuint ID = glCreateShader(shaderType == "vert" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		auto cSrc = shaderSrc.c_str();
		GLint len = shaderSrc.size();
		glShaderSource(ID, 1, &cSrc, &len);
		glCompileShader(ID);
		GLint successful;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &successful);
		if (!successful)
		{
			char buf[1024] = { 0 };
			glGetShaderInfoLog(ID, 1024, nullptr, buf);
			errorMsg = buf;
			return -1;
		}
		return ID;
	}

	GLuint Shader::createProgram(GLuint vertID, GLuint fragID, string& errorMsg)
	{
		GLuint ID = glCreateProgram();
		if (vertID != -1)
			glAttachShader(ID, vertID);
		if (fragID != -1)
			glAttachShader(ID, fragID);
		glLinkProgram(ID);
		GLint successful;
		glGetProgramiv(ID, GL_LINK_STATUS, &successful);
		if (!successful)
		{
			char buf[1024] = { 0 };
			glGetProgramInfoLog(ID, 1024, nullptr, buf);
			errorMsg = buf;
			return -1;
		}
		return ID;
	}

	GLuint Shader::LOCATION(const char* name)
	{
		return glGetUniformLocation(m_programID, name);
	}
	void Shader::setUniformInt(const string& name, int val)
	{
		glUniform1i(LOCATION(name.c_str()), val);
	}
	void Shader::setUniformVec2(const string& name, const glm::vec2& val)
	{
		glUniform2fv(LOCATION(name.c_str()), 1, &val[0]);
	}
	void Shader::setUniformVec3(const string& name, const glm::vec3& val)
	{
		glUniform3fv(LOCATION(name.c_str()), 1, &val[0]);
	}
	void Shader::setUniformVec4(const string& name, const glm::vec4& val)
	{
		glUniform4fv(LOCATION(name.c_str()), 1, &val[0]);
	}
	void Shader::setUniformMat3(const string& name, const glm::mat3& mat3)
	{
		glUniformMatrix3fv(LOCATION(name.c_str()), 1, GL_FALSE, &mat3[0][0]);
	}
	void Shader::setUniformMat4(const string& name, const glm::mat4& mat4)
	{
		glUniformMatrix4fv(LOCATION(name.c_str()), 1, GL_FALSE, &mat4[0][0]);
	}
}