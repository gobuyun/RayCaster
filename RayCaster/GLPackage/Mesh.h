#ifndef __MESH_H__
#define __MESH_H__

#include "CommonDefine.hpp"
#include "Shader.h"

#include <vector>
#include <functional>


namespace GLModule
{
	namespace VertexNS
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uvCoord;
		};
		const GLuint POSITION = 0;
		const GLuint NORMAL = 1;
		const GLuint UVCOORD = 2;
	};

	struct Texture
	{
		std::string path;
		std::string type;
		GLuint id;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<VertexNS::Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, GLenum drawMode = GL_TRIANGLES);
		void setDrawMode(GLenum drawType);
		bool initialize();
		void draw(Shader& shader);
		const std::vector<VertexNS::Vertex>& getVertices();
		const std::vector<unsigned int>& getIndices();

	private:
		bool setupMesh();
		void activeAllTexture(Shader& shader);

	private:
		std::function<void()> drawMesh;
		std::vector<VertexNS::Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Texture> m_textures;
		GLenum m_drawType;
		GLuint m_vao = -1;
	};
};

#endif