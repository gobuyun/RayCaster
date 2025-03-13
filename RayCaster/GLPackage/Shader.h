#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

#include "CommonDefine.hpp"


namespace GLModule
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		bool setShaderSource(const std::string& shaderPath);
		bool initialize();
		void use();
		void unuse();

	public:
		// set uniform api
		void setUniformInt(const std::string& name, int val);
		void setUniformVec2(const std::string& name, const glm::vec2& val);
		void setUniformVec3(const std::string& name, const glm::vec3& val);
		void setUniformVec4(const std::string& name, const glm::vec4& val);
		void setUniformMat3(const std::string& name, const glm::mat3& mat3);
		void setUniformMat4(const std::string& name, const glm::mat4& mat4);

	private:
		bool initShaderSource(const std::string& shaderPath);
		GLuint createShader(const std::string& shaderSrc, const std::string& shaderType, std::string& errorMsg);
		GLuint createProgram(GLuint vertID, GLuint fragID, std::string& errorMsg);
		bool parserShaderFile(const std::string& shaderPath, std::string& vertShaderSource, std::string& fragShaderSource);
		inline GLuint LOCATION(const char* name);

	private:
		GLuint m_vertShaderID = -1;
		GLuint m_fragShaderID = -1;
		GLuint m_programID = -1;

		std::string m_shaderPath;
		std::string m_vertShaderSource;
		std::string m_fragShaderSource;
	};
};

#endif